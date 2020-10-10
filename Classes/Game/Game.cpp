#include "Game.h"

Game::Game() : board(Game::WIDTH, Game::HEIGHT), score(0) {}

bool Game::hasLegalMoves()
{
	return board.hasLegalMoves();
}

std::vector<Position> Game::makeMove(const Move& move)
{
	std::vector<Position> blocksToDestroy = board.findBlocksToDestroy(move);
	if (blocksToDestroy.size() > 1)
	{
		board.replaceBlocks(blocksToDestroy);
		this->updateScore(blocksToDestroy.size());
	}
	return blocksToDestroy;
}

void Game::start()
{
	this->score = 0;
	board.setup();
}

void Game::end()
{
	board.clear();
}

void Game::updateScore(int destroyedBlocks)
{
	this->score += (destroyedBlocks - 1) * 80 + ((destroyedBlocks - 2) / 5) * ((destroyedBlocks - 2) / 5);
}

int Game::computeTimeIncrement(int destroyedBlocks)
{
	return 10 + ((destroyedBlocks - 2) / 3) * ((destroyedBlocks - 3) / 5) * 20;
}

int Game::getScore()
{
	return this->score;
}

std::vector<BlockType> Game::getBoardBlocks()
{
	return board.getBlocks();
}
