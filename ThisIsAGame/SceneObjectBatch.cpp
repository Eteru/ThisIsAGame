
#include <glm/gtc/matrix_inverse.hpp>

#include "SceneObjectBatch.h"
#include "Strings.h"
#include "SceneManager.h"
#include "Terrain.h"

SceneObjectBatch::SceneObjectBatch(glm::vec3 rotation, glm::vec3 scale, uint32_t count, bool depth_test, std::string id)
	: SceneObject(glm::vec3(0.f), rotation, scale, depth_test, id),
	m_object_count(count)
{
	m_transforms.resize(count);
}

SceneObjectBatch::~SceneObjectBatch()
{
}

void SceneObjectBatch::Init()
{
	if (true == m_init)
	{
		return;
	}

	if (nullptr == m_model)
	{
		throw std::runtime_error(std::string("Model is nullptr: ") + m_id);
	}

	if (nullptr == m_shader)
	{
		throw std::runtime_error(std::string("Shader is nullptr: ") + m_id);
	}

	m_model->Load();
	m_shader->Load();

	for (Texture * tex : m_textures)
	{
		if (nullptr == tex)
		{
			throw std::runtime_error(std::string("Texture is nullptr: ") + m_id);
		}

		tex->Load();
	}

	glBindVertexArray(m_model->GetVAO());

	glBindBuffer(GL_ARRAY_BUFFER, m_model->GetVBO(Model::POSITION_VB));
	m_shader->SendAttribute(ShaderStrings::POSITION_ATTRIBUTE, 3, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_model->GetVBO(Model::UV_VB));
	m_shader->SendAttribute(ShaderStrings::UV_ATTRIBUTE, 3, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_model->GetVBO(Model::NORMAL_VB));
	m_shader->SendAttribute(ShaderStrings::NORMAL_ATTRIBUTE, 3, 0, 0);

	glBindVertexArray(0);

	Terrain *t = dynamic_cast<Terrain *>
		(SceneManager::GetInstance()->GetSceneObject("terrain"));

	uint32_t sz = t->GetBlockSize();

	for (auto & transform : m_transforms)
	{
		float x = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / sz)) - sz * 0.5;
		float z = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / sz)) - sz * 0.5;

		transform.position = glm::vec3(x, t->GetTerrainHeight(x, z), z);
		transform.rotation = m_transform.rotation;
		transform.scale = m_transform.scale;

		transform.up = glm::vec3(0.f, 1.f, 0.f);
		transform.front = glm::vec3(0.f, 0.f, -1.f);
	}

	m_init = true;
}

void SceneObjectBatch::Update(float dt)
{
	for (auto & transform : m_transforms)
	{
		transform.right = glm::normalize(glm::cross(transform.front, WORLD_UP));
		transform.up = glm::normalize(glm::cross(transform.right, transform.front));
	}
}

void SceneObjectBatch::Draw(DrawType type)
{
	Shader *s = m_shader;
	//if (SceneObject::SHADOW_MAP == type) {
	//	s = ResourceManager::GetInstance()->LoadShader("12");
	//}
	//else {
	//	s = m_shader;
	//}

	// bind the program
	glUseProgram(m_shader->GetProgramID());
	// bind the VAO
	glBindVertexArray(m_model->GetVAO());
	// bind all shader properties

	Camera *cam = SceneManager::GetInstance()->GetActiveCamera();

	for (size_t i = 0; i < m_textures.size(); ++i) {
		glActiveTexture(static_cast<GLenum>(GL_TEXTURE0 + i));
		glBindTexture(m_textures[i]->GetTextureType(), m_textures[i]->GetID());

		s->SendUniform(ShaderStrings::TEXTURE_UNIFORMS[i], static_cast<int>(i));
	}

	glm::vec3 pos = cam->GetPosition();
	s->SendUniform(ShaderStrings::CAMERA_POSITION_UNIFORM, pos);

	{ // Fog
		const Fog fog = SceneManager::GetInstance()->GetFog();
		glm::vec3 fog_color = fog.GetColor();

		//s->SendUniform(ShaderStrings::FOG_ALPHA_UNIFORM, fog.ComputeAlpha(distance));
		s->SendUniform(ShaderStrings::FOG_COLOR_UNIFORM, fog_color);
	}

	{ // Lights
		const std::map<std::string, LightSource *> lights = SceneManager::GetInstance()->GetLights();
		AmbientalLight amb_light = SceneManager::GetInstance()->GetAmbientalLight();
		glm::vec3 amb_light_color = amb_light.GetColor();

		s->SendUniform(ShaderStrings::LIGHT_AMBIENTAL_COLOR_UNIFORM, amb_light_color);
		s->SendUniform(ShaderStrings::LIGHT_AMBIENTAL_RATIO_UNIFORM, amb_light.GetRatio());

		s->SendUniform(ShaderStrings::LIGHT_COUNT_UNIFORM, static_cast<int>(lights.size()));

		uint16_t count = 0;
		for (auto & ls : lights) {
			if (nullptr != ls.second) {
				LightSource::LightType type = ls.second->GetType();
				glm::vec3 vec_l_type;

				switch (type)
				{
				case LightSource::POINT_LIGHT:
					vec_l_type = glm::vec3(1, 0, 0);
					break;
				case LightSource::DIRECTIONAL_LIGHT:
					vec_l_type = glm::vec3(0, 1, 0);
					break;
				case LightSource::SPOT_LIGHT:
					vec_l_type = glm::vec3(0, 0, 1);
					break;
				default:
					vec_l_type = glm::vec3(1, 0, 0);
					break;
				}

				s->SendUniform(s->CreateStructArrayName(ShaderStrings::LIGHT_STRUCT_NAME_UNIFORM, ShaderStrings::LIGHT_STRUCT_TYPE_UNIFORM, count),
					vec_l_type);

				glm::vec3 pos = (LightSource::SPOT_LIGHT == type) ? cam->GetPosition() : ls.second->GetPosition();
				s->SendUniform(s->CreateStructArrayName(ShaderStrings::LIGHT_STRUCT_NAME_UNIFORM, ShaderStrings::LIGHT_STRUCT_POSITION_UNIFORM, count),
					pos);

				s->SendUniform(s->CreateStructArrayName(ShaderStrings::LIGHT_STRUCT_NAME_UNIFORM, ShaderStrings::LIGHT_STRUCT_ATTENUATION_UNIFORM, count),
					0.f);

				glm::vec3 dir = (LightSource::SPOT_LIGHT == type) ? (cam->GetFront() - cam->GetPosition()) : ls.second->GetDirection();

				s->SendUniform(s->CreateStructArrayName(ShaderStrings::LIGHT_STRUCT_NAME_UNIFORM, ShaderStrings::LIGHT_STRUCT_DIRECTION_UNIFORM, count),
					dir);

				glm::vec3 spec_light = ls.second->GetSpecularLight();
				s->SendUniform(s->CreateStructArrayName(ShaderStrings::LIGHT_STRUCT_NAME_UNIFORM, ShaderStrings::LIGHT_STRUCT_SPECULAR_UNIFORM, count),
					spec_light);

				s->SendUniform(s->CreateStructArrayName(ShaderStrings::LIGHT_STRUCT_NAME_UNIFORM, ShaderStrings::LIGHT_STRUCT_SPECULAR_RATIO_UNIFORM, count),
					ls.second->GetSpecularCoefficient());

				s->SendUniform(s->CreateStructArrayName(ShaderStrings::LIGHT_STRUCT_NAME_UNIFORM, ShaderStrings::LIGHT_STRUCT_SHININESS_UNIFORM, count),
					ls.second->GetShininess());

				glm::vec3 diff_light = ls.second->GetDiffuseLight();
				s->SendUniform(s->CreateStructArrayName(ShaderStrings::LIGHT_STRUCT_NAME_UNIFORM, ShaderStrings::LIGHT_STRUCT_DIFFUSE_UNIFORM, count),
					diff_light);

				s->SendUniform(s->CreateStructArrayName(ShaderStrings::LIGHT_STRUCT_NAME_UNIFORM, ShaderStrings::LIGHT_STRUCT_DIFFUSE_RATIO_UNIFORM, count),
					ls.second->GetDiffuseCoefficient());

				s->SendUniform(s->CreateStructArrayName(ShaderStrings::LIGHT_STRUCT_NAME_UNIFORM, ShaderStrings::LIGHT_STRUCT_CONE_ANGLE_UNIFORM, count),
					ls.second->GetSpotAngle());
			}
			++count;
		}
	}

	for (auto const & transform : m_transforms)
	{
		// bind model for each transform
		m_M = glm::mat4(1.f);
		m_M = glm::translate(m_M, transform.position);
		m_M = glm::scale(m_M, transform.scale);
		m_M = glm::rotate(m_M, glm::radians(transform.rotation.x), glm::vec3(1.f, 0.f, 0.f));
		m_M = glm::rotate(m_M, glm::radians(transform.rotation.y), glm::vec3(0.f, 1.f, 0.f));
		m_M = glm::rotate(m_M, glm::radians(transform.rotation.z), glm::vec3(0.f, 0.f, 1.f));

		s->SendUniform(ShaderStrings::MODEL_UNIFORM, m_M);

		glm::mat4 NM = glm::inverseTranspose(m_M);
		s->SendUniform(ShaderStrings::NORMAL_MODEL_UNIFORM, NM);

		glm::mat4 MV = cam->GetView() * m_M;
		s->SendUniform(ShaderStrings::VIEW_MODEL_UNIFORM, MV);

		glm::mat4 MVP = cam->GetProjection() * cam->GetView() * m_M;
		s->SendUniform(ShaderStrings::MVP_UNIFORM, MVP);

		glDrawElements(GL_TRIANGLES, m_model->GetIndincesCount(), GL_UNSIGNED_INT, 0);
	}

	// unbind the VAO
	glBindVertexArray(0);

	// unbind the program
	glUseProgram(0);
}
