
#include "InputMouseInterface.h"

bool InputMouseInterface::m_lbutton_pressed = false;
bool InputMouseInterface::m_rbutton_pressed = false;

InputMouseInterface::InputMouseInterface()
{
}

InputMouseInterface::~InputMouseInterface()
{
}

void InputMouseInterface::LeftButtonPressed(bool pressed)
{
	m_lbutton_pressed = pressed;
}

void InputMouseInterface::RightButtonPressed(bool pressed)
{
	m_rbutton_pressed = pressed;
}
