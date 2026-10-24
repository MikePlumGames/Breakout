#include "Paddle.h"

#include "Breakout.h"
#include "Powerups.h"

#include <glm/glm.hpp>

void Paddle::update()
{
	// change the speed according to the powerups
	if (Powerups::isActive(PLAYER_SPEED_INCREASE))
	{
		velocityConstant = velocityFasterConstant;
		velocity = velocityConstant * g_gameWidthFloat;
	}
	else
	{
		velocityConstant = velocitySlowerConstant;
		velocity = velocityConstant * g_gameWidthFloat;
	}
	// change the size according to the powerups
	if (Powerups::isActive(PLAYER_SIZE_INCREASE))
	{
		width = widthBigger;
	}
	else
	{
		width = widthNormal;
	}
	if (Powerups::isActive(PLAYER_STICKY_PADDLE))
	{
		color = colorSticky;
	}
	else
	{
		color = colorNormal;
	}


	// move the paddle
	if (movingRight)	pos.x += velocity * g_deltaTime;
	if (movingLeft)		pos.x -= velocity * g_deltaTime;

	// check if the paddle is not out of bounds
	if (pos.x + width > g_gameWidthFloat) pos.x = g_gameWidthFloat - width;
	if (pos.x < 0.0f) pos.x = 0.0f;
}
// updates the paddle's position if the window was resized
void Paddle::reset()
{
	widthNormal = g_gameWidthFloat / 6.0f;
	widthBigger = g_gameWidthFloat / 5.0f;

	width = widthNormal;
	height = g_gameHeightFloat / 20.0f;
	pos.x = g_gameWidthFloat / 2.0f - (width * 0.5f);
	pos.y = g_gameHeightFloat - height;
	velocity = velocityConstant * g_gameWidthFloat;	// relative velocity
}