
#include <iostream>
#include <vector>

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include "ResourceManager.h"
#include "SceneManager.h"
#include "Strings.h"
#include "Constants.h"


static void CallbackError(int error, const char* description)
{
	std::cerr << "Error: " << description << std::endl;
}

static void CallbackKey(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);


	if (key == GLFW_KEY_W && action == GLFW_PRESS)
		SceneManager::GetInstance()->GetActiveCamera()->Move(CameraMovement::FORWARD, 1);

	if (key == GLFW_KEY_S && action == GLFW_PRESS)
		SceneManager::GetInstance()->GetActiveCamera()->Move(CameraMovement::BACKWARD, 1);

	if (key == GLFW_KEY_A && action == GLFW_PRESS)
		SceneManager::GetInstance()->GetActiveCamera()->Move(CameraMovement::LEFT, 1);

	if (key == GLFW_KEY_D && action == GLFW_PRESS)
		SceneManager::GetInstance()->GetActiveCamera()->Move(CameraMovement::RIGHT, 1);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
static void WindowResizeCallback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
static void MouseMoveCallback(GLFWwindow* window, double xpos, double ypos)
{
	static bool first = true;
	static float last_x = 0.f;
	static float last_y = 0.f;

	if (true == first)
	{
		last_x = xpos;
		last_y = ypos;
		first = false;
	}

	float xoffset = xpos - last_x;
	float yoffset = last_y - ypos;

	last_x = xpos;
	last_y = ypos;

	SceneManager::GetInstance()->GetActiveCamera()->MouseMove(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	//camera.ProcessMouseScroll(yoffset);
}

static void End(const char *error)
{
	glfwTerminate();

	std::cerr << error << std::endl;
	exit(EXIT_FAILURE);
}

void AppMain()
{
	GLFWwindow* window = nullptr;

	// initialise GLFW
	glfwSetErrorCallback(CallbackError);
	if (!glfwInit())
	{
		throw std::runtime_error("glfwInit failed");
	}

	// open a window with GLFW
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	window = glfwCreateWindow(Window::WIDTH, Window::HEIGHT, GAME_NAME.c_str(), NULL, NULL);

	if (!window)
	{
		throw std::runtime_error("glfwCreateWindow failed. Can your hardware handle OpenGL 3.2?");
	}

	// GLFW settings
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSetKeyCallback(window, CallbackKey);
	glfwSetFramebufferSizeCallback(window, WindowResizeCallback);
	glfwSetCursorPosCallback(window, MouseMoveCallback);
	glfwSetScrollCallback(window, ScrollCallback);

	// initialise GLEW
	glewExperimental = GL_TRUE; //stops glew crashing on OSX :-/
	if (glewInit() != GLEW_OK)
	{
		throw std::runtime_error("glewInit failed");
	}

	// print out some info about the graphics drivers
	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
	std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;

	// make sure OpenGL version 3.2 API is available
	if (!GLEW_VERSION_3_2)
	{
		throw std::runtime_error("OpenGL 3.2 API is not available.");
	}

	// Init managers
	ResourceManager::GetInstance()->Init("./res/xmls/resources.xml");
	SceneManager::GetInstance()->Init("./res/xmls/scene_test.xml");

	// ------------------------------------

	// run while the window is open
	while (!glfwWindowShouldClose(window)) {
		// process pending events
		glfwPollEvents();

		SceneManager::GetInstance()->Update();
		std::cout << "After update: " << glGetError() << std::endl;

		SceneManager::GetInstance()->Draw();

		// swap the display buffers (displays what was just drawn)
		glfwSwapBuffers(window);

		std::cout << "End of loop: " << glGetError() << std::endl;
	}

	// clean up and exit
	glfwTerminate();
}

int main(void)
{
	try {
		AppMain();
	}
	catch (const std::exception& e) {
		std::cerr << "ERROR: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
