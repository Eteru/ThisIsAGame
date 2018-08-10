
#include "Water.h"
#include "ResourceManager.h"
#include "ObjLoader.h"
#include "Constants.h"
#include "Strings.h"

Water::Water(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale, const IndexedModel & im)
	: SceneObject(pos, rot, scale, true, ID::SHADER_WATER), m_time(0.f)
{
	//m_vertices.resize(verts.size());
	//
	//for (size_t i = 0; i < verts.size(); ++i)
	//{
	//	m_vertices.push_back(glm::vec3(verts[i].x, 0.f, verts[i].z));
	//}

	m_model = new Model();
	m_model->CreateMesh(im.positions, im.indices);

	m_shader = ResourceManager::GetInstance()->LoadShader(ID::SHADER_WATER);

	m_indicators.resize(im.positions.size());
	for (size_t i = 0; i < m_indicators.size(); i += 3)
	{ // increment by 3 to make sure we parse triangles
		m_indicators[i] = glm::vec4(
			im.positions[i].x - im.positions[i + 1].x,
			im.positions[i].z - im.positions[i + 1].z,
			im.positions[i].x - im.positions[i + 2].x,
			im.positions[i].z - im.positions[i + 2].z);

		m_indicators[i+1] = glm::vec4(
			im.positions[i + 1].x - im.positions[i + 2].x,
			im.positions[i + 1].z - im.positions[i + 2].z,
			im.positions[i + 1].x - im.positions[i].x,
			im.positions[i + 1].z - im.positions[i].z);

		m_indicators[i + 2] = glm::vec4(
			im.positions[i + 2].x - im.positions[i + 1].x,
			im.positions[i + 2].z - im.positions[i + 1].z,
			im.positions[i + 2].x - im.positions[i].x,
			im.positions[i + 2].z - im.positions[i].z);
	}
}

Water::~Water()
{ // Water has its own model that has to be deleted
	if (nullptr != m_model)
	{
		delete m_model;
	}
}

void Water::Init()
{
	if (true == m_init)
	{
		return;
	}

	if (nullptr == m_shader)
	{
		throw std::runtime_error(std::string("Shader is nullptr: ") + m_id);
	}

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

	glGenBuffers(1, &m_vbo_indicator);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_indicator);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_indicators[0]) * m_indicators.size(), &m_indicators[0], GL_STATIC_DRAW);
	m_shader->SendAttribute(ShaderStrings::INDICATORS_ATTRIBUTE, 4, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_model->GetVBO(Model::POSITION_VB));
	m_shader->SendAttribute(ShaderStrings::POSITION_ATTRIBUTE, 3, 0, 0);
	
	glBindVertexArray(0);

	m_init = true;
}

void Water::Update(float dt)
{
	m_time += WAVE_SPEED;
	GeneralUpdate();
}

void Water::Draw(DrawType type)
{
	if (SceneObject::SHADOW_MAP == type)
	{
		return;
	}

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

	//if (type == DEBUG)
	//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_model->GetIBO(true));
	//else
	//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_model->GetIBO(false));

	//s->SendUniform(ShaderStrings::HEIGHT_MAP_UNIFORM, m_heights);

	m_shader->SendUniform(ShaderStrings::TIME_UNIFORM, m_time);

	SharedDrawElements(type);

	// unbind the VAO
	glBindVertexArray(0);

	// unbind the program
	glUseProgram(0);
}
