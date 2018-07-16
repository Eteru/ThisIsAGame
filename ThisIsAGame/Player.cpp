
#include "Player.h"
#include "SceneManager.h"

Player::Player(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale, bool depth_test, std::string id)
	: SceneObject(pos, rot, scale, depth_test, id), m_movement_speed(0.f), m_turn_speed(0.f)
{
}

Player::~Player()
{
}

void Player::Update()
{
	CheckInputs();

	float velocity = m_movement_speed /* dt*/;
	float rot = m_turn_speed /* dt*/;

	m_rotation.y += rot;

	m_front.x = cos(glm::radians(m_rotation.y));
	m_front.y = 0.f;
	m_front.z = sin(glm::radians(m_rotation.y));
	m_front = glm::normalize(m_front);

	m_position += m_front * velocity;

	GeneralUpdate();
}

void Player::Move(Movement movement, float dt)
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
}
