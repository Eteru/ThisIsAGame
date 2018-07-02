#pragma once
#include <glm/glm.hpp>

struct Vertex 
{
	glm::vec3 pos;
	glm::vec3 color;
	glm::vec3 normal;
	glm::vec3 binormal;
	glm::vec3 tangent;
	glm::vec2 uv;
	glm::vec2 uv_blend;

	Vertex()
		: pos(glm::vec3(0.f, 0.f, 0.f)), color(glm::vec3(0.f, 0.f, 0.f)), normal(glm::vec3(0.f, 0.f, 0.f)), uv(glm::vec2(0.f, 0.f)), uv_blend(glm::vec2(0.f, 0.f))
	{}

	Vertex(const Vertex & vertex)
		: pos(vertex.pos), color(vertex.color), normal(vertex.normal), uv(vertex.uv), uv_blend(vertex.uv_blend)
	{}
};