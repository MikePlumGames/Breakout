#include "Ball.h"

#include "Audio.h"
#include "Blocks.h"
#include "Breakout.h"
#include "Colision.h"
#include "LevelLoader.h"
#include "Levels.h"
#include "Paddle.h"
#include "Powerups.h"
#include "Stats.h"

#include <glm/glm.hpp>

#include <cstdlib>
#include <iostream>
#include <random>

// ball contructor similar code to reset
Ball::Ball(const Paddle& paddle)
{
	// reseting the paddle to the start position on the paddle
	reset(paddle);
}

// update the ball each frame
void Ball::update(const Paddle& paddle)
{
	// reset the flag
	hitSolidBlock = false;

	// ball logic
	if (shouldLaunch && isStuck)
	{
		isStuck = false;
		// set the random velocity for the ball
		randomVelocity();
		shouldLaunch = false;
	}

	// powerups
	if (Powerups::isActive(BALL_PASS_THROUGH_BLOCKS))
	{
		color = colorPassThrough;
	}
	else
	{
		color = colorNormal;
	}

	if (resetParticles)
	{
		resetParticles = false;
		particles.fullRestart();
	}


	// movement
	if (isStuck) reset(paddle);	// reset if the ball is still moving with the paddle (reseting moves the ball on top of the paddle)
	else
	{
		// move the ball
		pos.x += velocity.x * g_deltaTime;
		pos.y += velocity.y * g_deltaTime;

		// check colisions with screen edge
		checkScreenColisions();
		// check colisions with the paddle
		checkPaddleColisions(paddle);
		// check colisions with the level
		checkBlockColisions(Levels::mainLevels.at(Levels::activeLevel));
	}
	if(isDead) g_gameState = GAME_LOSE;

	// update the particles if the ball is not stuck
	if (!isStuck)
	{
		glm::vec2 particleScale = glm::vec2(width * 0.4f, height * 0.4f);
		glm::vec2 particlePos = pos + particleScale;
		glm::vec2 particleVelocity = -velocity * 0.5f;

		particles.update(particlePos, particleVelocity, particleScale, 0.4f);
	}
}

// reset the ball if the window has resized / the ball died
void Ball::reset(const Paddle& paddle)
{
	// make sure to center the ball (by: isStuckToStickyPaddleTop = false;) if these conditions are true
	// we don't check for the powerup status cause if the ball is stuck it should remain stuck
	if(isDead || !isStuck || g_gameState == GAME_LOSE) isStuckToStickyPaddleTop = false;

	// balls size is not relative to the paddle because with powerups paddle size increases but ball stays the same
	defaultWidth = (g_gameWidthFloat / 6.0f) / 3.0f; // (paddle.width) / 3.0f

	width = defaultWidth;
	height = width;

	radius = width / 2.0f;

	isStuck = true;
	isDead = false;

	// reset the particles
	particles.fullRestart();

	

	// reset the position
	pos.x = paddle.pos.x + (paddle.width / 2.0f) - (width / 2.0f);

	if (isStuckToStickyPaddleTop)
	{
		pos.x += stickyBallOffset;
		checkScreenColisions();	// so the ball doesn't go out od screen bounds if stuck on the edge
	}

	pos.y = paddle.pos.y - height;
}

// gives the ball random velocity at the start of the movement
void Ball::randomVelocity()
{
	// generate a random number
	static std::random_device seed;
	static std::mt19937 gen(seed());
	//std::uniform_real_distribution<float> random(0.5f, 0.5f); // ended up cutting this out
	std::bernoulli_distribution isSigned(0.5);	// gives 50% chances for the velocity to be signed/unsigned

	// randomise the values
	velocityConstant.x = /*random(gen)*/ 0.5f;
	if (isSigned(gen)) velocityConstant.x *= -1.0f;	// only x since the ball should always move upwards
	velocityConstant.y = /*random(gen)*/ 0.5f * -1.0f;		// make it negative because since y = 0 is at the top this will make the ball move upwards

	// calculate relative velocity
	velocity.x = velocityConstant.x * g_gameWidthFloat;
	velocity.y = velocityConstant.y * g_gameWidthFloat;	// we scale both by width to avoid glitches
}

// checks for colisions with screen edges
void Ball::checkScreenColisions()
{
	// to prevent float inpresition
	float currentSpeed = glm::length(velocity);

	// left screen edge
	if (pos.x < 0.0f)
	{
		pos.x = 0.0f;
		// normal points to the right
		glm::vec2 normal(1.0f, 0.0f);
		velocity = velocity - 2.0f * glm::dot(velocity, normal) * normal;
	}
	// right screen edge
	if (pos.x + width > g_gameWidthFloat)
	{
		pos.x = g_gameWidthFloat - width;
		// normal point to the left
		glm::vec2 normal(-1.0f, 0.0f);
		velocity = velocity - 2.0f * glm::dot(velocity, normal) * normal;
	}
	// top screen edge
	if (pos.y < 0.0f)
	{
		pos.y = 0.0f;
		// normal points down
		glm::vec2 normal(0.0f, 1.0f);
		velocity = velocity - 2.0f * glm::dot(velocity, normal) * normal;
	}

	// check if the ball died - game is resetted based on g_gameState in gameUpdate();
	if (pos.y + height > g_gameHeightFloat + bias) isDead = true;

	// restore the original magnitude
	velocity = glm::normalize(velocity) * currentSpeed;
}

// checks for colisions with the paddle
void Ball::checkPaddleColisions(const Paddle& paddle)
{
	// check if the colision happened
	ColisionSide colisionSide = checkColision(paddle.pos, paddle.width, paddle.height);

	// if no colision return (or corner colision, which is handeled in checkColision(), as that function has all the necesary informations calculated already)
	if (colisionSide == NONE) return;

	// play a sfx
	playSFX(SoundEffects::bleepLow, 1.6f);

	// calculate current paddle velocity - used to transfer momentum onto the ball, only x since the paddle doesn't go up/down
	float currentPaddleVelocityX{ 0 };
	if (paddle.movingLeft) currentPaddleVelocityX = -paddle.velocity;
	if (paddle.movingRight) currentPaddleVelocityX = paddle.velocity;


	if (colisionSide == LEFT) std::cout << "Colision: LEFT" << std::endl;
	if (colisionSide == RIGHT) std::cout << "Colision: RIGHT" << std::endl;
	if (colisionSide == TOP) std::cout << "Colision: TOP" << std::endl;
	if (colisionSide == BOTTOM) std::cout << "Colision: BOTTOM" << std::endl;

	// powerups
	isStuckToStickyPaddleTop = false;
	if (colisionSide != NONE && Powerups::isActive(PLAYER_STICKY_PADDLE))
	{
		if (colisionSide == TOP)
		{
			stickyBallOffset = pos.x + (width / 2.0f) - (paddle.pos.x + (paddle.width / 2.0f));
			isStuckToStickyPaddleTop = true;
			isStuck = true;
			reset(paddle);
		}
		else
		{
			isStuck = true;
			reset(paddle);	
		}
	}


	switch (colisionSide)
	{
	case TOP:
	{
		// position the ball at the top
		pos.y -= (overlapY + 0.1f);

		// to prevent float inpresition
		float currentSpeed = glm::length(velocity);

		// use a dynamic normal for aiming
		float paddleCenterX{ paddle.pos.x + paddle.width / 2.0f };
		float ballCenterX{ pos.x + width / 2.0f };

		float hitFactor{ (ballCenterX - paddleCenterX) / (paddle.width / 2.0f) };
		hitFactor = glm::clamp(hitFactor, -1.0f, 1.0f);

		float tiltIntensity{ 0.5f };	// how much the hitFactor corresponds to the tilt of the normal
		glm::vec2 dynamicNormal(hitFactor * tiltIntensity, -1.0f);
		dynamicNormal = glm::normalize(dynamicNormal);

		// apply the reflection
		velocity = velocity - 2.0f * glm::dot(velocity, dynamicNormal) * dynamicNormal;

		// momentum transfer
		float momentumFactor{ 0.3f };
		velocity.x += currentPaddleVelocityX * momentumFactor;

		// prevent the ball from flying perfectly sideways
		float minYspeed{ currentSpeed * 0.5f };
		if (std::abs(velocity.y) < minYspeed)
		{
			velocity.y = -minYspeed;	
			std::cout << "VELOCITY CORRECTED\n";
		}

		// so the ball goes up
		velocity.y = -std::abs(velocity.y);

		// restore the original magnitude
		velocity = glm::normalize(velocity) * currentSpeed;
		break;
	}
	case LEFT:
		pos.x -= (overlapX + 0.1f);
		// bounce of to the left and up
		velocity.x = -std::abs(velocity.x);
		velocity.y = -std::abs(velocity.y);
		break;

	case RIGHT:
		pos.x += (overlapX + 0.1f);
		// bounce of to the right and up
		velocity.x = std::abs(velocity.x);
		velocity.y = -std::abs(velocity.y);
		break;

	case BOTTOM:	// this scenerio should never happen so we just act like the ball is outside the screen
		isDead = true;
		break;
	}

}

// checks for colisions with blocks
void Ball::checkBlockColisions(Level& level)
{
	float HalfHeight{ g_gameHeightFloat / 2.0f };	// because blocks are supposed to be only on half the screen height

	float blockWidth{ g_gameWidthFloat / level.columns };
	float blockHeight{ static_cast<float>(HalfHeight) / level.rows };
	glm::vec2 blockPos{ 0.0f, 0.0f };

	ColisionSide FirstColisionSide{};
	bool hasColided{ false };

	// what type of block was broken first - used for powerups
	FirstBrockenBlock::type = EMPTY;

	// check all the colsions destroy all the blocks
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

			if (!hasColided)
			{
				FirstColisionSide = colisionSide;
				hasColided = true;
			}

			if (level.data.at(i) == BLOCK_SOLID)
			{
				hitSolidBlock = true;
				std::cout << "Block solid at: " << i << std::endl;
				// play a sfx
				playSFX(SoundEffects::metalImpact, 0.4f);
			}
			else if (level.data.at(i) != BLOCK_SOLID)
			{
				// powerup logic
				if (FirstBrockenBlock::type == EMPTY)
				{
					FirstBrockenBlock::type = level.data.at(i);
					FirstBrockenBlock::pos = blockPos;
				}

				level.data.at(i) = EMPTY;
				std::cout << "Block Broken at: " << i << "\n";
				Stats::brokenBlocks++;

				// play a sfx
				playSFX(SoundEffects::bleepHigh, 0.2f);
			}

			
			// go to next block
			blockPos.x += blockWidth;
		}
	}

	// resolve the colisions, do the physics

	// if the powerup is active don't reverse the velocity
	if (Powerups::isActive(BALL_PASS_THROUGH_BLOCKS) && !hitSolidBlock)
		return;

	const float ExtraStep{ 0.1f };	// ball is slightly moved away from the blocks

	float currentSpeed = glm::length(velocity);
	float minYspeed = currentSpeed * 0.5f;	// so the ball doesn't travel sideways

	switch (FirstColisionSide)
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
}

// checks for colisons with any rectangular object
ColisionSide Ball::checkColision(glm::vec2 rectangularPos, float rectangularWidth, float rectangularHeight)
{
	// center of the ball - pos holds top left corner, radius is equal to half the width/height
	glm::vec2 ballCenter(pos + radius);
	// half width and half height of the rectangle
	glm::vec2 rectangleSizeHalved(rectangularWidth / 2.0f, rectangularHeight / 2.0f);
	// center of the rectangle
	glm::vec2 rectangleCenter(rectangularPos.x + rectangleSizeHalved.x, rectangularPos.y + rectangleSizeHalved.y);
	// difference between both centers
	glm::vec2 difference = ballCenter - rectangleCenter;
	// clamp the difference between half width and half height of the rectangle
	glm::vec2 clampedDifference = glm::clamp(difference, -rectangleSizeHalved, rectangleSizeHalved);
	// get the closest point on the rectangle to the circle
	glm::vec2 closestPoint = rectangleCenter + clampedDifference;
	// get the difference between the closest point and the ball's center
	glm::vec2 contactVector = closestPoint - ballCenter;
	// if the contactVector is larger than the radius objects don't colide
	//return glm::length(contactVector) < radius;
	float distance{ glm::length(contactVector) };

	if (distance > radius) return NONE;

	// check the colisions

	// check how much does the ball overlap
	overlapX = rectangleSizeHalved.x - std::abs(clampedDifference.x);
	overlapY = rectangleSizeHalved.y - std::abs(clampedDifference.y);

	// corner colisions thrown out the window :)
	/*
 	// corners
	
	// if the clampedDifference.x has a max value it means the object is on the  left/right
	bool hitVerticalEdge{ std::abs(clampedDifference.x) == rectangleSizeHalved.x };
	// if the clampedDifference.y has a max value it means the object is on the  top/bottom
	bool hitHorizontalEdge{ std::abs(clampedDifference.y) == rectangleSizeHalved.y };
	// if both are true we hit the corner, distance > 0.0f to prevent division by 0
	if (hitVerticalEdge && hitHorizontalEdge && distance > 0.0f)
	{
		// skip the corner logic and threat it like the edge because it is way simpler
		
		// we handle the corner colision internaly here
		std::cout << "Colision: CORNER" << std::endl;

		// to prevent float inpresition
		float currentSpeed = glm::length(velocity);

		
		// distance is length of the contactVector so it is normalized, we get normal vector diagonally away from the corner
		//- because contact vector points towards the rectangle's center
		glm::vec2 normal = -(contactVector / distance);

		// specular reflection: v_new = v - 2 * dot(v, n) * n
		velocity = velocity - 2.0f * glm::dot(velocity, normal) * normal;

		velocity = glm::normalize(velocity) * currentSpeed;

		// check how deep the ball is inside the paddle
		float overlapDepth{ radius - distance };
		// correct the ball
		pos += normal * (overlapDepth + 0.5f);
	
		// because we handled the colision logic here we can just return none
		return CORNER;
	}
	*/
	// edges

	// to avoid issues with screen ratios
	glm::vec2 contactDir = (distance > 0.0f) ? (contactVector / distance) : glm::vec2(0.0f);

	// the ball entered from left/right, here the contactVector points from the closest point toward ball center
	// std::abs(contactDir.x) > std::abs(contactDir.y)
	// overlapX < overlapY
	if (std::abs(contactDir.x) > std::abs(contactDir.y))
	{
		if (contactVector.x < 0.0f && velocity.x < 0.0f)
			return RIGHT;
	
		else if (contactVector.x > 0.0f && velocity.x > 0.0f)
			return LEFT;
	}
	else
	{
		if (contactVector.y < 0.0f && velocity.y < 0.0f)
			return BOTTOM;
		
		else if (contactVector.y > 0.0f && velocity.y > 0.0f)
			return TOP;
	}
	// else
	return NONE;
}