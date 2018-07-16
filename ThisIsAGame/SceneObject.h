#pragma once

#include <vector>
#include <string>
#include <glm/glm.hpp>

#include "Model.h"
#include "Shader.h"
#include "Texture.h"
//#include "Trajectory.h"

class SceneObject
{
public:
	enum DrawType { NORMAL = 0, DEBUG, SHADOW_MAP };

	SceneObject(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale, bool depth_test, std::string id);
	virtual ~SceneObject();

	void SetWired(bool is_wired);
	void SetBlend(bool use_blend);
	void SetModel(Model *model);
	void SetShader(Shader *shader);
	//void SetTrajectory(Trajectory *trajectory);
	void AddTexture(Texture *texture);
	void AddLightID(std::string id);

	inline glm::mat4 GetModel()
	{
		return m_M;
	}

	inline const BoundingBox GetLocalSpaceBB() const
	{
		return m_model->GetBB();
	}

	inline const BoundingBox GetBB()
	{
		BoundingBox bb = m_model->GetBB();

		bb.bb_min = glm::vec3(glm::vec4(bb.bb_min, 1.0) * m_M);
		bb.bb_max = glm::vec3(glm::vec4(bb.bb_max, 1.0) * m_M);

		return bb;
	}

	inline const std::string & GetName() const
	{
		return m_id;
	}

	inline glm::vec3 GetPosition() const
	{
		return m_position;
	}

	inline const Model *GetMesh() const
	{
		return m_model;
	}

	inline glm::vec3 GetScale() const
	{
		return m_scale;
	}

	inline glm::vec3 GetRotation() const
	{
		return m_rotation;
	}

	virtual void Init();
	virtual void Update();
	virtual void Draw(DrawType type = NORMAL);
	virtual bool Collides(SceneObject * obj);
	virtual bool Contains(const glm::vec3 & point);

protected:
	bool m_depth_test;
	bool m_is_wired;
	std::string m_id;
	glm::vec3 m_position;
	glm::vec3 m_rotation;
	glm::vec3 m_scale;
	glm::mat4 m_M;
	Model *m_model;
	Shader *m_shader;
	//Trajectory *m_trajectory;
	std::vector<Texture *> m_textures;
	std::vector<std::string> m_light_ids;

	glm::vec3 m_up;
	glm::vec3 m_front;
	glm::vec3 m_right;

	void SharedDrawElements(DrawType type = NORMAL);
	void GeneralUpdate();
};
