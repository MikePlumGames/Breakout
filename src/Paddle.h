#pragma once

#include "Breakout.h"

#include <glm/glm.hpp>

struct Paddle
{
	// for powerups
	float widthNormal{ g_gameWidthFloat / 6.0f };
	float widthBigger{ g_gameWidthFloat / 4.5f };
	
	float width{ widthNormal };
	float height{ g_gameHeightFloat / 20.0f };

	glm::vec2 pos{ g_gameWidthFloat / 2.0f - (width * 0.5f),
		g_gameHeightFloat - height };

	glm::vec4 colorSticky = glm::vec4(0.0f, 0.8f, 1.0f, 1.0f);
	glm::vec4 colorNormal = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	glm::vec4 color = colorNormal;

	// for powerups
	const float velocitySlowerConstant{ 0.70f };				// %  how much screen does the paddle traverse in 1s
	const float velocityFasterConstant{ 0.95f };

	float velocityConstant{ velocitySlowerConstant };
	float velocity{ velocityConstant * g_gameWidthFloat };	// relative velocity

	// used to transfer the paddle's momentum on the ball
	bool movingRight{ false };
	bool movingLeft{ false };

	// updates the paddle each frame
	void update();
	// resets the paddle to starting position
	void reset();
};