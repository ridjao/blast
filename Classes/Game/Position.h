#pragma once
struct Position
{
	int xPos;
	int yPos;
	Position(int xPos, int yPos) : xPos(xPos), yPos(yPos) {}
};

typedef Position Move;
