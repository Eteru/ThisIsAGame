
#include <iostream>

#include "Camera.h"
#include "SceneManager.h"
#include "Constants.h"

Camera::Camera(glm::vec3 position, glm::vec3 target, glm::vec3 up, GLfloat moveSpeed, GLfloat rotateSpeed, GLfloat cnear, GLfloat cfar, GLfloat fov)
	: m_movement_speed(moveSpeed), m_sensitivity(rotateSpeed), m_near(cnear), m_far(cfar), m_fov(fov)
{
	m_aspect_ratio = static_cast<float>(Window::WIDTH) / Window::HEIGHT;

	m_position = position;
	m_front = target;
	m_world_up = up;
	m_yaw = DEFAULT_YAW;
	m_pitch = DEFAULT_PITCH;

	m_angle_around_target = DEFAULT_ANGLE_AROUND_TARGET;
	m_distance_from_target = DEFAULT_DISTANCE_FROM_TARGET;

	m_target = nullptr;
	//m_target = SceneManager::GetInstance()->GetSceneObject("3");

	//UpdateWorldView();

	SceneManager::GetInstance()->RegisterMouseListeners(this);
}

Camera::~Camera()
{
}

void Camera::SetTarget(SceneObject * so)
{
	m_target = so;
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

	UpdateWorldView();
}

//void Camera::MouseMove(float xoffset, float yoffset, GLboolean constrainPitch)
//{
//	xoffset *= m_sensitivity;
//	yoffset *= m_sensitivity;
//
//	m_yaw -= xoffset;
//	m_pitch -= yoffset;
//
//	// Make sure that when pitch is out of bounds, screen doesn't get flipped
//	if (true == constrainPitch)
//	{
//		if (m_pitch > 89.0f)
//			m_pitch = 89.0f;
//
//		if (m_pitch < -89.0f)
//			m_pitch = -89.0f;
//	}
//
//	// Update Front, Right and Up Vectors using the updated Euler angles
//	UpdateWorldView();
//}

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
	m_target = SceneManager::GetInstance()->GetSceneObject("3");

	float vert_d = m_distance_from_target * sin(glm::radians(m_pitch));
	float horiz_d = m_distance_from_target * cos(glm::radians(m_pitch));

	float theta = m_angle_around_target /* + m_target.getRotY()*/;
	float offset_x = horiz_d * sin(glm::radians(theta));
	float offset_z = horiz_d * cos(glm::radians(theta));

	m_position.x = m_target->GetPosition().x - offset_x;
	m_position.y = m_target->GetPosition().y + vert_d;
	m_position.z = m_target->GetPosition().z - offset_z;

	m_yaw = 180.f - (m_angle_around_target /* + m_target.getRotY()*/);

	m_front = glm::normalize(m_target->GetPosition() - m_position);
	m_right = glm::normalize(glm::cross(m_front, m_world_up));
	m_up = glm::normalize(glm::cross(m_right, m_front));

	std::cout << "position: " << m_position.x << ", " << m_position.y << ", " << m_position.z << std::endl;
}

//void Camera::KeyPress(int key, int mods)
//{
//	switch (key)
//	{
//	case GLFW_KEY_W:
//		Move(CameraMovement::FORWARD, 1);
//		break;
//	case GLFW_KEY_S:
//		Move(CameraMovement::BACKWARD, 1);
//		break;
//	case GLFW_KEY_A:
//		Move(CameraMovement::LEFT, 1);
//		break;
//	case GLFW_KEY_D:
//		Move(CameraMovement::RIGHT, 1);
//		break;
//	default:
//		break;
//	}
//}

void Camera::MouseScroll(float y_offset)
{
	m_distance_from_target -= (y_offset /* m_sensitivity*/);

	if (m_distance_from_target < MIN_DISTANCE_FROM_TARGET)
	{
		m_distance_from_target = MIN_DISTANCE_FROM_TARGET;
	}
	else if (m_distance_from_target > MAX_DISTANCE_FROM_TARGET)
	{
		m_distance_from_target = MAX_DISTANCE_FROM_TARGET;
	}

	UpdateWorldView();
}

void Camera::MouseMove(float x_offset, float y_offset)
{
	if (true == m_lbutton_pressed)
	{
		x_offset *= m_sensitivity;
		m_angle_around_target -= x_offset;
		UpdateWorldView();
	}

	if (true == m_rbutton_pressed)
	{
		y_offset *= m_sensitivity;
		m_pitch -= y_offset;
		
		if (m_pitch < -MAX_PITCH)
		{
			m_pitch = -MAX_PITCH;
		}
		else if (m_pitch > MAX_PITCH)
		{
			m_pitch = MAX_PITCH;
		}

		UpdateWorldView();
	}
}
