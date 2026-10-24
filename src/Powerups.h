#pragma once

#include "Breakout.h"
#include "Blocks.h"
#include "LevelLoader.h"

#include <glm/glm.hpp>
#include <array>

enum PowerupType
{
	POWERUP_NONE				= 0,
	// positive powerups
	PLAYER_SPEED_INCREASE		= 1,
	PLAYER_SIZE_INCREASE		= 2,
	PLAYER_STICKY_PADDLE		= 3,
	BALL_PASS_THROUGH_BLOCKS	= 4,
	// negative powerups
	SCREEN_CONFUSE				= 5,
	SCREEN_CHAOS				= 6
};

struct FirstBrockenBlock
{
	inline static Blocks type{ EMPTY };
	inline static glm::vec2 pos{};
};

struct Powerup
{
	PowerupType type{ POWERUP_NONE };

	bool active{ false };
	bool falling{ false };

	float duration{};

	glm::vec2 pos{};	
	glm::vec2 size{ g_gameWidthFloat / 12.0f, g_gameHeightFloat / 20.0f };	// paddle.width / 2.0f, paddle.height
	
};

struct Powerups
{
	inline static constexpr unsigned int maxPowerups{ 6 };
	inline static unsigned int powerupsInUse{ 0 };
	inline static std::array<Powerup, maxPowerups> powerupsList;
	// which levels can have powerups
	inline static std::array<int, 20>levelsWithPositivePowerups{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 };
	inline static std::array<int, 20>levelsWithNegativePowerups{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 };	// negative essentially disables

	// velocity of a falling powerup
	inline static const float velocityConstant{ 0.4f };
	inline static float velocity{ velocityConstant * g_gameHeightFloat };	// relative to the screen size


	static void spawn(int index, const Level& level);
	static void update(glm::vec2 paddlePos, glm::vec2 paddleSize);
	static int freePowerupIndex();	// returns first free powerup index or -1 if there are none
	static bool checkScreenColisions(const Powerup& powerup);
	static bool checkPaddleColisions(const Powerup& powerup, glm::vec2 paddlePos, glm::vec2 paddleSize);
	static bool isActive(PowerupType type);
	static float getActiveTime(PowerupType type);
	static void deactivate(Powerup& powerup);
	static void clear();
};