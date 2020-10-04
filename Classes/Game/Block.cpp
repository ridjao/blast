#include "Block.h"
#include "BlockTypeIds.h"
#include <random>

#if ! defined(_WINDOWS)
namespace std {
    template<class T> struct _Unique_if {
        typedef unique_ptr<T> _Single_object;
    };

    template<class T> struct _Unique_if<T[]> {
        typedef unique_ptr<T[]> _Unknown_bound;
    };

    template<class T, size_t N> struct _Unique_if<T[N]> {
        typedef void _Known_bound;
    };

    template<class T, class... Args>
        typename _Unique_if<T>::_Single_object
        make_unique(Args&&... args) {
            return unique_ptr<T>(new T(std::forward<Args>(args)...));
        }

    template<class T>
        typename _Unique_if<T>::_Unknown_bound
        make_unique(size_t n) {
            typedef typename remove_extent<T>::type U;
            return unique_ptr<T>(new U[n]());
        }

    template<class T, class... Args>
        typename _Unique_if<T>::_Known_bound
        make_unique(Args&&...) = delete;
}
#endif

class BlockImpl
{
public:
	BlockImpl(const BlockType& blockType, const Position& position) : blockType(blockType), position(position) {}
	virtual std::vector<Position> findBlocksToDestroy(const std::vector<std::vector<Block>>& grid, int xPos, int yPos, int color)
	{
		std::vector<std::vector<BlockType>> colorGrid;
		for (size_t y = 0; y < grid.size(); y++)
		{
			std::vector<BlockType> row;
			for (size_t x = 0; x < grid[y].size(); x++)
				row.push_back(grid[y][x].pimpl->blockType);
			colorGrid.push_back(row);
		}
		colorGrid[yPos][xPos] = BlockType::MARKED_DESTROY;
		return findBlocksToDestroy(grid, colorGrid, xPos, yPos, color);
	}

	std::vector<Position> findBlocksToDestroy(const std::vector<std::vector<Block>>& grid, std::vector<std::vector<BlockType>>& colorGrid, int xPos, int yPos, int color)
	{
		std::vector<Position> blocksToDestroy;

		for (int x = xPos - 1; x >= 0; x--)
		{
			BlockType& blockColor = colorGrid[yPos][x];
			if (blockColor != color)
				break;
			{
				blockColor = BlockType::MARKED_DESTROY;
				blocksToDestroy.push_back(Position(x, yPos));
			}
		}

		for (size_t x = xPos + 1; x < grid[yPos].size(); x++)
		{
			BlockType& blockColor = colorGrid[yPos][x];
			if (blockColor != color)
				break;
			{
				blockColor = BlockType::MARKED_DESTROY;
				blocksToDestroy.push_back(Position(x, yPos));
			}
		}

		for (int y = yPos - 1; y >= 0; y--)
		{
			BlockType& blockColor = colorGrid[y][xPos];
			if (blockColor != color)
				break;
			{
				blockColor = BlockType::MARKED_DESTROY;
				blocksToDestroy.push_back(Position(xPos, y));
			}
		}

		for (size_t y = yPos + 1; y < grid.size(); y++)
		{
			BlockType& blockColor = colorGrid[y][xPos];
			if (blockColor != color)
				break;
			{
				blockColor = BlockType::MARKED_DESTROY;
				blocksToDestroy.push_back(Position(xPos, y));
			}
		}

		std::vector<Position> blocksToCheck = blocksToDestroy;
		for (const auto& pos : blocksToCheck)
		{
			std::vector<Position> more = grid[pos.yPos][pos.xPos].pimpl->findBlocksToDestroy(grid, colorGrid, pos.xPos, pos.yPos, color);
			blocksToDestroy.insert(blocksToDestroy.end(), more.begin(), more.end());
		}

		return blocksToDestroy;
	}

private:
	BlockType blockType;
	Position position;
	friend class Block;
};

class StripeBlockImpl : public BlockImpl
{
public:
	StripeBlockImpl(const Position& position) : BlockImpl(BlockType::STRIPE, position) {}
	virtual std::vector<Position> findBlocksToDestroy(const std::vector<std::vector<Block>>& grid,
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

class BombBlockImpl : public BlockImpl
{
public:
	BombBlockImpl(const Position& position) : BlockImpl(BlockType::BOMB, position) {}
	virtual std::vector<Position> findBlocksToDestroy(const std::vector<std::vector<Block>>& grid,
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

Block::Block(int xPos, int yPos)
{
	if (rollDice(engine) < 5)
	{
		if (getRandomSpecial(engine) == 0)
		{
			this->pimpl = std::make_unique<StripeBlockImpl>(Position(xPos, yPos));
		}
		else
		{
			this->pimpl = std::make_unique<BombBlockImpl>(Position(xPos, yPos));
		}
	}
	else
	{
		BlockType blockType = static_cast<BlockType>(getRandomColor(engine));
		this->pimpl = std::make_unique<BlockImpl>(blockType, Position(xPos, yPos));
	}
}

Block::~Block() = default;
Block::Block(Block&& block) = default;
Block& Block::operator=(Block&&) = default;

std::vector<Position> Block::findBlocksToDestroy(const std::vector<std::vector<Block>>& grid)
{
	std::vector<Position> blocksToDestroy = this->pimpl->findBlocksToDestroy(grid, this->pimpl->position.xPos, this->pimpl->position.yPos, this->pimpl->blockType);
	if (blocksToDestroy.size() > 0)
		blocksToDestroy.push_back(this->pimpl->position);
	return blocksToDestroy;
}

BlockType Block::getBlockType()
{
	return pimpl->blockType;
}

