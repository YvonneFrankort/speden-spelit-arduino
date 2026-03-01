#ifndef LEDS_H
#define LEDS_H
#include <arduino.h>
#include "buttons.h"

//Initializing LEDs from 0 to 3
void initializeLeds();

//Setting desired LED on
void setLed(int ledNumber);

//Turning all LEDs off
void clearAllLeds(void);

//Turning all LEDs on
void setAllLeds(void);

//Amazing light show number 1, Looping through every binary number from 0 to 15
void show1(void);

//Fantastic light show number 2, looping through LEDs 0 to 3 back and forth with increasing speed for every round.
void show2(int rounds);

void show3();




#endif