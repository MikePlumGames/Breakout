#pragma once

// enum holding all possible block states
enum Blocks
{
	END_LEVEL = -4,
	START_LEVEL = -3,
	ERROR_BLOCK_MISSING = -2,
	END_LINE = -1,
	EMPTY = 0,
	BLOCK_SOLID = 1,
	BLOCK_RED = 2,
	BLOCK_GREEN = 3,
	BLOCK_BLUE = 4,
	BLOCK_YELLOW = 5,
	BLOCK_PINK = 6,
};