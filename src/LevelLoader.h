#pragma once

#include "Blocks.h"

#include <vector>



struct Level
{
	// number informin on which level is the user
	unsigned int ID{};
	// number of columns in the level
	unsigned int columns{ 0 };
	// number of rows in the level
	unsigned int rows{ 0 };
	// starting level data 
	std::vector<Blocks> startingData;
	// level data
	std::vector<Blocks> data;

	// load the level
	Level(unsigned int _ID, const char* path);

	// reset the level
	void reset();

};