#pragma once

class InputKeyboardInterface
{
public:
	InputKeyboardInterface();
	virtual ~InputKeyboardInterface();

	virtual void KeyPress(int key, int mods) = 0;
};

