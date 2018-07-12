#pragma once

#include <glm\glm.hpp>

class AmbientalLight
{
public:
	AmbientalLight();
	AmbientalLight(float ratio, glm::vec3 color);
	virtual ~AmbientalLight();

	void SetValue(float ratio, glm::vec3 & color);

	inline glm::vec3 GetColor()
	{
		return m_color;
	}

	inline const float GetRatio()
	{
		return m_ratio;
	}

private:
	float m_ratio;
	glm::vec3 m_color;
};

