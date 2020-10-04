#pragma once
#include "Board.h"
#include "Position.h"
#include "SceneDTO.h"

class Game
{
	int score;
	Board board;
	void updateScore(int destroyedBlocks);
	int computeTimeIncrement(int destroyedBlocks);
public:
	Game();
	SceneDTO start();
	SceneDTO makeMove(const Move& move);
	bool hasLegalMoves();
	int getScore();

	static const int WIDTH = 8;
	static const int HEIGHT = 8;
};

