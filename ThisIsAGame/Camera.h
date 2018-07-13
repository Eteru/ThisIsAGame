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
	Camera(glm::vec3 position, glm::vec3 target, glm::vec3 up, GLfloat moveSpeed = SPEED, GLfloat rotateSpeed = SENSITIVITY, GLfloat cnear = 0.2f, GLfloat cfar = 1000.f, GLfloat fov = ZOOM);
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

	void SetTarget(SceneObject *so);

	void Move(CameraMovement movement, float dt);
	//void MouseMove(float xoffset, float yoffset, GLboolean constrainPitch = true);

	void RotateOX(int dir);
	void RotateOY(int dir);
	void RotateOZ(int dir);

	void UpdateWorldView();

	// interfaces
	//virtual void KeyPress(int key, int mods) override;

	virtual void MouseScroll(float y_offset) override;
	virtual void MouseMove(float x_offset, float y_offset) override;

private:
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
	glm::vec3 m_world_up;

	// Angles
	GLfloat m_yaw;
	GLfloat m_pitch;

	GLfloat m_movement_speed;
	GLfloat m_sensitivity;

	GLfloat m_near;
	GLfloat m_far;
	GLfloat m_fov;
	GLfloat m_aspect_ratio;

	GLfloat m_angle_around_target;
	GLfloat m_distance_from_target;

	SceneObject *m_target;
};

