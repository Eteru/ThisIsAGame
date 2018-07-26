#pragma once

#include <gl/glew.h>
#include <glm/glm.hpp>
#include <string>

enum ObjectType {
	OT_NORMAL = 0,
	OT_SKYBOX,
	OT_TERRAIN,
	OT_ANIMATED,
	OT_PLAYER,
	OT_BATCH
};

struct ModelResource
{
	std::string id;
	std::string model_path;
};

struct TextureResource
{
	GLenum type;
	GLenum wrap_s;
	GLenum wrap_t;
	GLfloat min_filter;
	GLfloat mag_filter;
	std::string id;
	std::string texture_path;
};

struct ShaderResource
{
	std::string id;
	std::string vs_path;
	std::string fs_path;
};

struct SoundResource
{
	bool repeat;
	std::string id;
	std::string sound_path;
};

struct Transform
{
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	glm::vec3 up;
	glm::vec3 front;
	glm::vec3 right;
};

struct BoundingBox
{
	glm::vec3 bb_min;
	glm::vec3 bb_max;

	BoundingBox()
		: bb_min(glm::vec3(0, 0, 0)), bb_max(glm::vec3(0, 0, 0))
	{}

	BoundingBox(glm::vec3 min_p, glm::vec3 max_p)
		: bb_min(min_p), bb_max(max_p)
	{}

	inline bool Contains(const glm::vec3 & point) const
	{
		if (bb_min.x > point.x) return false;
		if (bb_min.y > point.y) return false;
		if (bb_min.z > point.z) return false;
		if (bb_max.x < point.x) return false;
		if (bb_max.y < point.y) return false;
		if (bb_max.z < point.z) return false;

		return true;
	}

	inline bool Collides(const BoundingBox & rhs_bb) const
	{
		if (bb_min.x > rhs_bb.bb_max.x) return false;
		if (bb_min.y > rhs_bb.bb_max.y) return false;
		if (bb_min.z > rhs_bb.bb_max.z) return false;
		if (bb_max.x < rhs_bb.bb_min.x) return false;
		if (bb_max.y < rhs_bb.bb_min.y) return false;
		if (bb_max.z < rhs_bb.bb_min.z) return false;

		return true;
	}
};

struct DebugSettings
{
	std::string shader_prim_id;
	std::string shader_axis_id;
};