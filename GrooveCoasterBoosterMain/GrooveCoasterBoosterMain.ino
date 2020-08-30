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
const double inputCheckDelay = 4;       //8 is for 125Hz, 4 is for 250Hz, 2 is for 500Hz, 1 for 1000Hz
double inputCheckLast = 0;              //Store when the last input check loop iterated
const double lightingUpdateDelay = 50;
double lightingUpdateLast = 0;
const double idleLightingDelay = 25;
double idleLightingLast = 0;
double minuteCounterLast = 0;

//Variables for button/switch logic
bool LUpLast = 0;             //Flags for left joystick
bool LDownLast = 0;
bool LLeftLast = 0;
bool LRightLast = 0;
bool RUpLast = 0;             //Flags for right joystick
bool RDownLast = 0;
bool RLeftLast = 0;
bool RRightLast = 0;
bool LBtnLast = 0;            //Flag for left button
bool RBtnLast = 0;            //Flag for right button

//Variables and definitions for FastLED library
#include <FastLED.h>
#define NUM_LEDS 256
#define MAX_BRIGHTNESS 120
#define MIN_BRIGHTNESS 50
#define DATA_PIN 12
CRGB internalLEDs[NUM_LEDS];

//Variables for lighting of the boosters
bool lightingChange = false;
const byte btnBrightnessMin = 55;       //Min light for push button built in LEDs
const byte btnBrightnessMax = 255;      //Max light for push button built in LEDs
const int allLEDs[] = {80,82,84,86,89,91,93,95,112,114,116,118,121,123,125,
                       127,175,173,171,169,166,164,162,160};
const int boostTops[] = {80,82,84,86,89,91,93,95};
const int boostWalls[] = {112,114,116,118,121,123,125,127,175,173,171,169,
                        166,164,162,160};
const int tiltLEDCount = 3;
const int allLEDcount = 24;
const int LBoostTiltRight[] = {80,112,175};
const int LBoostTiltDown[] = {82,114,173};
const int LBoostTiltLeft[] = {84,116,171};
const int LBoostTiltUp[] = {86,118,169};
byte globalHue = 120;
byte globalSat = 255;
byte globalVal = MAX_BRIGHTNESS;
byte tiltHue = 160;
bool LUprev = 0;
bool LRprev = 0;
bool solidColor = true;
bool boostersIdle = true;
bool boostersUsed = false;

void setup() 
{
  //Initialize all the pins needed for the hardware
  pinMode(LJoyUp, INPUT_PULLUP);          //Left joystick pin enables
  pinMode(LJoyRight, INPUT_PULLUP);
  pinMode(LJoyDown, INPUT_PULLUP);
  pinMode(LJoyLeft, INPUT_PULLUP);
  pinMode(RJoyUp, INPUT_PULLUP);          //Right joystick pin enables
  pinMode(RJoyDown, INPUT_PULLUP);
  pinMode(RJoyRight, INPUT_PULLUP);
  pinMode(RJoyLeft, INPUT_PULLUP);
  pinMode(btnLeft, INPUT_PULLUP);         //Left booster button pin enable
  pinMode(btnRight, INPUT_PULLUP);        //Right booster button pin enable
  pinMode(btnBack, INPUT_PULLUP);         //unused
  pinMode(btnEscape, INPUT_PULLUP);       //unused
  pinMode(btnLEDleft, OUTPUT);            //Left booster button LED enable
  pinMode(btnLEDright, OUTPUT);           //Right booster button LED enable

  //Enable the usage of the keyboard
  Keyboard.begin(); 

  analogWrite(btnLEDleft, btnBrightnessMin);      //Turns on the LED in the left booster button
  analogWrite(btnLEDright, btnBrightnessMin);     //Turns on the right LED

  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(internalLEDs, NUM_LEDS);  //Initialize LEDs
  FastLED.clear();        //Sets all LEDs to black
  FastLED.show();         //Pushes new LED data to LEDs

  IdleAnimation();
}

void loop() 
{
  if((millis() - inputCheckLast) >= inputCheckDelay)    //**CONTROLLER INPUT HANDLING
  {
    boostersUsed = false;               //If boosters not touched, stays false. Triggers idle.
    
    //LEFT JOYSTICK TILT CHECKS
    if(digitalRead(LJoyDown) == 0)      //Check the pin for LOW signal (switch closed)
    {
      Keyboard.press('s');              //Send an 's' key downpress to the computer (held down)
      LDownLast = 1;                    //Set flag to keep key press activated till switch opened
      boostersUsed = true;             //Mark the system as not in idle while being pressed
    }
    else if(LDownLast == 1)             //Check if pin is HIGH and switch was just opened (after closed)
    {
      Keyboard.release('s');            //Send message to PC that 's' key is no longer pressed
      LDownLast = 0;                    //Reset flag to prepare for next joystick tilt (switch closed)
    }
    
    if(digitalRead(LJoyUp) == 0)
    {
      Keyboard.press('w');
      LUpLast = 1;
      boostersUsed = true;
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
      boostersUsed = true;
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
      boostersUsed = true;
    }
    else if(LRightLast == 1)
    {
      Keyboard.release('d');
      LRightLast = 0;
    }

    //LEFT BOOSTER BUTTON CHECK
    if(digitalRead(btnLeft) == 0)
    {
      Keyboard.press('b');
      LBtnLast = 1;
      boostersUsed = true;
      analogWrite(btnLEDleft, btnBrightnessMax);  //Set LED inside left booster bright
    }
    else if(LBtnLast == 1)
    {
      Keyboard.release('b');
      LBtnLast = 0;
      analogWrite(btnLEDleft, btnBrightnessMin);  //Dim LED inside left booster
    }

    //RIGHT JOYSTICK TILT CHECKS
    if(digitalRead(RJoyDown) == 0)
    {
      Keyboard.press('k');
      RDownLast = 1;
      boostersUsed = true;
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
      boostersUsed = true;
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
      boostersUsed = true;
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
      boostersUsed = true;
    }
    else if(RRightLast == 1)
    {
      Keyboard.release('l');
      RRightLast = 0;
    }

    //RIGHT BOOSTER BUTTON CHECKS
    if(digitalRead(btnRight) == 0)
    {
      Keyboard.press('n');
      RBtnLast = 1;
      boostersUsed = true;
      analogWrite(btnLEDright, btnBrightnessMax);
    }
    else if(RBtnLast == 1)
    {
      Keyboard.release('n');
      RBtnLast = 0;
      analogWrite(btnLEDright, btnBrightnessMin);
    }

    inputCheckLast = millis();    //Update the last time the loop ran
  }

  if((boostersIdle == false) && ((millis() - lightingUpdateLast) >= lightingUpdateDelay))
  {
    //There are two LEDs per booster wall side (4 walls) and one LED
    // per cardinal direction on the moving booster itself
    // the LEDs will respond to joystick tilts and button taps.
    // Button taps can raise the brightness (and then will fade), while
    // tilts will illumate the wall side and booster side in the tilted
    // direction with a specific color

    //CHECK FOR TAPS
    if(LBtnLast == 1 || RBtnLast == 1)
    {
      globalVal = MAX_BRIGHTNESS;
      lightingChange = true;
    }

    //UPDATE LEDS WITH BACKGROUND THEME
    if(solidColor == true)
    {
      //Background layer of LED lighting is a solid color for both boosters
      if(globalVal > MIN_BRIGHTNESS)
      {
        //dim the lights by one value while brightness is above minimum
        globalVal-=15;
        for(int i=0;i<allLEDcount;i++)
        {
          internalLEDs[allLEDs[i]] = CHSV(globalHue, globalSat, globalVal);
        }
        lightingChange = true;
      }
    }
    
    //CHECK FOR TILTS
    if((LUpLast == 1) && (LUprev == 0))
    {
      LUprev = 1;
      lightingChange = true;
      for(int i=0;i<tiltLEDCount;i++)
      {
        internalLEDs[LBoostTiltUp[i]] = CHSV(tiltHue, globalSat, globalVal);
      }
    }
    else if((LUpLast == 0) && (LUprev == 1))
    {
      LUprev = 0;
      for(int i=0;i<tiltLEDCount;i++)
      {
        internalLEDs[LBoostTiltUp[i]] = CHSV(globalHue, globalSat, globalVal);
      }
      lightingChange = true;
    }

    if((LRightLast == 1) && (LRprev == 0))
    {
      LRprev = 1;
      lightingChange = true;
      for(int i=0;i<tiltLEDCount;i++)
      {
        internalLEDs[LBoostTiltRight[i]] = CHSV(tiltHue, globalSat, globalVal);
      }
    }
    else if((LRightLast == 0) && (LRprev == 1))
    {
      LRprev = 0;
      lightingChange = true;
      for(int i=0;i<tiltLEDCount;i++)
      {
        internalLEDs[LBoostTiltRight[i]] = CHSV(globalHue, globalSat, globalVal);
      }
    }

    if(lightingChange == true)
    {
      FastLED.show();
      lightingChange = false;
    }
    lightingUpdateLast = millis();
  }

  
  if(boostersUsed == false)                       //**BOOSTERS IDLE HANDLING/CHECK
  {
    if((millis() - minuteCounterLast) >= 5000)
    {
      boostersIdle = true;
    }
  }
  else
  {
    boostersIdle = false;
    minuteCounterLast = millis();
  }
  
  if(boostersIdle == true)
  {
    if((millis() - idleLightingLast) >= idleLightingDelay)
    {
      IdleAnimation();
      idleLightingLast = millis();
    }
  }
}

void IdleAnimation()
{
  //Function that runs when the boosters are not in use.
  //Checks for if any button presses happen for 1 minute: if no
  //buttons are pressed, the idle animation begins.

  //Current idle animation applies a rainbow effect to all LEDs, shifting through
  //hue range.
  for(int i=0;i<allLEDcount;i++)
  {
    internalLEDs[allLEDs[i]] = CHSV(globalHue, globalSat, MAX_BRIGHTNESS);
    globalHue+=3;
  }
  globalHue = globalHue - ((allLEDcount*3)+3);
  FastLED.show();
}
