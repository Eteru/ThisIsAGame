#pragma once

const float M_PI = 3.141592f;

namespace Window
{
	const unsigned WIDTH = 1792;
	const unsigned HEIGHT = 1008;

	const unsigned MIN_OPENGL_MAJOR = 2;
	const unsigned MIN_OPENGL_MINOR = 0;
}

enum Movement {FORWARD = 0, BACKWARD, LEFT, RIGHT};

// Physics
const glm::vec3 WORLD_UP = glm::vec3(0.f, 1.f, 0.f);

namespace Physics
{
	const float GRAVITY = -5.f;
}

// IDs
const std::string WATER_SHADER_ID = "14";