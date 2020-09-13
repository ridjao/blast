#pragma once
#include <vector>
#include "Position.h"

struct SceneDTO
{
	int score;
	int timeIncrement;
	std::vector<int> boardBlocks;
	std::vector<Position> blocksToDestroy;
};

