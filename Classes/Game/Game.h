#pragma once
#include "Board.h"
#include "Position.h"

class Game
{
	int score;
	Board board;
	void updateScore(int destroyedBlocks);
public:
	Game();
	void start();
	void end();	
	bool hasLegalMoves();
	int getScore();
	int computeTimeIncrement(int destroyedBlocks);
	std::vector<Position> makeMove(const Move& move);
	std::vector<BlockType> getBoardBlocks();

	static const int WIDTH = 8;
	static const int HEIGHT = 8;
};

