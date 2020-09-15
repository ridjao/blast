#pragma once
#include<vector>
#include<memory>
#include"Position.h"

class Block
{
public:
	Block(int xPos, int yPOs);
	~Block();
	Block(Block&&);
	Block& operator=(Block&&);

	std::vector<Position> findBlocksToDestroy(const std::vector<std::vector<Block>>& grid);
	int blockTypeId;
	Position position;
private:
	class BlockImpl;
	class StripeBlockImpl;
	class BombBlockImpl;
	std::unique_ptr<BlockImpl> pimpl;
};

