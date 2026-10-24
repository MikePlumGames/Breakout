#include "GameUpdate.h"

#include "Audio.h"
#include "Ball.h"
#include "Blocks.h"
#include "Breakout.h"
#include "LevelLoader.h"
#include "Levels.h"
#include "Menus.h"
#include "Paddle.h"
#include "Powerups.h"
#include "Stats.h"

#include <glm/glm.hpp>

#include <iostream>

// update the game each frame
void gameUpdate(Paddle& paddle, Ball& ball)
{
	// pause the background music if the window is minimized
	if (g_screenWidth == 0 && g_screenHeight == 0)
	{
		pauseMusic(g_backgroundMusic);
		return;	// if window minimalized do not update
	}
	else
		startMusic(g_backgroundMusic);

	// menus
	if (g_hasWindowResized)
		Menus::resize();

	if (Menus::activeMenu == MAIN_MENU)
	{
		Menus::mainMenu.update(paddle.pos, ball.pos, ball.velocity, ball.isStuck, ball.resetParticles);
		return;
	}
	if (Menus::activeMenu == PLAYTHROUGH_END_MENU)
	{
		Menus::playthroughEndMenu.update();
		return;
	}
	if (Menus::activeMenu == CHOOSE_LEVEL_MENU)
	{
		Menus::levelPickerMenu.update();
		return;
	}
	if (Menus::activeMenu == VIEW_STATS_MENU)
	{
		Menus::statisticsMenu.update();
		return;
	}
	if (Menus::activeMenu == SETTINGS_MENU)
	{
		Menus::settingsMenu.update();
		return;
	}
	if (Menus::activeMenu == MAIN_ABOUT_MENU)
	{
		Menus::aboutMenu.update();
		return;
	}
	// resize update cause we can see the game behind the menu
	if (g_hasWindowResized)
	{
		gameReset(paddle, ball);
		if (Menus::gameplayMenu.gamePlayMenuType == GAME_PLAY_PLAYTHROUGH) Stats::attemptsPlaythrough++;
		else Stats::attemptsLevel++;
	}
	if (Menus::activeMenu == PLAYTHROUGH_PAUSE_MENU)
	{
		Menus::playthroughPauseMenu.update(paddle.pos, ball.pos, ball.velocity, ball.isStuck);
		return;
	}
	if (Menus::activeMenu == LEVEL_PAUSE_MENU)
	{
		Menus::levelPauseMenu.update();
		return;
	}


	// else render normal game
	if (Menus::activeMenu != GAME_PLAY)
	{
		return;
	}
	
	// actuall game play screen

	switch (g_gameState)
	{
	case GAME_ACTIVE:
	{
		paddle.update();
		ball.update(paddle);
		Powerups::update(paddle.pos, glm::vec2(paddle.width, paddle.height));
		if (Menus::gameplayMenu.gamePlayMenuType == GAME_PLAY_PLAYTHROUGH) Stats::timePlaythrough += g_deltaTime;
		else Stats::timeLevel += g_deltaTime;
		break;
	}
	case GAME_LOSE:
	{
		gameReset(paddle, ball);
		g_gameState = GAME_ACTIVE;
		if (Menus::gameplayMenu.gamePlayMenuType == GAME_PLAY_PLAYTHROUGH)
		{
			Stats::timePlaythrough += g_deltaTime;
			Stats::attemptsPlaythrough++;
		}
		else
		{
			Stats::timeLevel += g_deltaTime;
			Stats::attemptsLevel++;
		}
		
		Stats::attemptsTotal++;
		break;
	}
	case GAME_WIN:
	{
		// if played a level go back to level picker menu
		if (Menus::gameplayMenu.gamePlayMenuType == GAME_PLAY_LEVEL)
		{
			Menus::setActiveMenu(CHOOSE_LEVEL_MENU);
			return;
		}
		
		// unlock completed level
		if (Menus::gameplayMenu.gamePlayMenuType == GAME_PLAY_PLAYTHROUGH && Stats::levelsUnlocked < Levels::activeLevel)
			Stats::levelsUnlocked = Levels::activeLevel;
		
		Levels::activeLevel++;	// advance to next level
		if (Levels::activeLevel >= Levels::totalLevels)
		{
			// this has to be on to avoid exception: out of range error in: if (isLEvelEmpty(Levels::mainLevels.at(Levels::activeLevel)))
			Levels::activeLevel = 0; // after completing all levels loop back to the first level
			if (Menus::gameplayMenu.gamePlayMenuType == GAME_PLAY_PLAYTHROUGH)
			{
				Stats::finishedPlaythrough = true;	// so best stats in statistics are dispayed
				Menus::playthroughEndMenu.calculatedStats = false;	// to recalculate stats
				Menus::setActiveMenu(PLAYTHROUGH_END_MENU);
				return;
			}	
			// these cannot be on or they will break the end menu
			
			//Stats::timePlaythrough = 0.0f;		// reset the playtime
			//Stats::attemptsPlaythrough = 0;	// reset the attempts
		}
		gameReset(paddle, ball);
		g_gameState = GAME_ACTIVE;
		break;
	}
	case GAME_RESET:	// reset the playthrough
		totalGameReset(paddle, ball);
		g_gameState = GAME_ACTIVE;
		break;
	case GAME_LEVEL_RESET: // reset the level
		gameReset(paddle, ball);
		Stats::attemptsLevel = 0;
		Stats::timeLevel = 0;
		g_gameState = GAME_ACTIVE;
		break;
	default:
		std::cerr << "gameUpdate error: unexpected game state!\n";
	}

	// check if the level is empty
	if (isLEvelEmpty(Levels::mainLevels.at(Levels::activeLevel)))
		g_gameState = GAME_WIN;

	// process pause button
	Menus::gameplayMenu.update();
}

// total reset at the begining
void totalGameReset(Paddle& paddle, Ball& ball)
{
	Levels::activeLevel = 0;
	Stats::attemptsPlaythrough = 0;
	Stats::timePlaythrough = 0.0f;
	paddle.reset();
	ball.isStuckToStickyPaddleTop = false;	// to force the sticky powerup off
	ball.reset(paddle);
	Levels::mainLevels.at(Levels::activeLevel).reset();	// reset the current level
	Powerups::clear();
}

// resets the game
void gameReset(Paddle& paddle, Ball& ball)
{
	paddle.reset();
	ball.reset(paddle);
	//ball.particles.fullRestart();	// this is done in ball.reset(paddle);
	Levels::mainLevels.at(Levels::activeLevel).reset();	// reset the current level
	Powerups::clear();
}

// checks if the level is empty
bool isLEvelEmpty(Level level)
{
	for (int i = 0; i < level.data.size(); i++)
	{
		// if there is a breakable block (so exluding END_LINE, BLOCK_SOLID, EMPTY, ERROR_BLOCK_MISSING) the level is not empty
		if (level.data.at(i) != END_LINE && level.data.at(i) != BLOCK_SOLID && level.data.at(i) != EMPTY && level.data.at(i) != ERROR_BLOCK_MISSING)
			return false;
	}
	return true;
}