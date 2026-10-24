#pragma once

#include "Breakout.h"

#include <glm/glm.hpp>
#include <array>
#include <string>

enum MenuType
{
	MAIN_MENU					= 0,
	GAME_PLAY					= 1,
	SETTINGS_MENU				= 2,
	CHOOSE_LEVEL_MENU			= 3,
	VIEW_STATS_MENU				= 4,
	MAIN_ABOUT_MENU				= 5,
	PLAYTHROUGH_PAUSE_MENU		= 6,
	LEVEL_PAUSE_MENU			= 7,
	PLAYTHROUGH_END_MENU		= 8
};



struct Button
{
	// static for all buttons
	inline static glm::vec4 colorDefault{ 1.0f, 1.0f, 1.0f, 1.0f };
	inline static glm::vec4 colorDefaultHovered{ 1.0f, 1.0f, 1.0f, 1.0f };
	inline static glm::vec4 colorDefaultText{ 1.0f, 1.0f, 1.0f, 1.0f };
	
	// specific to a button
	glm::vec2 pos{ 0.0f, 0.0f };
	glm::vec2 size{ 10.0f, 100.0f };
	std::string text{ "Button" };
	glm::vec4 color = colorDefault;
	glm::vec4 colorHovered{ colorDefaultHovered };
	glm::vec4 colorText{ colorDefaultText };

	bool isHovered() const;
	void draw(unsigned int texture, glm::vec2 target) const;
	void drawCustomScale(unsigned int texture, float textscale, std::string textForCustomScale, glm::vec2 target) const;
};

struct TextBox	// basically a button without the button - just text
{
	// static for all TextBoxes
	inline static glm::vec4 colorDefaultText{ 1.0f, 1.0f, 1.0f, 1.0f };
	
	// specific to a TextBox
	glm::vec2 pos{ 0.0f, 0.0f };
	glm::vec2 size{ 10.0f, 100.0f };
	std::string text{ "TextBox" };
	glm::vec4 colorText{ colorDefaultText };

	void draw(glm::vec2 target) const;
	void drawCustomScale(float textscale, std::string textForCustomScale, glm::vec2 target) const;
};

struct SliderHandle
{
	// static for all slider handles
	inline static glm::vec4 colorDefault{ 1.0f, 1.0f, 1.0f, 1.0f };
	inline static glm::vec4 colorDefaultHovered{ 1.0f, 1.0f, 1.0f, 1.0f };

	glm::vec2 pos{};
	glm::vec2 size{};

	glm::vec4 color{ colorDefault };
	glm::vec4 colorHovered{ colorDefaultHovered };

	bool isHovered() const;
	void draw(unsigned int texture, glm::vec2 target) const;
};

struct Slider
{
	// static for all sliders
	inline static glm::vec4 colorDefault{ 1.0f, 1.0f, 1.0f, 1.0f };

	glm::vec2 pos{};
	glm::vec2 posWithHandle{};
	glm::vec2 size{};

	float rangeMin{};
	float rangeMax{};

	float value{};	// clamped to [rangeMin, rangeMax]

	glm::vec4 color{ colorDefault };

	bool isSliding{ false };

	SliderHandle handle{};

	bool isHovered() const;
	// handleValue must be between [0,1] or it gets clamped to [0,1]
	void resize(glm::vec2 sliderPos, glm::vec2 sliderSize, float handleValue);
	void update();
	void draw(unsigned int texture, unsigned int handleTexture, glm::vec2 target) const;
};

struct MainMenu
{
	Button newPlaythrough;		//{ .pos{ButtonPosX, g_screenHeight / 2.0f - 3.0f * ButtonSize.y - 2.5f * ButtonOffsetY}, .size{ButtonSize}, .text{"New Playthrough"}};
	Button continuePlaythrough;	//{ .pos{ButtonPosX, g_screenHeight / 2.0f - 2.0f * ButtonSize.y - 1.5f * ButtonOffsetY}, .size{ButtonSize}, .text{"Continue Playthrough"} };
	Button playLevel;			//{ .pos{ButtonPosX, g_screenHeight / 2.0f - 1.0f * ButtonSize.y - 0.5f * ButtonOffsetY}, .size{ButtonSize}, .text{"Play a Level"} };
	Button settings;				//{ .pos{ButtonPosX, g_screenHeight / 2.0f					   + 0.5f * ButtonOffsetY}, .size{ButtonSize}, .text{"Settings"} };
	Button viewStats;			//{ .pos{ButtonPosX, g_screenHeight / 2.0f + 1.0f * ButtonSize.y + 1.5f * ButtonOffsetY}, .size{ButtonSize}, .text{"Statistics"} };
	Button about;				//{ .pos{ButtonPosX, g_screenHeight / 2.0f + 2.0f * ButtonSize.y + 2.5f * ButtonOffsetY}, .size{ButtonSize}, .text{"About"} };

	void resize()
	{
		glm::vec2 buttonSize = { g_screenWidth / 5.0f, g_screenHeight / 20.0f };
		float buttonOffsetY =  g_screenHeight / 100.0f;
		float buttonPosX = g_screenWidth / 5.0f * 2.0f ;

		newPlaythrough =		{ .pos{buttonPosX, g_screenHeight / 2.0f - 3.0f * buttonSize.y - 2.5f * buttonOffsetY}, .size{buttonSize}, .text{"new playthrough"} };
		continuePlaythrough =	{ .pos{buttonPosX, g_screenHeight / 2.0f - 2.0f * buttonSize.y - 1.5f * buttonOffsetY}, .size{buttonSize}, .text{"continue playthrough"} };
		playLevel =				{ .pos{buttonPosX, g_screenHeight / 2.0f - 1.0f * buttonSize.y - 0.5f * buttonOffsetY}, .size{buttonSize}, .text{"play a level"} };
		settings =				{ .pos{buttonPosX, g_screenHeight / 2.0f					   + 0.5f * buttonOffsetY}, .size{buttonSize}, .text{"settings"} };
		viewStats =				{ .pos{buttonPosX, g_screenHeight / 2.0f + 1.0f * buttonSize.y + 1.5f * buttonOffsetY}, .size{buttonSize}, .text{"statistics"} };
		about =					{ .pos{buttonPosX, g_screenHeight / 2.0f + 2.0f * buttonSize.y + 2.5f * buttonOffsetY}, .size{buttonSize}, .text{"about"} };
	}

	void draw() const;
	void update(glm::vec2& paddlePos, glm::vec2& ballPos, glm::vec2& ballVelocity, bool& ballIsStuck, bool& ballResetParicles) const;
};

enum GamePlayMenuType
{
	GAME_PLAY_PLAYTHROUGH = 0,
	GAME_PLAY_LEVEL		  = 1,
};

struct GameplayMenu	// the game itself is not a part of the menu just the buttons
{
	inline static GamePlayMenuType gamePlayMenuType{ GAME_PLAY_PLAYTHROUGH };

	Button pause;

	TextBox levelNumber;
	TextBox attempts;
	TextBox time;

	void resize()
	{
		pause = { .pos{g_screenWidth - 45.0f, 0.0f}, .size{45.0f, 45.0f}, .text{""} };
		// text assigned in void GameplayMenu::draw();
		float oX{ (static_cast<float>(g_screenWidth) - pause.size.x - 50.0f) / 3.0f };
		//attempts = { .pos{10.0f, 0.0f}, .size{100.0f, 25.0f}, .text{"attempts"} };
		//levelNumber = { .pos{10.0f, 25.0f}, .size{100.0f, 25.0f}, .text{"level"} };
		levelNumber = { .pos{0.0f, 0.0f}, .size{oX, 50.0f}, .text{"level"} };
		attempts = { .pos{oX, 0.0f}, .size{oX - 10.0f, 50.0f}, .text{"attempts"} };
		time = { .pos{2.0f * oX, 0.0f}, .size{oX + 50.0f, 50.0f}, .text{"time"} };
	}

	void draw();
	void update() const;
};


struct PlaythroughPauseMenu
{
	Button resume;		//{ .pos{ButtonPosX, g_screenHeight / 2.0f - 2.5f * ButtonSize.y - 2.0f * ButtonOffsetY}, .size{ButtonSize}, .text{"resume"} };
	Button settings;		//{ .pos{ButtonPosX, g_screenHeight / 2.0f - 1.5f * ButtonSize.y - 1.0f * ButtonOffsetY}, .size{ButtonSize}, .text{"settings"} };
	Button viewStats;	//{ .pos{ButtonPosX, g_screenHeight / 2.0f - 0.5f * ButtonSize.y						 }, .size{ButtonSize}, .text{"playthrough statistics"} };
	Button saveAndQuit;	//{ .pos{ButtonPosX, g_screenHeight / 2.0f + 0.5f * ButtonSize.y + 1.0f * ButtonOffsetY}, .size{ButtonSize}, .text{"save and quit"} };
	Button quitNoSaving;	//{ .pos{ButtonPosX, g_screenHeight / 2.0f + 1.5f * ButtonSize.y + 2.0f * ButtonOffsetY}, .size{ButtonSize}, .text{"quit without saving"} };

	void resize()
	{
		glm::vec2 buttonSize = { g_screenWidth / 5.0f, g_screenHeight / 20.0f };
		float buttonOffsetY = g_screenHeight / 100.0f;
		float buttonPosX = g_screenWidth / 5.0f * 2.0f;

		resume =		{ .pos{buttonPosX, g_screenHeight / 2.0f - 2.5f * buttonSize.y - 2.0f * buttonOffsetY}, .size{buttonSize}, .text{"   resume   "} };
		settings =		{ .pos{buttonPosX, g_screenHeight / 2.0f - 1.5f * buttonSize.y - 1.0f * buttonOffsetY}, .size{buttonSize}, .text{"   settings   "} };
		viewStats =		{ .pos{buttonPosX, g_screenHeight / 2.0f - 0.5f * buttonSize.y						 }, .size{buttonSize}, .text{"playthrough statistics"} };
		saveAndQuit =	{ .pos{buttonPosX, g_screenHeight / 2.0f + 0.5f * buttonSize.y + 1.0f * buttonOffsetY}, .size{buttonSize}, .text{"save and quit"} };
		quitNoSaving =	{ .pos{buttonPosX, g_screenHeight / 2.0f + 1.5f * buttonSize.y + 2.0f * buttonOffsetY}, .size{buttonSize}, .text{"quit without saving"} };
	}

	void draw() const;
	void update(glm::vec2 paddlePos, glm::vec2 ballPos, glm::vec2 ballVelocity, bool ballIsStuck) const;
};

struct PlaythroughEndMenu
{
	bool calculatedStats{ false }; // to calculate whether the stats is a new best ot not only once

	TextBox completeMessage;
	TextBox time;
	TextBox bestTime;
	TextBox attempts;
	TextBox bestAttempts;

	Button newPlaythrough;
	Button toMainMenu;

	void resize()
	{
		// offsets to make the menu. for explanation see assets/data/menu drafts/menus_gameplayMenu_draft.png (labeled: playthrough complete menu)
		float offsetX{ static_cast<float>(g_screenWidth) / 67.0f };
		float offsetY{ static_cast<float>(g_screenHeight) / 92.0f };

		// text for time, bestTime, attempts, bestAttempts calculated in draw();
		completeMessage =	{ .pos{		   offsetX,  2.0f * offsetY}, .size{65.0f * offsetX, 30.0 * offsetY}, .text{"     Playthrough complete!     "} };
		time =				{ .pos{ 6.0f * offsetX, 34.0f * offsetY}, .size{25.0f * offsetX, 15.0 * offsetY}, .text{"time"} };
		bestTime =			{ .pos{36.0f * offsetX, 34.0f * offsetY}, .size{25.0f * offsetX, 15.0 * offsetY}, .text{"best time"} };
		attempts =			{ .pos{ 6.0f * offsetX, 50.0f * offsetY}, .size{25.0f * offsetX, 15.0 * offsetY}, .text{"attempts"} };
		bestAttempts =		{ .pos{36.0f * offsetX, 50.0f * offsetY}, .size{25.0f * offsetX, 15.0 * offsetY}, .text{"best attempts"} };

		newPlaythrough =	{ .pos{ 3.0f * offsetX, 70.0f * offsetY}, .size{26.0f * offsetX, 10.0 * offsetY}, .text{"new Playthrough"} };
		toMainMenu =		{ .pos{36.0f * offsetX, 70.0f * offsetY}, .size{26.0f * offsetX, 10.0 * offsetY}, .text{"main menu"} };
	}

	void draw();
	void update() const;
};

struct LevelPickerMenu
{
	TextBox pickLevel;

	Button goBack;
	std::array<Button, 20> levelButtons;

	void resize()
	{
		// offsets to make the menu. for explanation see assets/data/menu drafts/menus_gameplayMenu_draft.png (labeled: pick a level menu)
		float oX{ static_cast<float>(g_screenWidth) / 75.0f };
		float oY{ static_cast<float>(g_screenHeight) / 67.0f };

		glm::vec2 buttonSize{ 5.0f * oX, 5.0f * oY };

		pickLevel = { .pos{26.0f * oX, 2.0f * oY}, .size{23.0f * oX, 5.0f * oY}, .text{"pick a level"} };

		goBack = { .pos{2.0f * oX, oY}, .size{buttonSize}, .text{""} };

		int counter{ 0 };
		glm::vec2 buttonOffset{ 8.0f * oX, 8.0f * oY };
		// made for 10x10 grid currently only top two rows used
		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				levelButtons[counter].pos = buttonOffset;
				levelButtons[counter].size = buttonSize;
				levelButtons[counter].text = std::to_string(counter + 1);

				buttonOffset.x += 6.0f * oX;	// buttonSize + oX to have some space in between
				counter++;
			}
			buttonOffset.x = 8.0f * oX;	// reset in a new row
			buttonOffset.y += 6.0f * oY; // buttonSize + oY to have some space in between
		}
	}

	void draw();
	void update();
};


struct LevelPauseMenu
{
	Button resume;
	Button settings;
	Button viewStats;
	Button playDifferentLevel;
	Button toMainMenu;

	void resize()
	{
		glm::vec2 buttonSize = { g_screenWidth / 5.0f, g_screenHeight / 20.0f };
		float buttonOffsetY = g_screenHeight / 100.0f;
		float buttonPosX = g_screenWidth / 5.0f * 2.0f;

		resume =				{ .pos{buttonPosX, g_screenHeight / 2.0f - 2.5f * buttonSize.y - 2.0f * buttonOffsetY}, .size{buttonSize}, .text{"   resume   "} };
		settings =				{ .pos{buttonPosX, g_screenHeight / 2.0f - 1.5f * buttonSize.y - 1.0f * buttonOffsetY}, .size{buttonSize}, .text{"   settings   "} };
		viewStats =				{ .pos{buttonPosX, g_screenHeight / 2.0f - 0.5f * buttonSize.y						 }, .size{buttonSize}, .text{"level statistics"} };
		playDifferentLevel =	{ .pos{buttonPosX, g_screenHeight / 2.0f + 0.5f * buttonSize.y + 1.0f * buttonOffsetY}, .size{buttonSize}, .text{"play new level"} };
		toMainMenu =			{ .pos{buttonPosX, g_screenHeight / 2.0f + 1.5f * buttonSize.y + 2.0f * buttonOffsetY}, .size{buttonSize}, .text{"  main menu  "} };
	}

	void draw() const;
	void update() const;
};

enum StatisticsMenuType
{
	STATISTICS_DEFAULT,		// opened from main menu
	STATISTICS_PLAYTHROUGH,	// opened from playthrough pause menu
	STATISTICS_LEVEL		// opened from level pause menu
};

struct StatisticsMenu
{
	StatisticsMenuType statisticsMenuType{ STATISTICS_DEFAULT };

	Button goBack;
	//Button resetAll;	// disabled for now

	TextBox title;
	// general stats
	TextBox attempts;
	TextBox brockenBlocks;
	TextBox levelsUnlocked;
	// general stats only if a playthrough has been completed
	TextBox bestTime;
	TextBox bestAttempts;
	// stats for current playthrough/level - not shown in main menu stats
	TextBox currentTime;
	TextBox currentAttempts;
	TextBox currentLevel;

	void resize()
	{
		// offsets to make the menu. for explanation see assets/data/menu drafts/menus_StatsMenu_draft.png (labeled: statistics menu)
		float oX{ static_cast<float>(g_screenWidth) / 60.0f };
		float oY{ static_cast<float>(g_screenHeight) / 71.0f };

		// buttons
		goBack = { .pos{oX, oY}, .size{3.7f * oX, 5.0f * oY}, .text{""} };
		//resetAll = { .pos{20.0f * oX, 65.0f * oY}, .size{20.0f * oX, 5.0f * oY}, .text{"reset all stats"} };	// disabled for now
		// text boxes
		title = { .pos{10.0f * oX, oY}, .size{40.0f * oX, 8.0f * oY}, .text{"statistics"} };
		// general stats
		glm::vec2 statSize{ 30.0f * oX, 5.0f * oY };
		attempts = { .pos{15.0f * oX, 11.0f * oY}, .size{statSize}};	// the text for all other ones assigned in draw();
		brockenBlocks = { .pos{15.0f * oX, 17.0f * oY}, .size{statSize} };
		levelsUnlocked = { .pos{15.0f * oX, 23.0f * oY}, .size{statSize} };
		// general stats only if a playthrough has been completed
		bestTime = { .pos{15.0f * oX, 31.0f * oY}, .size{statSize} };
		bestAttempts = { .pos{15.0f * oX, 37.0f * oY}, .size{statSize} };
		// stats for current playthrough/level - not shown in main menu stats
		currentTime = { .pos{15.0f * oX, 45.0f * oY}, .size{statSize} };
		currentAttempts = { .pos{15.0f * oX, 51.0f * oY}, .size{statSize} };
		currentLevel = { .pos{15.0f * oX, 57.0f * oY}, .size{statSize} };
	}


	void draw();
	void update() const;
};

struct SettingsMenu
{
	TextBox tittle;
	TextBox musicVolume;
	TextBox sfxVolume;

	Slider musicVolumeSlider{ .rangeMin{0.0f}, .rangeMax{100.0f}, .value{75.0f} };
	Slider sfxVolumeSlider{ .rangeMin{0.0f}, .rangeMax{100.0f}, .value{50.0f} };

	Button goBack;

	void resize()
	{
		// offsets to make the menu. for explanation see assets/data/menu drafts/menus_settingsMenu_draft.png (labeled: settings menu)
		float oX{ static_cast<float>(g_screenWidth) / 60.0f };
		float oY{ static_cast<float>(g_screenHeight) / 71.0f };

		tittle = { .pos{10.0f * oX, oY}, .size{40.0f * oX, 8.0f * oY}, .text{"settings"} };

		goBack = { .pos{oX, oY}, .size{3.7f * oX, 5.0f * oY}, .text{""} };
		// text properly set in draw();
		musicVolume = { .pos{20.0f * oX, 12.0f * oY}, .size{20.0f * oX, 5.0f * oY}, .text{"music"} };
		sfxVolume = { .pos{20.0f * oX, 32.0f * oY}, .size{20.0f * oX, 5.0f * oY}, .text{"sfx"} };

		// musicVolumeSlider.value / 100.0f because value is in range [0,100] but the function needs in [0.1]
		musicVolumeSlider.resize(glm::vec2{ 15.0f * oX, 19.0f * oY }, glm::vec2{ 30.0f * oX, 5.0f * oY }, musicVolumeSlider.value / 100.0f);
		sfxVolumeSlider.resize(glm::vec2{ 15.0f * oX, 39.0f * oY }, glm::vec2{ 30.0f * oX, 5.0f * oY }, sfxVolumeSlider.value / 100.0f);
	}

	void draw();
	void update();
};

struct AboutMenu
{
	Button goBack;

	TextBox tittle;

	TextBox gameBy;
	TextBox inspiration;
	TextBox font;
	TextBox version;

	// my logo image is also here, rendered in draw();
	glm::vec2 logoPos{};
	glm::vec2 logoSize{};

	void resize()
	{
		// offsets to make the menu. for explanation see assets/data/menu drafts/menus_StatsMenu_draft.png (labeled: about menu)
		float oX{ static_cast<float>(g_screenWidth) / 60.0f };
		float oY{ static_cast<float>(g_screenHeight) / 71.0f };

		tittle = { .pos{10.0f * oX, oY}, .size{40.0f * oX, 8.0f * oY}, .text{"  about  "} };

		goBack = { .pos{oX, oY}, .size{3.7f * oX, 5.0f * oY}, .text{""} };

		// text managed in draw(); using drawCustomScale();
		gameBy =		{ .pos{oX, 15.0f * oY}, .size{5.0f * oX, 4.0f * oY}, .text{"game by:"} };
		inspiration =	{ .pos{oX, 27.0f * oY}, .size{5.0f * oX, 4.0f * oY}, .text{"inspired by:"} };
		font =			{ .pos{2.0f * oX, 35.0f * oY}, .size{5.0f * oX, 4.0f * oY}, .text{"font:"} };
		version =		{ .pos{oX, 65.0f * oY}, .size{5.0f * oX, 4.0f * oY}, .text{"version:"} };

		logoPos = { 15.0f * oX, 15.0f * oY };
		logoSize = { 40.0f * oX, 10.0f * oY };
	}

	void draw();
	void update() const;
};

struct Menus
{
	inline static MenuType activeMenu{ MAIN_MENU };
	inline static MenuType previousActiveMenu{ MAIN_MENU };

	inline static MainMenu mainMenu;
	inline static GameplayMenu gameplayMenu;
	inline static PlaythroughPauseMenu playthroughPauseMenu;
	inline static PlaythroughEndMenu playthroughEndMenu;
	inline static LevelPickerMenu levelPickerMenu;
	inline static LevelPauseMenu levelPauseMenu;
	inline static StatisticsMenu statisticsMenu;
	inline static SettingsMenu settingsMenu;
	inline static AboutMenu aboutMenu;

	// resizing a menu recalculates its buttons so that's all we have to do to initialize one
	static void init()
	{
		resize();
	}
	
	static void resize()
	{
		mainMenu.resize();
		gameplayMenu.resize();
		playthroughPauseMenu.resize();
		playthroughEndMenu.resize();
		levelPickerMenu.resize();
		levelPauseMenu.resize();
		statisticsMenu.resize();
		settingsMenu.resize();
		aboutMenu.resize();
	}

	static void setActiveMenu(MenuType type)
	{
		previousActiveMenu = activeMenu;
		activeMenu = type;
	}
};


// helper functions -----------------------------------------------------------------------

// returns as a string a given float with 2 decimal places only
std::string floatTwoDecimalsString(float value);