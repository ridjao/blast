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
	void setup();
	void clear();
	void replaceBlocks(const std::vector<Position>& positions);
	std::vector<Position> findBlocksToDestroy(const Position& start);
	std::vector<BlockType> getBlocks();
	bool hasLegalMoves();
};

