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

//DCS Bios Switch2Pos
#define UFC_COMM1_PULL 1
#define UFC_COMM2_PULL 0

//DCS Bios Switch3Pos

//DCS Bios RotaryEncoders

//DCS Bios Potentiometers

//DCS Bios LED

//Includes
#include "DcsBios.h"

//DCS Bios Register
//Switch2Pos
DcsBios::Switch2Pos ufcComm1Pull("UFC_COMM1_PULL", UFC_COMM1_PULL);
DcsBios::Switch2Pos ufcComm2Pull("UFC_COMM2_PULL", UFC_COMM2_PULL);

//Switch3Pos

//RotaryEncoder

//Potentiometer

//LED

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
ISR (PCINT0_vect) 
{
  /*
   * ---------- THIS CODE IS FAULTLY ----------  
   * NOTE Maybe switch the Debounce inside the Rising/Falling Edge detection???
  //for PCINT[0-7] 
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  // If interrupts come faster than 200ms, assume it's a bounce and ignore
  if (interrupt_time - last_interrupt_time > 100) 
  {
    if(!digitalRead(C1) && C1_state)
    {
      C1_state = LOW;
      Serial.println("C1 Falling");
    }
    else if(digitalRead(C1) && !C1_state)
    {
      C1_state = HIGH;
      Serial.println("C1 Rising");
    }

    if(!digitalRead(C2) && C2_state)
    {
      C2_state = LOW;
      Serial.println("C2 Falling");
    }
    else if(digitalRead(C2) && !C2_state)
    {
      C2_state = HIGH;
      Serial.println("C2 Rising");
    }
  }
  last_interrupt_time = interrupt_time;
  Serial.println(last_interrupt_time);
  */
}
 
ISR (PCINT1_vect) {
  //for PCINT[8-15] Note PCINT[11-15] not exposed physically
 }
 
ISR (PCINT2_vect) {
  //for PCINT[16-23] 
 }
