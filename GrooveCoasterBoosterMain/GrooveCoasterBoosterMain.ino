/*
 * Groove Coaster custom booster for Steam version (PC) of Groove Coaster
 * 
 * --Code emulates the keyboard using a USB-enabled Arduino (ATMEGA32U4).
 * --This was specifically created/tested on an Arduino Leonardo
 * 
 * Coded by Gabriel C. (10gabrielc on Github) in the Arduino IDE
 * 
 */
//Needed libraries
#include <Keyboard.h>     //Library for emulating a keyboard


//Define pins needed for joysticks and buttons
const byte LJoyUp = 3;        //Left Joystick
const byte LJoyRight = 4;
const byte LJoyDown = 2;
const byte LJoyLeft = 5;
const byte RJoyUp = 7;        //Right Joystick
const byte RJoyRight = 8;
const byte RJoyDown = 6;
const byte RJoyLeft = 9;
const byte btnLeft = A5;      //Large push buttons
const byte btnRight = A4;
const byte btnBack = A3;
const byte btnEscape = A2;
const byte btnLEDleft = 10;   //Push button internal LEDs
const byte btnLEDright = 11;  

//All varaiables for millis timing
const double inputCheckDelay = 8;       //8 is for 125Hz, 4 is for 250Hz, 2 is for 500Hz, 1 for 1000Hz
double inputCheckLast = 0;

//Variables for button/switch logic
bool LUpLast = 0;
bool LDownLast = 0;
bool LLeftLast = 0;
bool LRightLast = 0;
bool RUpLast = 0;
bool RDownLast = 0;
bool RLeftLast = 0;
bool RRightLast = 0;
bool LBtnLast = 0;
bool RBtnLast = 0;

//Variables for lighting of the boosters
const byte btnBrightnessMin = 30;
const byte btnBrightnessMax = 255;


void setup() 
{
  //Initialize all the pins needed for the hardware
  pinMode(LJoyUp, INPUT_PULLUP);
  pinMode(LJoyRight, INPUT_PULLUP);
  pinMode(LJoyDown, INPUT_PULLUP);
  pinMode(LJoyLeft, INPUT_PULLUP);
  pinMode(RJoyUp, INPUT_PULLUP);
  pinMode(RJoyDown, INPUT_PULLUP);
  pinMode(RJoyRight, INPUT_PULLUP);
  pinMode(RJoyLeft, INPUT_PULLUP);
  pinMode(btnLeft, INPUT_PULLUP);
  pinMode(btnRight, INPUT_PULLUP);
  pinMode(btnBack, INPUT_PULLUP);
  pinMode(btnEscape, INPUT_PULLUP);
  pinMode(btnLEDleft, OUTPUT);
  pinMode(btnLEDright, OUTPUT);

  //Enable the usage of the keyboard
  Keyboard.begin(); 
  
}

void loop() 
{
  if((millis() - inputCheckLast) >= inputCheckDelay)
  {
    if(digitalRead(LJoyDown) == 0)
    {
      Keyboard.press('s');
      LDownLast = 1;
    }
    else if(LDownLast == 1)
    {
      Keyboard.release('s');
      LDownLast = 0;
    }
    
    if(digitalRead(LJoyUp) == 0)
    {
      Keyboard.press('w');
      LUpLast = 1;
    }
    else if(LUpLast == 1)
    {
      Keyboard.release('w');
      LUpLast = 0;
    }

    if(digitalRead(LJoyLeft) == 0)
    {
      Keyboard.press('a');
      LLeftLast = 1;
    }
    else if(LLeftLast == 1)
    {
      Keyboard.release('a');
      LLeftLast = 0;
    }

    if(digitalRead(LJoyRight) == 0)
    {
      Keyboard.press('d');
      LRightLast = 1;
    }
    else if(LRightLast == 1)
    {
      Keyboard.release('d');
      LRightLast = 0;
    }

    if(digitalRead(btnLeft) == 0)
    {
      Keyboard.press('b');
      LBtnLast = 1;
      analogWrite(btnLEDleft, btnBrightnessMax);
    }
    else if(LBtnLast == 1)
    {
      Keyboard.release('b');
      LBtnLast = 0;
      analogWrite(btnLEDleft, btnBrightnessMin);
    }

    //Handling inputs for right booster
    if(digitalRead(RJoyDown) == 0)
    {
      Keyboard.press('k');
      RDownLast = 1;
    }
    else if(RDownLast == 1)
    {
      Keyboard.release('k');
      RDownLast = 0;
    }

    if(digitalRead(RJoyUp) == 0)
    {
      Keyboard.press('i');
      RUpLast = 1;
    }
    else if(RUpLast == 1)
    {
      Keyboard.release('i');
      RUpLast = 0;
    }

    if(digitalRead(RJoyLeft) == 0)
    {
      Keyboard.press('j');
      RLeftLast = 1;
    }
    else if(RLeftLast == 1)
    {
      Keyboard.release('j');
      RLeftLast = 0;
    }

    if(digitalRead(RJoyRight) == 0)
    {
      Keyboard.press('l');
      RRightLast = 1;
    }
    else if(RRightLast == 1)
    {
      Keyboard.release('l');
      RRightLast = 0;
    }

    if(digitalRead(btnRight) == 0)
    {
      Keyboard.press('n');
      RBtnLast = 1;
      analogWrite(btnLEDright, btnBrightnessMax);
    }
    else if(RBtnLast == 1)
    {
      Keyboard.release('n');
      RBtnLast = 0;
      analogWrite(btnLEDright, btnBrightnessMin);
    }

    inputCheckLast = millis();
  }
}
