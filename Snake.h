#ifndef Snake_h
#define Snake_h

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include "Blocks.h"

#define WIDTH           84
#define HEIGHT          48
#define BUTTON_COUNT     2
#define WEIGHT           2

class Snake{
private:
	void getDirection(byte &dx, byte &dy, byte d);
	Block* _blocks[20];
	void moveRight(Block* block);
	void moveUp(Block* block);
	void moveLeft(Block* block);
	void moveDown(Block* block);
public:
	static const byte BLOCK_COUNT_MAX = 20;
	Snake();
	byte _size;
	byte _headX;
	byte _headY;
	byte _speed;
	byte _direction;
	byte _weight;
	byte _selfTouch;
	void setWeight(byte weight);
	void addBlock(byte x, byte y);
	void addBlocks();
	void execute();
	void setDirection(byte direction);
	void turn(byte right = 3);
	void draw();
	void initialize(Adafruit_PCD8544 *display);
	Adafruit_PCD8544* _display;
};
#endif
