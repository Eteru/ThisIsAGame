#pragma once

#include <gl/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Constants.h"

class Camera
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

	void Move(CameraMovement movement, float dt);
	void MouseMove(float xoffset, float yoffset, GLboolean constrainPitch = true);

	void RotateOX(int dir);
	void RotateOY(int dir);
	void RotateOZ(int dir);

	void UpdateWorldView();

private:
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
};

