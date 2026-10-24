#pragma once

#include "Breakout.h"
#include "Colision.h"
#include "LevelLoader.h"
#include "Paddle.h"
#include "Particles.h"

#include <glm/glm.hpp>

struct Ball
{
	glm::vec2 pos{};

	float width{};
	float height{};

	float defaultWidth{ (g_gameWidthFloat / 6.0f) / 3.0f }; // (paddle.width) / 3.0f

	float radius{};

	bool isStuck{ true };					// true if the ball is stuck to the paddle
	bool isDead{ false };					// true if the ball falls outside the map
	bool shouldLaunch{ false };				// true if the ball should be launched by pressing up/w key
	bool hitSolidBlock{ false };			// true if the ball colides with a solid block, creates the shake effect also used by powerups
	bool resetParticles{ false };			// true if the particles need to be reset

	// for the sticky paddle powerup
	bool isStuckToStickyPaddleTop{ false };	// true if the ball is stuck to a paddle bc of a sticky powerup, makes the ball stick not in the center of the ball
	float stickyBallOffset{};	// offset from the paddle's center makes it so the ball sticks where it hit


	glm::vec4 colorNormal = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec4 colorPassThrough = glm::vec4(1.0f, 0.4f, 0.0f, 1.0f);
	glm::vec4 color = colorNormal;

	glm::vec2 velocityConstant{};	// %  how much screen does the paddle traverse in 1s
	glm::vec2 velocity{};

	const float bias{ 10.0f };	// makes the ball die after it's a bit of screen to make it more fair

	float overlapX{};	// overlaps - how far is the ball inside a colided object
	float overlapY{};

	// particles
	Particles particles{ 100 };


	void update(const Paddle &paddle);

	void reset(const Paddle& paddle);

	void checkScreenColisions();						// checks for colisions with screen edges
	void checkPaddleColisions(const Paddle& paddle);	// checks for colisions with the paddle
	void checkBlockColisions(Level& level);				// checks colisions with the level

	ColisionSide checkColision(glm::vec2 rectangularPos, float rectangularWidth, float rectangularHeight);

	// randomizes the starting velocity(velocityConstant) at the start of the movement
	void randomVelocity();

	// puts the ball on the paddle
	Ball(const Paddle& paddle);
};

//check block colisions function in case i mess up
/*



// checks for colisions with blocks
void Ball::checkBlockColisions(Level& level)
{
	float HalfHeight{ g_screenHeightFloat / 2.0f };	// because blocks are supposed to be only on half the screen height

	float blockWidth{ g_screenWidthFloat / level.columns };
	float blockHeight{ static_cast<float>(HalfHeight) / level.rows };
	glm::vec2 blockPos{ 0.0f, 0.0f };

	for (int i = 0; i < level.data.size(); i++)
	{
		if (level.data.at(i) == END_LINE)
		{
			// don't check colisions with new line, instead go to next row
			blockPos.y += blockHeight;
			blockPos.x = 0;
			continue;
		}
		else if (level.data.at(i) == EMPTY)
		{
			// go to next block
			blockPos.x += blockWidth;
			continue;
		}
		else
		{
			// handle colision
			ColisionSide colisionSide = checkColision(blockPos, blockWidth, blockHeight);

			if (colisionSide == LEFT) std::cout << "Colision: BLOCK LEFT at: " << i << std::endl;
			if (colisionSide == RIGHT) std::cout << "Colision: BLOCK RIGHT at: " << i << std::endl;
			if (colisionSide == TOP) std::cout << "Colision: BLOCK TOP at: " << i << std::endl;
			if (colisionSide == BOTTOM) std::cout << "Colision: BLOCK BOTTOM at: " << i << std::endl;


			if (colisionSide == NONE)
			{
				// go to next block
				blockPos.x += blockWidth;
				continue;
			}
			if (level.data.at(i) == BLOCK_SOLID)
			{
				hitSolidBlock = true;
				std::cout << "Block solid at: " << i << std::endl;
			}
			else if (level.data.at(i) != BLOCK_SOLID)
			{
				level.data.at(i) = EMPTY;
				std::cout << "Block Broken at: " << i << "\n";
			}

			const float ExtraStep{ 0.1f };	// ball is slightly moved away from the blocks

			float currentSpeed = glm::length(velocity);
			float minYspeed = currentSpeed * 0.5f;	// so the ball doesn't travel sideways

			switch (colisionSide)
			{
			case TOP:
			{
				pos.y -= (overlapY + ExtraStep);
				// bounce up
				velocity.y = -std::abs(velocity.y);

				velocity = glm::normalize(velocity) * currentSpeed;
				return;
			}
			case LEFT:
			{
				pos.x -= (overlapX + ExtraStep);
				// bounce left
				velocity.x = -std::abs(velocity.x);
				// fix the sideways movement
				if (std::abs(velocity.y) < minYspeed)
				{
					std::cout << "VELOCITY CORRECTED\n";
					velocity.y = (velocity.y < 0.0f) ? -minYspeed : minYspeed;
				}

				velocity = glm::normalize(velocity) * currentSpeed;
				return;
			}
			case RIGHT:
			{
				pos.x += (overlapX + ExtraStep);
				// bounce right
				velocity.x = std::abs(velocity.x);
				// fix the sideways movement
				if (std::abs(velocity.y) < minYspeed)
				{
					std::cout << "VELOCITY CORRECTED\n";
					velocity.y = (velocity.y < 0.0f) ? -minYspeed : minYspeed;
				}

				velocity = glm::normalize(velocity) * currentSpeed;
				return;
			}
			case BOTTOM:
			{
				pos.y += (overlapY + ExtraStep);
				// bounce down
				velocity.y = std::abs(velocity.y);

				velocity = glm::normalize(velocity) * currentSpeed;
				return;
			}
			}
			// go to next block
			blockPos.x += blockWidth;
		}
	}
}

*/