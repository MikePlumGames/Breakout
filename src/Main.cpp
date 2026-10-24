#include "Breakout.h"

#include "Audio.h"
#include "Ball.h"
#include "Debug.h"			// even though visual studio thinks it is not used it actually is needed
#include "GameUpdate.h"
#include "Keys.h"
#include "Levels.h"
#include "Menus.h"
#include "Scene.h"
#include "Setup.h"
#include "Stats.h"
#include "Paddle.h"
#include "TextRenderer.h"
#include "Textures.h"

#include <glad/glad.h>		// openGL functions
#include <GLFW/glfw3.h>		// window creation
//#include <stb_image.h>	// images
#include <glm/glm.hpp>		// matricies and vectores
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <string>


// hide the console in release mode - only works in visual studio - not needed after adding CMake to the project
//#if defined(_MSC_VER) && defined(_WIN32) && defined(NDEBUG)
//	#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
//#endif

// function declarations
void processInput(GLFWwindow* window, Paddle& paddle, Ball& ball);
void updateViewport(GLFWwindow* window);
void calculateDeltaTime();
void printFPS();
void getMousePos(GLFWwindow* window);


// settings

// game width and height so we can add UI around the scene
int g_gameWidth{ 800 };
int g_gameHeight{ 600 };
// offsets for UI
int g_windowTopOffset{ 50 };
int g_windowBottomOffset{ 50 };
int g_windowLeftOffset{ 0 };
int g_windowRightOffset{ 0 };
// float versions are commonly used across this project
float g_gameWidthFloat{ static_cast<float>(g_gameWidth) };
float g_gameHeightFloat{ static_cast<float>(g_gameHeight) };
// screen width and height
int g_screenWidth{ g_gameWidth + g_windowLeftOffset + g_windowRightOffset };
int g_screenHeight{ g_gameHeight + g_windowTopOffset + g_windowBottomOffset };

// const starting values
const int g_constScreenWidth{ g_screenWidth };
const int g_constScreenHeight{ g_screenHeight };

// for the projection
const float g_closePlane{ -1.0f };
const float g_farPlane{ 1.0f };

// global variable informing that the window has been resized
bool g_hasWindowResized{ false };

// background color
glm::vec4 g_backgroundColor{ 0.062f, 0.062f, 0.078f, 1.0f };

// game state
GameState g_gameState{ GAME_ACTIVE };

// mouse - pos and which buttons are clicked
Mouse g_mouse;

//delta time - making sure that you move at constant speed regardless the frame rate
float g_deltaTime{ 0.0f };	// Time between current frame and last frame
float g_LastFrame{ 0.0f };	// Time of last frame

// for fps
double g_timeLastSecond{ glfwGetTime() };
int g_numberOfFrames{ 0 };

const std::string g_gameVersion{ "1.0" };

int main()
{
	// initialize GLFW, glad and create a window, setup debug context
	GLFWwindow* window = setup();
	if (window == nullptr)
	{
		std::cout << "main error: Application setup failed" << std::endl;
		return -1;
	}

	//--------------------
	// configure openGL state
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//--------------------

	// initialize the audio
	bool audio = audioInit();
	if (!audio)
	{
		std::cout << "main error: audio setup failed" << std::endl;
	}
	// start the background music
	setMusicVolume(0.5f);
	startMusic(g_backgroundMusic);
	
	// load the textures
	Textures::loadAll();
	// load the levels
	Levels::loadAll();
	// activate the first level
	Levels::activeLevel = 0;

	// load the fonts
	Fonts::loadAllFonts();
	
	// load all menus
	Menus::init();
	
	
	//--------------------
	// load the entities

	Paddle paddle;

	Ball ball(paddle);

	// load stats
	Stats::readFromFile();

	// main loop
	while (!glfwWindowShouldClose(window))
	{
		// calculating per-frame time logic
		calculateDeltaTime();
		// to prevent delta time buildups and pasuing with the top bar
		if (g_deltaTime > 0.05f)
		{
			g_deltaTime = 0.05f;
			Keys::resetAll();
		}
		// print FPS
		printFPS();

		// processing user's input
		processInput(window, paddle, ball);
		// update the screen parameters and viewport
		updateViewport(window);
		// get mouse pos
		getMousePos(window);

		//--------------------

		// update the game
		gameUpdate(paddle, ball);

		//--------------------
		
		// bg colour
		glClearColor(g_backgroundColor.r, g_backgroundColor.g, g_backgroundColor.b, g_backgroundColor.a);
		// clear the color buffer and the z buffer / depth buffer and the stencil buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// draws everything
		drawScene(paddle, ball);

		//--------------------

		// if it was true during this frame
		g_hasWindowResized = false;
		// reset the mosue clicked buttons
		g_mouse.resetState();

		// update the key logic
		Keys::update();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	// de-allocating resources
	
	// save the stats
	Stats::writeToFile();

	// cleanup of the audio
	pauseMusic(g_backgroundMusic);
	audioCleanup();

	glfwTerminate();
	return 0;
}

// processing user's input
void processInput(GLFWwindow* window, Paddle& paddle, Ball& ball)
{
	// Esc
	if (Keys::keyClickedNow(Key::KEY_ESCAPE))
	{
		// close the game if in main menu
		if (Menus::activeMenu == MAIN_MENU)
			glfwSetWindowShouldClose(window, true);
		// pause if playing the game
		else if (Menus::activeMenu == GAME_PLAY)
		{
			// playthrough pause if in playthrough
			if (Menus::gameplayMenu.gamePlayMenuType == GAME_PLAY_PLAYTHROUGH)
				Menus::setActiveMenu(PLAYTHROUGH_PAUSE_MENU);
			// level pause if in a level
			else if (Menus::gameplayMenu.gamePlayMenuType == GAME_PLAY_LEVEL)
				Menus::setActiveMenu(LEVEL_PAUSE_MENU);
		}
		// go back if in settings/statistics/chhosing a level/about
		else if (Menus::activeMenu == SETTINGS_MENU || Menus::activeMenu == VIEW_STATS_MENU || Menus::activeMenu == CHOOSE_LEVEL_MENU || Menus::activeMenu == MAIN_ABOUT_MENU)
			Menus::setActiveMenu(Menus::previousActiveMenu);
		// start playing if pasued
		else if (Menus::activeMenu == PLAYTHROUGH_PAUSE_MENU || Menus::activeMenu == LEVEL_PAUSE_MENU)
			Menus::setActiveMenu(GAME_PLAY);
		// go to main menu if completed playthrough
		else if (Menus::activeMenu == PLAYTHROUGH_END_MENU)
			Menus::setActiveMenu(MAIN_MENU);
	}

	// R - reset
	if (Keys::keyClickedNow(Key::KEY_R) && Menus::activeMenu == GAME_PLAY)
		g_gameState = GAME_LOSE;


	// M - music on/off
	if (Keys::keyClickedNow(Key::KEY_M))
	{
		if (getMasterVolume() != 0.0f)
		{
			g_masterVolumeCurrent = getMasterVolume();
			setMasterVolume(0.0f);
			std::cout << "MUTED" << std::endl;
		}
		else
		{
			setMasterVolume(g_masterVolumeCurrent);
			std::cout << "UNMUTED" << std::endl;
		}
	}

	if (Keys::keyPressed(Key::KEY_LEFT) || Keys::keyPressed(Key::KEY_A))
		paddle.movingLeft = true;
	else
		paddle.movingLeft = false;


	if (Keys::keyPressed(Key::KEY_RIGHT) || Keys::keyPressed(Key::KEY_D))
		paddle.movingRight = true;
	else
		paddle.movingRight = false;

	// ball
	if (ball.isStuck == true && (Keys::keyPressed(Key::KEY_UP) || Keys::keyPressed(Key::KEY_W)))
		ball.shouldLaunch = true;
}

// update the screen parameters and viewport
void updateViewport(GLFWwindow* window)
{
	// update screen parameters
	glfwGetFramebufferSize(window, &g_screenWidth, &g_screenHeight);
	glViewport(0, 0, g_screenWidth, g_screenHeight);
	g_gameWidth = g_screenWidth - ( g_windowLeftOffset + g_windowRightOffset);
	g_gameHeight = g_screenHeight - (g_windowTopOffset + g_windowBottomOffset);
	g_gameWidthFloat = static_cast<float>(g_gameWidth);
	g_gameHeightFloat = static_cast<float>(g_gameHeight);
}

// calculating per-frame time logic
void calculateDeltaTime()
{
	float currentFrame = getTime();
	g_deltaTime = currentFrame - g_LastFrame;
	g_LastFrame = currentFrame;
}

// print fps to the console
void printFPS()
{
	g_numberOfFrames++;
	double currentTime{ glfwGetTime() };

	if (currentTime - g_timeLastSecond >= 1.0)
	{
		double FPS{ static_cast<double>(g_numberOfFrames) / (currentTime - g_timeLastSecond) };
		std::cout << "Fps: " << FPS << std::endl;
		g_timeLastSecond = currentTime;
		g_numberOfFrames = 0;
	}
}

// set the pos variable in g_mouse
void getMousePos(GLFWwindow* window)
{
	double posX{};
	double posY{};

	glfwGetCursorPos(window, &posX, &posY);

	g_mouse.pos.x = static_cast<float>(posX);
	g_mouse.pos.y = static_cast<float>(posY);

	//std::cout << "Mouse Pos: " << posX << ", " << posY << std::endl;
	//std::cout << "Left, right, middle : " << g_mouse.leftClick << ", " << g_mouse.rightClick << ", " << g_mouse.middleClick << std::endl;
}

// return time as float
float getTime()
{
	return static_cast<float>(glfwGetTime());
}