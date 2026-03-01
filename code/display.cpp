#include "display.h"

// Määritellään Arduino-nastojen numerot
const uint8_t RESET_PIN = 12;
const uint8_t SHIFT_CLOCK_PIN = 11;
const uint8_t LATCH_CLOCK_PIN = 10;
const uint8_t OUT_ENABLE_PIN = 9;
const uint8_t SERIAL_INPUT_PIN = 8;

// 7-segmenttien koodaus 0-9 numeroille (common cathode)
const uint8_t digitEncoding[10] = {
  0b00111111, // 0
  0b00000110, // 1
  0b01011011, // 2
  0b01001111, // 3
  0b01100110, // 4
  0b01101101, // 5
  0b01111101, // 6
  0b00000111, // 7
  0b01111111, // 8
  0b01101111  // 9
};

/*
  initializeDisplay subroutine initializes the necessary pins
  and sets initial states for the 7-segment displays.
*/
void initializeDisplay(void)
{
  // Asetetaan pin modes
  pinMode(RESET_PIN, OUTPUT);
  pinMode(SHIFT_CLOCK_PIN, OUTPUT);
  pinMode(LATCH_CLOCK_PIN, OUTPUT);
  pinMode(OUT_ENABLE_PIN, OUTPUT);
  pinMode(SERIAL_INPUT_PIN, OUTPUT);
  
  // Alustetaan pinnejä
  digitalWrite(RESET_PIN, HIGH);      // Ei resetti
  digitalWrite(SHIFT_CLOCK_PIN, LOW);
  digitalWrite(LATCH_CLOCK_PIN, LOW);
  digitalWrite(OUT_ENABLE_PIN, LOW);  // Ota käyttöön lähtö
  digitalWrite(SERIAL_INPUT_PIN, LOW);
}

/*
  writeByte subroutine writes an 8-bit number to the
  serial-to-parallel shift registers.

  Parameters:
  uint8_t number: 8-bittinen luku, joka kirjoitetaan
  bool last: jos true, ladataan data shift registersiin
*/
void writeByte(uint8_t number, bool last)
{
  // Varmistetaan, että shiftClock ja latchClock ovat LOW
  digitalWrite(SHIFT_CLOCK_PIN, LOW);
  digitalWrite(LATCH_CLOCK_PIN, LOW);
  
  // Siirretään bitit MSB:stä LSB:ään
  for(int i = 7; i >= 0; i--)
  {
    // Asetetaan data pin
    if (number & (1 << i))
      digitalWrite(SERIAL_INPUT_PIN, HIGH);
    else
      digitalWrite(SERIAL_INPUT_PIN, LOW);
    
    // Kytketään shift clock ylös ja alas
    digitalWrite(SHIFT_CLOCK_PIN, HIGH);
    digitalWrite(SHIFT_CLOCK_PIN, LOW);
  }
  
  // Jos last on true, päivitetään latch clock
  if(last)
  {
    digitalWrite(LATCH_CLOCK_PIN, HIGH);
    digitalWrite(LATCH_CLOCK_PIN, LOW);
  }
}

/*
  writeHighAndLowNumber subroutine writes the tens and ones
  digits to the 7-segment displays.

  Parameters:
  uint8_t tens: kymmenien paikka (0-9)
  uint8_t ones: ykkösten paikka (0-9)
*/
void writeHighAndLowNumber(uint8_t tens, uint8_t ones)
{
  // Koodataan numerot 7-segmenttiin
  uint8_t onesEncoded = digitEncoding[ones];
  uint8_t tensEncoded = digitEncoding[tens];
  
  // Kirjoitetaan ensin ykköset, sitten kymmenet
  writeByte(onesEncoded, false);   // Ykköset, ei lataa vielä
  writeByte(tensEncoded, true);    // Kymmenet, ladataan molemmat
}

/*
  showResult subroutine ottaa tuloksen ja jakaa sen kympiin
  ja yksiin, ja kirjoittaa ne näytölle.

  Parameters:
  byte result: tulos 0-99
*/
void showResult(byte result)
{
  if(result > 99)
    result = 99; // Rajataan yläraja
  
  uint8_t tens = result / 10;
  uint8_t ones = result % 10;
  
  writeHighAndLowNumber(tens, ones);
}