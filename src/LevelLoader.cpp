#include "LevelLoader.h"

#include "Blocks.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

Level::Level(unsigned int _ID, const char* filePath)
{
	// set the ID
	ID = _ID;

	// set the columns and rows to 0
	rows = 0;
	columns = 0;

	// load the level
	std::ifstream levelFile(filePath);

	// check if the file opened
	if (!levelFile.is_open())
	{
		std::cerr << "Trying to open level file failed: " << filePath << std::endl;
		return;
	}

	// go through the file line by line
	std::string currentLine;
	unsigned int rowCount{ 0 };
	unsigned int columnCount{ 0 };
	bool firstIteration{ true };	// loop counter
	while (std::getline(levelFile, currentLine))
	{
		std::stringstream currentLineStream(currentLine);
		int currentBlock{};	// because std::sstream >> Blocks doesn't work


		// check if the first data token is: START_LEVEL
		if (firstIteration && (!(currentLineStream >> currentBlock) || currentBlock != START_LEVEL))
		{
			std::cerr << "Error: Level file doesn't contain correct data: " << filePath << std::endl;
			levelFile.close();
			return;
		}
		firstIteration = false;

		// check next token - also configures amount of rows/columns
		while (currentLineStream >> currentBlock)
		{
			switch (currentBlock)
			{
			case START_LEVEL:
				startingData.clear();
				data.clear();
				break;
			case END_LEVEL:
				rows = rowCount;
				// set the data array to be the same as the startingData
				data = startingData;
				levelFile.close();
				return;
				break;
			case END_LINE:
				rowCount++;
				if(columns != 0 && columnCount != columns)
					std::cerr << "Error: Level file contains different amount of blocks in each row: " << filePath << std::endl;
				columns = columnCount;
				columnCount = 0;
				startingData.push_back(END_LINE);
				break;
			case EMPTY:
				columnCount++;
				startingData.push_back(EMPTY);
				break;
			case BLOCK_SOLID:
				columnCount++;
				startingData.push_back(BLOCK_SOLID);
				break;
			case BLOCK_RED:
				columnCount++;
				startingData.push_back(BLOCK_RED);
				break;
			case BLOCK_GREEN:
				columnCount++;
				startingData.push_back(BLOCK_GREEN);
				break;
			case BLOCK_BLUE:
				columnCount++;
				startingData.push_back(BLOCK_BLUE);
				break;
			case BLOCK_YELLOW:
				columnCount++;
				startingData.push_back(BLOCK_YELLOW);
				break;
			case BLOCK_PINK:
				columnCount++;
				startingData.push_back(BLOCK_PINK);
				break;
			default:
				std::cerr << "Error: Level file contains undefined block types: " << filePath << std::endl;
				columnCount++;
				startingData.push_back(ERROR_BLOCK_MISSING);
			}
		}
	}

	levelFile.close();
}

// resets the level to default state
void Level::reset()
{
	data = startingData;
}