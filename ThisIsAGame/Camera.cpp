
#include "Camera.h"
#include "Constants.h"

Camera::Camera(glm::vec3 position, glm::vec3 target, glm::vec3 up, GLfloat moveSpeed, GLfloat rotateSpeed, GLfloat cnear, GLfloat cfar, GLfloat fov)
	: m_movement_speed(moveSpeed), m_sensitivity(rotateSpeed), m_near(cnear), m_far(cfar), m_fov(fov)
{
	m_aspect_ratio = static_cast<float>(Window::WIDTH) / Window::HEIGHT;

	m_position = position;
	m_front = target;
	m_up = up;
	m_yaw = YAW;
	m_pitch = PITCH;
	UpdateWorldView();
}

Camera::~Camera()
{
}

void Camera::Move(CameraMovement movement, float dt)
{
	float velocity = m_movement_speed * dt;

	switch (movement)
	{
	case FORWARD:
		m_position += m_front * velocity;
		break;
	case BACKWARD:
		m_position -= m_front * velocity;
		break;
	case LEFT:
		m_position -= m_right * velocity;
		break;
	case RIGHT:
		m_position += m_right * velocity;
		break;
	default:
		break;
	}
}

void Camera::MouseMove(float xoffset, float yoffset, GLboolean constrainPitch)
{
	xoffset *= m_sensitivity;
	yoffset *= m_sensitivity;

	m_yaw -= xoffset;
	m_pitch -= yoffset;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (true == constrainPitch)
	{
		if (m_pitch > 89.0f)
			m_pitch = 89.0f;

		if (m_pitch < -89.0f)
			m_pitch = -89.0f;
	}

	// Update Front, Right and Up Vectors using the updated Euler angles
	UpdateWorldView();
}

void Camera::RotateOX(int dir)
{
	//glm::vec4 up4 = glm::vec4(0.f, 1.f, 0.f, 0.f) * Matrix().SetRotationX(dir * m_rotateSpeed * m_dt) * m_worldMatrix;
	//m_up = glm::vec3(up4.x, up4.y, up4.z).Normalize();
	//
	//glm::vec4 localTarget = glm::vec4(0.f, 0.f, -(m_target - m_position).Length(), 1.f);
	//glm::vec4 rotatedTarget = localTarget * Matrix().SetRotationX(dir * m_rotateSpeed * m_dt) * m_worldMatrix;
	//
	//m_target = glm::vec3(rotatedTarget.x, rotatedTarget.y, rotatedTarget.z);
	//
	//UpdateWorldView();
}

void Camera::RotateOY(int dir)
{
	//glm::vec4 localTarget = glm::vec4(0.f, 0.f, -(m_target - m_position).Length(), 1.f);
	//glm::vec4 rotatedTarget = localTarget * Matrix().SetRotationY(dir * m_rotateSpeed * m_dt) * m_worldMatrix;
	//
	//m_target = glm::vec3(rotatedTarget.x, rotatedTarget.y, rotatedTarget.z);
	//
	//UpdateWorldView();
}

void Camera::RotateOZ(int dir)
{
	//glm::vec4 up4 = glm::vec4(0.f, 1.f, 0.f, 0.f) * Matrix().SetRotationZ(dir * m_rotateSpeed * m_dt) * m_worldMatrix;
	//m_up = glm::vec3(up4.x, up4.y, up4.z).Normalize();
	//
	//UpdateWorldView();
}

void Camera::UpdateWorldView()
{
	// Calculate the new Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	front.y = sin(glm::radians(m_pitch));
	front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	m_front = glm::normalize(front);

	// Also re-calculate the Right and Up vector
	m_right = glm::normalize(glm::cross(m_front, m_world_up));
	m_up = glm::normalize(glm::cross(m_right, m_front));
}
