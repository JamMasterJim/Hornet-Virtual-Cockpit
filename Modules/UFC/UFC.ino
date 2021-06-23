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

//Includes
#include "DcsBios.h"

//Registry
DcsBios::LED masterCautionLt(0x7408, 0x0200, OnBoardLED);

void setup() {
  DcsBios::setup(); 
}

void loop() {
  DcsBios::loop();
}
