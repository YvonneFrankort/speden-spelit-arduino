#include "leds.h"
#include "SpedenSpelit.h"

int pins[] = {A2, A3, A4, A5};            //Initialize Pin array

unsigned long lastMillis = 0;       

void initializeLeds()
{
  for (int i = 0; i<4; i++){       //Initialize Pins from A2 to A5
    pinMode(pins[i], OUTPUT);
  }
}

void setLed(int ledNumber)
{

/*Takes user input and turns on corresponding LED.
 Possible function input parameters are 0, 1, 2 and 3*/


  switch (ledNumber) { 
    case 0:
      digitalWrite(A2, HIGH);

      
    break;

    case 1:
      digitalWrite(A3, HIGH);

      
    break;

    case 2:
      digitalWrite(A4, HIGH);

     
    break;

    case 3:
      digitalWrite(A5, HIGH);

    
    break;

    default:

    break;
  }
}


void clearAllLeds()
{

//Using For-loop to go through every pin and setting them all LOW

  for (int j = 0; j < 4; j++){
    digitalWrite(pins[j], LOW);
 
  }
}

void setAllLeds()
{

//Using For-loop to go through every pin and setting them all HIGH

  for (int k = 0; k < 4; k++){
    digitalWrite(pins[k], HIGH);
  }
}


void show1()
{

int delayConstantForShowOne = 400; //Delay constant
//Remove /* -- */ to use this mongo method if Loop does not work.
//Remember to add /* -- */ for loop.

/*

//Mongo Method:



digitalWrite(pins[0], LOW);
digitalWrite(pins[1], LOW);
digitalWrite(pins[2], LOW);
digitalWrite(pins[3], LOW);
delay(delayConstantForShowOne);

digitalWrite(pins[0], HIGH);
digitalWrite(pins[1], LOW);
digitalWrite(pins[2], LOW);
digitalWrite(pins[3], LOW);
delay(delayConstantForShowOne);

digitalWrite(pins[0], LOW);
digitalWrite(pins[1], HIGH);
digitalWrite(pins[2], LOW);
digitalWrite(pins[3], LOW);
delay(delayConstantForShowOne);

digitalWrite(pins[0], HIGH);
digitalWrite(pins[1], HIGH);
digitalWrite(pins[2], LOW);
digitalWrite(pins[3], LOW);
delay(delayConstantForShowOne);

digitalWrite(pins[0], LOW);
digitalWrite(pins[1], LOW);
digitalWrite(pins[2], HIGH);
digitalWrite(pins[3], LOW);
delay(delayConstantForShowOne);

digitalWrite(pins[0], HIGH);
digitalWrite(pins[1], LOW);
digitalWrite(pins[2], HIGH);
digitalWrite(pins[3], LOW);
delay(delayConstantForShowOne);

digitalWrite(pins[0], HIGH);
digitalWrite(pins[1], HIGH);
digitalWrite(pins[2], HIGH);
digitalWrite(pins[3], LOW);
delay(delayConstantForShowOne);

digitalWrite(pins[0], LOW);
digitalWrite(pins[1], LOW);
digitalWrite(pins[2], LOW);
digitalWrite(pins[3], HIGH);
delay(delayConstantForShowOne);

digitalWrite(pins[0], HIGH);
digitalWrite(pins[1], LOW);
digitalWrite(pins[2], LOW);
digitalWrite(pins[3], HIGH);
delay(delayConstantForShowOne);

digitalWrite(pins[0], LOW);
digitalWrite(pins[1], HIGH);
digitalWrite(pins[2], LOW);
digitalWrite(pins[3], HIGH);
delay(delayConstantForShowOne);

digitalWrite(pins[0], HIGH);
digitalWrite(pins[1], HIGH);
digitalWrite(pins[2], LOW);
digitalWrite(pins[3], HIGH);
delay(delayConstantForShowOne);

digitalWrite(pins[0], LOW);
digitalWrite(pins[1], LOW);
digitalWrite(pins[2], HIGH);
digitalWrite(pins[3], HIGH);
delay(delayConstantForShowOne);

digitalWrite(pins[0], HIGH);
digitalWrite(pins[1], LOW);
digitalWrite(pins[2], HIGH);
digitalWrite(pins[3], HIGH);
delay(delayConstantForShowOne);

digitalWrite(pins[0], LOW);
digitalWrite(pins[1], HIGH);
digitalWrite(pins[2], HIGH);
digitalWrite(pins[3], HIGH);
delay(delayConstantForShowOne);

digitalWrite(pins[0], HIGH);
digitalWrite(pins[1], HIGH);
digitalWrite(pins[2], HIGH);
digitalWrite(pins[3], HIGH);
delay(delayConstantForShowOne);
*/


/*Create a 16x4 matrix of binary numbers from 0 to 15.
Each row represents one binary number*/
const int binaryNumbers[16][4] = {
    {0, 0, 0, 0}, 
    {0, 0, 0, 1}, 
    {0, 0, 1, 0}, 
    {0, 0, 1, 1}, 
    {0, 1, 0, 0}, 
    {0, 1, 0, 1}, 
    {0, 1, 1, 0}, 
    {0, 1, 1, 1}, 
    {1, 0, 0, 0}, 
    {1, 0, 0, 1}, 
    {1, 0, 1, 0}, 
    {1, 0, 1, 1}, 
    {1, 1, 0, 0}, 
    {1, 1, 0, 1}, 
    {1, 1, 1, 0}, 
    {1, 1, 1, 1}  
};

    //Loops through every state of the matrix
    for (int i = 0; i < 16; i++) { // Loops through every row
      if(gameRunning){ //Stops the loop if game is running
        return;
      }
        for (int j = 0; j < 4; j++) { // Loops through every column
            digitalWrite(pins[j], binaryNumbers[i][j]); // Sets LED on or off depending on matrix element
        }
        delay(delayConstantForShowOne);

        if (buttonNumber == 4) {
            return; // Exit if button 4 is pressed
        }
      }



  }



void show2(int rounds)
{
  float delayTimeForShowTwo = 200; //Delay constant for show2

  for (int roundCounter = 1; roundCounter <= rounds; roundCounter++){ //Loops through every input round from 0 to 3

    for (int ledsClockwise = 0; ledsClockwise <= 3; ledsClockwise++){ //Loops through every LED
      digitalWrite(pins[ledsClockwise], HIGH); //Set LED on
      delay(delayTimeForShowTwo); //Keep LED on for delayTimeForShowTwo
      digitalWrite(pins[ledsClockwise], LOW); //Set LED off
    }

    for (int ledsCounterClockwise = 2; ledsCounterClockwise >= 1; ledsCounterClockwise--){ //Loops back through every input round from 3 to 0. Note only LED 1 & 2 needs to be set on and off.
      digitalWrite(pins[ledsCounterClockwise], HIGH); //Set LED on
      delay(delayTimeForShowTwo); //Keep LED on for delayTimeForShowTwo
      digitalWrite(pins[ledsCounterClockwise], LOW); //Set LED off

    }

    delayTimeForShowTwo *= 0.8; //Reduce delay constant to increase Loop-speed.
  }
}

void show3()
{
  setAllLeds();
  delay(400);
  clearAllLeds();
  delay(400);
  setAllLeds();
  delay(400);
  clearAllLeds();
  delay(400);
}

