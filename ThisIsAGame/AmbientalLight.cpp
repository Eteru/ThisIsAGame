
#include "AmbientalLight.h"


AmbientalLight::AmbientalLight()
	: m_ratio(0.f), m_color(glm::vec3(0,0,0))
{
}

AmbientalLight::AmbientalLight(float ratio, glm::vec3 color)
	: m_ratio(ratio), m_color(color)
{
}


AmbientalLight::~AmbientalLight()
{
}

void AmbientalLight::SetValue(float ratio, glm::vec3 & color)
{
	m_ratio = ratio;
	m_color = color;
}
