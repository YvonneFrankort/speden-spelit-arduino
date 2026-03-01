#include "buttons.h"


volatile bool isrTriggered = false;
volatile unsigned long lastDebounceTime[5] = {0}; // volatile = variable can change anytime, unsigned long used for time (millis), 5 pins, {0} empties the array when program starts
volatile bool pressedButton[5] = {false}; // bool stores true or false (button pressed or not), 5 pins, {flase} = no button is pressed
volatile int buttonNumber = -1; //button pressed
volatile int userNumbers[100];   // User button inputs
volatile int userNumberIndex = 0; //current index of the userNumber array

void initButtonsAndButtonInterrupts(void){

  for (byte pin = firstPin; pin <= startPin; pin ++){ // input pins 2 -5, game buttons
    pinMode(pin, INPUT_PULLUP); // sets pins with pull-up resistor, ensures pin remains high unless connected to low
    
  }
  PCICR |= 0b000000100; // pin change interrupts for PCINT2 group with pins D2 to D7
  PCMSK2 |= 0b01111100; //mask for pins D2 to D6 to trigger interrupts
  // if a pin changes it's state, the ISR will be triggered
}

ISR(PCINT2_vect) {

  unsigned long currentTime = millis(); // the current time

  for (byte pin = firstPin; pin <=startPin; pin ++){ // for function for the pins 2 - 6
    
    byte pinIndex = pin - firstPin; // calculates the array index for the pins
    
    if (digitalRead(pin) == LOW){ //checks the pin status

      if ((currentTime - lastDebounceTime[pinIndex]) > debounceDelay){ // checks if enough time (debounce delay) has passed dince last button change, if the button was pressed
      //has to be greater than debounceDelay
      if (!pressedButton[pinIndex]){ //checks if button was already pressed, ! = not
        pressedButton[pinIndex] = true; // button is stated as pressed
        buttonNumber = pinIndex; //store button number
        userNumbers[userNumberIndex] = buttonNumber; //stores the button press
        userNumberIndex++;//move forward in the userNumber array
        Serial.print("Nappi: ");
        Serial.println(buttonNumber);
        //Serial.print("User number index: ");
        //Serial.println(userNumbers[userNumberIndex]);

        isrTriggered = true;//interrupt occured
      
   Serial.print("Nappi "); //prints the text "Nappi "
   Serial.print(pin -2); // gives the pin number
   Serial.println(" painettu."); // prints the text " painettu"
      }
      lastDebounceTime[pinIndex] = currentTime; //updates debounce time
      }
  }else{
    pressedButton[pinIndex] = false; //reset the button state if no longer pressed
  }
  }
}