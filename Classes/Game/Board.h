#pragma once
#include <vector>
#include "Block.h"
#include "Position.h"

class Board
{
	Position max;
	std::vector<std::vector<Block>> grid;
public:
	Board::Board(int width, int height);
	std::vector<int> setup();
	std::vector<Position> findBlocksToDestroy(Position start);
	std::vector<int> replaceBlocks(std::vector<Position> positions);
	bool hasLegalMoves();
};

