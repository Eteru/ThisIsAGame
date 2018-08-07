#pragma once
#include "LightSource.h"
#include "Shader.h"
#include "ShadowBox.h"

class ShadowMap
{
public:
	ShadowMap(LightSource *light);
	virtual ~ShadowMap();

	inline glm::mat4 GetLightSpaceMatrix() const
	{
		return m_light_space_matrix;
	}

	inline GLuint GetFBO() const
	{
		return m_depth_fbo;
	}

	inline GLuint GetTexture() const
	{
		return m_depth_texture;
	}

	inline Shader *GetShader() const
	{
		return m_shader;
	}

	inline glm::mat4 GetView()
	{
		return m_light_view;
	}

	inline glm::mat4 GetProjection()
	{
		return m_light_projection;
	}

	void Update();
	void Draw(GLuint vbo);

protected:
	const uint32_t SHADOW_WIDTH = 1024;
	const uint32_t SHADOW_HEIGHT = 1024;

	GLuint m_depth_fbo;
	GLuint m_depth_texture;

	glm::mat4 m_light_space_matrix, m_light_view, m_light_projection;

	Shader *m_shader;
	ShadowBox *m_shadow_box;
	LightSource *m_light;
};

