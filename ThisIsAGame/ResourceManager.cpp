
#include <glm/glm.hpp>

#include "ResourceManager.h"

ResourceManager *ResourceManager::m_instance = nullptr;

ResourceManager::ResourceManager()
//	: m_sound_system(nullptr)
{
}

ResourceManager::~ResourceManager()
{
	for (auto it = m_model_map.begin(); it != m_model_map.end();) 
	{
		delete it->second;
		it->second = nullptr;
		it = m_model_map.erase(it);
	}

	for (auto it = m_shader_map.begin(); it != m_shader_map.end();)
	{
		delete it->second;
		it->second = nullptr;
		it = m_shader_map.erase(it);
	}

	for (auto it = m_texture_map.begin(); it != m_texture_map.end();) 
	{
		delete it->second;
		it->second = nullptr;
		it = m_texture_map.erase(it);
	}

	//for (auto it = m_sound_map.begin(); it != m_sound_map.end();) {
	//	delete it->second;
	//	it->second = nullptr;
	//	it = m_sound_map.erase(it);
	//}

	for (auto it = m_model_resource_map.begin(); it != m_model_resource_map.end();) 
	{
		delete it->second;
		it->second = nullptr;
		it = m_model_resource_map.erase(it);
	}

	for (auto it = m_shader_resource_map.begin(); it != m_shader_resource_map.end();)
	{
		delete it->second;
		it->second = nullptr;
		it = m_shader_resource_map.erase(it);
	}

	for (auto it = m_texture_resource_map.begin(); it != m_texture_resource_map.end();) 
	{
		delete it->second;
		it->second = nullptr;
		it = m_texture_resource_map.erase(it);
	}

	for (auto it = m_sound_resource_map.begin(); it != m_sound_resource_map.end();) {
		delete it->second;
		it->second = nullptr;
		it = m_sound_resource_map.erase(it);
	}
}

ResourceManager * ResourceManager::GetInstance()
{
	if (nullptr == m_instance) 
	{
		m_instance = new ResourceManager;
	}

	return m_instance;
}

Model * ResourceManager::GetModel(std::string id)
{
	if (m_model_map.find(id) == m_model_map.end()) 
	{
		return nullptr;
	}

	return m_model_map[id];
}

Texture * ResourceManager::GetTexture(std::string id)
{
	if (m_texture_map.find(id) == m_texture_map.end())
	{
		return nullptr;
	}

	return m_texture_map[id];
}

Shader * ResourceManager::GetShader(std::string id)
{
	if (m_shader_map.find(id) == m_shader_map.end())
	{
		return nullptr;
	}

	return m_shader_map[id];
}

bool ResourceManager::Init(std::string filepath)
{
	FILE *f = fopen(filepath.c_str(), "r");

	if (NULL == f)
		return false;

	fseek(f, 0, SEEK_END);
	long fsize = ftell(f);
	fseek(f, 0, SEEK_SET);

	char *string = new char[fsize + 1];
	int sz = fread(string, fsize, 1, f);
	fclose(f);

	string[fsize] = '\0';

	rapidxml::xml_document<> doc;
	doc.parse<0>(string);

	rapidxml::xml_node<> *pRoot = doc.first_node();

	// models
	rapidxml::xml_node<> *pModels = pRoot->first_node("models");
	for (rapidxml::xml_node<> *pNode = pModels->first_node("model");
		pNode;
		pNode = pNode->next_sibling())
	{
		ModelResource *mr = new ModelResource;
		// Get attributes -- only one field for now, no need to iterate
		//for (rapidxml::xml_attribute<> *pAttribute = pNode->first_attribute(); pAttribute; pAttribute = pAttribute->next_attribute()) {
		rapidxml::xml_attribute<> *pAttribute = pNode->first_attribute();
		mr->id = std::string(pAttribute->value());
		//}

		// Get child nodes -- only one field for now, no need to iterate
		//for (rapidxml::xml_node<> *pField = pNode->first_node(); pField; pField = pField->next_sibling()) {
		rapidxml::xml_node<> *pField = pNode->first_node();
		mr->model_path = std::string(pField->value());
		//}

		m_model_resource_map[mr->id] = mr;
	}

	// shaders
	rapidxml::xml_node<> *pShader = pRoot->first_node("shaders");
	for (rapidxml::xml_node<> *pNode = pShader->first_node("shader");
		pNode;
		pNode = pNode->next_sibling())
	{
		ShaderResource *sr = new ShaderResource;

		rapidxml::xml_attribute<> *pAttribute = pNode->first_attribute();
		sr->id = std::string(pAttribute->value());

		rapidxml::xml_node<> *pVS = pNode->first_node("vs");
		sr->vs_path = std::string(pVS->value());
		
		rapidxml::xml_node<> *pFS = pNode->first_node("fs");
		sr->fs_path = std::string(pFS->value());

		m_shader_resource_map[sr->id] = sr;
	}

	// textures
	rapidxml::xml_node<> *pTexture = pRoot->first_node("textures");
	for (rapidxml::xml_node<> *pNode = pTexture->first_node("texture");
		pNode;
		pNode = pNode->next_sibling())
	{
		TextureResource *tr = new TextureResource;

		for (rapidxml::xml_attribute<> *pAttribute = pNode->first_attribute(); 
			pAttribute; 
			pAttribute = pAttribute->next_attribute()) 
		{
			std::string name = pAttribute->name();

			if ("id" == name)
			{
				tr->id = std::string(pAttribute->value());
			}
			else if ("type" == name) 
			{
				std::string value = std::string(pAttribute->value());

				if ("2d" == value)
				{
					tr->type = GL_TEXTURE_2D;
				}
				else if ("cube" == value) 
				{
					tr->type = GL_TEXTURE_CUBE_MAP;
				}
				else 
				{
					std::cerr << "Not a 2d texture: " << value << std::endl;
					return false;
				}
			}
		}
		
		rapidxml::xml_node<> *pPath = pNode->first_node("path");
		tr->texture_path = std::string(pPath->value());

		rapidxml::xml_node<> *pMinFilter = pNode->first_node("min_filter");
		std::string minFilterValue = std::string(pMinFilter->value());

		if ("LINEAR" == minFilterValue)
		{
			tr->min_filter = GL_LINEAR;
		}
		else if ("MIPMAP_LINEAR" == minFilterValue)
		{
			tr->min_filter = GL_LINEAR_MIPMAP_LINEAR;
		}
		else
		{
			std::cerr << "Not linear min filter: " << minFilterValue << std::endl;
			return false;
		}

		rapidxml::xml_node<> *pMagFilter = pNode->first_node("mag_filter");
		std::string magFilterValue = std::string(pMagFilter->value());
		
		if ("LINEAR" == magFilterValue)
		{
			tr->mag_filter = GL_LINEAR;
		}
		else 
		{
			std::cerr << "Not linear mag filter: " << magFilterValue << std::endl;
			return false;
		}

		rapidxml::xml_node<> *pWrap_s = pNode->first_node("wrap_s");
		std::string wrap_sValue = std::string(pWrap_s->value());

		if ("CLAMP_TO_EDGE" == wrap_sValue)
		{
			tr->wrap_s = GL_CLAMP_TO_EDGE;
		}
		else if ("REPEAT" == wrap_sValue)
		{
			tr->wrap_s = GL_REPEAT;
		}
		else 
		{
			std::cerr << "Not CLAMP_TO_EDGE wrap_s filter: " << wrap_sValue << std::endl;
			return false;
		}

		rapidxml::xml_node<> *pWrap_t = pNode->first_node("wrap_t");
		std::string wrap_tValue = std::string(pWrap_t->value());

		if ("CLAMP_TO_EDGE" == wrap_tValue) 
		{
			tr->wrap_t = GL_CLAMP_TO_EDGE;
		}
		else if ("REPEAT" == wrap_tValue)
		{
			tr->wrap_t = GL_REPEAT;
		}
		else 
		{
			std::cerr << "Not CLAMP_TO_EDGE wrap_t filter: " << wrap_tValue << std::endl;
			return false;
		}

		m_texture_resource_map[tr->id] = tr;
	}

	// Sound
	//if (FMOD::System_Create(&m_sound_system) != FMOD_OK) {
	//	std::cerr << "Could not load fmod\n";
	//}
	//else {
	//	m_sound_system->init(36, FMOD_INIT_NORMAL, NULL);
	//}

	rapidxml::xml_node<> *pSounds = pRoot->first_node("sounds");
	for (rapidxml::xml_node<> *pNode = pSounds->first_node("sound");
		pNode;
		pNode = pNode->next_sibling()) 
	{
		SoundResource *sr = new SoundResource;

		rapidxml::xml_attribute<> *pAttribute = pNode->first_attribute();
		sr->id = std::string(pAttribute->value());
		
		rapidxml::xml_node<> *pField = pNode->first_node();
		sr->sound_path = std::string(pField->value());
		
		m_sound_resource_map[sr->id] = sr;
	}


	delete string;

	return true;
}

Model *ResourceManager::LoadModel(std::string id)
{
	if (m_model_resource_map.find(id) == m_model_resource_map.end())
	{
		return nullptr;
	}

	if (m_model_map.find(id) == m_model_map.end()) 
	{
		m_model_map[id] = new Model(m_model_resource_map[id]);
	}

	m_model_map[id]->Load();

	return m_model_map[id];
}

Shader *ResourceManager::LoadShader(std::string id)
{
	if (m_shader_resource_map.find(id) == m_shader_resource_map.end()) 
	{
		return nullptr;
	}

	if (m_shader_map.find(id) == m_shader_map.end())
	{
		m_shader_map[id] = new Shader(m_shader_resource_map[id]);
	}

	m_shader_map[id]->Load();

	return m_shader_map[id];
}

Texture *ResourceManager::LoadTexture(std::string id)
{
	if (m_texture_resource_map.find(id) == m_texture_resource_map.end())
	{
		return nullptr;
	}

	if (m_texture_map.find(id) == m_texture_map.end()) 
	{
		m_texture_map[id] = new Texture(m_texture_resource_map[id]);
	}

	m_texture_map[id]->Load();

	return m_texture_map[id];
}

//Sound * ResourceManager::LoadSound(std::string id)
//{
//	if (m_sound_resource_map.find(id) == m_sound_resource_map.end()) {
//		return nullptr;
//	}
//
//	if (m_sound_map.find(id) == m_sound_map.end()) {
//		m_sound_map[id] = new Sound(m_sound_resource_map[id], m_sound_system);
//	}
//
//	m_sound_map[id]->Load();
//
//	return m_sound_map[id];
//}

void ResourceManager::CleanUp()
{
	if (nullptr != m_instance) 
	{
		delete m_instance;
		m_instance = nullptr;
	}
}
