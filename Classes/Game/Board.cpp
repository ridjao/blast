#include "Board.h"

static std::vector<int> makeBlockList(std::vector<std::vector<Block>>& grid);

Board::Board(int width, int height) : max(width, height) {}

std::vector<int> Board::setup()
{
	for (int y = 0; y < this->max.yPos; y++)
	{
		std::vector<Block> row;
		for (int x = 0; x < this->max.xPos; x++)
		{			
			row.push_back(Block(x,y));
		}
		grid.push_back(std::move(row));
	}
	return makeBlockList(grid);
}

std::vector<Position> Board::findBlocksToDestroy(const Position& start)
{
	return grid[start.yPos][start.xPos].findBlocksToDestroy(grid);
}

std::vector<int> Board::replaceBlocks(const std::vector<Position>& positions)
{
	for (size_t i = 0; i < positions.size(); i++)
	{
		int xPos = positions[i].xPos;
		int yPos = positions[i].yPos;
		grid[yPos][xPos] = Block(xPos, yPos);
	}
	return makeBlockList(grid);
}

bool Board::hasLegalMoves()
{
	for (int y = 0; y < this->max.yPos; y++)
		for (int x = 0; x < this->max.xPos; x++)
			if (grid[y][x].findBlocksToDestroy(grid).size() > 1)
				return true;
	return false;
}

static std::vector<int> makeBlockList(std::vector<std::vector<Block>>& grid)
{
	std::vector<int> blocks;
	for (size_t y = 0; y < grid.size(); y++)
		for (size_t x = 0; x < grid[y].size(); x++)
			blocks.push_back(grid[y][x].blockTypeId);

	return blocks;
}