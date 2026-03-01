#ifndef BUTTONS_H
#define BUTTONS_H
#include <arduino.h>
#include <avr/io.h>
#include <avr/interrupt.h>


const byte firstPin = 2; // First PinChangeInterrupt on D-bus
const byte lastPin =  5; // Last PinChangeInterrupt on D-bus
const byte startPin = 6; // pin that starts the game
const unsigned long debounceDelay = 300; //delay 50ms
extern volatile unsigned long lastDebounceTime[5]; //last debounce time
extern volatile bool pressedButton[5]; //current debounce state
extern volatile int buttonNumber; //stores button number
extern volatile bool isrTriggered; //tells the programm if ISR is triggered
extern volatile int userNumbers[100]; //array to store button press
extern volatile int userNumberIndex; //position in the userNumbers array


/* 
  initButtonsAndButtonInterrupts subroutine is called from Setup() function
  during the initialization of Speden Spelit. This function does the following:
  1) Initializes 4 button pins for the game = Arduino pins 2,3,4,5
  2) Initializes 1 button pin for starting the game = Aruino pin 6
  3) Enables PinChangeInterrupt on D-bus in a way that interrupt
     is generated whenever some of pins 2,3,4,5,6 is connected to LOW state

*/
void initButtonsAndButtonInterrupts(void);


// Intoduce PCINT2_vect Interrupt SeRvice (ISR) function for Pin Change Interrupt.
ISR(PCINT2_vect); // Interrupt Service Routine, triggered by pin input changes, PCINT2 for the pins 2 to 7
#endif;