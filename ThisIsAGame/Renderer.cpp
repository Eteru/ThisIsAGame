
#include <iostream>

#include "Renderer.h"
#include "Strings.h"
#include "ResourceManager.h"
#include "SceneManager.h"

Renderer::Renderer()
	: m_shadow_map(nullptr), m_draw_type(SceneObject::NORMAL)
{
}

Renderer::~Renderer()
{

	if (nullptr != m_shadow_map)
	{
		delete m_shadow_map;
		m_shadow_map = nullptr;
	}

	glDeleteRenderbuffers(1, &m_depth_render_bo);
	glDeleteFramebuffers(MAX_FBOS, m_fbos);
	glDeleteTextures(MAX_FBOS, m_screen_textures);

	glDeleteBuffers(1, &m_screen_vbo);
	glDeleteVertexArrays(1, &m_screen_vao);
}

void Renderer::Init()
{
	m_shadow_map = new ShadowMap(SceneManager::GetInstance()->GetLightSource("1"));
	m_shadow_map->Init();

	InitShaders();
	InitScreenQuad();
	InitBuffers();
}

void Renderer::Update()
{
	m_shadow_map->Update();
}

void Renderer::Render()
{
	const std::map<std::string, SceneObject *> scene_objs =
		SceneManager::GetInstance()->GetAllSceneObjects();

	glCullFace(GL_FRONT);
	glBindFramebuffer(GL_FRAMEBUFFER, m_shadow_map->GetFBO());
	
	glClear(GL_DEPTH_BUFFER_BIT);
	
	for (auto & obj : scene_objs)
	{	
		obj.second->Draw(SceneObject::SHADOW_MAP);
	}
	glCullFace(GL_BACK);

	// render shadow depth to screen
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//PostRender(m_post_shaders[ID::SHADER_POST_TO_SCREEN], 0,
	//	m_shadow_map->GetTexture(), 1.f / Window::WIDTH, 1.f / Window::HEIGHT);

	glBindFramebuffer(GL_FRAMEBUFFER, m_fbos[0]);
	
	// check for framebuffer complete
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status == GL_FRAMEBUFFER_COMPLETE)
	{
		glClearColor(m_background_color.x, m_background_color.y, m_background_color.z, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
		for (auto & obj : scene_objs)
		{
			obj.second->Draw(m_draw_type);
		}
	}
	
	PostToScreen();
}

void Renderer::InitShaders()
{
	m_post_shaders[ID::SHADER_POST_TO_SCREEN] =
		ResourceManager::GetInstance()->LoadShader(ID::SHADER_POST_TO_SCREEN);
	m_post_shaders[ID::SHADER_POST_BLUR] =
		ResourceManager::GetInstance()->LoadShader(ID::SHADER_POST_BLUR);
	m_post_shaders[ID::SHADER_POST_SHARPEN] =
		ResourceManager::GetInstance()->LoadShader(ID::SHADER_POST_SHARPEN);
	m_post_shaders[ID::SHADER_POST_GRAYSCALE] =
		ResourceManager::GetInstance()->LoadShader(ID::SHADER_POST_GRAYSCALE);
	m_post_shaders[ID::SHADER_POST_THRESHOLD] =
		ResourceManager::GetInstance()->LoadShader(ID::SHADER_POST_THRESHOLD);
	m_post_shaders[ID::SHADER_POST_COMBINE_TEXTURES] =
		ResourceManager::GetInstance()->LoadShader(ID::SHADER_POST_COMBINE_TEXTURES);
}

void Renderer::InitScreenQuad()
{
	static const GLfloat quad_vbo_data[] =
	{
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		1.0f,  1.0f, 0.0f,
	};

	glGenVertexArrays(1, &m_screen_vao);
	glBindVertexArray(m_screen_vao);

	glGenBuffers(1, &m_screen_vbo);

	glBindBuffer(GL_ARRAY_BUFFER, m_screen_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vbo_data), quad_vbo_data, GL_STATIC_DRAW);
	m_post_shaders[ID::SHADER_POST_GRAYSCALE]->
		SendAttribute(ShaderStrings::POSITION_ATTRIBUTE, 3, 0, 0);

	glBindVertexArray(0);
}

void Renderer::InitBuffers()
{
	// Init FBO
	glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE, &m_max_render_buffer_size);

	// check if GL_MAX_RENDERBUFFER_SIZE is >= texWidth and texHeight
	if ((m_max_render_buffer_size <= Window::WIDTH) ||
		(m_max_render_buffer_size <= Window::HEIGHT))
	{
		// cannot use framebuffer objects as we need to create
		// a depth buffer as a renderbuffer object
		// return with appropriate error
		std::cerr << "Cannot use FBO due to render buffer size.\n";
		// TODO: decide what to do here.
	}

	glGenFramebuffers(MAX_FBOS, m_fbos);
	glGenRenderbuffers(1, &m_depth_render_bo);
	glBindRenderbuffer(GL_RENDERBUFFER, m_depth_render_bo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16,
		Window::WIDTH, Window::HEIGHT);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	// Init screen texture
	glGenTextures(MAX_FBOS, m_screen_textures);
	for (size_t i = 0; i < MAX_FBOS; ++i)
	{
		glBindTexture(GL_TEXTURE_2D, m_screen_textures[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Window::WIDTH, Window::HEIGHT,
			0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		// bind renderbuffer and create a 16-bit depth buffer
		// width and height of renderbuffer = width and height of
		// the texture

		// bind the framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, m_fbos[i]);
		// specify texture as color attachment
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
			GL_TEXTURE_2D, m_screen_textures[i], 0);
		// specify depth_renderbufer as depth attachment
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
			GL_RENDERBUFFER, m_depth_render_bo);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cerr << "ERROR::FRAMEBUFFER:: Framebuffer is not complete! " << i << std::endl;
		}
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::PostToScreen()
{
	PostRender(m_post_shaders[ID::SHADER_POST_TO_SCREEN], 0,
		m_screen_textures[0], 1.f / Window::WIDTH, 1.f / Window::HEIGHT);
}

void Renderer::PostGrayscale()
{
	PostRender(m_post_shaders[ID::SHADER_POST_GRAYSCALE], 0,
		m_screen_textures[0], 1.f / Window::WIDTH, 1.f / Window::HEIGHT);
}

void Renderer::PostBlur()
{
	PostRender(m_post_shaders[ID::SHADER_POST_BLUR], 0, 
		m_screen_textures[0], 1.f / Window::WIDTH, 1.f / Window::HEIGHT);
}

void Renderer::PostSharpen()
{
	PostRender(m_post_shaders[ID::SHADER_POST_SHARPEN], 0, 
		m_screen_textures[0], 1.f / Window::WIDTH, 1.f / Window::HEIGHT);
}

void Renderer::PostBloom()
{

	PostRender(m_post_shaders[ID::SHADER_POST_THRESHOLD], m_fbos[1],
		m_screen_textures[0], 1.f / Window::WIDTH, 1.f / Window::HEIGHT);
	PostRender(m_post_shaders[ID::SHADER_POST_BLUR], m_fbos[2],
		m_screen_textures[1], 2.f / Window::WIDTH, 2.f / Window::HEIGHT);
	PostRender(m_post_shaders[ID::SHADER_POST_BLUR], m_fbos[3], 
		m_screen_textures[2], 4.f / Window::WIDTH, 4.f / Window::HEIGHT);

	// LAST CALL MUST BE WITH FBO INDEX 0 AND LAST USED TEXTURE
	PostRender(m_post_shaders[ID::SHADER_POST_COMBINE_TEXTURES], 0, 
		m_screen_textures[3], 1.f / Window::WIDTH, 1.f / Window::HEIGHT);
}

void Renderer::PostRender(Shader * s, GLuint fbo, GLuint texID, float x_offset, float y_offset)
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status == GL_FRAMEBUFFER_COMPLETE)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(s->GetProgramID());
		glBindVertexArray(m_screen_vao);

		// Last texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texID);
		s->SendUniform(ShaderStrings::TEXTURE_UNIFORMS[0], 0);

		// Scene texture -- required for bloom
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_screen_textures[0]);
		s->SendUniform(ShaderStrings::TEXTURE_UNIFORMS[1], 1);

		// Distance between pxiels -- required for convolution
		s->SendUniform(ShaderStrings::FRAGMENT_OFFSET_X_UNIFORM, 1.f / Window::WIDTH);
		s->SendUniform(ShaderStrings::FRAGMENT_OFFSET_Y_UNIFORM, 1.f / Window::HEIGHT);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		// unbind the VAO
		glBindVertexArray(m_screen_vao);

		// unbind the program
		glUseProgram(0);
	}
}
