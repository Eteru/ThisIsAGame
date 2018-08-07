#pragma once
#include <utility>
#include <vector>

#include "Camera.h"

class ShadowBox
{
public:
	ShadowBox(glm::mat4 light_view);
	virtual ~ShadowBox();

	void Update(glm::mat4 & light_space);

	float GetWidth() const;
	float GetHeight() const;
	float GetLength() const;

	glm::vec3 GetCenter();
	std::vector<glm::vec3> m_frustrum_corners;

protected:
	void ComputeWidthAndHeight();
	void ComputeCenter();

private:
	const float SHADOW_DISTANCE = 100;

	glm::mat4 m_light_view;
	glm::mat4 m_light_view_inverted;
	Camera *m_camera;

	std::pair<float, float> m_near_dims;
	std::pair<float, float> m_far_dims;

	glm::vec3 m_mins;
	glm::vec3 m_maxs;
	glm::vec3 m_center;
};

