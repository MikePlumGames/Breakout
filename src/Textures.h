#pragma once

#include "TextureLoader.h"

// struct holding all assets
struct Textures
{
	// gameplay assets
	inline static unsigned int block{};
	inline static unsigned int blockSolid{};
	inline static unsigned int background{};
	inline static unsigned int playerPaddle{};
	inline static unsigned int ball{};
	inline static unsigned int particle{};
	inline static unsigned int logo{};

	// buttons
	inline static unsigned int button{};
	inline static unsigned int buttonHovered{};
	inline static unsigned int buttonPause{};
	inline static unsigned int buttonLevelPick{};
	inline static unsigned int buttonGoBack{};
	inline static unsigned int buttonGoBackHovered{};

	// sliders
	inline static unsigned int slider{};
	inline static unsigned int sliderHandle{};
	inline static unsigned int sliderHandleHovered{};

	// powerups
	inline static unsigned int powerupPlayerSpeedIncrease{};
	inline static unsigned int powerupPlayerSizeIncrease{};
	inline static unsigned int powerupPlayerStickyPaddle{};
	inline static unsigned int powerupBallPassThroughBlocks{};
	inline static unsigned int powerupScreenConfuse{};
	inline static unsigned int powerupScreenChaos{};

	// powerup icons
	inline static unsigned int iconPlayerSpeedIncrease{};
	inline static unsigned int iconPlayerSizeIncrease{};
	inline static unsigned int iconPlayerStickyPaddle{};
	inline static unsigned int iconBallPassThroughBlocks{};
	inline static unsigned int iconScreenConfuse{};
	inline static unsigned int iconScreenChaos{};

	// method that loads all assets
	static void loadAll()
	{
		// gameplay assets
		block = loadTexture("assets/textures/block.png");
		blockSolid = loadTexture("assets/textures/solidBlock.png");
		background = loadTexture("assets/textures/background.png");
		playerPaddle = loadTexture("assets/textures/paddle.png");
		ball = loadTexture("assets/textures/ball.png");
		particle = loadTexture("assets/textures/particle.png");
		logo = loadTexture("assets/textures/logo.png");

		// buttons
		button = loadTexture("assets/textures/Button.png");
		buttonHovered = loadTexture("assets/textures/Button_hovered.png");
		buttonPause = loadTexture("assets/textures/PauseButton.png");
		buttonLevelPick = loadTexture("assets/textures/LevelPickButton.png");
		buttonGoBack = loadTexture("assets/textures/GoBackButton.png");
		buttonGoBackHovered = loadTexture("assets/textures/GoBackButton_hovered.png");

		// sliders
		slider = loadTexture("assets/textures/slider.png");
		sliderHandle = loadTexture("assets/textures/sliderHandle.png");
		sliderHandleHovered = loadTexture("assets/textures/sliderHandle_hovered.png");

		// powerups
		powerupPlayerSpeedIncrease = loadTexture("assets/textures/powerup_speed.png");
		powerupPlayerSizeIncrease = loadTexture("assets/textures/powerup_increase.png");
		powerupPlayerStickyPaddle = loadTexture("assets/textures/powerup_sticky.png");
		powerupBallPassThroughBlocks = loadTexture("assets/textures/powerup_passthrough.png");
		powerupScreenConfuse = loadTexture("assets/textures/powerup_confuse.png");
		powerupScreenChaos = loadTexture("assets/textures/powerup_chaos.png");
		
		// powerup icons
		iconPlayerSpeedIncrease = loadTexture("assets/textures/powerup_speed_icon.png");
		iconPlayerSizeIncrease = loadTexture("assets/textures/powerup_increase_icon.png");
		iconPlayerStickyPaddle = loadTexture("assets/textures/powerup_sticky_icon.png");
		iconBallPassThroughBlocks = loadTexture("assets/textures/powerup_passthrough_icon.png");
		iconScreenConfuse = loadTexture("assets/textures/powerup_confuse_icon.png");
		iconScreenChaos = loadTexture("assets/textures/powerup_chaos_icon.png");
	}
};