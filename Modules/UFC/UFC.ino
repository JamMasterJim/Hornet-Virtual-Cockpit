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
#define UFC_COMM1_PULL 4
#define UFC_COMM2_PULL 5

//DCS Bios Switch3Pos

//DCS Bios RotaryEncoders
#define UFC_COMM1_CHANNEL_SELECT_CLK 2
#define UFC_COMM1_CHANNEL_SELECT_DT 3
#define UFC_COMM2_CHANNEL_SELECT_CLK 6
#define UFC_COMM2_CHANNEL_SELECT_DT 7

//DCS Bios Potentiometers

//DCS Bios LED

//Includes
//#include "DcsBios.h"

//DCS Bios Register
//Switch2Pos
//DcsBios::Switch2Pos ufcComm1Pull("UFC_COMM1_PULL", UFC_COMM1_PULL);
//DcsBios::Switch2Pos ufcComm2Pull("UFC_COMM2_PULL", UFC_COMM2_PULL);

//Switch3Pos

//RotaryEncoder
//DcsBios::RotaryEncoder ufcComm1ChannelSelect("UFC_COMM1_CHANNEL_SELECT", "DEC", "INC", UFC_COMM1_CHANNEL_SELECT_DT, UFC_COMM1_CHANNEL_SELECT_CLK);
//DcsBios::RotaryEncoder ufcComm2ChannelSelect("UFC_COMM2_CHANNEL_SELECT", "DEC", "INC", UFC_COMM2_CHANNEL_SELECT_DT, UFC_COMM2_CHANNEL_SELECT_CLK);

//Potentiometer

//LED


//Variables


void setup() {
  /*
  DcsBios::setup();

  pinMode(UFC_COMM1_PULL, INPUT_PULLUP);
  pinMode(UFC_COMM2_PULL, INPUT_PULLUP);
  */
  
  Serial.begin(9600);

  pinMode(8, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);

  digitalWrite(10, LOW);
  digitalWrite(11, LOW);
  
  //Setup the interups 
  PCICR |= B00000001;      //Bit0 = 1 -> "PCIE0" enabeled, Bit1 = 0 -> "PCIE1" disabeled, Bit2 = 0 -> "PCIE2" disabeled
  PCMSK0 |= B00000011;      //Nothin will trigger Interupt
  PCMSK1 |= B00000000;      //Nothin will trigger Interupt
  PCMSK2 |= B00000000;      //Nothin will trigger Interupt
  
}

void loop() {
  //DcsBios::loop();
}

//Interrupt Services
ISR (PCINT0_vect) 
{ 
  //for PCINT[0-7]   
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  // If interrupts come faster than 200ms, assume it's a bounce and ignore
  if (interrupt_time - last_interrupt_time > 200) 
  {
    char column = '\0';
    column = findColumn (PINB);
  } 
  last_interrupt_time = interrupt_time;
}
 
ISR (PCINT1_vect) 
{
  //for PCINT[8-15] Note PCINT[11-15] not exposed physically
}
 
ISR (PCINT2_vect) 
{
  //for PCINT[16-23] 
}

char findColumn (byte port)
{
    switch (port) //must be a better way of doing this... its grim!
    {
      case 0b11111110:
      return '0';
      break;
      case 0b11111101:
      return '1';
      break;
      case 0b11111011:
      return '2';
      break;
      case 0b11110111:
      return '3';
      break;
      case 0b11101110:
      return '4';
      break;
      case 0b11011101:
      return '5';
      break;
      case 0b10111111:
      return '6';
      break;
      case 0b01111111:
      return '7';
      break;
      default:
      return '\0';
      break;
    }
}
