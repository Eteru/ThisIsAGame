
#include "Terrain.h"
#include "Vertex.h"
//#include "SceneManager.h"
#include "Strings.h"

Terrain::Terrain(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale, glm::vec3 heights, bool depth_test, std::string id)
	: SceneObject(pos, rot, scale, depth_test, id)
{
	m_heights = heights;
}

Terrain::~Terrain()
{ // Terrains has its own model that has to be deleted
	if (nullptr != m_model)
		delete m_model;
}

void Terrain::Init()
{
	m_model = new Model();
	m_vertices = m_model->GenerateFlatModel(m_block_size, m_cell_size, m_offsetY);
	m_half_size = static_cast<uint32_t>(m_block_size * m_cell_size * 0.5f);

	// Center terrain to camera position
	//Camera *cam = SceneManager::GetInstance()->GetActiveCamera();
	m_position.x = 0;//cam->GetPosition().x - m_half_size;
	m_position.z = 0;//cam->GetPosition().z - m_half_size;
}

void Terrain::Update()
{
	//Camera *cam = SceneManager::GetInstance()->GetActiveCamera();
	//glm::vec3 camera_pos = cam->GetPosition();
	//
	//float d_X = camera_pos.x - (m_position.x + m_half_size);
	//float d_Z = camera_pos.z - (m_position.z + m_half_size);
	//float d_Value_X = 0, d_Value_Y = 0;
	//
	//if (d_X >= m_cell_size) {
	//	m_position.x += m_cell_size;
	//	d_Value_Y = 1.f / m_block_size;
	//}
	//else if (-d_X >= m_cell_size) {
	//	m_position.x -= m_cell_size;
	//	d_Value_Y = -1.f / m_block_size;
	//	// move uvs
	//}
	//if (d_Z >= m_cell_size) {
	//	m_position.z += m_cell_size;
	//	d_Value_X = 1.f / m_block_size;
	//	// move uvs
	//}
	//else if (-d_Z >= m_cell_size) {
	//	m_position.z -= m_cell_size;
	//	d_Value_X = -1.f / m_block_size;
	//	// move uvs
	//}
	//
	//for (Vertex & v : m_vertices) {
	//	v.uv_blend.x += d_Value_X;
	//	v.uv_blend.y += d_Value_Y;
	//}
	//
	//m_model->RebindBuffer(m_vertices);

	GeneralUpdate();
}

void Terrain::Draw(DrawType type)
{
	Shader *s = m_shader;
	//if (SceneObject::SHADOW_MAP == type) {
	//	s = ResourceManager::GetInstance()->LoadShader("12");
	//}
	//else {
	//	s = m_shader;
	//}

	glUseProgram(s->GetProgramID());

	glBindBuffer(GL_ARRAY_BUFFER, m_model->GetVBO());

	if (type == DEBUG)
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_model->GetIBO(true));
	else
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_model->GetIBO(false));
	
	s->SendUniform(ShaderStrings::HEIGHT_MAP_UNIFORM, m_heights);
	
	SharedDrawElements(type);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
