
#include "SkyBox.h"
#include "SceneManager.h"
#include "Strings.h"

SkyBox::SkyBox(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale, std::string id,
	float offset, float size)
	: SceneObject(pos, rot, scale, false, id), m_offsetY(offset), m_size(size), m_half_size(size * 0.5f)
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

	GeneralUpdate();
}

void SkyBox::Draw(DrawType type)
{
	glDisable(GL_CULL_FACE);

	// bind the program
	glUseProgram(m_shader->GetProgramID());
	// bind the VAO
	glBindVertexArray(m_model->GetVAO());

	int tex_loc = m_textures.size();
	glActiveTexture(GL_TEXTURE0 + tex_loc);
	glBindTexture(m_textures[0]->GetTextureType(), m_textures[0]->GetID());

	m_shader->SendUniform(ShaderStrings::TEXTURE_CUBE_UNIFORM, tex_loc);
	
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
