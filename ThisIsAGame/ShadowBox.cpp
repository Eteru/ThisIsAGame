
#include "ShadowBox.h"
#include "SceneManager.h"
#include "Constants.h"

#include <cmath>

ShadowBox::ShadowBox(glm::mat4 light_view)
	: m_light_view(light_view)
{
	m_camera = SceneManager::GetInstance()->GetActiveCamera();

	//m_light_view_inverted = light_view.Inverse();
	ComputeWidthAndHeight();
}

ShadowBox::~ShadowBox()
{
}

void ShadowBox::Update(glm::mat4 & light_view)
{
	m_light_view = light_view;
	//m_light_view_inverted = light_view.Inverse();

	// compute camera frustrum
	//float c_near = m_camera->GetNearPlane();
	//float c_far = m_camera->GetFarPlane();
	//glm::vec3 pos = m_camera->GetPosition();
	//glm::vec3 target = m_camera->GetTarget().Normalize();
	//glm::vec3 up = m_camera->GetUp().Normalize();
	//glm::vec3 right = target.Cross(up).Normalize();
	//
	//glm::vec3 nc = pos + target * c_near;
	//glm::vec3 fc = pos + target * c_far;
	//
	//glm::vec3 points[8];
	////ftl
	//points[0] = fc + up * (m_far_dims.second * 0.5f) - right * (m_far_dims.first * 0.5f);
	////ftr
	//points[1] = fc + up * (m_far_dims.second * 0.5f) + right * (m_far_dims.first * 0.5f);
	////fbl
	//points[2] = fc - up * (m_far_dims.second * 0.5f) - right * (m_far_dims.first * 0.5f);
	////fbr
	//points[3] = fc - up * (m_far_dims.second * 0.5f) + right * (m_far_dims.first * 0.5f);
	////ntl
	//points[4] = nc + up * (m_near_dims.second * 0.5f) - right * (m_near_dims.first * 0.5f);
	////ntr
	//points[5] = nc + up * (m_near_dims.second * 0.5f) + right * (m_near_dims.first * 0.5f);
	////nbl
	//points[6] = nc - up * (m_near_dims.second * 0.5f) - right * (m_near_dims.first * 0.5f);
	////nbr
	//points[7] = nc - up * (m_near_dims.second * 0.5f) + right * (m_near_dims.first * 0.5f);
	//
	//bool first = true;
	//for (int i = 0; i < 8; ++i) {
	//	points[i] = glm::vec3(m_light_view * glm::vec4(points[i], 1.0));
	//
	//	if (true == first) {
	//		m_mins = points[i];
	//		m_maxs = points[i];
	//
	//		first = false;
	//	}
	//
	//	if (points[i].x > m_maxs.x) {
	//		m_maxs.x = points[i].x;
	//	}
	//	else if (points[i].x < m_mins.x) {
	//		m_mins.x = points[i].x;
	//	}
	//	if (points[i].y > m_maxs.y) {
	//		m_maxs.y = points[i].y;
	//	}
	//	else if (points[i].y < m_mins.y) {
	//		m_mins.y = points[i].y;
	//	}
	//	if (points[i].z > m_maxs.z) {
	//		m_maxs.z = points[i].z;
	//	}
	//	else if (points[i].z < m_mins.z) {
	//		m_mins.z = points[i].z;
	//	}
	//}
}

void ShadowBox::ComputeWidthAndHeight()
{
	//float c_near = m_camera->GetNearPlane();
	//float c_far = m_camera->GetFarPlane();
	//float fov = m_camera->GetFoV();
	//float aspect = static_cast<float>(Window::WIDTH) / Window::HEIGHT;
	//
	//m_near_dims.second = 2 * tanf(fov * 0.5f) * c_near;
	//m_far_dims.second = 2 * tanf(fov * 0.5f) * c_far;
	//
	//m_near_dims.first = m_near_dims.second * aspect;
	//m_far_dims.first = m_far_dims.second * aspect;
}

void ShadowBox::ComputeCenter()
{
	glm::vec3 center = (m_mins + m_maxs) * 0.5f;
}

float ShadowBox::GetWidth() const
{
	return m_maxs.x - m_mins.x;
}

float ShadowBox::GetHeight() const
{
	return m_maxs.y - m_mins.y;
}

float ShadowBox::GetLength() const
{
	return m_maxs.z - m_mins.z;
}

glm::vec3 ShadowBox::GetCenter()
{
	glm::vec3 center = (m_mins + m_maxs) * 0.5f;
	glm::vec3 center_ws = glm::vec3(m_light_view_inverted * glm::vec4(center, 1.f));

	return center_ws;
}
