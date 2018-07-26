
#include "Fog.h"

Fog::Fog()
	: m_color(glm::vec3(0.f))
{
}

Fog::Fog(glm::vec3 & color)
	: m_color(color)
{
}

Fog::~Fog()
{
}

void Fog::SetColor(glm::vec3 color)
{
	m_color = color;
}
