#pragma once

#include "Vertex.h"
#include "SceneObject.h"
#include "HeightsGenerator.h"

class IndexedModel;

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

	inline uint32_t GetBlockSize() const
	{
		return m_block_size;
	}

	virtual void Init() override;
	virtual void Update(float dt) override;
	virtual void Draw(DrawType type = NORMAL) override;

	float GetTerrainHeight(float x, float z);

protected:
	float m_offsetY;
	uint32_t m_half_size;
	uint32_t m_cell_size;
	uint32_t m_block_size;

	glm::vec3 m_heights;
	std::vector<glm::vec2> m_uv_blend;

	float **m_height_map;
	HeightsGenerator m_hg;

	std::vector<glm::vec3> GenerateGridSquare(int col, int row, uint32_t cell_size, float offset_y);
	void GenerateFlatModel(uint32_t blockSize, uint32_t cellSize, float offsetY);
	void StoreDefaultVertex(IndexedModel & im, glm::vec3 pos, glm::vec3 normal, glm::vec2 uv, size_t index, uint32_t verts_per_line);

	void InitHeightMap(const IndexedModel & im);
	float BarryCentric(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec2 pos);
};

