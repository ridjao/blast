#pragma once
#include <vector>
#include "Position.h"
#include "BlockTypeIds.h"

struct SceneDTO
{
	int score;
	int timeIncrement;
	std::vector<BlockTypeId> boardBlocks;
	std::vector<Position> blocksToDestroy;
};
