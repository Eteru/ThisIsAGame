
#include "SkyBox.h"
#include "SceneManager.h"


SkyBox::SkyBox(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale, std::string id,
	float offset, float size)
	: SceneObject(pos, rot, scale, false, id), m_offsetY(offset), m_size(size), m_half_size(size * 0.5f)
{
}

SkyBox::~SkyBox()
{
}

void SkyBox::Init()
{
	m_scale = glm::vec3(m_size, m_size, m_size);
	// Center skybox to camera position
	Camera *cam = SceneManager::GetInstance()->GetActiveCamera();
	glm::vec3 camera_pos = cam->GetPosition();
	m_position.x = camera_pos.x - m_half_size;
	m_position.y = camera_pos.y - m_half_size + m_offsetY;
	m_position.z = camera_pos.z - m_half_size;
}

void SkyBox::Update()
{
	Camera *cam = SceneManager::GetInstance()->GetActiveCamera();
	glm::vec3 camera_pos = cam->GetPosition();
	m_position.x = camera_pos.x - m_half_size;
	m_position.y = camera_pos.y - m_half_size + m_offsetY;
	m_position.z = camera_pos.z - m_half_size;

	GeneralUpdate();
}

void SkyBox::Draw(DrawType type)
{
	glUseProgram(m_shader->GetProgramID());

	glBindBuffer(GL_ARRAY_BUFFER, m_model->GetVBO());

	if (type == DEBUG)
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_model->GetIBO(true));
	else
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_model->GetIBO(false));
	
	SharedDrawElements(type);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

bool SkyBox::Collides(SceneObject * obj)
{
	return false;
}

bool SkyBox::Contains(const glm::vec3 & point)
{
	return false;
}
