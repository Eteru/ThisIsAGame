#pragma once

#include <map>

#include "ShadowMap.h"

class Renderer
{
public:
	Renderer();
	virtual ~Renderer();

	inline void SetBackgroundColor(glm::vec3 color)
	{
		m_background_color = color;
	}

	inline void SetDrawType(SceneObject::DrawType type)
	{
		m_draw_type = type;
	}

	virtual void Init();
	virtual void Update();
	virtual void Render();

protected:
	const static unsigned MAX_FBOS = 10;

	SceneObject::DrawType m_draw_type;

	GLuint m_screen_vao, m_screen_vbo;

	GLuint m_fbos[MAX_FBOS];
	GLuint m_screen_textures[MAX_FBOS];
	GLuint m_depth_render_bo;
	GLint m_max_render_buffer_size;
	
	ShadowMap *m_shadow_map;
	std::map<std::string, Shader *> m_post_shaders;

	glm::vec3 m_background_color;

	void InitShaders();
	void InitScreenQuad();
	void InitBuffers();

	void PostToScreen();
	void PostGrayscale();
	void PostBlur();
	void PostSharpen();
	void PostBloom();
	void PostRender(Shader *s, uint32_t to, uint32_t from, float x_offset, float y_offset);
};

