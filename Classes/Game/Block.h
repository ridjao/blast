#pragma once
#include <vector>
#include <memory>
#include "Position.h"
#include "BlockTypeIds.h"

class Block
{
public:
	Block(int xPos, int yPos);
	~Block();
	Block(Block&&);
	Block& operator=(Block&&);

	std::vector<Position> findBlocksToDestroy(const std::vector<std::vector<Block>>& grid);
	BlockType getBlockType();

private:
	friend class BlockImpl;
	std::unique_ptr<BlockImpl> pimpl;
};
