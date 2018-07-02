
#include "Primitives.h"
//#include "ResourceManager.h"

const glm::vec3 Primitives::COLOR_RED = glm::vec3(1.f, 0.f, 0.f);
const glm::vec3 Primitives::COLOR_GREEN = glm::vec3(0.f, 1.f, 0.f);
const glm::vec3 Primitives::COLOR_BLUE = glm::vec3(0.f, 0.f, 1.f);
const glm::vec3 Primitives::COLOR_YELLOW = glm::vec3(1.f, 1.f, 0.f);

const std::string Primitives::m_cube_id = "2";
const std::string Primitives::m_sphere_id = "3";


Primitives::Primitives()
{
}

Primitives::~Primitives()
{
}

Model * Primitives::GetLine()
{
	return ResourceManager::GetInstance()->GetModel("line");
}

Model * Primitives::GetCube()
{
	return ResourceManager::GetInstance()->LoadModel(m_cube_id);
}

Model * Primitives::GetSphere()
{
	return ResourceManager::GetInstance()->LoadModel(m_sphere_id);
}

Model * Primitives::GetAxis()
{
	return ResourceManager::GetInstance()->GetModel("axis");
}
