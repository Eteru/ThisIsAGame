
#include "ShadowMap.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Constants.h"
#include "Strings.h"


ShadowMap::ShadowMap(LightSource *light)
{
	glGenFramebuffers(1, &m_depth_fbo);

	glGenTextures(1, &m_depth_texture);
	glBindTexture(GL_TEXTURE_2D, m_depth_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		Window::WIDTH, Window::HEIGHT, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, m_depth_fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		GL_TEXTURE_2D, m_depth_texture, 0);
	
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	m_light_view = glm::mat4(1.f);
	m_light_space_matrix = glm::mat4(1.f);

	m_shader = ResourceManager::GetInstance()->LoadShader("12");

	m_shadow_box = new ShadowBox(m_light_space_matrix);
	m_light = light;
}


ShadowMap::~ShadowMap()
{
	glDeleteFramebuffers(1, &m_depth_fbo);
	glDeleteTextures(1, &m_depth_texture);

	if (nullptr != m_shadow_box)
	{
		delete m_shadow_box;
		m_shadow_box = nullptr;
	}
}

void ShadowMap::Update()
{
	m_shadow_box->Update(m_light_view);
	//m_light_view = m_light->GetViewMatrix(m_light->GetPosition(),
	//	m_shadow_box->GetCenter());
	//
	//m_light_projection = Matrix().SetOrtho(m_shadow_box->GetWidth(),
	//	m_shadow_box->GetHeight(), m_shadow_box->GetLength());
	m_light_space_matrix = m_light_view * m_light_projection;
}

void ShadowMap::Draw(GLuint vbo)
{
	// debug frustrum
	//Camera *cam = SceneManager::GetInstance()->GetActiveCamera();
	//Shader *s = ResourceManager::GetInstance()->LoadShader("5");
	//glUseProgram(s->GetProgramID());
	//GLuint vbo_test;
	//glGenBuffers(1, &vbo_test);
	//glBindBuffer(GL_ARRAY_BUFFER, vbo_test);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3) * m_shadow_box->m_frustrum_corners.size(), &m_shadow_box->m_frustrum_corners[0], GL_STATIC_DRAW);
	//s->SendAttribute(Constants::POSITION_ATTRIBUTE, 3, 0, 0);
	//
	//s->SendUniform(Constants::COLOR_UNIFORM, Vector3(1, 0, 0));
	//s->SendUniform(Constants::MVP_UNIFORM, Matrix().SetScale(0.5f) * cam->GetView() * cam->GetProjection());
	//
	//
	//glDrawArrays(GL_LINES, 0, m_shadow_box->m_frustrum_corners.size());
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glDeleteBuffers(1, &vbo_test);

	glBindFramebuffer(GL_FRAMEBUFFER, m_depth_fbo);
	
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status == GL_FRAMEBUFFER_COMPLETE) {
		glClear(GL_DEPTH_BUFFER_BIT);
		glUseProgram(m_shader->GetProgramID());
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
	
		m_shader->SendAttribute(ShaderStrings::POSITION_ATTRIBUTE, 3, 0, 0);
	
	}
}
