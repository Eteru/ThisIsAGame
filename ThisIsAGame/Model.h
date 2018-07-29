#pragma once

#include <cinttypes>
#include <string>
#include <vector>

#include "Structs.h"

class IndexedModel;

class Model
{
public:
	enum BUFFER_TYPE { POSITION_VB = 0, UV_VB, NORMAL_VB, INDEX_VB, UV_BLEND_VB, NUM_BUFFERS };

	Model();
	Model(ModelResource *mr);

	~Model();

	void Load();
	void InitMesh(const IndexedModel & model);
	void CreateMesh(const std::vector<glm::vec3> & vertices, const std::vector<unsigned int> & indices);

	inline bool IsLoaded()
	{
		return m_loaded;
	}

	inline GLuint GetVAO() const
	{
		return m_vao;
	}

	inline GLuint GetVBO(BUFFER_TYPE type = POSITION_VB) const
	{
		return m_vbo[type];
	}

	inline uint32_t GetVerticesCount() const
	{
		return m_vertices_count;
	}

	inline uint32_t GetIndincesCount() const
	{
		return m_indices_count;
	}

	inline const BoundingBox & GetBB() const
	{
		return m_bb;
	}

	//inline const std::vector<Vertex> & GetVertices() const
	//{
	//	return m_vertices;
	//}

private:

	bool m_loaded;
	GLuint m_vao;
	GLuint m_vbo[NUM_BUFFERS];

	uint32_t m_indices_count;
	uint32_t m_vertices_count;

	BoundingBox m_bb;
	ModelResource *m_mr;

	//std::vector<Vertex> m_vertices;
};