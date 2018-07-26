
#include <iostream>

#include "Camera.h"
#include "SceneManager.h"
#include "Constants.h"

Camera::Camera(std::string following_id)
	: m_sensitivity(SENSITIVITY), m_near(0.2f), m_far(1000.f),
	m_fov(ZOOM), m_target_id(following_id), m_yaw(DEFAULT_YAW),
	m_pitch(DEFAULT_PITCH), m_angle_around_target(DEFAULT_ANGLE_AROUND_TARGET),
	m_distance_from_target(DEFAULT_DISTANCE_FROM_TARGET), m_target(nullptr)

{
	m_aspect_ratio = static_cast<float>(Window::WIDTH) / Window::HEIGHT;

	SceneManager::GetInstance()->RegisterMouseListener(this);
}

Camera::~Camera()
{
}

void Camera::SetTarget(SceneObject *player)
{
	m_target = player;
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

void Camera::Init()
{
	m_target = SceneManager::GetInstance()->GetSceneObject(m_target_id);
	UpdateWorldView();
}

void Camera::Update(float dt)
{
	UpdateWorldView();
}

void Camera::UpdateWorldView()
{
	float vert_d = m_distance_from_target * sin(glm::radians(m_pitch));
	float horiz_d = m_distance_from_target * cos(glm::radians(m_pitch));

	float theta = m_angle_around_target + m_target->GetRotation().y;
	float offset_x = horiz_d * sin(glm::radians(theta));
	float offset_z = horiz_d * cos(glm::radians(theta));

	m_position = m_target->GetPosition() + glm::vec3(-offset_x, vert_d, -offset_z);

	m_yaw = 180.f - (m_angle_around_target + m_target->GetRotation().y);

	m_front = glm::normalize(m_target->GetPosition() - m_position);
	m_right = glm::normalize(glm::cross(m_front, WORLD_UP));
	m_up = glm::normalize(glm::cross(m_right, m_front));
}

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
