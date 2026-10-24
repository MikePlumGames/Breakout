#include "Levels.h"

#include <string>
#include <vector>

void Levels::loadAll()
{
	mainLevels.reserve(Levels::totalLevels);
	for (unsigned int i = 0; i < Levels::totalLevels; i++)
	{
		mainLevels.emplace_back(i, ("assets/levels/level_" + std::to_string(i) + ".txt").c_str());
	}
}