#include "Game.h"

Game::Game() : board(Game::WIDTH, Game::HEIGHT), score(0) {}

bool Game::hasLegalMoves()
{
	return board.hasLegalMoves();
}

SceneDTO Game::makeMove(const Move& move)
{
	SceneDTO dto;
	dto.blocksToDestroy = board.findBlocksToDestroy(move);
	if (dto.blocksToDestroy.size() > 1)
	{
		dto.boardBlocks = board.replaceBlocks(dto.blocksToDestroy);
		this->updateScore(dto.blocksToDestroy.size());
		dto.score = this->score;
		dto.timeIncrement = this->computeTimeIncrement(dto.blocksToDestroy.size());
	}
	return dto;
}

SceneDTO Game::start()
{
	this->score = 0;
	SceneDTO dto;
	dto.boardBlocks = board.setup();
	return dto;
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
