
#include <iostream>
#include <vector>

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include "Strings.h"
#include "Constants.h"
#include "Shader.h"
#include "Texture.h"


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
	sr.vs_path = "D:\\workspace\\ThisIsAGame\\ThisIsAGame\\default.vs";
	sr.fs_path = "D:\\workspace\\ThisIsAGame\\ThisIsAGame\\default.fs";

	TextureResource tr;
	tr.id = "1";
	tr.texture_path = "D:\\workspace\\ThisIsAGame\\ThisIsAGame\\Resources\\Textures\\Croco.tga";
	tr.type = GL_TEXTURE_2D;
	tr.min_filter = GL_LINEAR;
	tr.mag_filter = GL_LINEAR;
	tr.wrap_s = GL_REPEAT;
	tr.wrap_t = GL_REPEAT;

	Shader shader(&sr);
	shader.Load();

	Texture tex(&tr);
	tex.Load();

	GLuint gVAO = 0;
	GLuint gVBO = 0;

	glGenVertexArrays(1, &gVAO);
	glBindVertexArray(gVAO);

	// make and bind the VBO
	glGenBuffers(1, &gVBO);
	glBindBuffer(GL_ARRAY_BUFFER, gVBO);

	// Put the three triangle verticies into the VBO
	GLfloat vertexData[] = {
		//  X     Y     Z       U     V
		0.0f, 0.8f, 0.0f,   0.5f, 1.0f,
		-0.8f,-0.8f, 0.0f,   0.0f, 0.0f,
		0.8f,-0.8f, 0.0f,   1.0f, 0.0f,
	};
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

	shader.SendAttribute(ShaderStrings::POSITION_ATTRIBUTE, 3, 0, 0);
	shader.SendAttribute(ShaderStrings::UV_ATTRIBUTE, 2, 5 * sizeof(GLfloat), (3 * sizeof(GLfloat)));

	// unbind the VBO and VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// ------------------------------------

	// run while the window is open
	while (!glfwWindowShouldClose(window)) {
		// process pending events
		glfwPollEvents();

		// clear everything
		glClearColor(0, 0, 0, 1); // black
		glClear(GL_COLOR_BUFFER_BIT);

		// bind the program (the shaders)
		glUseProgram(shader.GetProgramID());

		// bind the VAO (the triangle)
		glBindVertexArray(gVAO);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex.GetID());

		glm::mat4 MVP = glm::mat4(1.f);
		shader.SendUniform(ShaderStrings::MVP_UNIFORM, MVP);

		// draw the VAO
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// unbind the VAO
		glBindVertexArray(0);

		// unbind the program
		glUseProgram(0);

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
