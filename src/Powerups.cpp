#include "Powerups.h"

#include "Audio.h"
#include "Blocks.h"
#include "Breakout.h"
#include "Levels.h"

#include <algorithm>
#include <random>
#include <iostream>

void Powerups::spawn(int index, const Level& level)
{
	// generate a random number
	static std::random_device seed;
	static std::mt19937 gen(seed());

	std::bernoulli_distribution spawn(0.3f);	// 30% chance of spawning a powerup
	std::bernoulli_distribution spawnNegative(0.40);

	if (!spawn(gen) || FirstBrockenBlock::type == EMPTY) return;	// if we don't spawn or we haven't brocken a block we return

	// check if the active level allows for positive powerups
	bool allowsPositivePowerups{ std::find(levelsWithPositivePowerups.begin(), levelsWithPositivePowerups.end(), level.ID) != levelsWithPositivePowerups.end() };
	// check if the active level allows for negative powerups
	bool allowsNegativePowerups{ std::find(levelsWithNegativePowerups.begin(), levelsWithNegativePowerups.end(), level.ID) != levelsWithNegativePowerups.end() };

	// select which type of the powerup we will create
	if (spawnNegative(gen) && allowsNegativePowerups ||  (!allowsPositivePowerups && allowsNegativePowerups))
	{
		// spawn a negative powerup	(5, 6) - IDs of negative powerups
		std::uniform_int_distribution<int> powerupType(1, 5);

		switch (powerupType(gen))
		{
		// SCREEN_CONFUSE - 3/5 chances ~60%
		case 1: 
		case 2:
		case 3:
		{
			powerupsList[index].type = SCREEN_CONFUSE;
			powerupsList[index].duration = 3.0f;
			break;
		}
		// SCREEN_CHAOS - 2/5 chances ~40%
		case 4:
		case 5:
		{
			powerupsList[index].type = SCREEN_CHAOS;
			powerupsList[index].duration = 3.0f;
			break;
		}
		}

	}
	else if (allowsPositivePowerups)
	{
		// spawn a positive powerup (1, 4) - IDs of positive powerups
		std::uniform_int_distribution<int> powerupType(1, 11);

		switch (powerupType(gen))
		{
		// PLAYER_SPEED_INCREASE - 3/11 chances ~27%
		case 1:
		case 2:
		case 3:
		{
			powerupsList[index].type = PLAYER_SPEED_INCREASE;
			powerupsList[index].duration = 5.0f;
			break;
		}
		// PLAYER_SIZE_INCREASE - 3/11 chances ~27%
		case 4:
		case 5:
		case 6:
		{
			powerupsList[index].type = PLAYER_SIZE_INCREASE;
			powerupsList[index].duration = 5.0f;
			break;
		}
		// PLAYER_STICKY_PADDLE - 3/11 chances ~27%
		case 7:
		case 8:
		case 9:
		{
			powerupsList[index].type = PLAYER_STICKY_PADDLE;
			powerupsList[index].duration = 5.0f;
			break;
		}
		// BALL_PASS_THROUGH_BLOCKS - 2/11 chances ~18%
		case 10:
		case 11:
		{
			powerupsList[index].type = BALL_PASS_THROUGH_BLOCKS;
			powerupsList[index].duration = 3.0f;
			break;
		}
		}
	}
	else return;	// if we don't spawn any powerups return

	// actually spawn the powerup

	//std::cout << "Powerup spawned: " << powerupsList[index].type << std::endl;
	std::cout << "Powerup spawned: " << powerupsList[index].type << ", used: " << powerupsInUse << std::endl;

	// set it's variables
	powerupsList[index].falling = true;
	powerupsList[index].active = false;
	powerupsInUse++;

	// spawn it inside the broken block

	float HalfHeight{ g_gameHeightFloat / 2.0f };	// because blocks are supposed to be only on half the screen height

	float blockWidth{ g_gameWidthFloat / level.columns };
	float blockHeight{ static_cast<float>(HalfHeight) / level.rows };

	// get the center of the brocken block
	glm::vec2 blockPos{ FirstBrockenBlock::pos };
	blockPos.x += blockWidth / 2.0f;
	blockPos.y += blockHeight / 2.0f;

	// set the pos of the powerup at the center of the broken block (because pos is top left corner we subtract half of the size)
	powerupsList[index].pos = glm::vec2(blockPos.x - powerupsList[index].size.x / 2.0f, blockPos.y - powerupsList[index].size.y / 2.0f);

}

void Powerups::update(glm::vec2 paddlePos, glm::vec2 paddleSize)
{
	// recalculate the speed
	velocity = velocityConstant * g_gameHeightFloat;
	
	// recalculate the size
	for (int i = 0; i < powerupsList.size(); i++)
	{
		powerupsList[i].size = { g_gameWidthFloat / 12.0f, g_gameHeightFloat / 20.0f };
	}


	// attempt to spawn a powerup if there is a free powerup index
	int freeIndex = freePowerupIndex();
	// -1 means no index found
	if (freeIndex != -1)
		spawn(freeIndex, Levels::mainLevels[Levels::activeLevel]);

	// loop through all powerups
	for (int i = 0; i < powerupsList.size(); i++)
	{
		// if the poweup is falling
		if (powerupsList[i].falling)
		{
			// move the powerup down
			powerupsList[i].pos.y += velocity * g_deltaTime;

			// check colisions with screen
			if (checkScreenColisions(powerupsList[i]))
			{
				deactivate(powerupsList[i]);
				continue;
			}
			// check colisions with player's paddle
			if (checkPaddleColisions(powerupsList[i], paddlePos, paddleSize))
			{
				// if powerup was collected play a sfx
				playSFX(SoundEffects::powerup, 0.2f);

				// check to make sure we don't activate two screen effects at the same time
				if ((powerupsList[i].type == SCREEN_CHAOS && isActive(SCREEN_CONFUSE)) || (powerupsList[i].type == SCREEN_CONFUSE && isActive(SCREEN_CHAOS)))
				{
					std::cout << "incompatible powerups!!!!!\n";
					deactivate(powerupsList[i]);
				}

				powerupsList[i].falling = false;
				powerupsList[i].active = true;
				// duration is already set in spawn();
				powerupsList[i].pos = { 0.0f, 0.0f };
				continue;	// so we have the new powerups active from the next frame
			}
		}

		// if the powerup is active
		if (powerupsList[i].active)
		{
			// decrease duration
			powerupsList[i].duration -= g_deltaTime;
			//std::cout << "Duration: " << Powerups::powerupsList[i].duration << std::endl;
			if (powerupsList[i].duration <= 0.0f)
				deactivate(powerupsList[i]);

			// apply powerup effects
		}
	}

}

int Powerups::freePowerupIndex()
{
	// if all powerups are used instanly return. powerupsInUse shouldn't ever be greater but just in case
	if (powerupsInUse >= maxPowerups) return -1;

	for (int i = 0; i < powerupsList.size(); i++)
	{
		if (!powerupsList[i].active && !powerupsList[i].falling)
		{
			//std::cout << "Free index found at: " << i << ", used: " << powerupsInUse << std::endl;
			return i;
		}

	}
	// return -1 if all powerups are used
	return -1;
}

bool Powerups::checkScreenColisions(const Powerup& powerup)
{
	if (powerup.pos.y + powerup.size.y > g_gameHeightFloat || powerup.pos.y < 0 ||
		powerup.pos.x + powerup.size.x > g_gameWidthFloat || powerup.pos.x < 0)
		return true;
	else return false;
}

bool Powerups::checkPaddleColisions(const Powerup& powerup, glm::vec2 paddlePos, glm::vec2 paddleSize)
{
	bool colisionX{ powerup.pos.x + powerup.size.x >= paddlePos.x && paddlePos.x + paddleSize.x >= powerup.pos.x };
	bool colisionY{ powerup.pos.y + powerup.size.y >= paddlePos.y && paddlePos.y + paddleSize.y >= powerup.pos.y };

	return colisionX && colisionY;
}

bool Powerups::isActive(PowerupType type)
{
	if (type != PLAYER_SPEED_INCREASE && type != PLAYER_SIZE_INCREASE && type != PLAYER_STICKY_PADDLE &&
		type != BALL_PASS_THROUGH_BLOCKS && type != SCREEN_CONFUSE && type != SCREEN_CHAOS)
	{
		std::cout << "Powerups::isActive error: invalid type for argument: PowerupType type" << std::endl;
		return false;
	}

	for (int i = 0; i < powerupsList.size(); i++)
	{
		if (powerupsList[i].active && powerupsList[i].type == type) return true;
	}
	return false;
}

float Powerups::getActiveTime(PowerupType type)
{
	if (type != PLAYER_SPEED_INCREASE && type != PLAYER_SIZE_INCREASE && type != PLAYER_STICKY_PADDLE &&
		type != BALL_PASS_THROUGH_BLOCKS && type != SCREEN_CONFUSE && type != SCREEN_CHAOS)
	{
		std::cout << "Powerups::getActiveTime error: invalid type for argument: PowerupType type" << std::endl;
		return 0.0f;
	}
	float timeLeft{};
	for (int i = 0; i < powerupsList.size(); i++)
	{
		if (powerupsList[i].active && powerupsList[i].type == type)
		{
			if (powerupsList[i].duration > timeLeft) timeLeft = powerupsList[i].duration;
		}
	}
	return timeLeft;
}

void Powerups::deactivate(Powerup& powerup)
{
	std::cout << "Powerup destroyed: " << powerup.type << std::endl;

	powerup.active = false;
	powerup.falling = false;
	powerup.pos = { 0.0f, 0.0f };
	powerup.type = POWERUP_NONE;
	powerupsInUse = powerupsInUse == 0 ? 0 : powerupsInUse - 1; // decrease the variable without going below 0, -- doesn't work

	// remove powerup effects - all that handeled independently where necessary
}

void Powerups::clear()
{
	for (int i = 0; i < powerupsList.size(); i++)
	{
		deactivate(powerupsList[i]);
	}
	powerupsInUse = 0;
	// clear the last broken block to not cause issues
	FirstBrockenBlock::type = EMPTY;
	FirstBrockenBlock::pos = glm::vec2(0.0f, 0.0f);
}

/*

The plan:



#spawn();
#	- get the active level
#	- get first block broken this frame
#		- type
#		- ID
#
#	- calculate a random chance
#	- spawn if:
#		- chance
#		- active level allows
#		- a block was broken
#		- this powerup is not active and not falling
#
#	- set falling = true
#	- spawn at:
#		- center of the broken block:

update();
	- if window resized:
		- recalulate the powerup's speed

#	- spawn();
#
#	- loop through all:
#
#		- if falling:
#			- move down
#			- check colisions
#			- if falls out of the screen:
#				- set falling = false
#				- reset pos to 0.0f
#			- else if colides:
#				- set falling = false
#				- set active = true
#				- set duration	// that's set in spawn();
#				- reset pos to 0.0f
#	
#		- else if active
#			- decrease duration
#			- if duration <= 0.0f
#				deactivate();
!!!!		// everything else happens either here or in specific functions for a given thing affected by the powerup

deactivate();
#	- set falling = false
#	- set active = false
#	- set duration = 0.0f
!!!!// undo any changes if necessary as some will be made elsewere based on active == false

clear();
#	- loop through all:
#		- deactivate();











// TO DO!!!!!!!!!!!!!!!!!!!


- add functioanlity to powerups
# fix not respawning bug
- fix fbo with minimalized screen bug


*/