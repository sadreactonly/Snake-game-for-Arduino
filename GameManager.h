#ifndef GameManager_h
#define GameManager_h

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

#include "Blocks.h"
#include "Food.h"
#include "Snake.h"


class GameManager{
public:
	GameManager();
	int _delay;
	void initialize(Adafruit_PCD8544 *display, Snake* snake, Food* food);
	void execute();
	void draw();
	void moveSnake(byte direction);
	int score;
	bool gameOver;
  void setSound(int state);
private:
	byte _state;
  bool _sound;
	Snake* _snake;
	Food* _food;
	Adafruit_PCD8544* _display;
	bool _buttonsPress[BUTTON_COUNT];
	bool wasPressed(byte pin, byte index);
	void checkForCollision();
};
#endif
