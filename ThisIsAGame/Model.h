#pragma once

#include <cinttypes>
#include <string>
#include <vector>
#include "Vertex.h"
#include "HeightsGenerator.h"
#include "Structs.h"

class Model
{
public:
	Model();
	Model(ModelResource *mr);
	Model(std::vector<glm::vec3> & verts, std::vector<glm::vec3> & colors);

	~Model();

	bool Load();
	void CalcNormals(const std::vector<int>& indices);
	std::vector<Vertex> GenerateFlatModel(uint32_t blockSize, uint32_t cellSize, float offsetY);
	void RebindBuffer(std::vector<Vertex> & vertices);

	inline bool IsLoaded()
	{
		return m_loaded;
	}

	inline GLuint GetVBO() const
	{
		return m_vboID;
	}

	inline GLuint GetVBONormals() const
	{
		return m_vbo_normals;
	}

	inline GLuint GetIBO(bool is_wired) const
	{
		return (true == is_wired) ? m_wiredIboID : m_iboID;
	}

	inline uint32_t GetVerticesCount() const
	{
		return m_verticesCount;
	}

	inline uint32_t GetIBOCount(bool is_wired) const
	{
		return (true == is_wired) ? m_indicesWiredCount : m_indicesCount;
	}

	inline const BoundingBox & GetBB() const
	{
		return m_bb;
	}

	inline const std::vector<Vertex> & GetVertices() const
	{
		return m_vertices;
	}

private:
	bool m_loaded;
	GLuint m_iboID;
	GLuint m_wiredIboID;
	GLuint m_vboID;
	GLuint m_vbo_normals;
	uint32_t m_indicesCount;
	uint32_t m_indicesWiredCount;
	uint32_t m_verticesCount;

	BoundingBox m_bb;
	ModelResource *m_mr;
	HeightsGenerator m_hg;

	std::vector<Vertex> m_vertices;

	std::vector<glm::vec3> GenerateGridSquare(int col, int row, uint32_t cell_size, float offset_y);
	void StoreDefaultVertex(glm::vec3 pos, glm::vec3 normal, glm::vec2 uv, size_t index, uint32_t verts_per_line);
};