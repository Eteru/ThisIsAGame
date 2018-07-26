#pragma once


#include "SceneObject.h"
#include <GLFW\glfw3.h>

#include "InputKeyboardInterface.h"
#include "Constants.h"

class Player : public SceneObject
{
public:
	Player(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale, bool depth_test, std::string id);
	virtual ~Player();
	
	virtual void Update(float dt) override;
	
protected:
	const GLfloat RUN_SPEED = 10.f;
	const GLfloat TURN_SPEED = 60.f;
	const GLfloat JUMP_POWER = 2.f;

	bool m_jumping;
	GLfloat m_turn_speed;
	GLfloat m_movement_speed;
	GLfloat m_upwards_speed;

	void Move(Movement movement, float dt);
	void CheckInputs();
};

