
#include <iostream>
#include <vector>

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include "Strings.h"
#include "Constants.h"
#include "SceneObject.h"


static void CallbackError(int error, const char* description)
{
	std::cerr << "Error: " << description << std::endl;
}

static void CallbackKey(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
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

	// DELETE ----------------------------

	ShaderResource sr;
	sr.id = "1";
	sr.vs_path = "D:\\workspace\\ThisIsAGame\\ThisIsAGame\\default.vert";
	sr.fs_path = "D:\\workspace\\ThisIsAGame\\ThisIsAGame\\default.frag";

	TextureResource tr;
	tr.id = "1";
	tr.texture_path = "D:\\workspace\\ThisIsAGame\\ThisIsAGame\\res\\textures\\Bus.tga";
	tr.type = GL_TEXTURE_2D;
	tr.min_filter = GL_LINEAR;
	tr.mag_filter = GL_LINEAR;
	tr.wrap_s = GL_REPEAT;
	tr.wrap_t = GL_REPEAT;

	ModelResource mr;
	mr.id = "1";
	mr.model_path = "D:\\workspace\\ThisIsAGame\\ThisIsAGame\\res\\models\\sphere.obj";

	Shader shader(&sr);
	shader.Load();

	Texture tex(&tr);
	tex.Load();

	Model model(&mr);
	model.Load();

	SceneObject so(glm::vec3(0.f), glm::vec3(0.f), glm::vec3(1.f), false, "test");
	so.SetModel(&model);
	so.SetShader(&shader);
	so.AddTexture(&tex);

	// ------------------------------------

	// run while the window is open
	while (!glfwWindowShouldClose(window)) {
		// process pending events
		glfwPollEvents();

		so.Update();
		so.Draw();

		// swap the display buffers (displays what was just drawn)
		glfwSwapBuffers(window);
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
