#include "Snake.h"
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include "Blocks.h"
#include "SnakeGame.h"

Snake::Snake(){
	_size = 1;
	_headX = 0;
	_headY = 0;
	_speed = 1;
	_direction = RIGHT;
	_weight = WEIGHT;
	_selfTouch = false;
}

void Snake::initialize(Adafruit_PCD8544 *display)
{
	_display= display;
	addBlocks();
}


void Snake::setWeight(byte weight){
	_weight = weight;
}

void Snake::addBlock(byte x, byte y){
	if(_size<BLOCK_COUNT_MAX)
	{
		_size++;
		Block* b = new Block(x,y);
		_headX = x;
		_headY = y;

		_blocks[_size-1] = b;  
	}
}

void Snake::addBlocks(){

	byte block_start_x = 10;
	byte block_start_y = 24;

	for(int in = 0; in<_size; in++)
	{
		if(in==0)
		{
			_blocks[in] = new Block(block_start_x, block_start_y, _direction); 
			
		}
		else{
			_blocks[in] = new Block(_blocks[in-1]->_x+2, block_start_y, _direction);  
		}
	}
}

void Snake::execute(){
	Block* tail = _blocks[0];

	//SWITCH CASE HAVE BUG!!!!
	if(_direction == UP)
	{
		moveUp(tail);
	}
	else if(_direction == DOWN)
	{
		moveDown(tail);
	}
	else if(_direction == RIGHT)
	{
		moveRight(tail);
	}
	else if(_direction == LEFT)
	{
		moveLeft(tail);
	}

	for(int j = 0;j<_size;j++)
	{
		
		_blocks[j]=_blocks[j+1];
		
	}  

	_blocks[_size-1] = tail;
	_headX = tail->_x;
	_headY = tail->_y;

  // Self touch checking
	for(int j = 0;j<_size-1;j++)
	{
		if(_blocks[j]->_x == _headX &&
				_blocks[j]->_y == _headY)
		{
			_selfTouch=true;
		}
	}  

	draw();
}

void Snake::moveRight(Block* tail){
	if(_blocks[_size-1]->_x + WEIGHT != WIDTH)
	{
		tail->_x=_blocks[_size-1]->_x + WEIGHT;
		tail->_y=_blocks[_size-1]->_y;
	}
	else
	{
		tail->_x=0;
		tail->_y=_blocks[_size-1]->_y;
	}
}

void Snake::moveUp(Block* tail){
	if(_blocks[_size-1]->_y != 0)
	{
		tail->_y=_blocks[_size-1]->_y - WEIGHT;
		tail->_x = _blocks[_size-1]->_x;
	}
	else
	{
		tail->_y = HEIGHT - WEIGHT;
		tail->_x = _blocks[_size-1]->_x;
	}
}

void Snake::moveDown(Block* tail){
	if(_blocks[_size-1]->_y + WEIGHT != HEIGHT)
	{
		tail->_y = _blocks[_size-1]->_y + WEIGHT;
		tail->_x = _blocks[_size-1]->_x;
	}
	else
	{
		tail->_y = 0;
		tail->_x = _blocks[_size-1]->_x;
	}
}

void Snake::moveLeft(Block* tail){
	if(_blocks[_size-1]->_x != 0)
	{
		tail->_x=_blocks[_size-1]->_x - WEIGHT;
		tail->_y=_blocks[_size-1]->_y;
	}
	else
	{
		tail->_x = WIDTH - WEIGHT;
		tail->_y = _blocks[_size-1]->_y;

	}  
}

void Snake::setDirection(byte direction){

	if(_direction == 3 && direction == 4 ||
			_direction == 4 && direction == 3 ||
			_direction == 1 && direction == 2 ||
			_direction == 2 && direction == 1 ||
			_direction == direction)
	return;
	
	_direction = direction;

}

void Snake::draw(){
	for (byte i = 0; i < _size; i ++) {
		_display->fillRect(_blocks[i]->_x, _blocks[i]->_y, _weight, _weight, BLACK);
	}
}
