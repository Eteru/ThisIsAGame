#pragma once
#include "SceneObject.h"
#include <glm/glm.hpp>

class Primitives
{
public:
	~Primitives();

	static Model *GetLine();
	static Model *GetCube();
	static Model *GetSphere();
	static Model *GetAxis();

	static const glm::vec3 COLOR_RED;
	static const glm::vec3 COLOR_GREEN;
	static const glm::vec3 COLOR_BLUE;
	static const glm::vec3 COLOR_YELLOW;
private:
	static const std::string m_cube_id;
	static const std::string m_sphere_id;
protected:
	Primitives();
};

