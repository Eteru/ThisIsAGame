
#include "Terrain.h"
#include "Vertex.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Strings.h"

Terrain::Terrain(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale, glm::vec3 heights,
	bool depth_test, std::string id)
	: SceneObject(pos, rot, scale, depth_test, id), m_heights(heights),
	m_height_map(nullptr), m_water(nullptr)
{
}

Terrain::~Terrain()
{ // Terrains has its own model that has to be deleted
	if (nullptr != m_model)
	{
		delete m_model;
	}

	if (nullptr != m_height_map)
	{
		uint32_t verts = m_block_size / m_cell_size + 1;
		for (uint32_t i = 0; i < verts; ++i)
		{
			delete[] m_height_map[i];
		}

		delete[] m_height_map;
	}

	if (nullptr != m_water)
	{
		delete m_water;
	}
}

void Terrain::Init()
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

	m_model = new Model();

	try 
	{
		m_im = IndexedModel::LoadModel();
		InitHeightMap(m_im);

		m_model->InitMesh(m_im);
	}
	catch (const std::exception& e) {
		std::cerr << "ERROR: " << e.what() << std::endl;

		GenerateFlatModel(m_block_size, m_cell_size, m_offsetY);
	}

	m_half_size = static_cast<uint32_t>(m_block_size * 0.5f);

	// Center terrain to camera position
	Camera *cam = SceneManager::GetInstance()->GetActiveCamera();
	m_transform.position.x = cam->GetPosition().x - m_half_size;
	m_transform.position.z = cam->GetPosition().z - m_half_size;


	glBindVertexArray(m_model->GetVAO());

	glBindBuffer(GL_ARRAY_BUFFER, m_model->GetVBO(Model::POSITION_VB));
	m_shader->SendAttribute(ShaderStrings::POSITION_ATTRIBUTE, 3, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_model->GetVBO(Model::UV_VB));
	m_shader->SendAttribute(ShaderStrings::UV_ATTRIBUTE, 3, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_model->GetVBO(Model::NORMAL_VB));
	m_shader->SendAttribute(ShaderStrings::NORMAL_ATTRIBUTE, 3, 0, 0);

	//glBindBuffer(GL_ARRAY_BUFFER, m_model->GetVBO(Model::UV_BLEND_VB));
	//glBufferData(GL_ARRAY_BUFFER, sizeof(m_uv_blend[0]) * m_uv_blend.size(), &m_uv_blend[0], GL_STATIC_DRAW);
	//glBindBuffer(GL_ARRAY_BUFFER, m_model->GetVBO(Model::UV_BLEND_VB));
	//m_shader->SendAttribute(ShaderStrings::UV_BLEND_ATTRIBUTE, 3, 0, 0);

	glBindVertexArray(0);

	m_water = new Water(glm::vec3(m_transform.position.x, 0.f, m_transform.position.z),
		m_transform.rotation, m_transform.scale, m_im);
	m_water->Init();

	m_init = true;
}

void Terrain::Update(float dt)
{
	GeneralUpdate();

	m_water->Update(dt);
}

void Terrain::Draw(DrawType type)
{
	Shader *s = SceneObject::SHADOW_MAP != type ? m_shader
		: ResourceManager::GetInstance()->LoadShader(ID::SHADER_SHADOW);

	// bind the program
	glUseProgram(s->GetProgramID());
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

	//m_water->Draw(type);
}

float Terrain::GetTerrainHeight(float x, float z)
{
	float t_x = x - m_transform.position.x;
	float t_z = z - m_transform.position.z;

	if (t_x >= m_block_size || t_z >= m_block_size || t_x < 0 || t_z < 0)
	{
		std::cerr << "Outside of terrain\n";
		return 0.f;
	}

	int cells_per_line = m_block_size / m_cell_size;

	int grid_x = t_x / m_cell_size;
	int grid_z = t_z / m_cell_size;

	// between [0, 1]
	float inner_grid_x = std::fmodf(t_x, m_cell_size) / m_cell_size;
	float inner_grid_z = std::fmodf(t_z, m_cell_size) / m_cell_size;

	float value;
	if (inner_grid_x <= 1.f - inner_grid_z)
	{ // left triangle
		value = BarryCentric(glm::vec3(0, m_height_map[grid_x][grid_z], 0),
			glm::vec3(1, m_height_map[grid_x + 1][grid_z], 0),
			glm::vec3(0, m_height_map[grid_x][grid_z + 1], 1),
			glm::vec2(inner_grid_x, inner_grid_z));
	}
	else
	{ // right triangle
		value = BarryCentric(glm::vec3(1, m_height_map[grid_x + 1][grid_z], 0),
			glm::vec3(1, m_height_map[grid_x + 1][grid_z + 1], 1),
			glm::vec3(0, m_height_map[grid_x][grid_z + 1], 1),
			glm::vec2(inner_grid_x, inner_grid_z));
	}
	
	return value;
}

std::vector<glm::vec3> Terrain::GenerateGridSquare(int col, int row, uint32_t cell_size, float offset_y)
{
	std::vector<glm::vec3> verts(4);

	int crt_col = col * cell_size;
	int crt_row = row * cell_size;
	int next_col = crt_col + cell_size;
	int next_row = crt_row + cell_size;

	m_height_map[col][row] = offset_y + m_hg.GenerateHeight(crt_col, crt_row);
	m_height_map[col][row + 1] = offset_y + m_hg.GenerateHeight(crt_col, next_row);
	m_height_map[col + 1][row] = offset_y + m_hg.GenerateHeight(next_col, crt_row);
	m_height_map[col + 1][row + 1] = offset_y + m_hg.GenerateHeight(next_col, next_row);

	verts[0] = glm::vec3(crt_col, m_height_map[col][row], crt_row);
	verts[1] = glm::vec3(crt_col, m_height_map[col][row + 1], next_row);
	verts[2] = glm::vec3(next_col, m_height_map[col + 1][row], crt_row);
	verts[3] = glm::vec3(next_col, m_height_map[col + 1][row + 1], next_row);

	return verts;
}

void Terrain::GenerateFlatModel(uint32_t blockSize, uint32_t cellSize, float offsetY)
{
	size_t idx = 0;
	size_t cells_per_line = blockSize / cellSize;
	size_t verts_per_line = blockSize + 1;
	size_t vertices_count = blockSize * blockSize * 2 * 3;
		
	for (size_t row = 0; row < cells_per_line - 1; ++row)
	{
		for (size_t col = 0; col < cells_per_line - 1; ++col)
		{
			std::vector<glm::vec3> square_verts = GenerateGridSquare(row, col, cellSize, offsetY);

			glm::vec3 NTL = glm::normalize(glm::cross(square_verts[0], square_verts[1]));
			glm::vec3 NBR = glm::normalize(glm::cross(square_verts[2], square_verts[1]));

			// Top Left
			StoreDefaultVertex(m_im, square_verts[0], NTL, glm::vec2(col, row), idx++, verts_per_line);
			StoreDefaultVertex(m_im, square_verts[1], NTL, glm::vec2(col, row + 1), idx++, verts_per_line);
			StoreDefaultVertex(m_im, square_verts[2], NTL, glm::vec2(col + 1, row), idx++, verts_per_line);

			// Bottom Right
			StoreDefaultVertex(m_im, square_verts[2], NBR, glm::vec2(col + 1, row), idx++, verts_per_line);
			StoreDefaultVertex(m_im, square_verts[1], NBR, glm::vec2(col, row + 1), idx++, verts_per_line);
			StoreDefaultVertex(m_im, square_verts[3], NBR, glm::vec2(col + 1, row + 1), idx++, verts_per_line);

		}
	}

	m_im.CalcNormals();

	m_im.SaveModel();

	InitHeightMap(m_im);

	m_model->InitMesh(m_im);
}

void Terrain::StoreDefaultVertex(IndexedModel & im, glm::vec3 pos, glm::vec3 normal, glm::vec2 uv, size_t index, uint32_t verts_per_line)
{
	im.positions.push_back(pos);
	im.normals.push_back(normal);
	im.texCoords.push_back(uv);
	im.indices.push_back(index);

	//m_uv_blend.push_back(glm::vec2(static_cast<float>(uv.x) / verts_per_line, static_cast<float>(uv.y) / verts_per_line));

}

void Terrain::InitHeightMap(const IndexedModel & im)
{
	uint32_t verts = m_block_size / m_cell_size + 1;
	m_height_map = new float*[verts];

	for (uint32_t i = 0; i < verts; ++i)
	{
		m_height_map[i] = new float[verts];
	}

	for (size_t i = 0; i < im.positions.size(); ++i)
	{
		int x = im.positions[i].x / m_cell_size;
		int z = im.positions[i].z / m_cell_size;

		m_height_map[x][z] = im.positions[i].y;
	}
}

float Terrain::BarryCentric(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec2 pos)
{
	float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);
	float l1 = ((p2.z - p3.z) * (pos.x - p3.x) + (p3.x - p2.x) * (pos.y - p3.z)) / det;
	float l2 = ((p3.z - p1.z) * (pos.x - p3.x) + (p1.x - p3.x) * (pos.y - p3.z)) / det;
	float l3 = 1.0f - l1 - l2;

	return l1 * p1.y + l2 * p2.y + l3 * p3.y;
}