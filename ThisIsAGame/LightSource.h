#pragma once

#include <string>
#include <glm/glm.hpp>

class LightSource
{
public:
	enum LightType { POINT_LIGHT = 0, DIRECTIONAL_LIGHT, SPOT_LIGHT, NO_LIGHT };
	LightSource(float shininess, float diff_coef, float spec_coef, glm::vec3 diff_color, glm::vec3 spec_color, std::string id);
	virtual ~LightSource();

	void SetType(std::string type);
	void SetPosition(glm::vec3 pos);
	void SetDirection(glm::vec3 dir);
	void SetSpotAngle(float angle);

	glm::vec3 GetPosition();
	glm::vec3 GetDirection();
	glm::vec3 GetSpecularLight() const;
	glm::vec3 GetDiffuseLight() const;
	float GetSpecularCoefficient() const;
	float GetDiffuseCoefficient() const;
	float GetShininess() const;
	float GetSpotAngle() const;
	LightType GetType() const;

	glm::mat4 GetViewMatrix(glm::vec3 target);

private:
	LightType m_type;

	float m_shininess;
	float m_spot_angle;
	float m_diffuse_coef;
	float m_specular_coef;

	glm::vec3 m_diffuse_color;
	glm::vec3 m_specular_color;
	glm::vec3 m_position;
	glm::vec3 m_direction;

	std::string m_assoc_obj_id;
};

