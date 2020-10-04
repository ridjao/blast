#pragma once
#include <vector>
#include "Block.h"
#include "Position.h"
#include "BlockTypeIds.h"

class Board
{
	Position max;
	std::vector<std::vector<Block>> grid;
public:
	Board(int width, int height);
	std::vector<BlockType> setup();
	std::vector<Position> findBlocksToDestroy(const Position& start);
	std::vector<BlockType> replaceBlocks(const std::vector<Position>& positions);
	bool hasLegalMoves();
};

