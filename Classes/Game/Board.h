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
	std::vector<Position> findBlocksToDestroy(const Position& start);
	std::vector<int> replaceBlocks(const std::vector<Position>& positions);
	bool hasLegalMoves();
};

