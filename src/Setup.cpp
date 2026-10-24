//#include "Setup.h" // not needed and breaks as we attach: GLFW/glfw3.h before glad/glad.h

#include "Breakout.h"
#include "Debug.h"
#include "Keys.h"

#include <glad/glad.h>		// openGL functions
#include <GLFW/glfw3.h>		// window creation
#include <stb_image.h>

#include <iostream>
#include <string>

// function declarations

//resize the window
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
// get which mouse buttons were clicked
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
// called after key press
void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

// initialize GLFW, glad and create a window, setup debug context
GLFWwindow* setup()
{
	// initializing glfw
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);	// enable debug context step 1
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	// creating a window
	std::string gameTitle{ "Breakout "};
#if DEVELOPMENT_BUILD
	gameTitle += "(DEV TEST) ";
#endif 
	gameTitle += g_gameVersion;

	GLFWwindow* window = glfwCreateWindow(g_screenWidth, g_screenHeight, gameTitle.c_str(), NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return nullptr;
	}
	glfwMakeContextCurrent(window);

	// initializing glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return nullptr;
	}

	// telling the size of the window to opengl
	glViewport(0, 0, g_screenWidth, g_screenHeight);

	// telling glfw to resize the viewport according to the window
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// ensure a minimum window size
	glfwSetWindowSizeLimits(window, g_windowLeftOffset + g_windowRightOffset + 400, g_windowTopOffset + g_windowBottomOffset + 350, GLFW_DONT_CARE, GLFW_DONT_CARE);

	// set the callback for mouse clicks
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	// set the callback after pressing a key
	glfwSetKeyCallback(window, glfw_key_callback);

	// enable debug context step 2
	setupOpenGLDebugOutput();

	// set up the window icon
	GLFWimage image{};
	int channels{};
	image.pixels = stbi_load("assets/textures/icon.png", &image.width, &image.height, &channels, 4);
	if (image.pixels)
	{
		glfwSetWindowIcon(window, 1, &image);
		stbi_image_free(image.pixels);
	}
	else
		std::cout << "setup error: failed to load game icon" << std::endl;

	return window;
}

//changing viewport after resizing the window to match new dimensions
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	g_hasWindowResized = true;
}

// get which mouse buttons were clicked
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	// check if any button was clicked
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		g_mouse.leftClick = true;
	}
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		g_mouse.rightClick = true;
	}
	if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS)
	{
		g_mouse.middleClick = true;
	}
	// check if any button is held
	if (button == GLFW_MOUSE_BUTTON_LEFT)
	{
		g_mouse.leftClickHeld = (action == GLFW_PRESS);
	}
	if (button == GLFW_MOUSE_BUTTON_RIGHT)
	{
		g_mouse.rightClickHeld = (action == GLFW_PRESS);
	}
	if (button == GLFW_MOUSE_BUTTON_MIDDLE)
	{
		g_mouse.middleClickHeld = (action == GLFW_PRESS);
	}
}

// called after key press
void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Keys::keyInputCallback(key, action);
}