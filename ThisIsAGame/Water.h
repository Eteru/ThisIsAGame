#pragma once

#include "SceneObject.h"

class Water :
	public SceneObject
{
public:
	Water(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale, const IndexedModel & im);
	virtual ~Water();

	virtual void Init() override;
	virtual void Update(float dt) override;
	virtual void Draw(DrawType type = NORMAL) override;

protected:
	GLuint m_vbo_indicator;
	GLfloat m_time;

	std::vector<glm::vec4> m_indicators;

private:
	const GLfloat WAVE_SPEED = 0.002f;
};

