#pragma once

const float M_PI = 3.141592f;

namespace Window
{
	const unsigned WIDTH = 1920;
	const unsigned HEIGHT = 1080;

	const unsigned MIN_OPENGL_MAJOR = 2;
	const unsigned MIN_OPENGL_MINOR = 0;
}

// Camera
enum CameraMovement {FORWARD = 0, BACKWARD, LEFT, RIGHT};

const float YAW = 0.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;
