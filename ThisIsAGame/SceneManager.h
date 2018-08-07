#pragma once

#include <map>
#include <string>
#include <iostream>
#include <rapidxml\rapidxml.hpp>
#include <glm/glm.hpp>
#include "SceneObject.h"
#include "Camera.h"
#include "Fog.h"
#include "AmbientalLight.h"
#include "LightSource.h"
#include "Terrain.h"
#include "ShadowMap.h"
//#include "TargetSpawner.h"

#define MAX_FBOS 10

class SceneManager : public InputKeyboardInterface, public InputMouseInterface
{
public:
	~SceneManager();

	static SceneManager *GetInstance();

	bool Init(std::string filepath);
	void Update(float dt);
	void Draw(bool debug = false);
	void CleanUp();
	
	void SetGLFWWindow(GLFWwindow *window);
	inline GLFWwindow *GetWindow() const
	{
		return m_window;
	}

	inline const Fog & GetFog() const
	{
		return m_fog;
	}

	inline const AmbientalLight & GetAmbientalLight()
	{
		return m_ambiental_light;
	}
	
	inline const std::map<std::string, LightSource *> & GetLights() const
	{
		return m_lights;
	}
	
	inline LightSource *GetLightSource(std::string id)
	{
		if (m_lights.find(id) == m_lights.end()) {
			return nullptr;
		}
	
		return m_lights[id];
	}

	inline Camera *GetActiveCamera()
	{
		return m_cameras[m_active_camera];
	}

	inline SceneObject *GetSceneObject(std::string id)
	{
		if (m_objects.find(id) == m_objects.end()) {
			return nullptr;
		}

		return m_objects[id];
	}



	//inline ShadowMap *GetShadowMap() const
	//{
	//	return m_shadow_map;
	//}

	void PlaySound(std::string id);
	void AddObject(SceneObject *so);
	void DestroyObject(std::string id);

	void RegisterMouseListener(InputMouseInterface *listener);
	void RegisterKeyboardListener(InputKeyboardInterface *listener);

	// Interfaces
	virtual void KeyPress(int key, int mods);
	virtual void MouseMove(float x_offset, float y_offset);
	virtual void MouseScroll(float y_offset);

	virtual void MouseLeftClick(double x, double y) override;
	virtual void MouseRightClick(double x, double y) override;

	virtual void Raycast(glm::vec3 ray) override;

private:
	static SceneManager *m_instance;

	GLuint m_fbos[MAX_FBOS];
	GLuint m_depth_render_bos;
	GLint m_max_render_buffer_size;

	GLuint m_screen_vao, m_screen_vbo;
	GLuint m_screen_textures[MAX_FBOS];
	Shader *m_blur_shader, *m_grayscale_shader, *m_sharpen_shader, *m_threshold_shader, *m_combine_tex_shader;

	glm::vec3 m_background_color;
	std::map<std::string, Camera *> m_cameras;
	std::map<std::string, SceneObject *> m_objects;
	std::map<std::string, LightSource *> m_lights;
	//std::map<std::string, Sound *> m_sounds;
	std::string m_active_camera;

	Fog m_fog;
	AmbientalLight m_ambiental_light;
	//DebugSettings m_debug_settings;
	ShadowMap *m_shadow_map;
	//TargetSpawner *m_target_spawner;

	// Listeners
	std::vector<InputMouseInterface *> m_mouse_listeners;
	std::vector<InputKeyboardInterface *> m_keyboard_listeners;

	// GLFW window
	GLFWwindow *m_window;

	SceneManager();

	glm::vec3 ComputeRaycast(double x, double y);
	bool CheckTerrainRayIntersection(glm::vec3 ray);
	glm::vec3 FindTerrainRayIntersection(Terrain *t, glm::vec3 ray, float start, float end, uint32_t iter_left);

	void InitScreenQuad();
	void InitFBO();
	void DrawDebug();
	void ApplyGrayscale();
	void ApplyBlur();
	void ApplySharpen();
	void ApplyBloom();
	void RenderPost(Shader *s, uint32_t idx, uint32_t tex_idx, float x_offset, float y_offset);
};

