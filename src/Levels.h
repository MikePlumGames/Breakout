#pragma once

#include "LevelLoader.h"

#include <vector>

// stores the list of levels
struct Levels
{
	// amount of levels completed
	inline static unsigned int completedLevels{ 0 };
	// total amoun of levels
	inline static unsigned int totalLevels{ 20 };

	// vector containig all levels
	inline static std::vector<Level> mainLevels;
	// shows which level is active
	inline static unsigned int activeLevel{ 0 };
	// load all the levels
	static void loadAll();
};