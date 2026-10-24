#include "Scene.h"

#include "Ball.h"
#include "Breakout.h"
#include "Blocks.h"
#include "Framebuffer.h"
#include "LevelLoader.h"
#include "Levels.h"
#include "Menus.h"
#include "Paddle.h"
#include "Powerups.h"
#include "Renderer.h"
#include "Textures.h"

#include <glm/glm.hpp>
#include <glad/glad.h>

#include <iostream>

// draws the enitre scene
void drawScene(Paddle& paddle, Ball& ball)
{
	// to not render when window is minimized to avoid error. btw cannot be g_gameWidth. speaking from expierience.
	if (g_screenHeight == 0 && g_screenWidth == 0)
		return;


	// menus
	if (Menus::activeMenu == MAIN_MENU)
	{
		Menus::mainMenu.draw();
		return;
	}
	if (Menus::activeMenu == PLAYTHROUGH_END_MENU)
	{
		Menus::playthroughEndMenu.draw();
		return;
	}
	if (Menus::activeMenu == CHOOSE_LEVEL_MENU)
	{
		Menus::levelPickerMenu.draw();
		return;
	}
	if (Menus::activeMenu == VIEW_STATS_MENU)
	{
		Menus::statisticsMenu.draw();
		return;
	}
	if (Menus::activeMenu == SETTINGS_MENU)
	{
		Menus::settingsMenu.draw();
		return;
	}
	if (Menus::activeMenu == MAIN_ABOUT_MENU)
	{
		Menus::aboutMenu.draw();
		return;
	}
	// else draw the game

	static bool isFBOconfigured{ false };
	// create a multisampled fbo and a normal fbo
	static FramebufferMultisample mulitisampledFBO(g_gameWidth, g_gameHeight, 4);
	static Framebuffer normalFBO(g_gameWidth, g_gameHeight);
	if (!isFBOconfigured)
	{
		// configure multisampled fbo
		mulitisampledFBO.addTextureAttachment(0, GL_RGBA, GL_RGBA8);
		mulitisampledFBO.addRBO(GL_DEPTH24_STENCIL8);

		// configure normal fbo
		normalFBO.addTextureAttachment(0, GL_RGBA, GL_RGBA8, GL_REPEAT, GL_LINEAR, GL_LINEAR);

		isFBOconfigured = true;
	}

	// if the window got resized reset the fbo's
	if (g_hasWindowResized)
	{
		mulitisampledFBO.resize(g_gameWidth, g_gameHeight);
		normalFBO.resize(g_gameWidth, g_gameHeight);
	}

	// shake logic
	static float shakeDuration{};
	if (shakeDuration > 0.0f)
		shakeDuration -= g_deltaTime;
	else if (shakeDuration < 0.0f)
		shakeDuration = 0.0f;
	if (ball.hitSolidBlock)
		shakeDuration = 0.1f;

	// bind the multisampled fbo
	mulitisampledFBO.bind();
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	

	// draw background
	renderQuadTexture(Textures::background, glm::vec2(0.0f, 0.0f), glm::vec2(g_gameWidthFloat, g_gameHeightFloat), 0.0f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(g_gameWidthFloat, g_gameHeightFloat));

	// draw the particles if the ball is not stuck
	if(!ball.isStuck) ball.particles.draw();

	// draw level
	drawLevel(Levels::mainLevels.at(Levels::activeLevel));
	
	// render the falling powerups
	drawPowerups();

	// draw the ball
	renderQuadTexture(Textures::ball, ball.pos, glm::vec2(ball.width, ball.height), 0.0f, ball.color, glm::vec2(g_gameWidthFloat, g_gameHeightFloat));

	// draw the paddle
	renderQuadTexture(Textures::playerPaddle, paddle.pos, glm::vec2(paddle.width, paddle.height), 0.0f, paddle.color, glm::vec2(g_gameWidthFloat, g_gameHeightFloat));


	// unbind the multisampled fbo
	mulitisampledFBO.unbind();
	// blit the multisampled fbo to the normal one
	normalFBO.blitMultisapmledFBO(mulitisampledFBO);

	// draw the screen quad with the image from normal fbo
	renderPostprocessedQuad(normalFBO.textures[0].ID, glm::vec2(g_windowLeftOffset, g_windowTopOffset), glm::vec2(g_gameWidthFloat, g_gameHeightFloat),
		0.0f, shakeDuration > 0.0f, Powerups::isActive(SCREEN_CONFUSE), Powerups::isActive(SCREEN_CHAOS), glm::vec2(static_cast<float>(g_screenWidth), static_cast<float>(g_screenHeight)));

	// render all text
	Menus::gameplayMenu.draw();

	if (Menus::activeMenu == PLAYTHROUGH_PAUSE_MENU)
	{
		Menus::playthroughPauseMenu.draw();
	}
	else if (Menus::activeMenu == LEVEL_PAUSE_MENU)
	{
		Menus::levelPauseMenu.draw();
	}

	//renderText(Fonts::MartianMonoRegular, "test", glm::vec2(10.0f, 10.0f), 1.0f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(static_cast<float>(g_screenWidth), static_cast<float>(g_screenHeight)));
	
	

	// render the time since the start of the playtime, abbreviated to 2 decimal places
	//drawTime(textSize);
}

void drawLevel(Level level)
{
	float HalfHeight{ g_gameHeightFloat / 2.0f };	// because blocks are supposed to be only on half the screen height
	
	float blockWidth{ g_gameWidthFloat / level.columns };
	float blockHeight{ static_cast<float>(HalfHeight) / level.rows };

	glm::vec2 blockPosition{ 0.0f, 0.0f };
	glm::vec2 size{ blockWidth, blockHeight };
	
	for (int i = 0; i < level.data.size(); i++)
	{
		if (level.data.at(i) == END_LINE)
		{
			blockPosition.y += blockHeight;
			blockPosition.x = 0;
		}
		else
		{
			if (level.data.at(i) == BLOCK_SOLID)
				renderQuadTexture(Textures::blockSolid, blockPosition, size, 0.0f, getBlockColor(level.data.at(i)), glm::vec2(g_gameWidthFloat, g_gameHeightFloat));
			else
				renderQuadTexture(Textures::block, blockPosition, size, 0.0f, getBlockColor(level.data.at(i)), glm::vec2(g_gameWidthFloat, g_gameHeightFloat));
			blockPosition.x += blockWidth;
		}
	}
}

// get the color depending on the brick type
glm::vec4 getBlockColor(Blocks block)
{
	switch (block)
	{
	case EMPTY:
		return glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
		break;
	case BLOCK_SOLID:
		return glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		break;
	case BLOCK_RED:
		return glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
		break;
	case BLOCK_GREEN:
		return glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
		break;
	case BLOCK_BLUE:
		return glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
		break;
	case BLOCK_YELLOW:
		return glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
		break;
	case BLOCK_PINK:
		return glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
		break;
	default:
		return glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	}
}

void drawPowerups()
{
	for (int i = 0; i < Powerups::powerupsList.size(); i++)
	{
		if (!Powerups::powerupsList[i].falling) continue;

		unsigned int texture{};
		glm::vec4 color{ 1.0f };

		switch (Powerups::powerupsList[i].type)
		{
		case PLAYER_SPEED_INCREASE:
			//std::cout << "Draw powerup: PLAYER_SPEED_INCREASE: " << i << std::endl;
			texture = Textures::powerupPlayerSpeedIncrease;
			color = { 0.0f, 0.0f, 1.0f, 1.0f };
			break;
		case PLAYER_SIZE_INCREASE:
			//std::cout << "Draw powerup: PLAYER_SIZE_INCRESE: " << i << std::endl;
			texture = Textures::powerupPlayerSizeIncrease;
			color = { 0.0f, 0.5f, 1.0f, 1.0f };
			break;
		case PLAYER_STICKY_PADDLE:
			//std::cout << "Draw powerup: PLAYER_STICKY_PADDLE: " << i << std::endl;
			texture = Textures::powerupPlayerStickyPaddle;
			color = { 0.0f, 0.8f, 1.0f, 1.0f };
			break;
		case BALL_PASS_THROUGH_BLOCKS:
			//std::cout << "Draw powerup: BALL_PASS_THROUGH_BLOCKS: " << i << std::endl;
			texture = Textures::powerupBallPassThroughBlocks;
			color = { 0.0f, 1.0f, 0.0f, 1.0f };
			break;
		case SCREEN_CONFUSE:
			//std::cout << "Draw powerup: SCREEN_CONFUSE: " << i << std::endl;
			texture = Textures::powerupScreenConfuse;
			color = { 1.0f, 0.5f, 0.0f, 1.0f };
			break;
		case SCREEN_CHAOS:
			//std::cout << "Draw powerup: SCREEN_CHAOS: " << i << std::endl;
			texture = Textures::powerupScreenChaos;
			color = { 1.0f, 0.0f, 0.0f, 1.0f };
			break;
		default:
			std::cout << "Draw powerup: ERROR: " << i << std::endl;
			texture = Textures::powerupScreenChaos;	// cause it has to render something, can be distinguished by a different color
			color = { 0.0f, 0.0f, 0.0f, 1.0f };
		}

		renderQuadTexture(texture, Powerups::powerupsList[i].pos, Powerups::powerupsList[i].size, 0.0f, color, glm::vec2(g_gameWidthFloat, g_gameHeightFloat));
	}
}

void drawTime(float textSize)
{


}