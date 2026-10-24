#pragma once

#include "Ball.h"
#include "Paddle.h"

// update the game each frame
void gameUpdate(Paddle& paddle, Ball& ball);
// total reset at the begining
void totalGameReset(Paddle& paddle, Ball& ball);
// resets the game
void gameReset(Paddle& paddle, Ball& ball);
// checks if the level is empty
bool isLEvelEmpty(Level level);
