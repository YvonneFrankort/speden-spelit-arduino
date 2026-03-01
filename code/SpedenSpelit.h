#ifndef SPEDENSPELIT_H
#define SPEDENSPELIT_H
#include <arduino.h>
#include <avr/io.h>
#include <avr/interrupt.h>

extern volatile bool gameRunning;

void initializeTimer(void);

ISR(TIMER1_COMPA_vect);

void initializeGame(void);

void checkGame();

void startTheGame(void);

void stopTheGame(void);

void startSecretTheGame(void);

void increaseGameDifficulty();


#endif