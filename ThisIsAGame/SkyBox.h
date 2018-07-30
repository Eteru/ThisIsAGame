#pragma once
#include "SceneObject.h"
class SkyBox :
	public SceneObject
{
public:
	SkyBox(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale, std::string id,
		float offset, float size);
	~SkyBox();

	virtual void Init() override;
	virtual void Update(float dt) override;
	virtual void Draw(DrawType type = NORMAL) override;
	virtual bool Collides(SceneObject * obj) override;
	virtual bool Contains(const glm::vec3 & point) override;

private:
	const float ROTATION_SPEED = 1.f;
	const float TIME_INCRESEASE = 0.000694f * 24;

	float m_offsetY;
	float m_size;
	float m_half_size;
	float m_time;
};

