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

//DCS Bios Register
DcsBios::LED masterCautionLt(0x7408, 0x0200, OnBoardLED);
DcsBios::Switch2Pos masterCautionResetSw("MASTER_CAUTION_RESET_SW", MasterCautionReset);

void setup() {
  DcsBios::setup();
  
  //Setup the interups 
  PCICR |= B00000111;      //Bit0 = 1 -> "PCIE0" enabeled, Bit1 = 1 -> "PCIE1" enabeled, Bit2 = 1 -> "PCIE2" enabeled
  PCMSK0 |= B00000000;      //Nothin will trigger Interupt
  PCMSK1 |= B00000000;      //Nothin will trigger Interupt
  PCMSK2 |= B00000000;      //Nothin will trigger Interupt
}

void loop() {
  DcsBios::loop();
}

//Interrupt Services
ISR (PCINT0_vect) {
  //for PCINT[0-7] 
 }
 
ISR (PCINT1_vect) {
  //for PCINT[8-15] Note PCINT[11-15] not exposed physically
 }
 
ISR (PCINT2_vect) {
  //for PCINT[16-23] 
 }
