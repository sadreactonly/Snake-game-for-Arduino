#include "Food.h"
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

// Class Food represents snake's food
Food::Food(){
	_x = 0;
	_y = 0;
	_index = 0;
	points = 10;
}
void Food::initialize(Adafruit_PCD8544 *display)
{
	_display= display;
	_x = 36;
	_y = 4;
}
void Food::execute()
{
	draw();
}
void Food::randomize(byte width, byte height, byte weight){
	_x = random(weight,width - weight);
	if(_x % weight != 0)
	    _x+=1;
	_y = random(2,height -weight);
	if(_y % weight != 0)
	    _y+=1;
}
void Food::draw(){
	_display->fillRect(_x,_y, WEIGHT, WEIGHT, BLACK);
}
