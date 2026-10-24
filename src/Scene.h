#pragma once

#include "Ball.h"
#include "Blocks.h"
#include "LevelLoader.h"
#include "Paddle.h"

#include <glm/glm.hpp>

// get the color depending on the brick type
glm::vec4 getBlockColor(Blocks block);

// draws a level
void drawLevel(Level level);

// draws all powerups that are falling
void drawPowerups();

// draw the time spent in the game
void drawTime(float textSize);

// draws the enitre scene
void drawScene(Paddle& paddle, Ball& ball);
