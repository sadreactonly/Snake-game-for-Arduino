#include <EEPROM.h>
#include "GameManager.h"
(CLK,DIN,D/C,CE,RST);
Adafruit_PCD8544 display = Adafruit_PCD8544(13, 11, 9, 10, 8);

// Game objects
Snake snake = Snake();
Food food = Food();
GameManager gameManager = GameManager();

// Buttons
volatile boolean up = false;
volatile boolean down = false;
volatile boolean right = false;
volatile boolean left = false;

int downButtonState = 0;
int upButtonState = 0;  
int rightButtonState = 0;    
int leftButtonState = 0;

int lastLeftButtonState = 0;
int lastDownButtonState = 0;
int lastRightButtonState = 0;
int lastUpButtonState = 0;

// LCD
boolean backlight = true;
int contrast=LCD_INIT_CONTRAST;
int menuitem = 1;
int page = MENU_SCREEN;

int sound = true;

int highscore=0;

void setup() {
  Serial.begin(9600);

  pinMode(BUTTON_UP, INPUT_PULLUP);
  pinMode(BUTTON_DOWN, INPUT_PULLUP);
  pinMode(BUTTON_RIGHT, INPUT_PULLUP);
  pinMode(BUTTON_LEFT, INPUT_PULLUP);

  pinMode(LCD_BACKLIGHT_PIN,OUTPUT);

  highscore = readIntFromEEPROM(HIGHSCORE_ADDRESS);
  randomSeed(analogRead(0));
  display.begin();
  display.setContrast(LCD_INIT_CONTRAST);
  display.clearDisplay();
  display.display(); 

  gameManager.initialize(&display,&snake,&food);  
}

void loop() {
  display.clearDisplay();
  drawMenu();

  downButtonState = digitalRead(BUTTON_DOWN);
  rightButtonState = digitalRead(BUTTON_RIGHT);
  upButtonState =   digitalRead(BUTTON_UP);
  leftButtonState = digitalRead(BUTTON_LEFT);

  checkIfDownButtonIsPressed();
  checkIfUpButtonIsPressed();
  checkIfSelectButtonIsPressed();
  checkIfBackButtonIsPressed();

  handleBackButton();
  handlePages();
}

void gameHandler(){
  if(!gameManager.gameOver){
    if(!upButtonState){
      gameManager.moveSnake(1);
    }
    else if(!rightButtonState){
      gameManager.moveSnake(3);
    }
    else if(!leftButtonState){
      gameManager.moveSnake(4);
    }
    else if(!downButtonState){
      gameManager.moveSnake(2);
    } 
    gameManager.execute();
  }
  else
  {
    gameOverScreen();
    
    if(!rightButtonState)
    {
      resetGame();
      page = 1;
    } 
  }
  delay(150);
}

void resetGame(){
  snake = Snake();
  food = Food();
  gameManager = GameManager();
  gameManager.initialize(&display,&snake,&food);
}

void gameOverScreen(){
  display.setTextSize(1);
  display.setCursor(17,0);
  display.println("GAME OVER!");
  display.setTextSize(1);
  display.setCursor(10, 24);
  highscore = readIntFromEEPROM(HIGHSCORE_ADDRESS);


  if(gameManager.score >= highscore)
  {
    display.print("HIGHSCORE! ");
    writeIntIntoEEPROM(HIGHSCORE_ADDRESS,gameManager.score);
  }
  else
  {
    display.print("Score: ");
  }
  display.println(gameManager.score);
  display.display();
}

void handleBackButton(){
  if(page != GAME_SCREEN){
    if(left && page == HIGHSCORE_SCREEN)
    {
      left = false;
      page=1;
    }
    else if(left && page > MENU_SCREEN){
      left = false;
      page--;
    }
  }

}

void handlePages(){
  if(page == MENU_SCREEN){
    handleFirstPage();
  }
  else{
    handleSecondPage();
  }
}

void handleFirstPage(){
  if (up && page == MENU_SCREEN ) {
    up = false;
    menuitem--;
    if (menuitem == 0)
    {
      menuitem=3;
    }      
  }else if (up && page == SETTINGS_SCREEN ) {
    up = false;
  }
 
  if (down && page == MENU_SCREEN) {
    down = false;
    menuitem++;
    if (menuitem == 4) 
    {
      menuitem = 1;
    }      
  }else if (down && page == SETTINGS_SCREEN ) {
    down = false;
  }

  if (right && page == MENU_SCREEN) {
    right = false;

    if (page == MENU_SCREEN && menuitem == 1) {
      page = GAME_SCREEN;
    }
    else if(page == MENU_SCREEN && menuitem == 2)
    {
      page = HIGHSCORE_SCREEN;
    }
    else if(page == MENU_SCREEN && menuitem == 3)
    {
      menuitem = 1;
      page = SETTINGS_SCREEN;
    }
  }
}

void handleSecondPage(){
  if (up && page == SETTINGS_SCREEN ) {
    up = false;
    menuitem--;
    if (menuitem == 0)
    {
      menuitem = 3;
    }      
  }else if (up && page == SETTINGS_SCREEN ) {
    up = false;
    contrast--;
    setContrast();
  }

  if (down && page == SETTINGS_SCREEN) {
    down = false;
    menuitem++;
    if (menuitem == 4) 
    {
      menuitem=1;
    }      
  }
  else if (down && page == CONTRAST_SCREEN ) {
    down = false;
    contrast++;
    setContrast();
  }

  if (right) {
    right = false;
    if (page == SETTINGS_SCREEN && menuitem==2) 
    {
      if (backlight) 
      {
        backlight = false;
        setBacklight(LCD_BACKLIGHT_OFF);
      }
      else 
      {
        backlight = true; 
        setBacklight(LCD_BACKLIGHT_ON);
      }
    }

    if(page == SETTINGS_SCREEN && menuitem ==3)
    {
      if (sound) 
      {
        sound = false;
        gameManager.setSound(LOW);
      }
      else 
      {
        sound = true; 
        gameManager.setSound(HIGH);
      }
    }
    else if (page == SETTINGS_SCREEN && menuitem==1) {
      page = CONTRAST_SCREEN;
    }
    else if (page == CONTRAST_SCREEN) {
      page=SETTINGS_SCREEN;
    }
  }
}

void drawMenu(){
  if (page == MENU_SCREEN) 
  {    
    display.setTextSize(1);
    display.clearDisplay();
    display.setTextColor(BLACK, WHITE);
    display.setCursor(15, 0);
    display.print("Snake Game");
    display.drawFastHLine(0,10,83,BLACK);
    display.setCursor(0, 15);
    
    if (menuitem == 1) 
    { 
      display.setTextColor(WHITE, BLACK);
    }
    else 
    {
      display.setTextColor(BLACK, WHITE);
    }
    display.print(">Play");
    display.setCursor(0, 25);
    
    if (menuitem==2) 
    {
      display.setTextColor(WHITE, BLACK);
    }
    else 
    {
      display.setTextColor(BLACK, WHITE);
    }    
    display.print(">Highscore");
    
    if (menuitem==3) 
    { 
      display.setTextColor(WHITE, BLACK);
    }
    else 
    {
      display.setTextColor(BLACK, WHITE);
    }  
    display.setCursor(0, 35);
    display.print(">Settings");
    display.display();
  }
  
  else if (page == SETTINGS_SCREEN) 
  {    
    
    display.setTextSize(1);
    display.clearDisplay();
    display.setTextColor(BLACK, WHITE);
    display.setCursor(15, 0);
    display.print("Settings");
    display.drawFastHLine(0,10,83,BLACK);
    display.setCursor(0, 15);
    
    if (menuitem==1) 
    { 
      display.setTextColor(WHITE, BLACK);
    }
    else 
    {
      display.setTextColor(BLACK, WHITE);
    }
    display.print(">Contrast");
    display.setCursor(0, 25);
    
    if (menuitem==2) 
    {
      display.setTextColor(WHITE, BLACK);
    }
    else 
    {
      display.setTextColor(BLACK, WHITE);
    }    
    display.print(">Light: ");
    
    if (backlight) 
    {
      display.print("ON");
    }
    else 
    {
      display.print("OFF");
    }
    
    if (menuitem==3) 
    { 
      display.setTextColor(WHITE, BLACK);
    }
    else 
    {
      display.setTextColor(BLACK, WHITE);
    }  

    display.setCursor(0, 35);
    display.print(">Sound: ");
    if (sound) 
    {
      display.print("ON");
    }
    else 
    {
      display.print("OFF");
    }
    
    display.display();
  }
  else if (page == CONTRAST_SCREEN) 
  {
    
    display.setTextSize(1);
    display.clearDisplay();
    display.setTextColor(BLACK, WHITE);
    display.setCursor(15, 0);
    display.print("CONTRAST");
    display.drawFastHLine(0,10,83,BLACK);
    display.setCursor(5, 15);
    display.print("Value");
    display.setTextSize(2);
    display.setCursor(5, 25);
    display.print(contrast);

    display.setTextSize(2);
    display.display();
  }
  else if (page == HIGHSCORE_SCREEN) 
  {
    
    display.setTextSize(1);
    display.clearDisplay();
    display.setTextColor(BLACK, WHITE);
    display.setCursor(15, 0);
    display.print("HIGHSCORE:");
    display.drawFastHLine(0,10,83,BLACK);
    display.setCursor(5, 15);
    display.print("Value");
    display.setTextSize(2);
    display.setCursor(5, 25);
    
    int number = readIntFromEEPROM(HIGHSCORE_ADDRESS);
   
    display.print(number);
    display.setTextSize(2);
    display.display();
  }
  else if (page == GAME_SCREEN) 
  {
    gameHandler();
  }
}

void resetDefaults(){
  contrast = LCD_INIT_CONTRAST;
  setContrast();
  backlight = true;
  setBacklight(HIGH);
}

void setContrast(){
  display.setContrast(contrast);
  display.display();
}

void setBacklight(int state){
  digitalWrite(LCD_BACKLIGHT_PIN,state);
}

// R/W EEPROM
void writeIntIntoEEPROM(int address, int number){ 
  EEPROM.write(address, number >> 8);
  EEPROM.write(address + 1, number & 0xFF);
}

int readIntFromEEPROM(int address){
  byte byte1 = EEPROM.read(address);
  byte byte2 = EEPROM.read(address + 1);
  return (byte1 << 8) + byte2;
}

//BUTTONS FUNCTIONS
void checkIfDownButtonIsPressed(){
  if (downButtonState != lastDownButtonState) 
  {
    if (downButtonState == 0) 
    {
      down=true;
    }
    delay(50);
  }
  lastDownButtonState = downButtonState;
}

void checkIfBackButtonIsPressed(){
  if (leftButtonState != lastLeftButtonState) 
  {
    if (leftButtonState == 0) 
    {
      left=true;
    }
    delay(50);
  }
  lastLeftButtonState = leftButtonState;
}

void checkIfUpButtonIsPressed(){
  if (upButtonState != lastUpButtonState) 
  {
    if (upButtonState == 0) {
      up=true;
    }
    delay(50);
  }
  lastUpButtonState = upButtonState;
}

void checkIfSelectButtonIsPressed(){
  if (rightButtonState != lastRightButtonState) 
  {
    if (rightButtonState == 0) {
      right=true;
    }
    delay(50);
  }
  lastRightButtonState = rightButtonState;
}
