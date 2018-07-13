#pragma once

#include <map>
#include <string>
#include <iostream>
#include <rapidxml\rapidxml.hpp>
#include <glm/glm.hpp>
#include "SceneObject.h"
#include "Camera.h"
//#include "Fog.h"
#include "AmbientalLight.h"
#include "LightSource.h"
//#include "ShadowMap.h"
//#include "TargetSpawner.h"

#define MAX_FBOS 10

class SceneManager : public InputKeyboardInterface, public InputMouseInterface
{
public:
	~SceneManager();

	static SceneManager *GetInstance();

	bool Init(std::string filepath);
	void Update();
	void Draw(bool debug = false);
	void CleanUp();
	
	//inline const Fog & GetFog() const
	//{
	//	return m_fog;
	//}

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

	void RegisterMouseListeners(InputMouseInterface *listener);
	void RegisterKeyboardListeners(InputKeyboardInterface *listener);

	// Interfaces
	void KeyPress(int key, int mods);
	void MouseMove(float x_offset, float y_offset);
	void MouseScroll(float y_offset);

private:
	static SceneManager *m_instance;

	GLuint m_fbos[MAX_FBOS];
	GLuint m_depth_render_bos;
	GLint m_max_render_buffer_size;

	GLuint m_screen_vbo;
	GLuint m_screen_textures[MAX_FBOS];
	Shader *m_blur_shader, *m_grayscale_shader, *m_sharpen_shader, *m_threshold_shader, *m_combine_tex_shader;

	glm::vec3 m_background_color;
	std::map<std::string, Camera *> m_cameras;
	std::map<std::string, SceneObject *> m_objects;
	std::map<std::string, LightSource *> m_lights;
	//std::map<std::string, Sound *> m_sounds;
	std::string m_active_camera;

	//Fog m_fog;
	AmbientalLight m_ambiental_light;
	//DebugSettings m_debug_settings;
	//ShadowMap *m_shadow_map;
	//TargetSpawner *m_target_spawner;

	// Listeners
	std::vector<InputMouseInterface *> m_mouse_listeners;
	std::vector<InputKeyboardInterface *> m_keyboard_listeners;

	SceneManager();

	void InitScreenQuad();
	void InitFBO();
	void DrawDebug();
	void ApplyGrayscale();
	void ApplyBlur();
	void ApplySharpen();
	void ApplyBloom();
	void RenderPost(Shader *s, uint32_t idx, uint32_t tex_idx, float x_offset, float y_offset);
};

