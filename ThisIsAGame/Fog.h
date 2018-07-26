#pragma once

#include <glm/glm.hpp>

class Fog
{
public:
	Fog();
	Fog(glm::vec3 & color);
	virtual ~Fog();

	void SetColor(glm::vec3 color);

	inline glm::vec3 GetColor() const
	{
		return m_color;
	}

private:
	glm::vec3 m_color;
};

