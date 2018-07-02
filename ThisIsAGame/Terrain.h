#pragma once

#include "Vertex.h"
#include "SceneObject.h"

class Terrain :
	public SceneObject
{
public:
	Terrain(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale, glm::vec3 heights, bool depth_test, std::string id);
	~Terrain();

	inline void SetOffsetY(float offsetY)
	{
		m_offsetY = offsetY;
	}

	inline void SetCellSize(uint32_t cell_size)
	{
		m_cell_size = cell_size;
	}

	inline void SetBlockSize(uint32_t block_size)
	{
		m_block_size = block_size;
	}

	virtual void Init() override;
	virtual void Update() override;
	virtual void Draw(DrawType type = NORMAL) override;

protected:
	float m_offsetY;
	uint32_t m_half_size;
	uint32_t m_cell_size;
	uint32_t m_block_size;

	glm::vec3 m_heights;
	std::vector<Vertex> m_vertices;
};

