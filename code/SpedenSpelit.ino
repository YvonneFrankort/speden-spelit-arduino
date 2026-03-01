#include "display.h"
#include "buttons.h"
#include "leds.h"
#include "SpedenSpelit.h"
#include <EEPROM.h>

#define BUTTON_0 2
#define BUTTON_1 3
#define BUTTON_2 4
#define BUTTON_3 5
#define BUTTON_4 6

volatile bool newTimerInterrupt = false;      // Timer1 interrupt flag
volatile bool timeToCheckGameStatus = false;  // Check game status
volatile bool gameRunning = false;            // Determines if game is running or not
volatile bool secretGameRunning = false;      // Determines if secret game is running or not
volatile bool gameOver = false;               // Tracks if the game is already over

unsigned long lastButtonPushTime = 0;         // Tracks the time of the last button press
const unsigned long pushTimeout = 1500;       // 1.5 seconds timeout

volatile int randomNumbers[100];              // Random numbers
volatile int randomNumber;                    // Current random number
volatile int nbrOfButtonPush = 0;             // Button press count
volatile int score = 0;                       // Player score
volatile int lastButtonPressCount = 0;        // Previous button press, previous to nbrOfButtonPush, used in checkGame
volatile int lastNbrOfButtonPush = 0;         // Store the previous button press count
volatile int highscore = 0;                   // The highest points achieved (between games)

void setup() {
  pinMode(BUTTON_0, INPUT_PULLUP);
  pinMode(BUTTON_1, INPUT_PULLUP);
  pinMode(BUTTON_2, INPUT_PULLUP);
  pinMode(BUTTON_3, INPUT_PULLUP);
  pinMode(BUTTON_4, INPUT_PULLUP);

  Serial.begin(9600);
  initializeLeds();
  initButtonsAndButtonInterrupts();
  initializeTimer();
  initializeDisplay();
  TIMSK1 &= ~((1 << OCIE1A) | (1 << OCIE1B)); //Stops the timer1
}

void loop() 
{
  while (!gameRunning && !secretGameRunning)        // While the game is not running we will run the show1 (binary number count)
  {
    readHighScore();
    showResult(highscore);
    show1();                  

    if (buttonNumber == 4)    // If the pressed button is n4 = start button, the game will start
    {
      startTheGame();
      buttonNumber = -1;      // Reset button number
    }

    if(userNumbers[0] == 0){
     if(userNumbers[1] == 0){
      if(userNumbers[2] == 0){
        if(userNumbers[3] == 0){
          if(userNumbers[4] == 1){
              startSecretTheGame();
              buttonNumber = -1;      // Reset button number
            }
          }
        }      
      } 
    }
    return;                   // Do nothing if the game is not running 
  }

  while(gameRunning)          // While the game is going on 
  {
    if (newTimerInterrupt)    // If the interrupt is triggered, we reset it and save the generated random number into randomNumbers[] ...
    {                         // ... using nbrOfButtonPush as index
      newTimerInterrupt = false;
      randomNumbers[nbrOfButtonPush] = randomNumber;
    }

    if (buttonNumber >= 0 && buttonNumber < 4)    // If the pressed button is in between 0-3, we will save user's input into userNumbers[], again use nb
    {                                             // using nbrOfButtonPush as index, this index and the score will be increased, then we will check
    userNumbers[nbrOfButtonPush] = buttonNumber;  // if it matches the generated random number with the function checkGame 
    nbrOfButtonPush++;
    score++;
    buttonNumber = -1;  
    lastButtonPushTime = millis();                // Reset timeout timer
    timeToCheckGameStatus = true;
    }

    if (timeToCheckGameStatus) 
    {
      timeToCheckGameStatus = false;
      checkGame();
    }

    checkButtonPushTimeout();                      // Call the new timeout checking function
  }

  while(secretGameRunning)          // While the game is going on 
  {
    if (newTimerInterrupt)    // If the interrupt is triggered, we reset it and save the generated random number into randomNumbers[] ...
    {                         // ... using nbrOfButtonPush as index
      newTimerInterrupt = false;
      randomNumbers[nbrOfButtonPush] = randomNumber;
    }

    if (buttonNumber >= 0 && buttonNumber < 4)    // If the pressed button is in between 0-3, we will save user's input into userNumbers[], again use nb
    {                                             // using nbrOfButtonPush as index, this index and the score will be increased, then we will check
    userNumbers[nbrOfButtonPush] = buttonNumber;  // if it matches the generated random number with the function checkGame 
    nbrOfButtonPush++;
    score++;
    buttonNumber = -1;  
    lastButtonPushTime = millis();                // Reset timeout timer
    timeToCheckGameStatus = true;
    }

    if (timeToCheckGameStatus) 
    {
      timeToCheckGameStatus = false;
      checkGame();
    }

    checkButtonPushTimeout();                      // Call the new timeout checking function
  }
}

void initializeTimer() 
{
  TCCR1A = 0;                           // TCCR1A and B are control registers for Timer1, set to 0 to disable any previous settings
  TCCR1B = 0;   
  TCNT1 = 0;                            // Counter register set to 0 to start counting from 0

  OCR1A = 15625;                        // OCR1A and B are Output Compare Registers, A-value is equal to 1s intervals
  OCR1B = 12650;                        // B-value will trigger an interrupt slightly before A

  TCCR1B |= (1 << WGM12);               // CTC mode
  TCCR1B |= (1 << CS12) | (1 << CS10);  // Prescaler 1024

  TIMSK1 |= (1 << OCIE1A);              // Enable Timer1 Compare A interrupt
  TIMSK1 |= (1 << OCIE1B);              // EnableTimer1 Compare Match B interrupt
}

ISR(TIMER1_COMPA_vect)                  // We check the game, clear all leds, generate a random number, a led will light up, depending on the generated number
{                                       // Timer and buttonscpp-ISR triggered (booleans)
  timeToCheckGameStatus = true;

  clearAllLeds();    
  if (gameRunning){                   
  randomNumber = random(0, 4);
  //randomNumbers[nbrOfButtonPush] = randomNumber;
  setLed(randomNumber);
  }

  if (secretGameRunning){
    switch (randomNumber){
      case -1:
      randomNumber = 0;
      break;

      case 0:
      randomNumber = 1;
      break;

      case 1:
      randomNumber = 2;
      break;

      case 2:
      randomNumber = 3;
      break;

      case 3:
      randomNumber = 0;
      break;
    }
    setLed(randomNumber);
  }

  newTimerInterrupt = true;
  isrTriggered = true;
  // Serial.print("Random number generated: ");    *****DEBUGGING******
  // Serial.println(randomNumber);
  // Serial.print("Number of button presses: ");
  // Serial.println(nbrOfButtonPush);
  // Serial.print("Random number index: ");
  // Serial.println(randomNumbers[nbrOfButtonPush]);
  // Serial.println("##########################");
}


ISR(TIMER1_COMPB_vect)                   // Clears LEDs if same random number appears multiple times in a row
{
  clearAllLeds(); 
}


void checkGame()                         // Checks if randomNumber and userNumber match, if they dont match = Game over.
{                                        // When user got 10 ok presses, the difficulty will increase by 10 %
  if (nbrOfButtonPush <= 0) return;

  if (userNumbers[nbrOfButtonPush - 1] != randomNumbers[nbrOfButtonPush - 1]) 
  {
    stopTheGame();
    return;
  }

  showResult(score);

  if (nbrOfButtonPush > lastButtonPressCount && nbrOfButtonPush % 10 == 0) 
  {
    increaseGameDifficulty();
    lastButtonPressCount = nbrOfButtonPush;
  }
}

void initializeGame()                      // When we initialize the game all variables are reseted.
{
  nbrOfButtonPush = 0;
  score = 0;
  for (int i = 0; i < 100; i++) {
    randomNumbers[i] = -1;
    userNumbers[i] = -1;
  }
  userNumberIndex = 0;
}

void startTheGame()                           // Start the game, initialize game and timer.
{
  show3();
  Serial.println("New game started. Have fun!");
  initializeGame();
  lastNbrOfButtonPush = nbrOfButtonPush;

  TIMSK1 |= (1 << OCIE1A);                    // Enable Compare Match A interrupt
  TIMSK1 |= (1 << OCIE1B);                    // Enable Compare Match B interrupt

  OCR1A = 15625;                        // OCR1A and B are Output Compare Registers, A-value is equal to 1s intervals
  OCR1B = 12650;                        // B-value will trigger an interrupt slightly before A

  lastButtonPushTime = millis();              // Initialize timeout timer

  gameRunning = true;                         // Starts the game
  secretGameRunning = false;
  gameOver = false;                           // Enables game to stop
}

void startSecretTheGame()                           // Start the game, initialize game and timer.
{
  show3();
  Serial.println("New secret game started. Have fun!");
  initializeGame();
  lastNbrOfButtonPush = nbrOfButtonPush;

  TIMSK1 |= (1 << OCIE1A);                    // Enable Compare Match A interrupt
  TIMSK1 |= (1 << OCIE1B);                    // Enable Compare Match B interrupt

  OCR1A = 15625;                        // OCR1A and B are Output Compare Registers, A-value is equal to 1s intervals
  OCR1B = 12650;                        // B-value will trigger an interrupt slightly before A

  lastButtonPushTime = millis();              // Initialize timeout timer
  secretGameRunning = true;
  gameRunning = false;                         // Starts the game
  gameOver = false;                           // Enables game to stop
}


void stopTheGame()                            // Stop the game, check if the game is already over first. Prints out score
{
  if (gameOver)                               
  { 
   return;
  }

  gameOver = true;                            // Game over
  gameRunning = false;                        // Stop the game
  secretGameRunning = false;                  // Stop the secret game
  TIMSK1 &= ~((1 << OCIE1A) | (1 << OCIE1B)); // Stop the TIMER1
  userNumberIndex = 0;                        // Reset userNumber[] array index for the next game
  Serial.println("Game Over");
  Serial.print("Your score is ");
  Serial.println(score);

  if (score > highscore)
  {
    highscore = score;
    Serial.print("New highscore is: ");
    Serial.println(highscore);
    saveHighScore(highscore);
  }
  clearAllLeds();
  show2(10);
}

/* void stopTimer() //Well, this is useless
{
  // Disable Timer1 interrupts
  TIMSK1 &= ~((1 << OCIE1A) | (1 << OCIE1B));  // Disable interrupt for Compare Match A and B

  // Stop Timer1 by clearing the prescaler bits
  TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10));  // Disable the clock source for Timer1

  // Reset Timer1 counter
  TCNT1 = 0;  // Reset the timer count to 0

  // Reset the Compare Match Registers (optional)
  OCR1A = 0;  // Reset Compare Match A register
  OCR1B = 0;  // Reset Compare Match B register

  // Optionally, disable PWM on Timer1 if it was in use
  TCCR1A &= ~((1 << COM1A0) | (1 << COM1A1) | (1 << COM1B0) | (1 << COM1B1));  // Disable PWM output

  Serial.println("Timer1 stopped.");
} */ 

void increaseGameDifficulty()                // After 10 right presses, difficulty increases by 10 %. Starting rateA is OCR1A and rateB OCR1B
{
  uint16_t currentRateA = OCR1A;
  OCR1A = currentRateA * 0.9;                // Decrease match value by 10% to speed up the timer
  uint16_t currentRateB = OCR1B;
  OCR1B = currentRateB = OCR1B * 0.9;        //Decrease COMPB interrupt to keep up with COMPA
  Serial.print("Game difficulty increased. New rate: ");
  Serial.println(OCR1A);
}

void checkButtonPushTimeout()                      // Check if any of the buttons is pressed within 1.5s.
{                                                  // First check if game is already over from pressing wrong button
  if (gameOver) return;                       
  
  if (millis() - lastButtonPushTime > pushTimeout) // Check if the elapsed time since the last button push exceeds the timeout
  {
    Serial.println("Too slow! Boomerism doesn't come alone.");
    stopTheGame();
  }
}

void saveHighScore(int highscore) {
  // Save the high score to EEPROM (address 0)
  EEPROM.put(0, highscore);
  Serial.println("High Score Saved!");
}

int readHighScore() {
  // Read the high score from EEPROM (address 0)
  EEPROM.get(0, highscore);
  return highscore;
}