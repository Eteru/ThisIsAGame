
#include "LightSource.h"

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
	if ("point" == type) {
		m_type = POINT_LIGHT;
	}
	else if ("directional" == type) {
		m_type = DIRECTIONAL_LIGHT;
	}
	else if ("spot" == type) {
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

glm::mat4 LightSource::GetViewMatrix(glm::vec3 & dir, glm::vec3 & center)
{
	//glm::vec3 zAxis = -(center - dir).Normalize();
	//glm::vec3 yAxis = glm::vec3(0, 1, 0);
	//glm::vec3 xAxis = zAxis.Cross(yAxis).Normalize();
	//
	//Matrix R;
	//R.m[0][0] = xAxis.x; R.m[0][1] = xAxis.y;  R.m[0][2] = xAxis.z;  R.m[0][3] = 0.0f;
	//R.m[1][0] = yAxis.x;  R.m[1][1] = yAxis.y; R.m[1][2] = yAxis.z;  R.m[1][3] = 0.0f;
	//R.m[2][0] = zAxis.x;  R.m[2][1] = zAxis.y;  R.m[2][2] = zAxis.z; R.m[2][3] = 0.0f;
	//R.m[3][0] = 0.0f;  R.m[3][1] = 0.0f;  R.m[3][2] = 0.0f;  R.m[3][3] = 1.0f;
	//
	//return Matrix().SetTranslation(center) * R.Transpose();
	return glm::mat4(1.f);
}
