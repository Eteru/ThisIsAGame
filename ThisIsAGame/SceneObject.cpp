
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Vertex.h"
//#include "SceneManager.h"
#include "SceneObject.h"
#include "Strings.h"


SceneObject::SceneObject(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale, bool depth_test, std::string id)
	: m_position(pos), m_rotation(rot), m_scale(scale), m_depth_test(depth_test), m_id(id), m_is_wired(false),
	m_model(nullptr), m_shader(nullptr)//, m_trajectory(nullptr)
{
	if (true == depth_test)
	{
		glEnable(GL_DEPTH_TEST);
	}

	m_up = glm::vec3(0.f, 1.f, 0.f);
	m_target = glm::vec3(0.f, 0.f, -1.f);
}


SceneObject::~SceneObject()
{
	//if (nullptr != m_trajectory) {
	//	delete m_trajectory;
	//	m_trajectory = nullptr;
	//}
}

void SceneObject::SetWired(bool is_wired)
{
	m_is_wired = is_wired;
}

void SceneObject::SetBlend(bool use_blend)
{
	if (true == use_blend) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	else {
		glDisable(GL_BLEND);
	}
}

void SceneObject::SetModel(Model * model)
{
	m_model = model;
}

void SceneObject::SetShader(Shader * shader)
{
	m_shader = shader;
}

//void SceneObject::SetTrajectory(Trajectory * trajectory)
//{
//	m_trajectory = trajectory;
//}

void SceneObject::AddTexture(Texture * texture)
{
	m_textures.push_back(texture);
}

void SceneObject::AddLightID(std::string id)
{
	m_light_ids.push_back(id);
}

void SceneObject::Init()
{
}

void SceneObject::Update()
{
	//if (nullptr != m_trajectory) {
	//	m_trajectory->NextPosition(m_position, m_rotation, m_scale, m_M);
	//}
	//else {
	//	GeneralUpdate();
	//}
	GeneralUpdate();
}

void SceneObject::Draw(DrawType type)
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
	
	//Texture *sb = nullptr;// ResourceManager::GetInstance()->GetTexture("15");
	//if (nullptr != sb) {
	//	int tex_loc = static_cast<int>(m_textures.size());
	//	glActiveTexture(GL_TEXTURE0 + tex_loc);
	//	glBindTexture(sb->GetTextureType(), sb->GetID());
	//
	//	s->SendUniform(ShaderStrings::TEXTURE_CUBE_UNIFORM, tex_loc);
	//}

	SharedDrawElements(type);
	
	// draw the VAO
	//glDrawElements(GL_TRIANGLES, model.GetIndincesCount(), GL_UNSIGNED_INT, 0);
	
	// unbind the VAO
	glBindVertexArray(0);
	
	// unbind the program
	glUseProgram(0);
}

bool SceneObject::Collides(SceneObject * obj)
{
	return GetBB().Collides(obj->GetBB());
}

bool SceneObject::Contains(const glm::vec3 & point)
{
	return GetBB().Contains(point);
}

void SceneObject::SharedDrawElements(DrawType type)
{
	Shader *s = m_shader;
	//if (SceneObject::SHADOW_MAP == type) {
	//	s = ResourceManager::GetInstance()->LoadShader("12");
	//}
	//else {
	//	s = m_shader;
	//}

	//Camera *cam = SceneManager::GetInstance()->GetActiveCamera();

	for (size_t i = 0; i < m_textures.size(); ++i) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(m_textures[i]->GetTextureType(), m_textures[i]->GetID());
	
		s->SendUniform(ShaderStrings::TEXTURE_UNIFORMS[i], static_cast<int>(i));
	}

	// shadowmap
	//glActiveTexture(GL_TEXTURE0 + m_textures.size() + 1);
	//glBindTexture(GL_TEXTURE_2D, SceneManager::GetInstance()->GetShadowMap()->GetTexture());
	//s->SendUniform(ShaderStrings::TEXTURE_SHADOW_MAP_UNIFORM, static_cast<int>(m_textures.size() + 1));

	//s->SendAttribute(ShaderStrings::POSITION_ATTRIBUTE, 3, sizeof(Vertex), 0);
	//s->SendAttribute(ShaderStrings::COLOR_ATTRIBUTE, 3, sizeof(Vertex), sizeof(glm::vec3));
	//s->SendAttribute(ShaderStrings::NORMAL_ATTRIBUTE, 3, sizeof(Vertex), sizeof(glm::vec3) * 2);
	//s->SendAttribute(ShaderStrings::BINORM_ATTRIBUTE, 3, sizeof(Vertex), sizeof(glm::vec3) * 3);
	//s->SendAttribute(ShaderStrings::TANGENT_ATTRIBUTE, 3, sizeof(Vertex), sizeof(glm::vec3) * 4);
	//s->SendAttribute(ShaderStrings::UV_ATTRIBUTE, 2, sizeof(Vertex), sizeof(glm::vec3) * 5);
	//s->SendAttribute(ShaderStrings::UV_BLEND_ATTRIBUTE, 2, sizeof(Vertex), sizeof(glm::vec3) * 5 + sizeof(glm::vec2));
	
	s->SendUniform(ShaderStrings::MODEL_UNIFORM, m_M);
	//s->SendUniform(ShaderStrings::NORMAL_MODEL_UNIFORM, static_cast<glm::mat4>(glm::inverseTranspose(m_M)));
	//s->SendUniform(ShaderStrings::VIEW_MODEL_UNIFORM, m_M * cam->GetView());
	//s->SendUniform(ShaderStrings::MVP_UNIFORM, m_M * cam->GetView() * cam->GetProjection());
	//s->SendUniform(ShaderStrings::LIGHT_SPACE_UNIFORM, SceneManager::GetInstance()->GetShadowMap()->GetLightSpaceglm::mat4());
	//s->SendUniform(ShaderStrings::CAMERA_POSITION_UNIFORM, cam->GetPosition());

	//{ // Fog
	//	const Fog fog = SceneManager::GetInstance()->GetFog();
	//	float distance = m_position.Distance(cam->GetPosition());
	//
	//	s->SendUniform(ShaderStrings::FOG_ALPHA_UNIFORM, fog.ComputeAlpha(distance));
	//	s->SendUniform(ShaderStrings::FOG_COLOR_UNIFORM, fog.GetColor());
	//}
	//
	//{ // Lights
	//	const std::map<std::string, LightSource *> lights = SceneManager::GetInstance()->GetLights();
	//	const AmbientalLight amb_light = SceneManager::GetInstance()->GetAmbientalLight();
	//
	//	s->SendUniform(ShaderStrings::LIGHT_AMBIENTAL_COLOR_UNIFORM, amb_light.GetColor());
	//	s->SendUniform(ShaderStrings::LIGHT_AMBIENTAL_RATIO_UNIFORM, amb_light.GetRatio());
	//
	//	s->SendUniform(ShaderStrings::LIGHT_COUNT_UNIFORM, static_cast<int>(lights.size()));
	//
	//	uint16_t count = 0;
	//	for (auto & ls : lights) {
	//		if (nullptr != ls.second) {
	//			LightSource::LightType type = ls.second->GetType();
	//			glm::vec3 vec_l_type;
	//
	//			switch (type)
	//			{
	//			case LightSource::POINT_LIGHT:
	//				vec_l_type = glm::vec3(1, 0, 0);
	//				break;
	//			case LightSource::DIRECTIONAL_LIGHT:
	//				vec_l_type = glm::vec3(0, 1, 0);
	//				break;
	//			case LightSource::SPOT_LIGHT:
	//				vec_l_type = glm::vec3(0, 0, 1);
	//				break;
	//			default:
	//				vec_l_type = glm::vec3(1, 0, 0);
	//				break;
	//			}
	//
	//			s->SendUniform(s->CreateStructArrayName(ShaderStrings::LIGHT_STRUCT_NAME_UNIFORM, ShaderStrings::LIGHT_STRUCT_TYPE_UNIFORM, count),
	//				vec_l_type);
	//
	//			glm::vec3 pos = (LightSource::SPOT_LIGHT == type) ? cam->GetPosition() : ls.second->GetPosition();
	//			s->SendUniform(s->CreateStructArrayName(ShaderStrings::LIGHT_STRUCT_NAME_UNIFORM, ShaderStrings::LIGHT_STRUCT_POSITION_UNIFORM, count),
	//				pos);
	//			
	//			s->SendUniform(s->CreateStructArrayName(ShaderStrings::LIGHT_STRUCT_NAME_UNIFORM, ShaderStrings::LIGHT_STRUCT_ATTENUATION_UNIFORM, count),
	//				0.f);
	//
	//			glm::vec3 dir = (LightSource::SPOT_LIGHT == type) ? (cam->GetTarget() - cam->GetPosition()) : ls.second->GetDirection();
	//			
	//			s->SendUniform(s->CreateStructArrayName(ShaderStrings::LIGHT_STRUCT_NAME_UNIFORM, ShaderStrings::LIGHT_STRUCT_DIRECTION_UNIFORM, count),
	//				dir);
	//
	//			s->SendUniform(s->CreateStructArrayName(ShaderStrings::LIGHT_STRUCT_NAME_UNIFORM, ShaderStrings::LIGHT_STRUCT_SPECULAR_UNIFORM, count),
	//				ls.second->GetSpecularLight());
	//
	//			s->SendUniform(s->CreateStructArrayName(ShaderStrings::LIGHT_STRUCT_NAME_UNIFORM, ShaderStrings::LIGHT_STRUCT_SPECULAR_RATIO_UNIFORM, count),
	//				ls.second->GetSpecularCoefficient());
	//
	//			s->SendUniform(s->CreateStructArrayName(ShaderStrings::LIGHT_STRUCT_NAME_UNIFORM, ShaderStrings::LIGHT_STRUCT_SHININESS_UNIFORM, count),
	//				ls.second->GetShininess());
	//
	//			s->SendUniform(s->CreateStructArrayName(ShaderStrings::LIGHT_STRUCT_NAME_UNIFORM, ShaderStrings::LIGHT_STRUCT_DIFFUSE_UNIFORM, count),
	//				ls.second->GetDiffuseLight());
	//
	//			s->SendUniform(s->CreateStructArrayName(ShaderStrings::LIGHT_STRUCT_NAME_UNIFORM, ShaderStrings::LIGHT_STRUCT_DIFFUSE_RATIO_UNIFORM, count),
	//				ls.second->GetDiffuseCoefficient());
	//
	//			s->SendUniform(s->CreateStructArrayName(ShaderStrings::LIGHT_STRUCT_NAME_UNIFORM, ShaderStrings::LIGHT_STRUCT_CONE_ANGLE_UNIFORM, count),
	//				ls.second->GetSpotAngle());
	//		}
	//
	//		++count;
	//	}
	//}
	//
	//if (type == DEBUG) {
	//	glDrawElements(GL_LINES, m_model->GetIBOCount(true), GL_UNSIGNED_INT, (void*)0);
	//}
	//else {
	//	glDrawElements(GL_TRIANGLES, m_model->GetIBOCount(false), GL_UNSIGNED_INT, (void*)0);
	//
	//}
	//glDrawElements(GL_TRIANGLES, m_model->GetIndincesCount(false), GL_UNSIGNED_INT, (void*)0);
	glDrawElements(GL_TRIANGLES, m_model->GetIndincesCount(), GL_UNSIGNED_INT, 0);
	
}

void SceneObject::GeneralUpdate()
{
	m_M = glm::mat4(1.f);
	//m_M = glm::scale(m_M, m_scale);
	//m_M = glm::rotate(m_M, m_rotation.x, glm::vec3(1.f, 0.f, 0.f));
	//m_M = glm::rotate(m_M, m_rotation.y, glm::vec3(0.f, 1.f, 0.f));
	//m_M = glm::rotate(m_M, m_rotation.z, glm::vec3(0.f, 0.f, 1.f));
	//m_M = glm::translate(m_M, m_position);
	
	/*m_M = glm::mat4().SetScale(m_scale) *
		(glm::mat4().SetRotationX(m_rotation.x) *
			glm::mat4().SetRotationY(m_rotation.y) *
			glm::mat4().SetRotationZ(m_rotation.z)) *
		glm::mat4().SetTranslation(m_position);*/
}
