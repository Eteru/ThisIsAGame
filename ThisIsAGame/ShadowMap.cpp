
#include "ShadowMap.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Constants.h"
#include "Strings.h"


ShadowMap::ShadowMap(LightSource *light)
{
	m_light_view = glm::mat4(1.f);
	m_light_space_matrix = glm::mat4(1.f);

	m_shader = ResourceManager::GetInstance()->LoadShader(ID::SHADER_SHADOW);

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

void ShadowMap::Init()
{
	glGenFramebuffers(1, &m_depth_fbo);

	glGenTextures(1, &m_depth_texture);
	glBindTexture(GL_TEXTURE_2D, m_depth_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		Window::WIDTH, Window::HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//glBindTexture(GL_TEXTURE_2D, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, m_depth_fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		GL_TEXTURE_2D, m_depth_texture, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowMap::Update()
{
	//m_light_view = m_light->GetViewMatrix(m_shadow_box->GetCenter());
	m_light_view = m_light->GetViewMatrix(glm::vec3(0.f));
	m_shadow_box->Update(m_light_view);
	
	//m_light_projection = glm::ortho(-0.5f * m_shadow_box->GetWidth(), 0.5f * m_shadow_box->GetWidth(),
	//	-0.5f * m_shadow_box->GetHeight(), 0.5f * m_shadow_box->GetHeight(),
	//	-0.5f * m_shadow_box->GetLength(), 0.5f * m_shadow_box->GetLength());

	m_light_projection = glm::ortho(-50.f, 50.f, -50.f, 50.f, -10.f, 20.f);

	m_light_space_matrix = m_light_projection * m_light_view;
}
