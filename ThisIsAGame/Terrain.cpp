
#include "Terrain.h"
#include "Vertex.h"
#include "SceneManager.h"
#include "ObjLoader.h"
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

	m_model = new Model();
	GenerateFlatModel(m_block_size, m_cell_size, m_offsetY);
	m_half_size = static_cast<uint32_t>(m_block_size * m_cell_size * 0.5f);

	// Center terrain to camera position
	Camera *cam = SceneManager::GetInstance()->GetActiveCamera();
	m_position.x = cam->GetPosition().x - m_half_size;
	m_position.z = cam->GetPosition().z - m_half_size;


	glBindVertexArray(m_model->GetVAO());

	glBindBuffer(GL_ARRAY_BUFFER, m_model->GetVBO(Model::POSITION_VB));
	m_shader->SendAttribute(ShaderStrings::POSITION_ATTRIBUTE, 3, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_model->GetVBO(Model::UV_VB));
	m_shader->SendAttribute(ShaderStrings::UV_ATTRIBUTE, 3, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_model->GetVBO(Model::NORMAL_VB));
	m_shader->SendAttribute(ShaderStrings::NORMAL_ATTRIBUTE, 3, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_model->GetVBO(Model::UV_BLEND_VB));
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_uv_blend[0]) * m_uv_blend.size(), &m_uv_blend[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, m_model->GetVBO(Model::UV_BLEND_VB));
	m_shader->SendAttribute(ShaderStrings::UV_BLEND_ATTRIBUTE, 3, 0, 0);

	glBindVertexArray(0);
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

	// bind the program
	glUseProgram(m_shader->GetProgramID());
	// bind the VAO
	glBindVertexArray(m_model->GetVAO());

	//if (type == DEBUG)
	//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_model->GetIBO(true));
	//else
	//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_model->GetIBO(false));
	
	//s->SendUniform(ShaderStrings::HEIGHT_MAP_UNIFORM, m_heights);
	
	SharedDrawElements(type);

	// unbind the VAO
	glBindVertexArray(0);

	// unbind the program
	glUseProgram(0);
}

std::vector<glm::vec3> Terrain::GenerateGridSquare(int col, int row, uint32_t cell_size, float offset_y)
{
	std::vector<glm::vec3> verts(4);

	verts[0] = glm::vec3(col, offset_y + m_hg.GenerateHeight(col, row), row);
	verts[1] = glm::vec3(col, offset_y + m_hg.GenerateHeight(col, row + 1), row + 1);
	verts[2] = glm::vec3(col + 1, offset_y + m_hg.GenerateHeight(col + 1, row), row);
	verts[3] = glm::vec3(col + 1, offset_y + m_hg.GenerateHeight(col + 1, row + 1), row + 1);

	return verts;
}

void Terrain::GenerateFlatModel(uint32_t blockSize, uint32_t cellSize, float offsetY)
{
	size_t idx = 0;
	size_t verts_per_line = blockSize + 1;
	size_t vertices_count = blockSize * blockSize * 2 * 3;

	IndexedModel im;
	
	for (size_t row = 0; row < blockSize - 1; ++row)
	{
		for (size_t col = 0; col < blockSize - 1; ++col)
		{
			std::vector<glm::vec3> square_verts = GenerateGridSquare(row, col, cellSize, offsetY);

			glm::vec3 NTL = glm::normalize(glm::cross(square_verts[0], square_verts[1]));
			glm::vec3 NBR = glm::normalize(glm::cross(square_verts[2], square_verts[1]));

			// Top Left
			StoreDefaultVertex(im, square_verts[0], NTL, glm::vec2(col, row), idx++, verts_per_line);
			StoreDefaultVertex(im, square_verts[1], NTL, glm::vec2(col, row + 1), idx++, verts_per_line);
			StoreDefaultVertex(im, square_verts[2], NTL, glm::vec2(col + 1, row), idx++, verts_per_line);

			// Bottom Right
			StoreDefaultVertex(im, square_verts[2], NBR, glm::vec2(col + 1, row), idx++, verts_per_line);
			StoreDefaultVertex(im, square_verts[1], NBR, glm::vec2(col, row + 1), idx++, verts_per_line);
			StoreDefaultVertex(im, square_verts[3], NBR, glm::vec2(col + 1, row + 1), idx++, verts_per_line);

		}
	}

	im.CalcNormals();

	m_model->InitMesh(im);
}

void Terrain::StoreDefaultVertex(IndexedModel & im, glm::vec3 pos, glm::vec3 normal, glm::vec2 uv, size_t index, uint32_t verts_per_line)
{
	im.positions.push_back(pos);
	im.normals.push_back(normal);
	im.texCoords.push_back(uv);
	im.indices.push_back(index);

	m_uv_blend.push_back(glm::vec2(static_cast<float>(uv.x) / verts_per_line, static_cast<float>(uv.y) / verts_per_line));

}
