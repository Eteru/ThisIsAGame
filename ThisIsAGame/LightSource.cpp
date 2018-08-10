
#include <glm/gtc/matrix_transform.hpp>

#include "LightSource.h"
#include "Constants.h"

LightSource::LightSource(float shininess, float diff_coef, float spec_coef, glm::vec3 diff_color, glm::vec3 spec_color, std::string id)
	: m_shininess(shininess), m_diffuse_coef(diff_coef), m_specular_coef(spec_coef), m_diffuse_color(diff_color),
	m_specular_color(spec_color), m_assoc_obj_id(id), m_spot_angle(90.f), m_position(glm::vec3(0,0,0)), m_direction(glm::vec3(0,0,0)),
	m_type(NO_LIGHT)
{
}

LightSource::~LightSource()
{
}

void LightSource::SetType(std::string type)
{
	if ("point" == type)
	{
		m_type = POINT_LIGHT;
	}
	else if ("directional" == type)
	{
		m_type = DIRECTIONAL_LIGHT;
	}
	else if ("spot" == type)
	{
		m_type = SPOT_LIGHT;
	}
}

void LightSource::SetPosition(glm::vec3 pos)
{
	m_position = pos;
}

void LightSource::SetDirection(glm::vec3 dir)
{
	m_direction = dir;
}

void LightSource::SetSpotAngle(float angle)
{
	m_spot_angle = angle;
}

glm::vec3 LightSource::GetPosition()
{
	return m_position;
}

glm::vec3 LightSource::GetDirection()
{
	return m_direction;
}

glm::vec3 LightSource::GetSpecularLight() const
{
	return m_specular_color;
}

glm::vec3 LightSource::GetDiffuseLight() const
{
	return m_diffuse_color;
}

float LightSource::GetSpecularCoefficient() const
{
	return m_specular_coef;
}

float LightSource::GetDiffuseCoefficient() const
{
	return m_diffuse_coef;
}

float LightSource::GetShininess() const
{
	return m_shininess;
}

float LightSource::GetSpotAngle() const
{
	return m_spot_angle;
}

LightSource::LightType LightSource::GetType() const
{
	return m_type;
}

glm::mat4 LightSource::GetViewMatrix(glm::vec3 target)
{
	return glm::lookAt(glm::vec3(0.5f, 2, 2), -m_position, WORLD_UP);
}
