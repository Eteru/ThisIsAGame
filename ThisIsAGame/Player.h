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
	
	virtual void Update() override;
	
protected:
	const GLfloat RUN_SPEED = 2.5f;
	const GLfloat TURN_SPEED = 15.f;

	GLfloat m_turn_speed;
	GLfloat m_movement_speed;

	void Move(Movement movement, float dt);
	void CheckInputs();
};

