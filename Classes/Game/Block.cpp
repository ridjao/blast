#include "Block.h"
#include "BlockTypeIds.h"
#include <random>

class Block::BlockImpl
{
public:
	virtual std::vector<Position> BlockImpl::findBlocksToDestroy(std::vector<std::vector<Block>>& grid, int xPos, int yPos, int color)
	{
		std::vector<std::vector<int>> colorGrid;
		for (size_t y = 0; y < grid.size(); y++)
		{
			std::vector<int> row;
			for (size_t x = 0; x < grid[y].size(); x++)
				row.push_back(grid[y][x].blockTypeId);
			colorGrid.push_back(row);
		}
		colorGrid[yPos][xPos] = BlockTypes::MARKED;
		return findBlocksToDestroy(grid, colorGrid, xPos, yPos, color);
	}

	std::vector<Position> BlockImpl::findBlocksToDestroy(std::vector<std::vector<Block>>& grid, std::vector<std::vector<int>>& colorGrid, int xPos, int yPos, int color)
	{
		std::vector<Position> blocksToDestroy;

		for (int x = xPos - 1; x >= 0; x--)
		{
			int& blockColor = colorGrid[yPos][x];
			if (blockColor != color)
				break;
			{
				blockColor = BlockTypes::MARKED;
				blocksToDestroy.push_back(Position(x, yPos));
			}
		}

		for (size_t x = xPos + 1; x < grid[yPos].size(); x++)
		{
			int& blockColor = colorGrid[yPos][x];
			if (blockColor != color)
				break;
			{
				blockColor = BlockTypes::MARKED;
				blocksToDestroy.push_back(Position(x, yPos));
			}
		}

		for (int y = yPos - 1; y >= 0; y--)
		{
			int& blockColor = colorGrid[y][xPos];
			if (blockColor != color)
				break;
			{
				blockColor = BlockTypes::MARKED;
				blocksToDestroy.push_back(Position(xPos, y));
			}
		}

		for (size_t y = yPos + 1; y < grid.size(); y++)
		{
			int& blockColor = colorGrid[y][xPos];
			if (blockColor != color)
				break;
			{
				blockColor = BlockTypes::MARKED;
				blocksToDestroy.push_back(Position(xPos, y));
			}
		}

		std::vector<Position> blocksToCheck = blocksToDestroy;
		for (Position pos : blocksToCheck)
		{
			std::vector<Position> more = grid[pos.yPos][pos.xPos].pimpl->findBlocksToDestroy(grid, colorGrid, pos.xPos, pos.yPos, color);
			blocksToDestroy.insert(blocksToDestroy.end(), more.begin(), more.end());
		}

		return blocksToDestroy;
	}
};

class Block::StripeBlockImpl : public Block::BlockImpl
{
public:
	virtual std::vector<Position> StripeBlockImpl::findBlocksToDestroy(std::vector<std::vector<Block>>& grid,
		int xPos, int yPos, int color)
	{
		std::vector<Position> blocksToDestroy;
		for (size_t x = 0; x < grid[yPos].size(); x++)
		{
			if (x == xPos)
				continue;
			blocksToDestroy.push_back(Position(x, yPos));
		}
		return blocksToDestroy;
	}
};

class Block::BombBlockImpl : public Block::BlockImpl
{
public:
	virtual std::vector<Position> BombBlockImpl::findBlocksToDestroy(std::vector<std::vector<Block>>& grid,
		int xPos, int yPos, int color)
	{
		std::vector<Position> blocksToDestroy;
		if (xPos - 1 >= 0) {
			blocksToDestroy.push_back(Position(xPos - 1, yPos));
			if (yPos - 1 >= 0)
				blocksToDestroy.push_back(Position(xPos - 1, yPos - 1));
			if ((size_t)yPos + 1 < grid.size())
				blocksToDestroy.push_back(Position(xPos - 1, yPos + 1));
		}
		if ((size_t)xPos + 1 < grid[yPos].size()) {
			blocksToDestroy.push_back(Position(xPos + 1, yPos));
			if (yPos - 1 >= 0)
				blocksToDestroy.push_back(Position(xPos + 1, yPos - 1));
			if ((size_t)yPos + 1 < grid.size())
				blocksToDestroy.push_back(Position(xPos + 1, yPos + 1));
		}
		if (yPos - 1 >= 0)
			blocksToDestroy.push_back(Position(xPos, yPos - 1));
		if ((size_t)yPos + 1 < grid.size())
			blocksToDestroy.push_back(Position(xPos, yPos + 1));
		return blocksToDestroy;
	}
};

static std::random_device device;
static std::mt19937 engine(device());
static std::uniform_int_distribution<int> rollDice(0, 99);
static std::uniform_int_distribution<int> getRandomSpecial(0, 1);
static std::uniform_int_distribution<int> getRandomColor(0, 5);

Block::Block(int xPos, int yPos) : position(xPos, yPos)
{
	if (rollDice(engine) < 5)
	{
		if (getRandomSpecial(engine) == 0)
		{
			this->blockTypeId = BlockTypes::STRIPE;
			this->pimpl = std::make_unique<Block::StripeBlockImpl>();
		}
		else
		{
			this->blockTypeId = BlockTypes::BOMB;
			this->pimpl = std::make_unique<Block::BombBlockImpl>();
		}
	}
	else
	{
		this->blockTypeId = getRandomColor(engine);
		this->pimpl = std::make_unique<Block::BlockImpl>();
	}
}

Block::~Block() = default;
Block::Block(Block&& block) = default;
Block& Block::operator=(Block&&) = default;

std::vector<Position> Block::findBlocksToDestroy(std::vector<std::vector<Block>>& grid)
{
	std::vector<Position> blocksToDestroy = this->pimpl->findBlocksToDestroy(grid, this->position.xPos, this->position.yPos, this->blockTypeId);
	if (blocksToDestroy.size() > 0)
		blocksToDestroy.push_back(this->position);
	return blocksToDestroy;
}
