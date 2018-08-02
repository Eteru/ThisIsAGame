
#include <typeinfo>
#include <ctime>

#include "SceneManager.h"
#include "ResourceManager.h"
#include "SkyBox.h"
#include "Player.h"
#include "SceneObjectBatch.h"
//#include "AnimatedObject.h"
//#include "TTLSceneObject.h"
#include "Primitives.h"
#include "EventManager.h"
#include "Constants.h"
#include "Strings.h"


SceneManager *SceneManager::m_instance = nullptr;

void SceneManager::SetGLFWWindow(GLFWwindow * window)
{
	m_window = window;
}

void SceneManager::PlaySound(std::string id)
{
	//if (m_sounds.find(id) != m_sounds.end()) 
	//{
	//	m_sounds[id]->Play();
	//}
}

void SceneManager::AddObject(SceneObject * so)
{
	if (nullptr == so)
	{
		return;
	}

	so->Update(0);
	m_objects[so->GetName()] = so;
}

void SceneManager::DestroyObject(std::string id)
{
	if (m_objects.find(id) != m_objects.end()) 
	{
		delete m_objects[id];
		m_objects[id] = nullptr;
		m_objects.erase(id);
	}
}

void SceneManager::RegisterMouseListener(InputMouseInterface * listener)
{
	m_mouse_listeners.push_back(listener);
}

void SceneManager::RegisterKeyboardListener(InputKeyboardInterface * listener)
{
	m_keyboard_listeners.push_back(listener);
}

void SceneManager::KeyPress(int key, int mods)
{
	for (InputKeyboardInterface *listener : m_keyboard_listeners)
	{
		listener->KeyPress(key, mods);
	}
}

void SceneManager::MouseMove(float x_offset, float y_offset)
{
	for (InputMouseInterface *listener : m_mouse_listeners)
	{
		listener->MouseMove(x_offset, y_offset);
	}
}

void SceneManager::MouseScroll(float y_offset)
{
	for (InputMouseInterface *listener : m_mouse_listeners)
	{
		listener->MouseScroll(y_offset);
	}
}

void SceneManager::Raycast(glm::vec3 ray)
{
}

void SceneManager::MouseLeftClick(double x, double y)
{
	glm::vec3 ray = ComputeRaycast(x, y);
	CheckTerrainRayIntersection(ray);

	for (InputMouseInterface *listener : m_mouse_listeners)
	{
		listener->MouseLeftClick(x, y);
		listener->Raycast(ray);
	}
}

void SceneManager::MouseRightClick(double x, double y)
{
	glm::vec3 ray = ComputeRaycast(x, y);

	for (InputMouseInterface *listener : m_mouse_listeners)
	{
		listener->MouseRightClick(x, y);
		listener->Raycast(ray);
	}
}

SceneManager::SceneManager()
	: m_blur_shader(nullptr), m_grayscale_shader(nullptr), m_combine_tex_shader(nullptr),
	m_sharpen_shader(nullptr), m_threshold_shader(nullptr), m_window(nullptr)//, m_shadow_map(nullptr), m_target_spawner(nullptr)
{
}

glm::vec3 SceneManager::ComputeRaycast(double x, double y)
{
	glm::mat4 view = GetActiveCamera()->GetView();
	glm::mat4 proj = GetActiveCamera()->GetProjection();

	// Normalzied device coords
	x = (2 * x) / Window::WIDTH - 1.f;
	y = 1.f - (2 * y) / Window::HEIGHT;

	// Clip space
	glm::vec4 ray_aux = glm::vec4(x, y, -1.f, 1.f);

	// Eye space
	ray_aux = glm::inverse(proj) * ray_aux;
	ray_aux.z = -1.f; // set direction
	ray_aux.w = 0.f; // set as direction, not point

	// World space
	ray_aux = glm::inverse(view) * ray_aux;

	glm::vec3 ray = ray_aux;
	ray = glm::normalize(ray);

	return ray;
}

bool SceneManager::CheckTerrainRayIntersection(glm::vec3 ray)
{
	const float RAY_SIZE = 600.f;
	const uint32_t MAX_ITERS = 200;

	if (m_objects.find("terrain") == m_objects.end())
	{
		return false;
	}

	Terrain *t = dynamic_cast<Terrain *>(m_objects["terrain"]);

	glm::vec3 pos = FindTerrainRayIntersection(t, ray, 0.f, RAY_SIZE, MAX_ITERS);

	// test code
	SceneObjectBatch *batch = dynamic_cast<SceneObjectBatch *>(m_objects["batch_1"]);
	batch->AddObject(pos, glm::vec3(0.f, -90.f, 0.f), glm::vec3(1.f));

	// ---

	return true;
}

glm::vec3 SceneManager::FindTerrainRayIntersection(Terrain * t, glm::vec3 ray, float start, float end, uint32_t iter_left)
{
	float t_height = 0.f;
	glm::vec3 scaled_ray = glm::vec3(0.f);

	float half = start + (end - start) * 0.5f;
	scaled_ray = GetActiveCamera()->GetPosition() + ray * half;
	t_height = t->GetTerrainHeight(scaled_ray.x, scaled_ray.z);

	if (iter_left == 0)
	{
		return glm::vec3(scaled_ray.x, t_height, scaled_ray.z);
	}

	if (scaled_ray.y >= t_height)
	{
		return FindTerrainRayIntersection(t, ray, half, end, iter_left - 1);
	}
	else
	{
		return FindTerrainRayIntersection(t, ray, start, half, iter_left - 1);
	}

	
}

void SceneManager::DrawDebug()
{
	//Shader *s = ResourceManager::GetInstance()->GetShader(m_debug_settings.shader_prim_id);
	//Camera *cam = GetActiveCamera();
	//
	//Model *cube = Primitives::GetCube();
	//Model *sphere = Primitives::GetSphere();
	//Model *axis = Primitives::GetAxis();
	//
	//glm::vec3 aabb_color = Primitives::COLOR_RED;
	//glm::vec3 normal_color = Primitives::COLOR_YELLOW;
	//
	//bool wired = true;
	//
	//glUseProgram(s->GetProgramID());
	//
	//if (nullptr != cube) {
	//	for (auto & obj : m_objects) {
	//		std::string obj_name = obj.second->GetName();
	//		if ("SkyBox" == obj_name || "teren" == obj_name) {
	//			continue;
	//		}
	//
	//		BoundingBox bb = obj.second->GetLocalSpaceBB();
	//
	//		glm::vec3 G = glm::vec3(bb.bb_max.x - bb.bb_min.x, bb.bb_max.y - bb.bb_min.y, bb.bb_max.z - bb.bb_min.z) * 0.5f;
	//		//glm::mat4 M = Matrix().SetScale(G) * Matrix().SetTranslation(G + bb.bb_min) *  obj.second->GetModel();
	//		//M = Matrix().SetScale(glm::vec3(10, 10, 10));
	//
	//		glBindBuffer(GL_ARRAY_BUFFER, cube->GetVBO());
	//		// ask for wireframe ibo
	//		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube->GetIBO(wired));
	//
	//		s->SendAttribute(ShaderStrings::POSITION_ATTRIBUTE, 3, sizeof(Vertex), 0);
	//
	//		s->SendUniform(ShaderStrings::COLOR_UNIFORM, aabb_color);
	//		s->SendUniform(ShaderStrings::MVP_UNIFORM, M * cam->GetView() * cam->GetProjection());
	//
	//		glDrawElements(GL_LINES, cube->GetIBOCount(true), GL_UNSIGNED_INT, (void*)0);
	//
	//		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	//		glBindBuffer(GL_ARRAY_BUFFER, 0);
	//		
	//		
	//		glBindBuffer(GL_ARRAY_BUFFER, obj.second->GetMesh()->GetVBONormals());
	//		s->SendAttribute(ShaderStrings::POSITION_ATTRIBUTE, 3, 0, 0);
	//		
	//		s->SendUniform(ShaderStrings::COLOR_UNIFORM, normal_color);
	//		s->SendUniform(ShaderStrings::MVP_UNIFORM, obj.second->GetModel() * cam->GetView() * cam->GetProjection());
	//		
	//		glDrawArrays(GL_LINES, 0, obj.second->GetMesh()->GetVerticesCount() * 2);
	//		glBindBuffer(GL_ARRAY_BUFFER, 0);
	//	}
	//}
	//
	//if (nullptr != sphere) {
	//	for (auto & light : m_lights) {
	//		glm::vec3 lpos = light.second->GetPosition();
	//		glm::mat4 M = Matrix().SetScale(0.01f) * Matrix().SetTranslation(lpos);
	//
	//		glBindBuffer(GL_ARRAY_BUFFER, sphere->GetVBO());
	//		// ask for wireframe ibo
	//		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere->GetIBO(wired));
	//
	//		s->SendAttribute(ShaderStrings::POSITION_ATTRIBUTE, 3, sizeof(Vertex), 0);
	//
	//		s->SendUniform(ShaderStrings::COLOR_UNIFORM, light.second->GetSpecularLight());
	//		s->SendUniform(ShaderStrings::MVP_UNIFORM, M * cam->GetView() * cam->GetProjection());
	//
	//		glDrawElements(
	//			true == wired ? GL_LINES : GL_TRIANGLES,      // mode
	//			sphere->GetIBOCount(wired),    // count
	//			GL_UNSIGNED_INT,   // type
	//			(void*)0           // element array buffer offset
	//		);
	//
	//		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	//		glBindBuffer(GL_ARRAY_BUFFER, 0);
	//	}
	//}
	//
	//s = ResourceManager::GetInstance()->GetShader(m_debug_settings.shader_axis_id);
	//glUseProgram(s->GetProgramID());
	//
	//// axis
	//if (nullptr != axis) {
	//	glLineWidth(3.f);
	//	glm::mat4 M = Matrix().SetScale(1000.f);
	//
	//	glBindBuffer(GL_ARRAY_BUFFER, axis->GetVBO());
	//	// ask for wireframe ibo
	//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, axis->GetIBO(wired));
	//
	//	s->SendAttribute(ShaderStrings::POSITION_ATTRIBUTE, 3, sizeof(Vertex), 0);
	//	s->SendAttribute(ShaderStrings::COLOR_ATTRIBUTE, 3, sizeof(Vertex), sizeof(glm::vec3));
	//
	//	s->SendUniform(ShaderStrings::MVP_UNIFORM, M * cam->GetView() * cam->GetProjection());
	//
	//	glDrawElements(
	//		true == wired ? GL_LINES : GL_TRIANGLES,      // mode
	//		axis->GetIBOCount(wired),    // count
	//		GL_UNSIGNED_INT,   // type
	//		(void*)0           // element array buffer offset
	//	);
	//
	//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	//	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//
	//
	//	for (auto & obj : m_objects) {
	//		std::string obj_name = obj.second->GetName();
	//		if ("SkyBox" == obj_name || "teren" == obj_name) {
	//			continue;
	//		}
	//
	//		M = Matrix().SetScale(50.f) * Matrix().SetRotationY(3.1415f) * obj.second->GetModel();
	//
	//		glBindBuffer(GL_ARRAY_BUFFER, axis->GetVBO());
	//		// ask for wireframe ibo
	//		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, axis->GetIBO(wired));
	//
	//		s->SendAttribute(ShaderStrings::POSITION_ATTRIBUTE, 3, sizeof(Vertex), 0);
	//		s->SendAttribute(ShaderStrings::COLOR_ATTRIBUTE, 3, sizeof(Vertex), sizeof(glm::vec3));
	//
	//		s->SendUniform(ShaderStrings::MVP_UNIFORM, M * cam->GetView() * cam->GetProjection());
	//
	//		glDrawElements(
	//			true == wired ? GL_LINES : GL_TRIANGLES,      // mode
	//			axis->GetIBOCount(wired),    // count
	//			GL_UNSIGNED_INT,   // type
	//			(void*)0           // element array buffer offset
	//		);
	//
	//		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	//		glBindBuffer(GL_ARRAY_BUFFER, 0);
	//	}
	//	glLineWidth(1.f);
	//}
}

void SceneManager::ApplyGrayscale()
{
	RenderPost(m_grayscale_shader, 0, 0, 1.f / Window::WIDTH, 1.f / Window::HEIGHT);
}

void SceneManager::ApplyBlur()
{
	RenderPost(m_blur_shader, 0, 0, 1.f / Window::WIDTH, 1.f / Window::HEIGHT);
}

void SceneManager::ApplySharpen()
{
	RenderPost(m_sharpen_shader, 0, 0, 1.f / Window::WIDTH, 1.f / Window::HEIGHT);
}

void SceneManager::ApplyBloom()
{

	RenderPost(m_threshold_shader, 1, 0, 1.f / Window::WIDTH, 1.f / Window::HEIGHT);
	RenderPost(m_blur_shader, 2, 1, 2.f / Window::WIDTH, 2.f / Window::HEIGHT);
	RenderPost(m_blur_shader, 3, 2, 4.f / Window::WIDTH, 4.f / Window::HEIGHT);

	// LAST CALL MUST BE WITH FBO INDEX 0 AND LAST USED TEXTURE
	RenderPost(m_combine_tex_shader, 0, 3, 1.f / Window::WIDTH, 1.f / Window::HEIGHT);
}

void SceneManager::RenderPost(Shader * s, uint32_t idx, uint32_t tex_idx, float x_offset, float y_offset)
{
	if (idx >= MAX_FBOS || tex_idx >= MAX_FBOS)
	{
		return;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, idx == 0 ? 0 : m_fbos[idx]);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status == GL_FRAMEBUFFER_COMPLETE) 
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(s->GetProgramID());
		glBindBuffer(GL_ARRAY_BUFFER, m_screen_vbo);

		// Last texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_screen_textures[tex_idx]);
		s->SendUniform(ShaderStrings::TEXTURE_UNIFORMS[0], 0);

		// Scene texture
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_screen_textures[0]);
		s->SendUniform(ShaderStrings::TEXTURE_UNIFORMS[1], 1);

		s->SendAttribute(ShaderStrings::POSITION_ATTRIBUTE, 3, 0, 0);
		s->SendUniform(ShaderStrings::FRAGMENT_OFFSET_X_UNIFORM, 1.f / Window::WIDTH);
		s->SendUniform(ShaderStrings::FRAGMENT_OFFSET_Y_UNIFORM, 1.f / Window::HEIGHT);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}


SceneManager::~SceneManager()
{
	for (auto it = m_objects.begin(); it != m_objects.end();) 
	{
		delete it->second;
		it->second = nullptr;
		it = m_objects.erase(it);
	}

	for (auto it = m_cameras.begin(); it != m_cameras.end();)
	{
		delete it->second;
		it->second = nullptr;
		it = m_cameras.erase(it);
	}

	for (auto it = m_lights.begin(); it != m_lights.end();) 
	{
		delete it->second;
		it->second = nullptr;
		it = m_lights.erase(it);
	}

	//if (nullptr != m_shadow_map) {
	//	delete m_shadow_map;
	//	m_shadow_map = nullptr;
	//}
	//
	//if (nullptr != m_target_spawner) {
	//	delete m_target_spawner;
	//	m_target_spawner = nullptr;
	//}

	glDeleteRenderbuffers(1, &m_depth_render_bos);
	glDeleteFramebuffers(MAX_FBOS, m_fbos);
	glDeleteTextures(MAX_FBOS, m_screen_textures);
}

SceneManager * SceneManager::GetInstance()
{
	if (nullptr == m_instance) 
	{
		m_instance = new SceneManager;
	}

	return m_instance;
}

bool SceneManager::Init(std::string filepath)
{
	std::srand(time(0));

	FILE *f = fopen(filepath.c_str(), "r");

	if (NULL == f)
	{
		std::cerr << "Invallid path for scene manager xml file." << std::endl;
		return false;
	}

	fseek(f, 0, SEEK_END);
	long fsize = ftell(f);
	fseek(f, 0, SEEK_SET);

	char *string = new char[fsize + 1];
	size_t sz = fread(string, fsize, 1, f);
	fclose(f);

	string[fsize] = '\0';

	rapidxml::xml_document<> doc;
	doc.parse<0>(string);

	rapidxml::xml_node<> *pRoot = doc.first_node();

	//background
	rapidxml::xml_node<> *pBkg = pRoot->first_node("backgroundColor");

	if (nullptr == pBkg) 
	{
		m_background_color = glm::vec3(0.f, 0.f, 0.f);
	}
	else 
	{
		m_background_color.x = std::stof(pBkg->first_node("r")->value());
		m_background_color.y = std::stof(pBkg->first_node("g")->value());
		m_background_color.z = std::stof(pBkg->first_node("b")->value());
	}

	//cameras
	rapidxml::xml_node<> *pCameras = pRoot->first_node("cameras");
	if (nullptr == pCameras) 
	{
		std::cerr << "Cameras are missing" << std::endl;
		return false;
	}

	for (rapidxml::xml_node<> *pCamera = pCameras->first_node("camera");
		pCamera; 
		pCamera = pCamera->next_sibling())
	{
		rapidxml::xml_attribute<> *pAttribute = pCamera->first_attribute("id");

		if (nullptr == pAttribute) 
		{
			std::cerr << "Camera id missing" << std::endl;
			return false;
		}

		std::string id = std::string(pAttribute->value());

		std::string target_id = "";
		rapidxml::xml_node<> *pFollowingObject = pCamera->first_node("following");
		if (nullptr != pFollowingObject)
		{
			target_id = pFollowingObject->value();
		}
		else
		{
			throw std::runtime_error(std::string("Camera has no following object id."));
		}

		m_cameras[id] = new Camera(target_id);
	}

	rapidxml::xml_node<> *pActiveCamera = pRoot->first_node("activeCamera");
	if (nullptr == pActiveCamera)
	{
		m_active_camera = "1";
	}
	else 
	{
		m_active_camera = pActiveCamera->value();
	}

	// fog
	rapidxml::xml_node<> *pFog = pRoot->first_node("fog");
	if (nullptr != pFog) 
	{
		rapidxml::xml_node<> *pFogColor = pFog->first_node("color");

		glm::vec3 fog_color(0.f);
		if (nullptr != pFogColor) 
		{
			rapidxml::xml_node<> *pFogColorR = pFogColor->first_node("r");
			rapidxml::xml_node<> *pFogColorG = pFogColor->first_node("g");
			rapidxml::xml_node<> *pFogColorB = pFogColor->first_node("b");

			fog_color.x = (nullptr == pFogColorR) ? 1.f : std::stof(pFogColorR->value());
			fog_color.y = (nullptr == pFogColorG) ? 1.f : std::stof(pFogColorG->value());
			fog_color.z = (nullptr == pFogColorB) ? 1.f : std::stof(pFogColorB->value());
		}

		m_fog.SetColor(fog_color);
	}

	// lights
	rapidxml::xml_node<> *pAmbientalLight = pRoot->first_node("ambientalLight");

	float amb_light_ratio = 0.1f;
	glm::vec3 amb_light = glm::vec3(0, 0, 0);
	if (nullptr != pAmbientalLight) 
	{
		rapidxml::xml_node<> *pAmbientalLightColor = pAmbientalLight->first_node("color");
		if (nullptr != pAmbientalLightColor) 
		{
			rapidxml::xml_node<> *pAmbientalLightColorR = pAmbientalLightColor->first_node("r");
			rapidxml::xml_node<> *pAmbientalLightColorG = pAmbientalLightColor->first_node("g");
			rapidxml::xml_node<> *pAmbientalLightColorB = pAmbientalLightColor->first_node("b");

			amb_light.x = (nullptr == pAmbientalLightColorR) ? 0.f : std::stof(pAmbientalLightColorR->value());
			amb_light.y = (nullptr == pAmbientalLightColorG) ? 0.f : std::stof(pAmbientalLightColorG->value());
			amb_light.z = (nullptr == pAmbientalLightColorB) ? 0.f : std::stof(pAmbientalLightColorB->value());
		}

		rapidxml::xml_node<> *pAmbientalLightRatio = pAmbientalLight->first_node("ratio");
		amb_light_ratio = (nullptr == pAmbientalLightRatio) ? 0.1f : std::stof(pAmbientalLightRatio->value());
	}

	m_ambiental_light.SetValue(amb_light_ratio, amb_light);

	rapidxml::xml_node<> *pLights = pRoot->first_node("lights");
	for (rapidxml::xml_node<> *pLight = pLights->first_node("light");
		pLight;
		pLight = pLight->next_sibling()) 
	{
		rapidxml::xml_attribute<> *pLightAttribute = pLight->first_attribute("id");

		if (nullptr == pLightAttribute)
		{
			std::cerr << "Light id missing" << std::endl;
			continue;
		}

		std::string light_id = pLightAttribute->value();
		if (m_lights.find(light_id) != m_lights.end())
		{
			std::cerr << "Light id already used. Skipping." << std::endl;
			continue;
		}

		rapidxml::xml_node<> *pAssociatedObject = pLight->first_node("associatedObject");
		rapidxml::xml_node<> *pDiffuseColor = pLight->first_node("diffuseColor");
		rapidxml::xml_node<> *pSpecularColor = pLight->first_node("specularColor");
		rapidxml::xml_node<> *pLightType = pLight->first_node("type");
		rapidxml::xml_node<> *pSpecCoef = pLight->first_node("specCoef");
		rapidxml::xml_node<> *pDiffCoef = pLight->first_node("diffCoef");
		rapidxml::xml_node<> *pShininess = pLight->first_node("shininess");
		rapidxml::xml_node<> *pSpotAngle = pLight->first_node("spotAngle");
		rapidxml::xml_node<> *pLightDir = pLight->first_node("direction");
		rapidxml::xml_node<> *pLightPos = pLight->first_node("position");
		///TODO: finish dir

		std::string asoc_obj = (nullptr == pAssociatedObject) ? "" : pAssociatedObject->value();

		glm::vec3 light_diff_color = glm::vec3(0, 0, 0);
		if (nullptr != pDiffuseColor)
		{
			rapidxml::xml_node<> *pDiffuseColorR = pDiffuseColor->first_node("r");
			rapidxml::xml_node<> *pDiffuseColorG = pDiffuseColor->first_node("g");
			rapidxml::xml_node<> *pDiffuseColorB = pDiffuseColor->first_node("b");

			light_diff_color.x = (nullptr == pDiffuseColorR) ? 0.f : std::stof(pDiffuseColorR->value());
			light_diff_color.y = (nullptr == pDiffuseColorG) ? 0.f : std::stof(pDiffuseColorG->value());
			light_diff_color.z = (nullptr == pDiffuseColorB) ? 0.f : std::stof(pDiffuseColorB->value());
		}

		glm::vec3 light_spec_color = glm::vec3(0, 0, 0);
		if (nullptr != pSpecularColor) 
		{
			rapidxml::xml_node<> *pSpecularColorR = pSpecularColor->first_node("r");
			rapidxml::xml_node<> *pSpecularColorG = pSpecularColor->first_node("g");
			rapidxml::xml_node<> *pSpecularColorB = pSpecularColor->first_node("b");

			light_spec_color.x = (nullptr == pSpecularColorR) ? 0.f : std::stof(pSpecularColorR->value());
			light_spec_color.y = (nullptr == pSpecularColorG) ? 0.f : std::stof(pSpecularColorG->value());
			light_spec_color.z = (nullptr == pSpecularColorB) ? 0.f : std::stof(pSpecularColorB->value());
		}

		glm::vec3 light_dir = glm::vec3(0, 0, 0);
		if (nullptr != pLightDir) 
		{
			rapidxml::xml_node<> *pLightDirX = pLightDir->first_node("x");
			rapidxml::xml_node<> *pLightDirY = pLightDir->first_node("y");
			rapidxml::xml_node<> *pLightDirZ = pLightDir->first_node("z");

			light_dir.x = (nullptr == pLightDirX) ? 0.f : std::stof(pLightDirX->value());
			light_dir.y = (nullptr == pLightDirY) ? 0.f : std::stof(pLightDirY->value());
			light_dir.z = (nullptr == pLightDirZ) ? 0.f : std::stof(pLightDirZ->value());
		}

		glm::vec3 light_pos = glm::vec3(0, 0, 0);
		if (nullptr != pLightPos) 
		{
			rapidxml::xml_node<> *pLightPosX = pLightPos->first_node("x");
			rapidxml::xml_node<> *pLightPosY = pLightPos->first_node("y");
			rapidxml::xml_node<> *pLightPosZ = pLightPos->first_node("z");

			light_pos.x = (nullptr == pLightPosX) ? 0.f : std::stof(pLightPosX->value());
			light_pos.y = (nullptr == pLightPosY) ? 0.f : std::stof(pLightPosY->value());
			light_pos.z = (nullptr == pLightPosZ) ? 0.f : std::stof(pLightPosZ->value());
		}

		std::string light_type = (nullptr == pLightType) ? "" : pLightType->value();
		float spec_coef = (nullptr == pSpecCoef) ? 0.f : std::stof(pSpecCoef->value());
		float diff_coef = (nullptr == pDiffCoef) ? 0.f : std::stof(pDiffCoef->value());
		float shininess = (nullptr == pShininess) ? 0.f : std::stof(pShininess->value());
		float spot_angle = (nullptr == pSpotAngle) ? 0.f : std::stof(pSpotAngle->value());

		LightSource *ls = new LightSource(shininess, diff_coef, spec_coef, light_diff_color, light_spec_color, asoc_obj);
		ls->SetType(light_type);
		ls->SetDirection(light_dir);
		ls->SetPosition(light_pos);
		ls->SetSpotAngle(spot_angle);

		m_lights[light_id] = ls;
	}
	//m_shadow_map = new ShadowMap(m_lights["1"]);

	//objects
	rapidxml::xml_node<> *pObjects = pRoot->first_node("objects");
	if (nullptr == pObjects) 
	{
		std::cerr << "Objects are missing" << std::endl;
		return false;
	}

	for (rapidxml::xml_node<> *pObject = pObjects->first_node("object");
		pObject; 
		pObject = pObject->next_sibling())
	{
		rapidxml::xml_attribute<> *pAttribute = pObject->first_attribute("id");

		if (nullptr == pAttribute)
		{
			std::cerr << "Object id missing" << std::endl;
			return false;
		}

		std::string id = std::string(pAttribute->value());

		rapidxml::xml_node<> *pShader = pObject->first_node("shader");
		if (nullptr == pShader)
		{
			std::cerr << "Shader id missing" << std::endl;
			return false;
		}

		std::string shaderID = pShader->value();

		rapidxml::xml_node<> *pType = pObject->first_node("type");
		if (nullptr == pType)
		{
			std::cerr << "Object type missing" << std::endl;
			return false;
		}

		std::string type = pType->value();
		ObjectType ot = OT_NORMAL;

		if ("normal" == type) 
		{
			ot = OT_NORMAL;
		}
		else if ("skybox" == type) 
		{
			ot = OT_SKYBOX;
		}
		else if ("terrain" == type) 
		{
			ot = OT_TERRAIN;
		}
		else if ("animated" == type) 
		{
			ot = OT_ANIMATED;
		}
		else if ("player" == type)
		{
			ot = OT_PLAYER;
		}
		else if ("batch" == type)
		{
			ot = OT_BATCH;
		}

		bool depthTest = true;
		rapidxml::xml_node<> *pDepthTest = pObject->first_node("depthTest");
		if (nullptr != pDepthTest) 
		{
			depthTest = pDepthTest->value() == "true" ? true : false;
		}

		bool blend = true;
		rapidxml::xml_node<> *pBlend = pObject->first_node("blend");
		if (nullptr != pBlend) 
		{
			blend = pBlend->value() == "true" ? true : false;
		}

		std::string name = "";
		rapidxml::xml_node<> *pName = pObject->first_node("name");
		if (nullptr != pName)
		{
			name = pName->value();
		}

		rapidxml::xml_node<> *pTextures = pObject->first_node("textures");
		std::vector<std::string> texture_ids;
		if (nullptr != pTextures)
		{
			for (rapidxml::xml_node<> *pTexture = pTextures->first_node("texture");
				pTexture; 
				pTexture = pTexture->next_sibling())
			{
				rapidxml::xml_attribute<> *pAttribute = pTexture->first_attribute("id");

				if (nullptr == pAttribute) 
				{
					std::cerr << "Texture id missing" << std::endl;
					return false;
				}

				texture_ids.push_back(pAttribute->value());
			}
		}

		rapidxml::xml_node<> *pObjLights = pObject->first_node("lights");
		std::vector<std::string> light_ids;
		if (nullptr != pObjLights) 
		{
			for (rapidxml::xml_node<> *pObjLight = pObjLights->first_node("light");
				pObjLight;
				pObjLight = pObjLight->next_sibling())
			{
				light_ids.push_back(pObjLight->value());
			}
		}

		glm::vec3 pos;
		rapidxml::xml_node<> *pPosition = pObject->first_node("position");

		if (nullptr == pPosition) 
		{
			pos = glm::vec3(0.f, 0.f, 0.f);
		}
		else 
		{
			pos.x = std::stof(pPosition->first_node("x")->value());
			pos.y = std::stof(pPosition->first_node("y")->value());
			pos.z = std::stof(pPosition->first_node("z")->value());
		}

		glm::vec3 rot;
		rapidxml::xml_node<> *pRotation = pObject->first_node("rotation");

		if (nullptr == pRotation)
		{
			pos = glm::vec3(0.f, 0.f, 0.f);
		}
		else 
		{
			rot.x = std::stof(pRotation->first_node("x")->value());
			rot.y = std::stof(pRotation->first_node("y")->value());
			rot.z = std::stof(pRotation->first_node("z")->value());
		}

		glm::vec3 scale;
		rapidxml::xml_node<> *pScale = pObject->first_node("scale");

		if (nullptr == pScale)
		{
			pos = glm::vec3(0.f, 0.f, 0.f);
		}
		else 
		{
			scale.x = std::stof(pScale->first_node("x")->value());
			scale.y = std::stof(pScale->first_node("y")->value());
			scale.z = std::stof(pScale->first_node("z")->value());
		}

		glm::vec3 color;
		rapidxml::xml_node<> *pColor = pObject->first_node("color");

		if (nullptr == pColor)
		{
			color = glm::vec3(0.f, 0.f, 0.f);
		}
		else 
		{
			color.x = std::stof(pColor->first_node("r")->value());
			color.y = std::stof(pColor->first_node("g")->value());
			color.z = std::stof(pColor->first_node("b")->value());
		}

		float selfRotateSpeed;
		rapidxml::xml_node<> *pSelfRotateSpeed = pObject->first_node("selfRotateSpeed");

		if (nullptr == pSelfRotateSpeed) 
		{
			selfRotateSpeed = 0.1f;
		}
		else
		{
			selfRotateSpeed = std::stof(pSelfRotateSpeed->value());
		}

		rapidxml::xml_node<> *pWired = pObject->first_node("wired");
		bool wired = pWired ? true : false;

		SceneObject *object = nullptr;

		switch (ot)
		{
		case OT_ANIMATED:
		{
			float displacement = 0.5f;
			rapidxml::xml_node<> *pDisplacementMax = pObject->first_node("displacement_max");
			if (nullptr != pDisplacementMax)
			{
				displacement = std::stof(pDisplacementMax->value());
			}

			//AnimatedObject *ao = new AnimatedObject(pos, rot, scale, depthTest, name, displacement);

			//object = ao;

			break;
		}
		case OT_TERRAIN:
		{
			uint32_t blockSize = 4; // default
			rapidxml::xml_node<> *pBlockSize = pObject->first_node("blockSize");
			if (nullptr != pBlockSize)
			{
				blockSize = std::stoi(pBlockSize->value());
			}

			uint32_t cellSize = 1; // default
			rapidxml::xml_node<> *pCellSize = pObject->first_node("cellSize");
			if (nullptr != pCellSize) 
			{
				cellSize = std::stoi(pCellSize->value());
			}

			float offsetY = 0.f; // default
			rapidxml::xml_node<> *pOffsetY = pObject->first_node("offsetY");
			if (nullptr != pOffsetY)
			{
				offsetY = std::stof(pOffsetY->value());
			}

			glm::vec3 heights;
			rapidxml::xml_node<> *pHeights = pObject->first_node("heights");

			if (nullptr == pHeights) 
			{
				heights = glm::vec3(0.f, 0.f, 0.f);
			}
			else
			{
				heights.x = std::stof(pHeights->first_node("r")->value());
				heights.y = std::stof(pHeights->first_node("g")->value());
				heights.z = std::stof(pHeights->first_node("b")->value());
			}

			Terrain *t = new Terrain(pos, rot, scale, heights, depthTest, name);
			t->SetBlockSize(blockSize);
			t->SetCellSize(cellSize);
			t->SetOffsetY(offsetY);

			object = t;
			break;
		}
		case OT_SKYBOX:
		{
			float offsetY = 0.f; // default
			rapidxml::xml_node<> *pOffsetY = pObject->first_node("offsetY");
			if (nullptr != pOffsetY) 
			{
				offsetY = std::stof(pOffsetY->value());
			}

			float sz = 0.f; // default
			rapidxml::xml_node<> *pSz = pObject->first_node("size");
			if (nullptr != pSz) 
			{
				sz = std::stof(pSz->value());
			}

			SkyBox *sb = new SkyBox(pos, rot, scale, name, offsetY, sz);

			object = sb;
			break;
		}
		case OT_NORMAL:
		{
			object = new SceneObject(pos, rot, scale, depthTest, name);
			break;
		}
		case OT_PLAYER:
		{
			object = new Player(pos, rot, scale, depthTest, name);
			break;
		}
		case OT_BATCH:
		{
			int count = 0; // default
			rapidxml::xml_node<> *pBatchCount = pObject->first_node("count");
			if (nullptr != pBatchCount)
			{
				count = std::stoi(pBatchCount->value());
			}

			object = new SceneObjectBatch(rot, scale, count, depthTest, name);
			break;
		}
		default:
			break;
		}

		if (OT_TERRAIN != ot) 
		{
			rapidxml::xml_node<> *pModel = pObject->first_node("model");
			if (nullptr == pModel)
			{
				std::cerr << "Model id missing" << std::endl;
				return false;
			}

			std::string modelID = pModel->value();
			object->SetModel(ResourceManager::GetInstance()->LoadModel(modelID));
		}

		object->SetBlend(blend);
		object->SetShader(ResourceManager::GetInstance()->LoadShader(shaderID));
		for (std::string texID : texture_ids)
		{
			object->AddTexture(ResourceManager::GetInstance()->LoadTexture(texID));
		}

		for (std::string lightID : light_ids) 
		{
			object->AddLightID(lightID);
		}

		// Trajectory
		rapidxml::xml_node<> *pTrajectory = pObject->first_node("trajectory");
		if (nullptr != pTrajectory) 
		{
			// default values
			uint32_t iter_count = 1;
			float traj_speed = 1.f;
			std::string type = "";
			std::string direction = "";

			// get actual values
			rapidxml::xml_attribute<> *pTjType = pTrajectory->first_attribute("type");
			rapidxml::xml_attribute<> *pTjIterCount = pTrajectory->first_attribute("iteration-count");
			rapidxml::xml_attribute<> *pTjDirection = pTrajectory->first_attribute("direction");
			rapidxml::xml_attribute<> *pTjSpeed = pTrajectory->first_attribute("speed");

			if (nullptr != pTjType) 
			{
				type = pTjType->value();
			}
			if (nullptr != pTjIterCount)
			{
				std::string iter_value = pTjIterCount->value();
				if ("infinite" == iter_value) 
				{
					iter_count = -1;
				}
				else 
				{
					iter_count = std::stoi(iter_value);
				}
			}
			if (nullptr != pTjDirection)
			{
				direction = pTjDirection->value();
			}
			if (nullptr != pTjSpeed)
			{
				traj_speed = std::stof(pTjSpeed->value());
			}

			//Trajectory *tj = new Trajectory(iter_count, traj_speed, Trajectory::GetDirectionTypeFromString(direction),
			//	Trajectory::GetTrajectoryTypeFromString(type));
			//
			//if ("circle" == type) {
			//	float radius;
			//	glm::vec3 angles = glm::vec3(0.f, 0.f, 0.f);
			//	rapidxml::xml_node<> *pTrajectoryRadius = pTrajectory->first_node("radius");
			//	rapidxml::xml_node<> *pTrajectoryRotationPlane = pTrajectory->first_node("rotationPlane");
			//
			//	radius = (nullptr != pTrajectoryRadius) ? std::stof(pTrajectoryRadius->value()) : 1.f;
			//
			//	if (nullptr != pTrajectoryRotationPlane) {
			//		angles.x = std::stof(pTrajectoryRotationPlane->first_node("x")->value());
			//		angles.y = std::stof(pTrajectoryRotationPlane->first_node("y")->value());
			//		angles.z = std::stof(pTrajectoryRotationPlane->first_node("z")->value());
			//	}
			//
			//	tj->SetCircleTrajectoryParams(Trajectory::GetTrajectoryTypeFromString(type), radius, angles);
			//}
			//else {
			//	tj->AddPoint(pos);
			//
			//	rapidxml::xml_node<> *pTrajectoryPoints = pTrajectory->first_node("points");
			//
			//	if (nullptr != pTrajectoryPoints) {
			//		for (rapidxml::xml_node<> *pTjPoint = pTrajectoryPoints->first_node("point"); pTjPoint; pTjPoint = pTjPoint->next_sibling()) {
			//			glm::vec3 point;
			//			point.x = std::stof(pTjPoint->first_node("x")->value());
			//			point.y = std::stof(pTjPoint->first_node("y")->value());
			//			point.z = std::stof(pTjPoint->first_node("z")->value());
			//
			//			tj->AddPoint(point);
			//		}
			//	}
			//}
			//
			//object->SetTrajectory(tj);
		}

		object->SetWired(wired);
		m_objects[id] = object;
	}

	//debug settings
	//rapidxml::xml_node<> *pDebug = pRoot->first_node("debugSettings");
	//if (nullptr != pDebug) {
	//	rapidxml::xml_node<> *pDebugShader = pDebug->first_node("primitiveShader");
	//
	//	if (nullptr != pDebugShader) {
	//		m_debug_settings.shader_prim_id = pDebugShader->value();
	//		ResourceManager::GetInstance()->LoadShader(m_debug_settings.shader_prim_id);
	//	}
	//
	//	pDebugShader = pDebug->first_node("axisShader");
	//
	//	if (nullptr != pDebugShader) {
	//		m_debug_settings.shader_axis_id = pDebugShader->value();
	//		ResourceManager::GetInstance()->LoadShader(m_debug_settings.shader_axis_id);
	//	}
	//}
	//
	//// sounds
	//rapidxml::xml_node<> *pSounds = pRoot->first_node("sounds");
	//if (nullptr != pSounds) {
	//	rapidxml::xml_node<> *pSound = pSounds->first_node("sound");
	//	for (rapidxml::xml_node<> *pSound = pSounds->first_node("sound"); pSound; pSound = pSound->next_sibling()) {
	//		rapidxml::xml_attribute<> *pAttribute = pSound->first_attribute("id");
	//		std::string sound_id = pAttribute->value();
	//		m_sounds[sound_id] = ResourceManager::GetInstance()->LoadSound(sound_id);
	//
	//		rapidxml::xml_node<> *pSoundRepeat = pSound->first_node("repeat");
	//		if (nullptr != pSoundRepeat && pSoundRepeat->value() == "true") {
	//			m_sounds[sound_id]->SetRepeat(true);
	//		}
	//	}
	//}


	delete string;

	InitScreenQuad();
	InitFBO();
	m_blur_shader = ResourceManager::GetInstance()->LoadShader("7");
	m_sharpen_shader = ResourceManager::GetInstance()->LoadShader("8");
	m_grayscale_shader = ResourceManager::GetInstance()->LoadShader("9");
	m_threshold_shader = ResourceManager::GetInstance()->LoadShader("10");
	m_combine_tex_shader = ResourceManager::GetInstance()->LoadShader("11");

	// Init objects
	m_objects["terrain"]->Init(); // force terrain to be initialized first
	for (auto model : m_objects) 
	{
		model.second->Init();
	}

	for (auto cam : m_cameras)
	{
		cam.second->Init();
	}

	glClearColor(m_background_color.x, m_background_color.y, m_background_color.z, 1.f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//m_target_spawner = new TargetSpawner("1", "4", "3");

	return true;
}

void SceneManager::Update(float dt)
{
	if (true == m_lbutton_pressed)
	{

	}

	for (auto & obj : m_objects) 
	{
		obj.second->Update(dt);
	}

	m_cameras[m_active_camera]->Update(dt);

	// Collision detection
	for (auto obj = m_objects.begin(); obj != m_objects.end(); ++obj)
	{
		for (auto obj2 = std::next(obj); obj2 != m_objects.end(); ++obj2) 
		{
			if ("SkyBox" == obj2->second->GetName())
			{
				continue;
			}

			if (true == obj->second->Collides(obj2->second)) 
			{
				//std::cout << obj->second->GetName() << " collides with " << obj2->second->GetName() << std::endl;
			}
		}

		if (true == obj->second->Contains(m_cameras[m_active_camera]->GetPosition())) 
		{
			std::cout << "Camera collides with " << obj->second->GetName() << std::endl;
		}
	}

	//m_shadow_map->Update();
	//m_target_spawner->Update();

	while (EventManager::Poll()) {}
}

void SceneManager::Draw(bool debug)
{
	/*
	if (false == debug) {
		glBindFramebuffer(GL_FRAMEBUFFER, m_shadow_map->GetFBO());

		// check for framebuffer complete
		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status == GL_FRAMEBUFFER_COMPLETE) {
			glClear(GL_DEPTH_BUFFER_BIT);

			for (auto & obj : m_objects) {
				if (obj.second->GetName() == "SkyBox" || obj.second->GetName() == "teren")
					continue;

				obj.second->Draw(SceneObject::SHADOW_MAP);
			}
		}
	}

	// shadowmap on screen
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status == GL_FRAMEBUFFER_COMPLETE) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(m_grayscale_shader->GetProgramID());
		glBindBuffer(GL_ARRAY_BUFFER, m_screen_vbo);

		// Last texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_shadow_map->GetTexture());
		m_grayscale_shader->SendUniform(ShaderStrings::TEXTURE_UNIFORMS[0], 0);

		// Scene texture
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_screen_textures[0]);
		m_grayscale_shader->SendUniform(ShaderStrings::TEXTURE_UNIFORMS[1], 1);

		m_grayscale_shader->SendAttribute(ShaderStrings::POSITION_ATTRIBUTE, 3, 0, 0);
		m_grayscale_shader->SendUniform(ShaderStrings::FRAGMENT_OFFSET_X_UNIFORM, 1.f / Window::WIDTH);
		m_grayscale_shader->SendUniform(ShaderStrings::FRAGMENT_OFFSET_Y_UNIFORM, 1.f / Window::HEIGHT);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	*/
	///*
	//glBindFramebuffer(GL_FRAMEBUFFER, m_fbos[0]);

	// check for framebuffer complete
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status == GL_FRAMEBUFFER_COMPLETE) 
	{
		glClearColor(m_background_color.x, m_background_color.y, m_background_color.z, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (auto & obj : m_objects) 
		{
			if (true == debug) 
			{
				DrawDebug();
				obj.second->Draw(SceneObject::DEBUG);
			}
			else 
			{
				obj.second->Draw(SceneObject::NORMAL);
			}
		}
	}
}

void SceneManager::CleanUp()
{
	if (nullptr != m_instance) {
		delete m_instance;
		m_instance = nullptr;
	}
}

void SceneManager::InitScreenQuad()
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

	glGenBuffers(1, &m_screen_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_screen_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vbo_data), quad_vbo_data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void SceneManager::InitFBO()
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
	glGenRenderbuffers(1, &m_depth_render_bos);
	glBindRenderbuffer(GL_RENDERBUFFER, m_depth_render_bos);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16,
		Window::WIDTH, Window::HEIGHT);

	// Init screen texture
	glGenTextures(MAX_FBOS, m_screen_textures);
	for (size_t i = 0; i < MAX_FBOS; ++i)
	{
		glBindTexture(GL_TEXTURE_2D, m_screen_textures[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Window::WIDTH, Window::HEIGHT,
			0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, NULL);
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
			GL_RENDERBUFFER, m_depth_render_bos);
	}

	//glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
