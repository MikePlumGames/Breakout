#pragma once

#include "Blocks.h"
#include "Powerups.h"

#include <array>
#include <vector>

struct Stats
{
	// stats general:
	inline static unsigned long long attemptsTotal{ 1 };
	inline static unsigned long long brokenBlocks{ 0 };
	inline static unsigned int levelsUnlocked{ 0 };	// which level ID is the highest unlocked
	inline static bool finishedPlaythrough{ false };	// false means a playthrough was never finished, so best stats shouldn't be displayed
	inline static float bestPlaythroughTime{ -1.0f };	// if < 0 they are initilaized to max possible value in void Stats::readFromFile();
	inline static int bestPlaythroughAttempts{ -1 };

	// stats for playthrough
	inline static float timePlaythrough{ 0.0f };
	inline static unsigned int attemptsPlaythrough{ 1 };
	inline static unsigned int activeLevelPlaythrough{ 0 };
	inline static std::vector<Blocks> levelDataPlaythrough{};
	inline static std::array<Powerup, Powerups::maxPowerups> powerupsDataPlaythrough{};
	inline static glm::vec2 ballPosPlaythrough{};
	inline static glm::vec2 ballVelocityPlaythrough{};
	inline static bool ballIsStuckPlaythrough{};
	inline static glm::vec2 paddlePosPlaythrough{};

	// stats for playthrough at the begining -  to make quit no saving work // save the starting state in case of quit no saving
	inline static float timePlayBegin{ 0.0f };
	inline static unsigned int attemptsPlayBegin{ 1 };
	inline static unsigned int activeLevelPlayBegin{ 0 };
	inline static std::vector<Blocks> levelDataPlayBegin{};
	inline static std::array<Powerup, Powerups::maxPowerups> powerupsDataPlayBegin{};
	inline static glm::vec2 ballPosPlayBegin{};
	inline static glm::vec2 ballVelocityPlayBegin{};
	inline static bool ballIsStuckPlayBegin{};
	inline static glm::vec2 paddlePosPlayBegin{};

	// stats for level(not saved):
	inline static float timeLevel{ 0.0f };
	inline static unsigned int attemptsLevel{ 1 };
	inline static unsigned int currentLevel{ 0 };

	// functions
	static void readFromFile();
	static void writeToFile();
	static void totalReset();
};