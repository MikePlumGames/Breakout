#include "Stats.h"

#include "Blocks.h"
#include "Powerups.h"

#include <nlohmann/json.hpp>

#include <array>
#include <fstream>
#include <iostream>
#include <limits>
#include <vector>

NLOHMANN_JSON_SERIALIZE_ENUM(Blocks, {
	{ERROR_BLOCK_MISSING,"ERROR_BLOCK_MISSING"},
	{END_LINE,"END_LINE"},
	{EMPTY,"EMPTY"},
	{BLOCK_SOLID,"BLOCK_SOLID"},
	{BLOCK_RED,"BLOCK_RED"},
	{BLOCK_GREEN, "BLOCK_GREEN"},
	{BLOCK_BLUE, "BLOCK_BLUE"},
	{BLOCK_YELLOW, "BLOCK_YELLOW"},
	{BLOCK_PINK, "BLOCK_PINK"}
	});

NLOHMANN_JSON_SERIALIZE_ENUM(PowerupType, {
	{POWERUP_NONE,"POWERUP_NONE"},
	{PLAYER_SPEED_INCREASE,"PLAYER_SPEED_INCREASE"},
	{PLAYER_SIZE_INCREASE,"PLAYER_SIZE_INCREASE"},
	{PLAYER_STICKY_PADDLE,"PLAYER_STICKY_PADDLE"},
	{BALL_PASS_THROUGH_BLOCKS,"BALL_PASS_THROUGH_BLOCKS"},
	{SCREEN_CONFUSE, "SCREEN_CONFUSE"},
	{SCREEN_CHAOS, "SCREEN_CHAOS"}
	});

namespace glm
{
	// write a glm::vec2 to json
	void to_json(nlohmann::json& file, const glm::vec2& vector)
	{
		file = nlohmann::json{ {"x", vector.x}, {"y", vector.y} };
	}
	// read a glm::vec2 from json
	void from_json(const nlohmann::json& file, glm::vec2& vector)
	{
		vector.x = file.at("x").get<float>();
		vector.y = file.at("y").get<float>();
	}
}

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Powerup, type, active, falling, duration, pos, size)

void Stats::readFromFile()
{
	std::ifstream statsFile("assets/data/stats.json");
	if (!statsFile.is_open())
	{
		std::cout << "Stats::readFromFile error: assets/data/stats.json does not exist" << std::endl;
		return;
	}

	nlohmann::json statsData;
	statsFile >> statsData;
	statsFile.close();
	// extract all the stats

	// stats general:
	attemptsTotal = statsData.value("attemptsTotal", 0);
	brokenBlocks = statsData.value("brokenBlocks", 0);
	levelsUnlocked = statsData.value("levelsUnlocked", 0);
	finishedPlaythrough = statsData.value("finishedPlaythrough", false);
	bestPlaythroughTime = statsData.value("bestPlaythroughTime", std::numeric_limits<float>::max());
	bestPlaythroughAttempts = statsData.value("bestPlaythroughAttempts", std::numeric_limits<int>::max());
	// fail save
	if (bestPlaythroughTime < 0.0f) bestPlaythroughTime = std::numeric_limits<float>::max();
	if (bestPlaythroughAttempts < 0) bestPlaythroughAttempts = std::numeric_limits<int>::max();

	// stats for playthrough:
	timePlaythrough = statsData.value("timePlaythrough", 0.0f);
	attemptsPlaythrough = statsData.value("attemptsPlaythrough", 0);
	activeLevelPlaythrough = statsData.value("activeLevelPlaythrough", 0);
	// vector
	if (statsData.contains("levelDataPlaythrough") && statsData["levelDataPlaythrough"].is_array())
		statsData["levelDataPlaythrough"].get_to(levelDataPlaythrough);
	// array
	if (statsData.contains("powerupsDataPlaythrough") && statsData["powerupsDataPlaythrough"].is_array())
	{
		auto& jsonArray = statsData["powerupsDataPlaythrough"];
		if (jsonArray.size() <= Powerups::maxPowerups)
			powerupsDataPlaythrough = jsonArray.get<std::array<Powerup, Powerups::maxPowerups>>();
		else
			std::cout << "Stats::readFromFile error: powerupsDataPlaythrough array to large in assets/data/stats.json" << std::endl;
	}
	ballPosPlaythrough = statsData.value("ballPosPlaythrough", glm::vec2(0.0f));
	ballVelocityPlaythrough = statsData.value("ballVelocityPlaythrough", glm::vec2(0.0f));
	ballIsStuckPlaythrough = statsData.value("ballIsStuckPlaythrough", true);
	paddlePosPlaythrough = statsData.value("paddlePosPlaythrough", glm::vec2(0.0f));

}

void Stats::writeToFile()
{
	nlohmann::json statsData;

	// stats general:
	statsData["attemptsTotal"] = attemptsTotal;
	statsData["brokenBlocks"] = brokenBlocks;
	statsData["levelsUnlocked"] = levelsUnlocked;
	statsData["finishedPlaythrough"] = finishedPlaythrough;
	statsData["bestPlaythroughTime"] = bestPlaythroughTime;
	statsData["bestPlaythroughAttempts"] = bestPlaythroughAttempts;

	// stats for playthrough:
	statsData["timePlaythrough"] = timePlaythrough;
	statsData["attemptsPlaythrough"] = attemptsPlaythrough;
	statsData["activeLevelPlaythrough"] = activeLevelPlaythrough;
	statsData["levelDataPlaythrough"] = levelDataPlaythrough;
	statsData["powerupsDataPlaythrough"] = powerupsDataPlaythrough;
	statsData["ballPosPlaythrough"] = ballPosPlaythrough;
	statsData["ballVelocityPlaythrough"] = ballVelocityPlaythrough;
	statsData["ballIsStuckPlaythrough"] = ballIsStuckPlaythrough;
	statsData["paddlePosPlaythrough"] = paddlePosPlaythrough;

	std::ofstream statsFile("assets/data/stats.json");
	if (!statsFile.is_open())
	{
		std::cout << "Stats::writeToFile error: unable to write to assets/data/stats.json" << std::endl;
		return;
	}
	statsFile << statsData.dump(4);
	statsFile.close();
}

void Stats::totalReset()
{
	attemptsTotal = 0;
	brokenBlocks = 0;
	levelsUnlocked = 0;
	finishedPlaythrough = false;
	bestPlaythroughTime = std::numeric_limits<float>::max();
	bestPlaythroughAttempts = std::numeric_limits<int>::max();

	timePlaythrough = 0.0f;
	attemptsPlaythrough = 0;
	activeLevelPlaythrough = 0;
	levelDataPlaythrough.clear();
	powerupsDataPlaythrough = {};
	ballPosPlaythrough = glm::vec2{ 0.0f };
	ballVelocityPlaythrough = glm::vec2{ 0.0f };
	ballIsStuckPlaythrough = true;
	paddlePosPlaythrough = glm::vec2{ 0.0f };

	timePlayBegin = 0.0f;
	attemptsPlayBegin = 0;
	activeLevelPlayBegin = 0;
	levelDataPlayBegin.clear();
	powerupsDataPlayBegin = {};
	ballPosPlayBegin = glm::vec2{ 0.0f };
	ballVelocityPlayBegin = glm::vec2{ 0.0f };
	ballIsStuckPlayBegin = true;
	paddlePosPlayBegin = glm::vec2{ 0.0f };

	timeLevel = 0.0f;
	attemptsLevel = 0;
	currentLevel = 0;

	// to make sure it saves correctly
	writeToFile();
	readFromFile();
}