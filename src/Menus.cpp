#include "Menus.h"

#include "audio.h"
#include "Breakout.h"
#include "Levels.h"
#include "Powerups.h"
#include "Renderer.h"
#include "Stats.h"
#include "TextRenderer.h"
#include "Textures.h"

#include <glm/glm.hpp>
#include <cmath>
#include <string>
#include <iomanip>
#include <sstream>
#include <iostream>
#include <ios>

// Button struct -----------------------------------------------------------------------

bool Button::isHovered() const
{
	bool isHoveredX{ g_mouse.pos.x > pos.x && g_mouse.pos.x < (pos.x + size.x) };
	bool isHoveredY{ g_mouse.pos.y > pos.y && g_mouse.pos.y < (pos.y + size.y) };

	return isHoveredX && isHoveredY;
}

void Button::draw(unsigned int texture, glm::vec2 target) const
{
	// draw the button
	if(isHovered())
		renderQuadTexture(texture, pos, size, 0.0f, colorHovered, target);
	else
		renderQuadTexture(texture, pos, size, 0.0f, color, target);

	// render text

	// text size
	float textSize{ scaleTextToFit(Fonts::MartianMonoRegular, text, size) };

	// text pos
	glm::vec2 standardTextSize{ textDimensions(Fonts::MartianMonoRegular, text)};
	glm::vec2 currentTextSize{ standardTextSize * textSize };

	glm::vec2 posText{};
	posText.x = pos.x + (size.x - currentTextSize.x) / 2.0f;	// button pos + half of the differnece between button and text size
	posText.y = pos.y + (size.y - currentTextSize.y) / 2.0f;	// +(Fonts::MartianMonoRegular.characters['H'].Bearing.y * textSize) * 0.2f;


	renderText(Fonts::MartianMonoRegular, text, posText, textSize, colorText, target);
}

void Button::drawCustomScale(unsigned int texture, float textscale, std::string textForCustomScale, glm::vec2 target) const
{
	// draw the button
	if(isHovered())
		renderQuadTexture(texture, pos, size, 0.0f, colorHovered, target);
	else
		renderQuadTexture(texture, pos, size, 0.0f, color, target);

	// render text

	// text size - from user
	float textSize{ textscale };

	// text pos
	glm::vec2 standardTextSize{ textDimensions(Fonts::MartianMonoRegular, textForCustomScale)};
	glm::vec2 currentTextSize{ standardTextSize * textSize };

	glm::vec2 posText{};
	posText.x = pos.x + (size.x - currentTextSize.x) / 2.0f;	// button pos + half of the differnece between button and text size
	posText.y = pos.y + (size.y - currentTextSize.y) / 2.0f;


	renderText(Fonts::MartianMonoRegular, text, posText, textSize, colorText, target);
}
// TextBox struct -----------------------------------------------------------------------

void TextBox::draw(glm::vec2 target) const // render text
{
	// text size
	float textSize{ scaleTextToFit(Fonts::MartianMonoRegular, text, size) };

	// text pos
	glm::vec2 standardTextSize{ textDimensions(Fonts::MartianMonoRegular, text) };
	glm::vec2 currentTextSize{ standardTextSize * textSize };

	glm::vec2 posText{};
	posText.x = pos.x + (size.x - currentTextSize.x) / 2.0f;	// button pos + half of the differnece between button and text size
	posText.y = pos.y + (size.y - currentTextSize.y) / 2.0f;

	renderText(Fonts::MartianMonoRegular, text, posText, textSize, colorText, target);
}

void TextBox::drawCustomScale(float textscale, std::string textForCustomScale, glm::vec2 target) const
{
	// text size - from user
	float textSize{ textscale };

	// text pos
	glm::vec2 standardTextSize{ textDimensions(Fonts::MartianMonoRegular, textForCustomScale) };
	glm::vec2 currentTextSize{ standardTextSize * textSize };

	glm::vec2 posText{};
	posText.x = pos.x + (size.x - currentTextSize.x) / 2.0f;	// button pos + half of the differnece between button and text size
	posText.y = pos.y + (size.y - currentTextSize.y) / 2.0f;


	renderText(Fonts::MartianMonoRegular, text, posText, textSize, colorText, target);
}

// SliderHandle struct -----------------------------------------------------------------------

bool SliderHandle::isHovered() const
{
	bool isHoveredX{ g_mouse.pos.x > pos.x && g_mouse.pos.x < (pos.x + size.x) };
	bool isHoveredY{ g_mouse.pos.y > pos.y && g_mouse.pos.y < (pos.y + size.y) };

	return isHoveredX && isHoveredY;
}

void SliderHandle::draw(unsigned int texture, glm::vec2 target) const
{
	// draw the handle
	if (isHovered())
		renderQuadTexture(texture, pos, size, 0.0f, colorHovered, target);
	else
		renderQuadTexture(texture, pos, size, 0.0f, color, target);
}

// Slider struct -----------------------------------------------------------------------

bool Slider::isHovered() const
{
	bool isHoveredX{ g_mouse.pos.x > pos.x && g_mouse.pos.x < (pos.x + size.x) };
	bool isHoveredY{ g_mouse.pos.y > pos.y && g_mouse.pos.y < (pos.y + size.y) };

	return isHoveredX && isHoveredY;
}

void Slider::resize(glm::vec2 sliderPos, glm::vec2 sliderSize, float handleValue)
{
	// resize the slider
	posWithHandle = sliderPos;
	size = glm::vec2{ sliderSize.x, sliderSize.y / 3.0f };
	pos = glm::vec2{ sliderPos.x, sliderPos.y + (sliderSize.y / 3.0f) };

	// resize the handle
	handle.size = glm::vec2{ sliderSize.y };	// to make it square
	handle.pos.y = sliderPos.y;

	float handleValueClamped{ glm::clamp(handleValue, 0.0f, 1.0f) };
	value = std::lerp( rangeMin, rangeMax, handleValueClamped );
	float positonHandle = std::lerp(pos.x, pos.x + size.x, handleValueClamped);
	handle.pos.x = positonHandle - (handle.size.x / 2.0f);	// center -> left edge
}

void Slider::update()
{
	if (!isSliding)
	{
		if (!(handle.isHovered() || isHovered()) || !g_mouse.leftClick)
			return;
	}

	if (g_mouse.leftClick)
		isSliding = true;
	else if (!g_mouse.leftClickHeld)	// if we are not sliding return
	{
		isSliding = false;
		return;
	}

	// move the handle
	float handleCenterX = g_mouse.pos.x;
	handle.pos.x = handleCenterX - (handle.size.x / 2.0f);	// center -> left edge
	
	// make sure it is not out of bounds
	if (handle.pos.x < pos.x)
		handle.pos.x = pos.x;
	else if (handle.pos.x + handle.size.x > pos.x + size.x)
		handle.pos.x = pos.x + size.x - handle.size.x;

	// get the value

	// normalise the value(handleCenterX) from [pos.x, pos.x + size.x] to [0,1]
	float normalizedFraction{ (handleCenterX - pos.x) / (pos.x + size.x - pos.x) };
	// scale it to the new range[rangeMin, rangeMax]
	float scaledFraction{ normalizedFraction * (rangeMax - rangeMin) };
	// set the value
	value = scaledFraction + rangeMin;
	// clamp the value - the same as making sure the handle.pos is not out of bounds
	value = glm::clamp(value, rangeMin, rangeMax);
}

void Slider::draw(unsigned int texture, unsigned int handleTexture, glm::vec2 target) const
{
	// draw the slider
	renderQuadTexture(texture, pos, size, 0.0f, color, target);
	// draw the handle
	handle.draw(handleTexture, target);
}

// MainMenu struct -----------------------------------------------------------------------

void MainMenu::draw() const
{
	
	// draw background
	glm::vec2 target{ static_cast<float>(g_screenWidth), static_cast<float>(g_screenHeight) };
	renderQuad(glm::vec2{ 0.0f, 0.0f }, target, 0.0f, g_backgroundColor, target);

	// draw buttons
	newPlaythrough.draw(newPlaythrough.isHovered() ? Textures::buttonHovered : Textures::button, target);
	continuePlaythrough.draw(continuePlaythrough.isHovered() ? Textures::buttonHovered : Textures::button, target);
	playLevel.draw(playLevel.isHovered() ? Textures::buttonHovered : Textures::button, target);
	settings.draw(settings.isHovered() ? Textures::buttonHovered : Textures::button, target);
	viewStats.draw(viewStats.isHovered() ? Textures::buttonHovered : Textures::button, target);
	about.draw(about.isHovered() ? Textures::buttonHovered : Textures::button, target);
}

void MainMenu::update(glm::vec2& paddlePos, glm::vec2& ballPos, glm::vec2& ballVelocity, bool& ballIsStuck, bool& ballResetParicles) const
{
	if (!g_mouse.leftClick) return;	// return if the left mouse button is not clicked
	// if the left mouse button is clicked we check if any button is hovered id so we perform a necessary action
	if (newPlaythrough.isHovered())
	{
		// reset the playthrough stats
		Stats::timePlaythrough = 0.0f;
		Stats::attemptsPlaythrough = 0;
		Stats::activeLevelPlaythrough = 0;

		Stats::attemptsTotal++;	// because we start a new attempt

		g_gameState = GAME_RESET;	// to reset the level if a player played and exited
		Menus::gameplayMenu.gamePlayMenuType = GAME_PLAY_PLAYTHROUGH;
		Menus::setActiveMenu(GAME_PLAY);
	}
	else if (continuePlaythrough.isHovered())
	{
		// if the data is incomplete simply do nothing
		if (Stats::levelDataPlaythrough.empty() || Stats::powerupsDataPlaythrough.empty())
		{
			return;
		}
		// get the stats form file
		Levels::activeLevel = Stats::activeLevelPlaythrough;
		Levels::mainLevels.at(Levels::activeLevel).data = Stats::levelDataPlaythrough;
		Powerups::powerupsList = Stats::powerupsDataPlaythrough;
		ballPos = Stats::ballPosPlaythrough;
		ballVelocity = Stats::ballVelocityPlaythrough;
		ballIsStuck = Stats::ballIsStuckPlaythrough;
		paddlePos = Stats::paddlePosPlaythrough;

		// so there are no particles from the previous unsaved playthroughs
		ballResetParicles = true;

		// save the starting state in case of quit no saving
		Stats::timePlayBegin = Stats::timePlaythrough;
		Stats::attemptsPlayBegin = Stats::attemptsPlaythrough;
		Stats::activeLevelPlayBegin = Stats::activeLevelPlaythrough;
		Stats::levelDataPlayBegin = Stats::levelDataPlaythrough;
		Stats::powerupsDataPlayBegin = Stats::powerupsDataPlaythrough;
		Stats::ballPosPlayBegin = Stats::ballPosPlaythrough;
		Stats::ballVelocityPlayBegin = Stats::ballVelocityPlaythrough;
		Stats::ballIsStuckPlayBegin = Stats::ballIsStuckPlaythrough;
		Stats::paddlePosPlayBegin = Stats::paddlePosPlaythrough;

		g_gameState = GAME_ACTIVE;
		Menus::gameplayMenu.gamePlayMenuType = GAME_PLAY_PLAYTHROUGH;
		Menus::setActiveMenu(GAME_PLAY);
	}
	else if (playLevel.isHovered())
	{
		Menus::setActiveMenu(CHOOSE_LEVEL_MENU);
	}
	else if (settings.isHovered())
	{
		Menus::setActiveMenu(SETTINGS_MENU);
	}
	else if (viewStats.isHovered())
	{
		Menus::statisticsMenu.statisticsMenuType = STATISTICS_DEFAULT;
		Menus::setActiveMenu(VIEW_STATS_MENU);
	}
	else if (about.isHovered())
	{
		Menus::setActiveMenu(MAIN_ABOUT_MENU);
	}

}

// GameplayMenu struct -----------------------------------------------------------------------

void GameplayMenu::draw()
{
	glm::vec2 target{ static_cast<float>(g_screenWidth), static_cast<float>(g_screenHeight) };

	// draw buttons	- the game itself is not a part of the menu
	pause.draw(Textures::buttonPause, target);

	// render attempts
	if (Menus::gameplayMenu.gamePlayMenuType == GAME_PLAY_PLAYTHROUGH) attempts.text = "attempt: " + std::to_string(Stats::attemptsPlaythrough);
	else attempts.text = "attempt: " + std::to_string(Stats::attemptsLevel);
	attempts.drawCustomScale(scaleTextToFit(Fonts::MartianMonoRegular, "attempt: 00", attempts.size), "attempt: 00", target);
	// render level	(+1 so the first level in 1 not 0)
	levelNumber.text = "level: " + std::to_string(Levels::activeLevel + 1) + "/" + std::to_string(Levels::totalLevels);
	levelNumber.drawCustomScale(scaleTextToFit(Fonts::MartianMonoRegular, "level: 00/00", levelNumber.size), "level: 00/00", target);

	float textSize{ static_cast<float>(g_screenWidth) / static_cast<float>(g_constScreenWidth) };
	textSize = textSize > 1.0f ? 1.0f : textSize;

	// render the attempt counter
	//renderText(Fonts::MartianMonoRegular, "attempt: " + std::to_string(Stats::attemptsPlaythrough), glm::vec2(10.0f, 10.0f), textSize,
	//	glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), target);

	// render the time since the start of the playtime, abbreviated to 2 decimal places
	std::string timePlayed{};
	if (Menus::gameplayMenu.gamePlayMenuType == GAME_PLAY_PLAYTHROUGH) timePlayed = floatTwoDecimalsString(Stats::timePlaythrough);
	else timePlayed = floatTwoDecimalsString(Stats::timeLevel);

	//glm::vec2 timeTextSizeDefault{ textDimensions(Fonts::MartianMonoRegular, "time: " + timePlayed) };
	//glm::vec2 timeTextSize{ timeTextSizeDefault * textSize };
	//float posX{ static_cast<float>(g_screenWidth) - timeTextSize.x - 55.0f};	// - 45.0f because the pause button and - 10.0f to leave a bit of room
	//glm::vec2 timeTextPos{ 2.0f * levelNumber.size.x , (g_windowTopOffset - timeTextSize.y) / 2.0f };
	//renderText(Fonts::MartianMonoRegular, "time: " + timePlayed, timeTextPos, textSize, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), target);


	time.text = "time: " + timePlayed;
	//time.draw(target);
	time.drawCustomScale(scaleTextToFit(Fonts::MartianMonoRegular, "time: 00.00", time.size), "time: 00.00", target);

	// render powerups and their's time

	// setup variables that don't change throughout all rendered powerups
	const float screenWidth{ static_cast<float>(g_screenWidth) };
	const float offsetScreenWidth{ screenWidth < g_constScreenWidth ? screenWidth : g_constScreenWidth };
	const float offset{ offsetScreenWidth / 78.0f };	// for the layout, explained in menus_gameplayMenu_draft.png
	const float UIoffset{ static_cast<float>(g_windowBottomOffset) - 45.0f };
	const glm::vec2 size{ 5.0f * offset, static_cast<float>(g_windowBottomOffset) - UIoffset * 2.0f };
	const float posY{ static_cast<float>(g_screenHeight) - (static_cast<float>(g_windowBottomOffset) - UIoffset) };
	// const size(means we check "00.00"  not the actual text) so the text doesn't change size and flicker
	const float scaleToFit{ scaleTextToFit(Fonts::MartianMonoRegular, "00.00", size) };
	// also const to avoid flickering
	const glm::vec2 standardTextSize{ textDimensions(Fonts::MartianMonoRegular, "00.00") };
	const glm::vec2 scaledTextSize{ standardTextSize * scaleToFit };

	glm::vec4 powerupColor{};
	// PLAYER_SPEED_INCREASE
	if (Powerups::isActive(PLAYER_SPEED_INCREASE)) powerupColor = { 0.0f, 0.0f, 1.0f, 1.0f };
	else powerupColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	glm::vec2 powerupPos = { screenWidth - 77.0f * offset, posY };
	renderQuadTexture(Textures::iconPlayerSpeedIncrease, powerupPos, size, 0.0f, powerupColor, target);
	// get the time
	std::string powerupTimeString = floatTwoDecimalsString(Powerups::getActiveTime(PLAYER_SPEED_INCREASE));
	// render the time
	glm::vec2 powerupTexPos = { screenWidth - 71.0f * offset, posY};
	powerupTexPos.x += (size.x - scaledTextSize.x) / 2.0f;
	powerupTexPos.y += (size.y - scaledTextSize.y) / 2.0f;
	renderText(Fonts::MartianMonoRegular, powerupTimeString, powerupTexPos, scaleToFit, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), target);

	// PLAYER_SIZE_INCREASE
	if (Powerups::isActive(PLAYER_SIZE_INCREASE)) powerupColor = { 0.0f, 0.5f, 1.0f, 1.0f };
	else powerupColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	powerupPos = { screenWidth - 64.0f * offset, posY };
	renderQuadTexture(Textures::iconPlayerSizeIncrease, powerupPos, size, 0.0f, powerupColor, target);
	// get the time
	powerupTimeString = floatTwoDecimalsString(Powerups::getActiveTime(PLAYER_SIZE_INCREASE));
	// render the time
	powerupTexPos = { screenWidth - 58.0f * offset,  posY };
	powerupTexPos.x += (size.x - scaledTextSize.x) / 2.0f;
	powerupTexPos.y += (size.y - scaledTextSize.y) / 2.0f;
	renderText(Fonts::MartianMonoRegular, powerupTimeString, powerupTexPos, scaleToFit, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), target);

	// PLAYER_STICKY_PADDLE
	if (Powerups::isActive(PLAYER_STICKY_PADDLE)) powerupColor = { 0.0f, 0.8f, 1.0f, 1.0f };
	else powerupColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	powerupPos = { screenWidth - 51.0f * offset, posY };
	renderQuadTexture(Textures::iconPlayerStickyPaddle, powerupPos, size, 0.0f, powerupColor, target);
	// get the time
	powerupTimeString = floatTwoDecimalsString(Powerups::getActiveTime(PLAYER_STICKY_PADDLE));
	// render the time
	powerupTexPos = { screenWidth - 45.0f * offset,  posY };
	powerupTexPos.x += (size.x - scaledTextSize.x) / 2.0f;
	powerupTexPos.y += (size.y - scaledTextSize.y) / 2.0f;
	renderText(Fonts::MartianMonoRegular, powerupTimeString, powerupTexPos, scaleToFit, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), target);

	// BALL_PASS_THROUGH_BLOCKS
	if (Powerups::isActive(BALL_PASS_THROUGH_BLOCKS)) powerupColor = { 0.0f, 1.0f, 0.0f, 1.0f };
	else powerupColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	powerupPos = { screenWidth - 38.0f * offset, posY };
	renderQuadTexture(Textures::iconBallPassThroughBlocks, powerupPos, size, 0.0f, powerupColor, target);
	// get the time
	powerupTimeString = floatTwoDecimalsString(Powerups::getActiveTime(BALL_PASS_THROUGH_BLOCKS));
	// render the time
	powerupTexPos = { screenWidth - 32.0f * offset,  posY };
	powerupTexPos.x += (size.x - scaledTextSize.x) / 2.0f;
	powerupTexPos.y += (size.y - scaledTextSize.y) / 2.0f;
	renderText(Fonts::MartianMonoRegular, powerupTimeString, powerupTexPos, scaleToFit, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), target);

	// SCREEN_CONFUSE
	if (Powerups::isActive(SCREEN_CONFUSE)) powerupColor = { 1.0f, 0.5f, 0.0f, 1.0f };
	else powerupColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	powerupPos = { screenWidth - 25.0f * offset, posY };
	renderQuadTexture(Textures::iconScreenConfuse, powerupPos, size, 0.0f, powerupColor, target);
	// get the time
	powerupTimeString = floatTwoDecimalsString(Powerups::getActiveTime(SCREEN_CONFUSE));
	// render the time
	powerupTexPos = { screenWidth - 19.0f * offset,  posY };
	powerupTexPos.x += (size.x - scaledTextSize.x) / 2.0f;
	powerupTexPos.y += (size.y - scaledTextSize.y) / 2.0f;
	renderText(Fonts::MartianMonoRegular, powerupTimeString, powerupTexPos, scaleToFit, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), target);

	// SCREEN_CHAOS
	if (Powerups::isActive(SCREEN_CHAOS)) powerupColor = { 1.0f, 0.0f, 0.0f, 1.0f };
	else powerupColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	powerupPos = { screenWidth - 12.0f * offset, posY };
	renderQuadTexture(Textures::iconScreenChaos, powerupPos, size, 0.0f, powerupColor, target);
	// get the time
	powerupTimeString = floatTwoDecimalsString(Powerups::getActiveTime(SCREEN_CHAOS));
	// render the time
	powerupTexPos = { screenWidth - 6.0f * offset,  posY };
	powerupTexPos.x += (size.x - scaledTextSize.x) / 2.0f;
	powerupTexPos.y += (size.y - scaledTextSize.y) / 2.0f;
	renderText(Fonts::MartianMonoRegular, powerupTimeString, powerupTexPos, scaleToFit, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), target);
}

void GameplayMenu::update() const
{
	if (!g_mouse.leftClick) return;	// return if the left mouse button is not clicked

	// if the left mouse button is clicked we check if any button is hovered id so we perform a necessary action
	if (pause.isHovered())
	{
		if (gamePlayMenuType == GAME_PLAY_PLAYTHROUGH)
		{
			Menus::setActiveMenu(PLAYTHROUGH_PAUSE_MENU);
		}
		else if (gamePlayMenuType == GAME_PLAY_LEVEL)
		{
			Menus::setActiveMenu(LEVEL_PAUSE_MENU);
		}
	}
}

// PlaythroughPauseMenu struct -----------------------------------------------------------------------

void PlaythroughPauseMenu::draw() const
{
	// draw background - slightly opaque
	glm::vec2 target{ static_cast<float>(g_screenWidth), static_cast<float>(g_screenHeight) };
	renderQuad(glm::vec2{ 0.0f, 0.0f }, target, 0.0f, glm::vec4(g_backgroundColor.r, g_backgroundColor.g, g_backgroundColor.b, g_backgroundColor.a * 0.7), target);

	// draw buttons
	resume.draw(resume.isHovered() ? Textures::buttonHovered : Textures::button, target);
	settings.draw(settings.isHovered() ? Textures::buttonHovered : Textures::button, target);
	viewStats.draw(viewStats.isHovered() ? Textures::buttonHovered : Textures::button, target);
	saveAndQuit.draw(saveAndQuit.isHovered() ? Textures::buttonHovered : Textures::button, target);
	quitNoSaving.draw(quitNoSaving.isHovered() ? Textures::buttonHovered : Textures::button, target);
}

void PlaythroughPauseMenu::update(glm::vec2 paddlePos, glm::vec2 ballPos, glm::vec2 ballVelocity, bool ballIsStuck) const
{
	if (!g_mouse.leftClick) return;	// return if the left mouse button is not clicked

	// if the left mouse button is clicked we check if any button is hovered id so we perform a necessary action
	if (resume.isHovered())
	{
		Menus::setActiveMenu(GAME_PLAY);
	}
	else if (settings.isHovered())
	{
		Menus::setActiveMenu(SETTINGS_MENU);
	}
	else if (viewStats.isHovered())
	{
		Menus::statisticsMenu.statisticsMenuType = STATISTICS_PLAYTHROUGH;
		Menus::setActiveMenu(VIEW_STATS_MENU);
	}
	else if (saveAndQuit.isHovered())
	{
		// save the stats form playthrough
		Stats::activeLevelPlaythrough = Levels::activeLevel;
		Stats::levelDataPlaythrough = Levels::mainLevels.at(Levels::activeLevel).data;
		Stats::powerupsDataPlaythrough = Powerups::powerupsList;
		Stats::ballPosPlaythrough = ballPos;
		Stats::ballVelocityPlaythrough = ballVelocity;
		Stats::ballIsStuckPlaythrough = ballIsStuck;
		Stats::paddlePosPlaythrough = paddlePos;
		Stats::writeToFile();

		Menus::setActiveMenu(MAIN_MENU);
	}
	else if (quitNoSaving.isHovered())
	{
		// set the playthrough stats to how they were at the start of the playthrough -  unodoes everything that isn't supposed to be saved

		Stats::timePlaythrough = Stats::timePlayBegin;
		Stats::attemptsPlaythrough = Stats::attemptsPlayBegin;
		Stats::activeLevelPlaythrough = Stats::activeLevelPlayBegin;
		Stats::levelDataPlaythrough = Stats::levelDataPlayBegin;
		Stats::powerupsDataPlaythrough = Stats::powerupsDataPlayBegin;
		Stats::ballPosPlaythrough = Stats::ballPosPlayBegin;
		Stats::ballVelocityPlaythrough = Stats::ballVelocityPlayBegin;
		Stats::ballIsStuckPlaythrough = Stats::ballIsStuckPlayBegin;
		Stats::paddlePosPlaythrough = Stats::paddlePosPlayBegin;


		Menus::setActiveMenu(MAIN_MENU);
	}
}

// PlaythroughEndMenu struct -----------------------------------------------------------------------

void PlaythroughEndMenu::draw()
{
	glm::vec2 target{ static_cast<float>(g_screenWidth), static_cast<float>(g_screenHeight) };

	// draw background
	renderQuad(glm::vec2{ 0.0f, 0.0f }, target, 0.0f, g_backgroundColor, target);

	// set the text for TextBoxes: time, bestTime, attempts,  - only once so tthat new best show up correctly
	if (!calculatedStats)
	{
		calculatedStats = true;

		// time
		time.text = "  time: " + floatTwoDecimalsString(Stats::timePlaythrough) + "  ";

		// bestTime
		if (Stats::timePlaythrough < Stats::bestPlaythroughTime)
		{
			bestTime.text = "  new best!  ";
			Stats::bestPlaythroughTime = Stats::timePlaythrough;
		}
		else
		{
			bestTime.text = "  best: " + floatTwoDecimalsString(Stats::bestPlaythroughTime) + "  ";
		}

		// attempts
		attempts.text = "  attempts: " + std::to_string(Stats::attemptsPlaythrough) + "  ";

		// bestAttempts
		if (Stats::attemptsPlaythrough < static_cast<unsigned int>(Stats::bestPlaythroughAttempts))
		{
			bestAttempts.text = "  new best!  ";
			Stats::bestPlaythroughAttempts = Stats::attemptsPlaythrough;
		}
		else
		{
			bestAttempts.text = "  best: " + std::to_string(Stats::bestPlaythroughAttempts) + "  ";
		}
	}

	// render
	completeMessage.draw(target);
	time.draw(target);
	bestTime.draw(target);
	attempts.draw(target);
	bestAttempts.draw(target);

	newPlaythrough.draw(newPlaythrough.isHovered() ? Textures::buttonHovered : Textures::button, target);
	toMainMenu.draw(toMainMenu.isHovered() ? Textures::buttonHovered : Textures::button, target);
}

void PlaythroughEndMenu::update() const
{
	if (!g_mouse.leftClick) return;	// return if the left mouse button is not clicked

	// if the left mouse button is clicked we check if any button is hovered id so we perform a necessary action
	if (newPlaythrough.isHovered()) // acts like newPlaythrough in MainMenu
	{
		// reset the playthrough stats
		Stats::timePlaythrough = 0.0f;
		Stats::attemptsPlaythrough = 0;
		Stats::activeLevelPlaythrough = 0;

		g_gameState = GAME_RESET;	// to reset the level if a player played and exited
		Menus::gameplayMenu.gamePlayMenuType = GAME_PLAY_PLAYTHROUGH;
		Menus::setActiveMenu(GAME_PLAY);
	}
	else if (toMainMenu.isHovered()) // acts like quitNoSaving - becasue what is the point of saving if you completed
	{
		Stats::timePlaythrough = Stats::timePlayBegin;
		Stats::attemptsPlaythrough = Stats::attemptsPlayBegin;
		Stats::activeLevelPlaythrough = Stats::activeLevelPlayBegin;
		Stats::levelDataPlaythrough = Stats::levelDataPlayBegin;
		Stats::powerupsDataPlaythrough = Stats::powerupsDataPlayBegin;
		Stats::ballPosPlaythrough = Stats::ballPosPlayBegin;
		Stats::ballVelocityPlaythrough = Stats::ballVelocityPlayBegin;
		Stats::ballIsStuckPlaythrough = Stats::ballIsStuckPlayBegin;
		Stats::paddlePosPlaythrough = Stats::paddlePosPlayBegin;

		Menus::setActiveMenu(MAIN_MENU);
	}
}

// LevelPickerMenu struct -----------------------------------------------------------------------

void LevelPickerMenu::draw()
{
	glm::vec2 target{ static_cast<float>(g_screenWidth), static_cast<float>(g_screenHeight) };

	// draw background
	renderQuad(glm::vec2{ 0.0f, 0.0f }, target, 0.0f, g_backgroundColor, target);

	pickLevel.draw(target);
	goBack.draw(goBack.isHovered() ? Textures::buttonGoBackHovered : Textures::buttonGoBack, target);

	// draw all level buttons
	for (unsigned int i = 0; i < levelButtons.size(); i++)
	{
		if (i <= Stats::levelsUnlocked)
		{
			levelButtons[i].color = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
			levelButtons[i].colorHovered = levelButtons[i].color;
		}
		else
		{
			levelButtons[i].color = Button::colorDefault;
			levelButtons[i].colorHovered = Button::colorDefaultHovered;
		}
		levelButtons[i].draw(Textures::buttonLevelPick, target);
	}
}

void LevelPickerMenu::update()
{
	if (!g_mouse.leftClick) return;	// return if the left mouse button is not clicked

	// if the left mouse button is clicked we check if any button is hovered id so we perform a necessary action
	if (goBack.isHovered())
	{
		if(g_gameState == GAME_WIN)	// if a level is completed always go to the main menu not to the empty level
			Menus::setActiveMenu(MAIN_MENU);
		else
			Menus::setActiveMenu(Menus::previousActiveMenu);
	}
	for (unsigned int i = 0; i < levelButtons.size(); i++)
	{
		// if the level is unlocked start the gameplay, otherwise do nothing
		if (levelButtons[i].isHovered() && i <= Stats::levelsUnlocked)
		{
			Levels::activeLevel = static_cast<int>(i);
			Menus::gameplayMenu.gamePlayMenuType = GAME_PLAY_LEVEL;
			Stats::attemptsTotal++;	// because we start a new attempt
			Menus::setActiveMenu(GAME_PLAY);
			g_gameState = GAME_LEVEL_RESET;	// to reset the level
		}
	}
}

// LevelPauseMenu struct -----------------------------------------------------------------------

void LevelPauseMenu::draw() const
{
	glm::vec2 target{ static_cast<float>(g_screenWidth), static_cast<float>(g_screenHeight) };

	// draw background
	renderQuad(glm::vec2{ 0.0f, 0.0f }, target, 0.0f, glm::vec4(g_backgroundColor.r, g_backgroundColor.g, g_backgroundColor.b, g_backgroundColor.a * 0.7), target);

	// draw buttons
	resume.draw(resume.isHovered() ? Textures::buttonHovered : Textures::button, target);
	settings.draw(settings.isHovered() ? Textures::buttonHovered : Textures::button, target);
	viewStats.draw(viewStats.isHovered() ? Textures::buttonHovered : Textures::button, target);
	playDifferentLevel.draw(playDifferentLevel.isHovered() ? Textures::buttonHovered : Textures::button, target);
	toMainMenu.draw(toMainMenu.isHovered() ? Textures::buttonHovered : Textures::button, target);
}

void LevelPauseMenu::update() const
{
	if (!g_mouse.leftClick) return;	// return if the left mouse button is not clicked

	// if the left mouse button is clicked we check if any button is hovered id so we perform a necessary action
	if (resume.isHovered())
	{
		Menus::setActiveMenu(GAME_PLAY);
	}
	else if (settings.isHovered())
	{
		Menus::setActiveMenu(SETTINGS_MENU);
	}
	else if (viewStats.isHovered())
	{
		Menus::statisticsMenu.statisticsMenuType = STATISTICS_LEVEL;
		Menus::setActiveMenu(VIEW_STATS_MENU);
	}
	else if (playDifferentLevel.isHovered())
	{
		Menus::setActiveMenu(CHOOSE_LEVEL_MENU);
	}
	else if (toMainMenu.isHovered())
	{
		Menus::setActiveMenu(MAIN_MENU);
	}

}

// StatisticsMenu struct -----------------------------------------------------------------------

void StatisticsMenu::draw()
{
	glm::vec2 target{ static_cast<float>(g_screenWidth), static_cast<float>(g_screenHeight) };

	// draw background
	renderQuad(glm::vec2{ 0.0f, 0.0f }, target, 0.0f, g_backgroundColor, target);

	// buttons
	goBack.draw(goBack.isHovered() ? Textures::buttonGoBackHovered : Textures::buttonGoBack, target);
	//resetAll.draw(resetAll.isHovered() ? Textures::buttonHovered : Textures::button, target);	// disabled for now

	// text boxes
	title.draw(target);

	// general stats
	attempts.text = "attempts: " + std::to_string(Stats::attemptsTotal);
	attempts.draw(target);
	brockenBlocks.text = "brocken blocks: " + std::to_string(Stats::brokenBlocks);
	brockenBlocks.draw(target);
	levelsUnlocked.text = "completed levels: " + std::to_string(Stats::levelsUnlocked + 1) + "/" + std::to_string(Levels::totalLevels); // levelsUnlocked + 1 as in game levels start at 1 not 0
	levelsUnlocked.draw(target);
	// general stats only if a playthrough has been completed
	if (Stats::finishedPlaythrough)
	{
		bestTime.text = "best playthrough time: " + floatTwoDecimalsString(Stats::bestPlaythroughTime);
		bestAttempts.text = "best playthrough attempts: " + std::to_string(Stats::bestPlaythroughAttempts);
	}
	else
	{
		bestTime.text = "best playthrough time: --:--";
		bestAttempts.text = "best playthrough attempts: --";
	}
	bestTime.draw(target);
	bestAttempts.draw(target);
	// stats for current playthrough/level - not shown in main menu stats
	if (statisticsMenuType == STATISTICS_PLAYTHROUGH)
	{
		currentTime.text = "current playthrough time: " + floatTwoDecimalsString(Stats::timePlaythrough);
		currentTime.draw(target);
		currentAttempts.text = "current playthrough attempts: " + std::to_string(Stats::attemptsPlaythrough);
		currentAttempts.draw(target);
		currentLevel.text = "current level: " + std::to_string(Levels::activeLevel + 1) + "/" + std::to_string(Levels::totalLevels);	// activeLevel + 1 as in game levels start at 1 not 0
		currentLevel.draw(target);
	}
	else if (statisticsMenuType == STATISTICS_LEVEL)
	{
		currentTime.text = "current level time: " + floatTwoDecimalsString(Stats::timeLevel);
		currentTime.draw(target);
		currentAttempts.text = "current level attempts: " + std::to_string(Stats::attemptsLevel);
		currentAttempts.draw(target);
		currentLevel.text = "played level: " + std::to_string(Levels::activeLevel + 1) + "/" + std::to_string(Levels::totalLevels);	// activeLevel + 1 as in game levels start at 1 not 0
		currentLevel.draw(target);
	}
	// else don't show this option (if we are here from main menu)
}

void StatisticsMenu::update() const
{
	if (!g_mouse.leftClick) return;	// return if the left mouse button is not clicked

	// if the left mouse button is clicked we check if any button is hovered id so we perform a necessary action
	if (goBack.isHovered())
	{
		Menus::setActiveMenu(Menus::previousActiveMenu);	// go back
	}
	//else if (resetAll.isHovered())	// disabled for now
	//{
	//	Stats::totalReset();
	//	Menus::setActiveMenu(MAIN_MENU);	//  always to main menu because i don't want the player to clear stats in the middle of a playthrough
	//}
}

// SettingsMenu struct -----------------------------------------------------------------------

void SettingsMenu::draw()
{
	glm::vec2 target{ static_cast<float>(g_screenWidth), static_cast<float>(g_screenHeight) };

	// draw background
	renderQuad(glm::vec2{ 0.0f, 0.0f }, target, 0.0f, g_backgroundColor, target);

	// buttons
	goBack.draw(goBack.isHovered() ? Textures::buttonGoBackHovered : Textures::buttonGoBack, target);

	// tittle
	tittle.draw(target);

	// settings - textBoxes drawn with custom scale to avoid flickering text as the volume can change
	musicVolume.text = "music volume: " + floatTwoDecimalsString(musicVolumeSlider.value);
	musicVolume.drawCustomScale(scaleTextToFit(Fonts::MartianMonoRegular, "music volume: 00.00", musicVolume.size), "music volume: 00.00", target);
	musicVolumeSlider.draw(Textures::slider, musicVolumeSlider.handle.isHovered() ? Textures::sliderHandleHovered : Textures::sliderHandle, target);

	sfxVolume.text = "sfx volume: " + floatTwoDecimalsString(sfxVolumeSlider.value);
	sfxVolume.drawCustomScale(scaleTextToFit(Fonts::MartianMonoRegular, "music volume: 00.00", sfxVolume.size), "music volume: 00.00", target);
	sfxVolumeSlider.draw(Textures::slider, sfxVolumeSlider.handle.isHovered() ? Textures::sliderHandleHovered : Textures::sliderHandle, target);
}

void SettingsMenu::update()
{
	musicVolumeSlider.update();
	setMusicVolume(musicVolumeSlider.value / 100.0f);	// musicVolumeSlider.value is in range [0,100] whereas setMusicVolume needs a value in range [0,1]
	sfxVolumeSlider.update();
	setSFXvolume(sfxVolumeSlider.value / 100.0f); // sfxVolumeSlider.value is in range [0,100] whereas setSFXvolume needs a value in range [0,1]

	if (!g_mouse.leftClick) return;	// return if the left mouse button is not clicked

	// if the left mouse button is clicked we check if any button is hovered id so we perform a necessary action
	if (goBack.isHovered())
	{
		Menus::setActiveMenu(Menus::previousActiveMenu);	// go back
	}
}

// aboutMenu struct -----------------------------------------------------------------------

void AboutMenu::draw()
{
	glm::vec2 target{ static_cast<float>(g_screenWidth), static_cast<float>(g_screenHeight) };

	// draw background
	renderQuad(glm::vec2{ 0.0f, 0.0f }, target, 0.0f, g_backgroundColor, target);

	// buttons
	goBack.draw(goBack.isHovered() ? Textures::buttonGoBackHovered : Textures::buttonGoBack, target);

	// tittle
	tittle.draw(target);

	// drawn with scale for "0" so the text can be as long as it needs to be
	gameBy.text = "game by:";
	gameBy.drawCustomScale(scaleTextToFit(Fonts::MartianMonoRegular, "0", gameBy.size), "0", target);
	inspiration.text = "inspired by:     learnopengl.com";
	inspiration.drawCustomScale(scaleTextToFit(Fonts::MartianMonoRegular, "0", inspiration.size), "0", target);
	font.text = "font:    Martian Mono - free font from google fonts";
	font.drawCustomScale(scaleTextToFit(Fonts::MartianMonoRegular, "0000", font.size), "0000", target);
	version.text = "version: (CMake edition) " + g_gameVersion;
	version.drawCustomScale(scaleTextToFit(Fonts::MartianMonoRegular, "0", version.size), "0", target);

	//  draw the logo
	renderQuadTexture(Textures::logo, logoPos, logoSize, 0.0f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), target);
}

void AboutMenu::update() const
{
	if (!g_mouse.leftClick) return;	// return if the left mouse button is not clicked

	// if the left mouse button is clicked we check if any button is hovered id so we perform a necessary action
	if (goBack.isHovered())
	{
		Menus::setActiveMenu(Menus::previousActiveMenu);	// go back
	}
}

// helper functions -----------------------------------------------------------------------

// returns as a string a given float with 2 decimal places only
std::string floatTwoDecimalsString(float value)
{
	std::stringstream ss;
	ss << std::fixed << std::setprecision(2) << value;
	std::string text = ss.str();
	return text;
}