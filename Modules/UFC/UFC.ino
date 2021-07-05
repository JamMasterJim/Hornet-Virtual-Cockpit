/*
  Description: Arduino Code for the custom Up Front Controller (UFC)
  
  Author: James Hall
  Created: 23/06/21

  Platform:
  This should work on any Arduino that has an ATMega328 controller
  (Uno, Pro Mini, many others).
 */

//Definitions
#define DCSBIOS_IRQ_SERIAL
#define OnBoardLED 13
#define MasterCautionReset 2

//Includes
#include "DcsBios.h"
#include <Keypad.h>

//Registry
DcsBios::LED masterCautionLt(0x7408, 0x0200, OnBoardLED);
DcsBios::Switch2Pos masterCautionResetSw("MASTER_CAUTION_RESET_SW", MasterCautionReset);
DcsBios::PotentiometerEWMA<5, 128, 5> ufcBrt("UFC_BRT", A0);

//Create the keypad Matrix for button inputs
const byte ROWS = 8; // eight rows
const byte COLS = 8; // eight columns
// Define the Keymap
char keys[ROWS][COLS] = {
  {'1','2','3','\0','\0','\0','\0','\0'},
  {'4','5','6','\0','\0','\0','\0','\0'},
  {'7','8','9','\0','\0','\0','\0','\0'},
  {'C','0','E','\0','\0','\0','\0','\0'},
  {'\0','\0','\0','\0','\0','\0','\0','\0'},
  {'\0','\0','\0','\0','\0','\0','\0','\0'},
  {'\0','\0','\0','\0','\0','\0','\0','\0'},
  {'\0','\0','\0','\0','\0','\0','\0','\0'}
};

// Connect keypad to these Arduino pins.
//byte rowPins[ROWS] = { 37, 36, 35, 34, 33, 32, 31, 30 };
//byte colPins[COLS] = { 22, 23, 24, 25, 26, 27, 28, 29 }; 

// Create the Keypad
//Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup() {
  DcsBios::setup(); 
}

void loop() {
  DcsBios::loop();
  //keypad();
}
/*
void keypad(){
  char key = kpd.getKey();
  if(key)  // Check for a valid key.
  {
    switch (key)
    {
      case '*':
        digitalWrite(OnBoardLED, LOW);
        break;
      case '#':
        digitalWrite(OnBoardLED, HIGH);
        break;
      default:
        delay(10);
        //Serial.println(key);
    }
  }
 }
 */
