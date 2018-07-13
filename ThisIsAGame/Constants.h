#pragma once

const float M_PI = 3.141592f;

namespace Window
{
	const unsigned WIDTH = 1280;
	const unsigned HEIGHT = 960;

	const unsigned MIN_OPENGL_MAJOR = 2;
	const unsigned MIN_OPENGL_MINOR = 0;
}

// Camera
enum CameraMovement {FORWARD = 0, BACKWARD, LEFT, RIGHT};

const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;
