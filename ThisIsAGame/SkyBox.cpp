
#include "SkyBox.h"
#include "SceneManager.h"
#include "Strings.h"

SkyBox::SkyBox(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale, std::string id,
	float offset, float size)
	: SceneObject(pos, rot, scale, false, id), m_offsetY(offset), m_size(size),
	m_half_size(size * 0.5f), m_time(0.f)
{
}

SkyBox::~SkyBox()
{
}

void SkyBox::Init()
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

	m_transform.scale = glm::vec3(m_size, m_size, m_size);

	m_transform.position = SceneManager::GetInstance()->GetActiveCamera()->GetPosition();

	glBindVertexArray(m_model->GetVAO());

	glBindBuffer(GL_ARRAY_BUFFER, m_model->GetVBO(Model::POSITION_VB));
	m_shader->SendAttribute(ShaderStrings::POSITION_ATTRIBUTE, 3, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_model->GetVBO(Model::UV_VB));
	m_shader->SendAttribute(ShaderStrings::UV_ATTRIBUTE, 3, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_model->GetVBO(Model::NORMAL_VB));
	m_shader->SendAttribute(ShaderStrings::NORMAL_ATTRIBUTE, 3, 0, 0);

	glBindVertexArray(0);

	m_init = true;
}

void SkyBox::Update(float dt)
{
	m_transform.position = SceneManager::GetInstance()->GetActiveCamera()->GetPosition();
	m_transform.rotation.y += ROTATION_SPEED * dt;
	m_time = fmod(m_time + TIME_INCRESEASE, 24.f);

	GeneralUpdate();
}

void SkyBox::Draw(DrawType type)
{
	if (SceneObject::SHADOW_MAP == type)
	{
		return;
	}

	glDisable(GL_CULL_FACE);

	// bind the program
	glUseProgram(m_shader->GetProgramID());
	// bind the VAO
	glBindVertexArray(m_model->GetVAO());

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(m_textures[0]->GetTextureType(), m_textures[0]->GetID());
	m_shader->SendUniform(ShaderStrings::TEXTURE_CUBE_UNIFORM_0, 0);

	if (m_textures.size() > 1)
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(m_textures[1]->GetTextureType(), m_textures[1]->GetID());
		m_shader->SendUniform(ShaderStrings::TEXTURE_CUBE_UNIFORM_1, 1);
	}

	m_shader->SendUniform(ShaderStrings::TIME_UNIFORM, m_time);
	
	SharedDrawElements(type);

	// unbind the VAO
	glBindVertexArray(0);

	// unbind the program
	glUseProgram(0);

	glEnable(GL_CULL_FACE);
}

bool SkyBox::Collides(SceneObject * obj)
{
	return false;
}

bool SkyBox::Contains(const glm::vec3 & point)
{
	return false;
}
