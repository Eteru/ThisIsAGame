#pragma once

#include <glm\glm.hpp>

class InputMouseInterface
{
public:
	InputMouseInterface();
	virtual ~InputMouseInterface();

	virtual void MouseScroll(float y_offset) = 0;
	virtual void MouseMove(float x_offset, float y_offset) = 0;

	virtual void MouseLeftClick(double x, double y) = 0;
	virtual void MouseRightClick(double x, double y) = 0;

	virtual void Raycast(glm::vec3 ray) = 0;

	virtual void LeftButtonPressed(bool pressed);
	virtual void RightButtonPressed(bool pressed);

protected:
	static bool m_lbutton_pressed;
	static bool m_rbutton_pressed;
};

