#pragma once

#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "InputMouseInterface.h"
#include "InputKeyboardInterface.h"
#include "SceneObject.h"
#include "Constants.h"

class Camera : public InputMouseInterface
{
public:
	Camera(std::string following_id);
	~Camera();

	inline glm::mat4 GetView() const
	{
		return glm::lookAt(m_position, m_position + m_front, m_up);
	}

	inline glm::mat4 GetProjection() const
	{
		return glm::perspective(glm::radians(m_fov), m_aspect_ratio, m_near, m_far);
	}

	inline glm::vec3 GetPosition() const
	{
		return m_position;
	}

	inline glm::vec3 GetFront() const
	{
		return m_front;
	}

	inline glm::vec3 GetUp() const
	{
		return m_up;
	}

	inline glm::vec3 GetRight() const
	{
		return m_right;
	}

	inline float GetNear() const
	{
		return m_near;
	}

	inline float GetFar() const
	{
		return m_far;
	}

	inline float GetFOV() const
	{
		return m_fov;
	}

	void SetTarget(SceneObject *player);
	
	void RotateOX(int dir);
	void RotateOY(int dir);
	void RotateOZ(int dir);

	void Init();
	void Update(float dt);

	// interfaces
	virtual void MouseScroll(float y_offset) override;
	virtual void MouseMove(float x_offset, float y_offset) override;

	virtual void MouseLeftClick(double x, double y) override;
	virtual void MouseRightClick(double x, double y) override;

	virtual void Raycast(glm::vec3 ray) override;

private:
	const GLfloat ZOOM = 45.0f;
	const GLfloat SENSITIVITY = 0.1f;
	const GLfloat MAX_PITCH = 89.f;
	const GLfloat DEFAULT_PITCH = 45.f;
	const GLfloat DEFAULT_YAW = -90.f;
	const GLfloat MAX_DISTANCE_FROM_TARGET = 80.f;
	const GLfloat MIN_DISTANCE_FROM_TARGET = 15.f;
	const GLfloat DEFAULT_ANGLE_AROUND_TARGET = 45.f;
	const GLfloat DEFAULT_DISTANCE_FROM_TARGET = 30.f;

	glm::vec3 m_position;
	glm::vec3 m_front;
	glm::vec3 m_up;
	glm::vec3 m_right;

	// Angles
	GLfloat m_yaw;
	GLfloat m_pitch;

	GLfloat m_sensitivity;

	GLfloat m_near;
	GLfloat m_far;
	GLfloat m_fov;
	GLfloat m_aspect_ratio;

	GLfloat m_angle_around_target;
	GLfloat m_distance_from_target;

	SceneObject *m_target;
	std::string m_target_id;

	void UpdateWorldView();
};

