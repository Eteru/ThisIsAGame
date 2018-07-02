
#include <iostream>
#include <algorithm>

#include <glm/gtc/matrix_transform.hpp>

#include "Model.h"

Model::Model() : m_mr(nullptr), m_loaded(false)
{
}

Model::Model(ModelResource *mr) : m_mr(mr), m_loaded(false)
{
}

Model::Model(std::vector<glm::vec3>& verts, std::vector<glm::vec3>& colors)
{
	if (verts.size() != colors.size()) {
		std::cerr << "Color vector should be the same size as vertex vector\n";
	}

	m_vertices.resize(verts.size());

	for (int i = 0; i < verts.size(); ++i) {
		m_vertices[i].pos = verts[i];
		m_vertices[i].color = colors[i];
		m_vertices[i].normal = glm::vec3(0, 0, 0);

		m_vertices[i].uv = glm::vec2(1, 1);
		m_vertices[i].uv_blend = glm::vec2(1, 1);
	}

	// Generate and create vbo
	glGenBuffers(1, &m_vboID);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * verts.size(), &m_vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Read indices
	m_indicesCount = static_cast<uint32_t>(verts.size());

	std::vector<int> indices(m_indicesCount);
	for (uint32_t i = 0; i < m_indicesCount; ++i) {
		indices[i] = i;
	}

	// Generate and create ibo for filled mesh
	glGenBuffers(1, &m_iboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * m_indicesCount, &indices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	m_indicesWiredCount = m_indicesCount;
	m_wiredIboID = m_iboID;

	m_loaded = true;
}

Model::~Model()
{
	glDeleteBuffers(1, &m_vboID);
	glDeleteBuffers(1, &m_iboID);
	glDeleteBuffers(1, &m_wiredIboID);
}

bool Model::Load()
{
	if (true == m_loaded) {
		return true;
	}

	if (nullptr == m_mr) {
		return false;
	}

	FILE *f =  fopen(m_mr->model_path.c_str(), "r");

	fscanf(f, "NrVertices: %d\n", &m_verticesCount);
	if (m_verticesCount <= 0)
		return false;


	// Read m_vertices
	m_bb.bb_min = glm::vec3(FLT_MAX, FLT_MAX, FLT_MAX);
	m_bb.bb_max = glm::vec3(FLT_MIN, FLT_MIN, FLT_MIN);
	m_vertices.resize(m_verticesCount);
	for (uint32_t i = 0; i < m_verticesCount; ++i) {
		fscanf(f, "   %*d. pos:[%f, %f, %f]; norm:[%f, %f, %f]; binorm:[%f, %f, %f]; tgt:[%f, %f, %f]; uv:[%f, %f];\n",
			&m_vertices[i].pos.x, &m_vertices[i].pos.y, &m_vertices[i].pos.z,
			&m_vertices[i].normal.x, &m_vertices[i].normal.y, &m_vertices[i].normal.z,
			&m_vertices[i].binormal.x, &m_vertices[i].binormal.y, &m_vertices[i].binormal.z,
			&m_vertices[i].tangent.x, &m_vertices[i].tangent.y, &m_vertices[i].tangent.z,
			&m_vertices[i].uv.x, &m_vertices[i].uv.y);

		m_bb.bb_min.x = std::min(m_bb.bb_min.x, m_vertices[i].pos.x);
		m_bb.bb_min.y = std::min(m_bb.bb_min.y, m_vertices[i].pos.y);
		m_bb.bb_min.z = std::min(m_bb.bb_min.z, m_vertices[i].pos.z);

		m_bb.bb_max.x = std::max(m_bb.bb_max.x, m_vertices[i].pos.x);
		m_bb.bb_max.y = std::max(m_bb.bb_max.y, m_vertices[i].pos.y);
		m_bb.bb_max.z = std::max(m_bb.bb_max.z, m_vertices[i].pos.z);

		m_vertices[i].color = glm::vec3(1, 1, 1);
		m_vertices[i].uv_blend = glm::vec2(1, 1);
	}

	// Read indices
	fscanf(f, "NrIndices: %d\n", &m_indicesCount);
	if (m_indicesCount <= 0) {
		glDeleteBuffers(1, &m_vboID);
		return false;
	}

	std::vector<int> indices(m_indicesCount);
	for (uint32_t i = 0; i < m_indicesCount; i += 3) {
		fscanf(f, "   %*d.    %d,    %d,    %d\n", &indices[i], &indices[i + 1], &indices[i + 2]);
	}

	CalcNormals(indices);

	// Generate and create vbo
	glGenBuffers(1, &m_vboID);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_vertices.size(), &m_vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Generate and create ibo for filled mesh
	glGenBuffers(1, &m_iboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * m_indicesCount, &indices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	std::vector<glm::vec3> normals(m_vertices.size() * 2);

	for (int i = 0, index = 0; i < m_vertices.size(); ++i, index += 2) {
		normals[index] = m_vertices[i].pos;
		normals[index + 1] = m_vertices[i].pos + glm::vec3(glm::vec4(m_vertices[i].normal, 1.f) * glm::scale(glm::mat4(), glm::vec3(10.f)));
	}

	glGenBuffers(1, &m_vbo_normals);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_normals);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(), &normals[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Create ibo buffer for wired mesh
	m_indicesWiredCount = m_indicesCount * 2;
	std::vector<int> indicesWired(m_indicesWiredCount);
	for (uint32_t i = 0, wiredCount = 0; i < m_indicesCount; i += 3, wiredCount += 6) {
		indicesWired[wiredCount] = indices[i];
		indicesWired[wiredCount + 1] = indices[i + 1];
		indicesWired[wiredCount + 2] = indices[i + 1];
		indicesWired[wiredCount + 3] = indices[i + 2];
		indicesWired[wiredCount + 4] = indices[i + 2];
		indicesWired[wiredCount + 5] = indices[i];
	}

	// Generate and create ibo for wired mesh
	glGenBuffers(1, &m_wiredIboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_wiredIboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * m_indicesWiredCount, &indicesWired[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	m_loaded = true;
	return true;
}

void Model::CalcNormals(const std::vector<int> & indices)
{
	for (unsigned int i = 0; i < indices.size(); i += 3)
	{
		int i0 = indices[i];
		int i1 = indices[i + 1];
		int i2 = indices[i + 2];

		glm::vec3 v1 = m_vertices[i1].pos - m_vertices[i0].pos;
		glm::vec3 v2 = m_vertices[i2].pos - m_vertices[i0].pos;

		glm::vec3 normal = glm::normalize(glm::cross(v1, v2));

		m_vertices[i0].normal += normal;
		m_vertices[i1].normal += normal;
		m_vertices[i2].normal += normal;
	}

	for (unsigned int i = 0; i < m_vertices.size(); i++)
		m_vertices[i].normal = glm::normalize(m_vertices[i].normal);
}

std::vector<Vertex> Model::GenerateFlatModel(uint32_t blockSize, uint32_t cellSize, float offsetY)
{
	uint32_t idx = 0;
	uint32_t verts_per_line = blockSize + 1;
	uint32_t vertices_count = blockSize * blockSize * 2 * 3;
	std::vector<uint32_t> indices(vertices_count);

	m_vertices.resize(vertices_count);

	for (uint32_t row = 0; row < blockSize - 1; ++row) {
		for (uint32_t col = 0; col < blockSize - 1; ++col) {
			std::vector<glm::vec3> square_verts = GenerateGridSquare(row, col, cellSize, offsetY);

			glm::vec3 NTL = glm::normalize(glm::cross(square_verts[0], square_verts[1]));
			glm::vec3 NBR = glm::normalize(glm::cross(square_verts[2], square_verts[1]));

			// Top Left
			indices[idx] = idx;
			StoreDefaultVertex(square_verts[0], NTL, glm::vec2(col, row), idx++, verts_per_line);
			indices[idx] = idx;
			StoreDefaultVertex(square_verts[1], NTL, glm::vec2(col, row + 1), idx++, verts_per_line);
			indices[idx] = idx;
			StoreDefaultVertex(square_verts[2], NTL, glm::vec2(col + 1, row), idx++, verts_per_line);

			// Bottom Right
			indices[idx] = idx;
			StoreDefaultVertex(square_verts[2], NBR, glm::vec2(col + 1, row), idx++, verts_per_line);
			indices[idx] = idx;
			StoreDefaultVertex(square_verts[1], NBR, glm::vec2(col, row + 1), idx++, verts_per_line);
			indices[idx] = idx;
			StoreDefaultVertex(square_verts[3], NBR, glm::vec2(col + 1, row + 1), idx++, verts_per_line);

		}
	}



	//size_t verts_count = line_size * line_size;
	//m_vertices.resize(verts_count);
	//
	//for (int i = 0; i < line_size; ++i) {
	//	for (int j = 0; j < line_size; ++j) {
	//		size_t idx = i * line_size + j;
	//		m_vertices[idx].pos = glm::vec3(j * cellSize, offsetY, i * cellSize);
	//		m_vertices[idx].normal = glm::vec3(0, 1, 0);
	//		m_vertices[idx].color = glm::vec3(1, 1, 1);
	//		m_vertices[idx].uv = glm::vec2(i, j);
	//		m_vertices[idx].uv_blend = glm::vec2(static_cast<float>(i) / line_size, static_cast<float>(j) / line_size);
	//
	//		if (i < blockSize && j < blockSize) {
	//			indices.push_back(idx);
	//			indices.push_back(idx + line_size + 1);
	//			indices.push_back(idx + line_size);
	//			indices.push_back(idx);
	//			indices.push_back(idx + 1);
	//			indices.push_back(idx + line_size + 1);
	//		}
	//	}
	//}

	// Generate and create vbo
	glGenBuffers(1, &m_vboID);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices_count, &m_vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	m_indicesCount = static_cast<uint32_t>(indices.size());
	// Generate and create ibo for filled mesh
	glGenBuffers(1, &m_iboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * m_indicesCount, &indices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	m_loaded = true;

	return m_vertices;
}

void Model::RebindBuffer(std::vector<Vertex>& m_vertices)
{
	for (Vertex & v : m_vertices) {
		m_bb.bb_min.x = std::min(m_bb.bb_min.x, v.pos.x);
		m_bb.bb_min.y = std::min(m_bb.bb_min.y, v.pos.y);
		m_bb.bb_min.z = std::min(m_bb.bb_min.z, v.pos.z);

		m_bb.bb_max.x = std::max(m_bb.bb_max.x, v.pos.x);
		m_bb.bb_max.y = std::max(m_bb.bb_max.y, v.pos.y);
		m_bb.bb_max.z = std::max(m_bb.bb_max.z, v.pos.z);
	}

	glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_vertices.size(), &m_vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

std::vector<glm::vec3> Model::GenerateGridSquare(int col, int row, uint32_t cell_size, float offset_y)
{
	std::vector<glm::vec3> verts(4);

	verts[0] = glm::vec3(col, offset_y + m_hg.GenerateHeight(col, row), row);
	verts[1] = glm::vec3(col, offset_y + m_hg.GenerateHeight(col, row + 1), row + 1);
	verts[2] = glm::vec3(col + 1, offset_y + m_hg.GenerateHeight(col + 1, row), row);
	verts[3] = glm::vec3(col + 1, offset_y + m_hg.GenerateHeight(col + 1, row + 1), row + 1);

	return verts;
}

void Model::StoreDefaultVertex(glm::vec3 pos, glm::vec3 normal, glm::vec2 uv, size_t index, uint32_t verts_per_line)
{
	m_vertices[index].pos = pos;
	m_vertices[index].normal = normal;
	m_vertices[index].color = glm::vec3(1, 1, 1);
	m_vertices[index].uv = uv;
	m_vertices[index].uv_blend = glm::vec2(static_cast<float>(uv.x) / verts_per_line, static_cast<float>(uv.y) / verts_per_line);
}
