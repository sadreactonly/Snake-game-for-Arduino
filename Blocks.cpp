#include "Blocks.h"

// Class Block represents part of the snake
Block::Block()
{
	_x = 0;
	_y = 0;
	_direction = 0;
}
Block::Block(int x, int y)
{
	_x = x;
	_y = y;
}
Block::Block(int x, int y, int block_direction)
{
	_x = x;
	_y = y;
	_direction = block_direction;
}
