
#pragma once
#include "SceneObject.h"

class SceneObjectBatch :
	public SceneObject
{
public:
	SceneObjectBatch(glm::vec3 rotation, glm::vec3 scale, uint32_t count, bool depth_test, std::string id);
	virtual ~SceneObjectBatch();

	virtual void Init() override;
	virtual void Update(float dt) override;
	virtual void Draw(DrawType type = NORMAL) override;

	void AddObject(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);

protected:
	uint32_t m_object_count;

	std::vector<Transform> m_transforms;
};

