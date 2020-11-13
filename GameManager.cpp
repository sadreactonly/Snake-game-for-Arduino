#include "GameManager.h"
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

#include "Blocks.h"
#include "Food.h"
#include "Snake.h"
#include "pitches.h"
#include "SnakeGame.h"

bool gameOver = false;

GameManager::GameManager(){
	_state = 0;
	gameOver = false;
	score = 0;
  _sound = true;
}

void GameManager::initialize(Adafruit_PCD8544 *display, Snake* snake, Food* food){
	_display =display;
	_snake = snake;
	_food = food;
	snake->initialize(_display);
	food->initialize(_display);
}

void GameManager::moveSnake(byte direction)
{
	_snake->setDirection(direction);
}
void GameManager::execute()
{ 
	if(!_snake->_selfTouch)
	{
     checkForCollision();
		_snake->execute();
		_food->execute();
		
		draw();
	}
	else
	{
		gameOver=true;
	}

}
void GameManager::checkForCollision()
{
	//check collision with food
	if(_food->_x == _snake->_headX &&
			_food->_y == _snake->_headY)
	{
    if(_sound)
    {
      tone(SPEAKER, NOTE_A7, TONE_FOOD_DURATION);
    }

		score+=_food->points;
		_snake->addBlock(_food->_x, _food->_y);
		_food->randomize(WIDTH - WEIGHT, HEIGHT - WEIGHT, WEIGHT);
	}
}

void GameManager::setSound(int sound){
  _sound = sound;
  if(_sound)
  {
    tone(SPEAKER, NOTE_A7, TONE_FOOD_DURATION);
  }
}
void GameManager::draw(){
	//draw rectangle border
	_display->drawRect(0, 0, WIDTH, HEIGHT, BLACK);
	_display->display();
}
