
#include "Player.h"
#include "Terrain.h"
#include "SceneManager.h"

Player::Player(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale, bool depth_test, std::string id)
	: SceneObject(pos, rot, scale, depth_test, id), m_movement_speed(0.f), m_turn_speed(0.f),
	m_upwards_speed(0.f), m_jumping(false)
{
}

Player::~Player()
{
}

void Player::Update(float dt)
{
	CheckInputs();

	float velocity = m_movement_speed * dt;
	float rot = m_turn_speed * dt;

	m_upwards_speed += Physics::GRAVITY * dt;

	m_transform.rotation.y += rot;

	m_transform.front.x = sin(glm::radians(m_transform.rotation.y));
	m_transform.front.y = 0.f;
	m_transform.front.z = cos(glm::radians(m_transform.rotation.y));
	m_transform.front = glm::normalize(m_transform.front);

	m_transform.position += m_transform.front * velocity;
	m_transform.position.y += m_upwards_speed;

	Terrain *t = dynamic_cast<Terrain *>
		(SceneManager::GetInstance()->GetSceneObject("terrain"));

	if (nullptr != t)
	{
		float t_height = t->GetTerrainHeight(m_transform.position.x,
			m_transform.position.z) + 0.5f;
		//std::cout << "Terrain height at " << m_position.x << ", " << m_position.z << " is: " << t_height << std::endl;
		if (m_transform.position.y < t_height)
		{
			m_jumping = false;
			m_upwards_speed = 0.f;
			m_transform.position.y = t_height;
		}
	}

	GeneralUpdate();
}

void Player::Move(Movement movement, float dt)
{
	float velocity = m_movement_speed * dt;

	switch (movement)
	{
	case FORWARD:
		m_transform.position += m_transform.front * velocity;
		break;
	case BACKWARD:
		m_transform.position -= m_transform.front * velocity;
		break;
	case LEFT:
		m_transform.position -= m_transform.right * velocity;
		break;
	case RIGHT:
		m_transform.position += m_transform.right * velocity;
		break;
	default:
		break;
	}
}

void Player::CheckInputs()
{
	GLFWwindow *window = SceneManager::GetInstance()->GetWindow();

	if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_W))
	{
		m_movement_speed = RUN_SPEED;
	}
	else if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_S))
	{
		m_movement_speed = -RUN_SPEED;
	}
	else
	{
		m_movement_speed = 0.f;
	}


	if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_A))
	{
		m_turn_speed = TURN_SPEED;
	}
	else if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_D))
	{
		m_turn_speed = -TURN_SPEED;
	}
	else
	{
		m_turn_speed = 0.f;
	}

	if (false == m_jumping && GLFW_PRESS == glfwGetKey(window, GLFW_KEY_SPACE))
	{
		m_upwards_speed = JUMP_POWER;
		m_jumping = true;
	}
}
