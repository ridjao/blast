#include "Board.h"

static std::vector<BlockType> makeBlockTypeIdList(std::vector<std::vector<Block>>& grid);

Board::Board(int width, int height) : max(width, height) {}

void Board::setup()
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
}

void Board::clear()
{
	grid.clear();
}

std::vector<Position> Board::findBlocksToDestroy(const Position& start)
{
	return grid[start.yPos][start.xPos].findBlocksToDestroy(grid);
}

void Board::replaceBlocks(const std::vector<Position>& positions)
{
	for (size_t i = 0; i < positions.size(); i++)
	{
		int xPos = positions[i].xPos;
		int yPos = positions[i].yPos;
		grid[yPos][xPos] = Block(xPos, yPos);
	}
}

std::vector<BlockType> Board::getBlocks()
{
	return makeBlockTypeIdList(grid);
}

bool Board::hasLegalMoves()
{
	for (int y = 0; y < this->max.yPos; y++)
		for (int x = 0; x < this->max.xPos; x++)
			if (grid[y][x].findBlocksToDestroy(grid).size() > 1)
				return true;
	return false;
}

static std::vector<BlockType> makeBlockTypeIdList(std::vector<std::vector<Block>>& grid)
{
	std::vector<BlockType> blocks;
	for (size_t y = 0; y < grid.size(); y++)
		for (size_t x = 0; x < grid[y].size(); x++)
			blocks.push_back(grid[y][x].getBlockType());

	return blocks;
}
