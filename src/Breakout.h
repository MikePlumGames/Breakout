#pragma once

//#include "Menus.h" for some reason breaks my code
#include <glm/glm.hpp>
#include <string>

// all extern variables in Breakout.h are from main.cpp


// screen width and height extern from main.cpp allows to use in every .cpp file
// they are updated in main loop

// float versions are comonly used in this project

extern int g_gameWidth;
extern int g_gameHeight;

extern int g_windowTopOffset;
extern int g_windowBottomOffset;
extern int g_windowLeftOffset;
extern int g_windowRightOffset;

extern float g_gameWidthFloat;
extern float g_gameHeightFloat;

extern int g_screenWidth;
extern int g_screenHeight;

extern const int g_constScreenWidth;
extern const int g_constScreenHeight;

// for the projection
extern const float g_closePlane;
extern const float g_farPlane;

// delta time
extern float g_deltaTime;

// variable informing that the window has been resized
extern bool g_hasWindowResized;


extern glm::vec4 g_backgroundColor;

extern const std::string g_gameVersion;

// enum showing the game state
enum GameState
{
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN,
    GAME_LOSE,
    GAME_COMPLETED,
    GAME_RESET,
    GAME_LEVEL_RESET
};

// global variable showing the state of the game
extern GameState g_gameState;


// function from Main.cpp to avoid including glfw in other project files
float getTime();

// mouse - pos and which buttons are clicked
struct Mouse
{
    glm::vec2 pos{ 0.0f, 0.0f };

    bool leftClick{ false };
    bool rightClick{ false };
    bool middleClick{ false };

    bool leftClickHeld{ false };
    bool rightClickHeld{ false };
    bool middleClickHeld{ false };

    // reset the bools after the and of the frame
    void resetState()
    {
        leftClick = false;
        rightClick = false;
        middleClick = false;
    }
};
extern Mouse g_mouse;