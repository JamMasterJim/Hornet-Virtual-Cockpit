/*
  Description: Arduino Code for the custom Up Front Controller (UFC)
  
  Author: James Hall
  Created: 23/06/21

  Platform:
  This is designed to work on the ATMEGA 2560 Platform
 */

//Variables
int columns []={62, 63, 64 ,65 ,66, 67, 68, 69}; //Array of digital pins used as columns for the digital matrix Port K
int rows []={37, 36, 35, 34, 33, 32, 31, 30}; //Array of digital pins used as rows for the digital matrix
int UFCBrt = 0;
int UFCBrtPWM = 0;
const char True[] = "1";
const char False[] = "0"; 

#define NOP __asm__ __volatile__ ("nop\n\t")


//Definitions
#define DCSBIOS_IRQ_SERIAL
#define OnBoardLED 13

//DCS Bios Switch2Pos
#define UFC_COMM1_PULL 24
#define UFC_COMM2_PULL 27

//DCS Bios Switch3Pos

//DCS Bios RotaryEncoders
#define UFC_COMM1_CHANNEL_SELECT_CLK 22
#define UFC_COMM1_CHANNEL_SELECT_DT 23
#define UFC_COMM2_CHANNEL_SELECT_CLK 25
#define UFC_COMM2_CHANNEL_SELECT_DT 26

//DCS Bios Potentiometers
#define UFC_BRT A0

//DCS Bios LED
#define UFC_BRT_PWM 4

//Includes
#include "DcsBios.h"

//DCS Bios Register
//Switch2Pos
DcsBios::Switch2Pos ufcComm1Pull("UFC_COMM1_PULL", UFC_COMM1_PULL);
DcsBios::Switch2Pos ufcComm2Pull("UFC_COMM2_PULL", UFC_COMM2_PULL);

//Switch3Pos

//RotaryEncoder
DcsBios::RotaryEncoder ufcComm1ChannelSelect("UFC_COMM1_CHANNEL_SELECT", "DEC", "INC", UFC_COMM1_CHANNEL_SELECT_DT, UFC_COMM1_CHANNEL_SELECT_CLK);
DcsBios::RotaryEncoder ufcComm2ChannelSelect("UFC_COMM2_CHANNEL_SELECT", "DEC", "INC", UFC_COMM2_CHANNEL_SELECT_DT, UFC_COMM2_CHANNEL_SELECT_CLK);

//Potentiometer
//DcsBios::PotentiometerEWMA<5, 128, 5> ufcBrt("UFC_BRT", UFC_BRT);

//LED

//IntegerBuffer
void onUfcBrtChange(unsigned int newValue) {
    UFCBrt = newValue;
}
DcsBios::IntegerBuffer ufcBrtBuffer(0x741e, 0xffff, 0, onUfcBrtChange);



void setup() {

  DcsBios::setup();
  
  /*
   * --Help--
   * DDxn
   * 0 = input, 1 = Output
   * Example 1: DDRA |= B00000000; (Port A digital Pins configured as input)
   * Example 2: DDRB = (1<<DDB3) | (1<<DDB2); (Configures PortB Pins 2 & 3 as Outputs)
   * 
   * PORTxn
   * 0 = Low, 1 = High
   * Example 1: DDRK |= B00110000; (Port K pins 5&6 high - source i, rest are low - sink i)
   * 
   * DDxn | PORTxn | I/O | Pull-up | Comment
   *  0   |   0    |  I  |   No    | Tri-State (Hi-Z)
   *  0   |   1    |  I  |   Yes   | Pxn will source current if pulled low (externally)
   *  1   |   0    |  O  |   No    | Output Low (Sink)
   *  1   |   1    |  O  |   No    | Output High (Source)
   */

  //Configure Port A 
  //Configure Port B
  //Configure Port C (Digital Matrix Rows)
  DDRC  = B11111111;  //Configure as Output  
  PORTC = B00000000;  //Output low (Sink)
  //Configure Port D
  //Configure Port E
  //Configure Port F
  //Configure Port G
  //Configure Port H
  //Configure Port J
  //Configure Port K (Digital Matrix Columns)
  DDRK  = B00000000;  //Configure as Input
  PORTK = B11111111;  //Enable Internal Pull-up
  //Configure Port L
  
  //Setup the interups 
  PCICR  = B00000100;      //Bit0 = 1 -> "PCIE0" enabeled, Bit1 = 0 -> "PCIE1" disabeled, Bit2 = 1 -> "PCIE2" enabeled
  PCMSK0 = B00000000;      //Nothin will trigger Interupt
  PCMSK1 = B00000000;      //Nothin will trigger Interupt
  PCMSK2 = B11111111;      //Nothin will trigger Interupt

  NOP; // Delay 1 clock cycle for synchronization

}

void loop() {
  DcsBios::loop();
  //UFCBrtPWM = map(UFCBrt, 0, 65535, 0, 255);
  //analogWrite(UFC_BRT_PWM, UFCBrtPWM);
}

//Interrupt Services
ISR (PCINT0_vect) 
{ 
  //for PCINT[0-7] --> Port B
}
 
ISR (PCINT1_vect) 
{
  //for PCINT[8-15] Note PCINT[11-15] not exposed physically
}
 
ISR (PCINT2_vect) 
{
  
  //for PCINT[16-23] --> Port K
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  static unsigned int button = 0;
  unsigned int PortK = 0;
  unsigned int PortC = 0;
  
  // If interrupts come faster than 20ms, assume it's a bounce and ignore
  //Also ignore any return to 1 i.e. button release
  if (interrupt_time - last_interrupt_time > 10 && PINK != 0b11111111) 
  {

    PortK = ~PINK & 0b0000000011111111;
    PortC = ~findRow() << 8;
    button = PortC + PortK;
    
    //Reset ports K = Input(Pull-up) C = Output(Low)
    DDRK  = B00000000;  //Configure as Input
    PORTK = B11111111;  //Enable Internal Pull-up  

    DDRC  = B11111111;  //Configure as Output  
    PORTC = B00000000;  //Output low (Sink)

    buttonPress(button, True);
  } 

  if (interrupt_time - last_interrupt_time > 10 && PINK == 0b11111111)
  {
    buttonPress(button, False);
  }

  last_interrupt_time = interrupt_time;
}



unsigned int findRow ()
{

  DDRC  = B00000000;  //Change Port C from output to input 
  PORTC = B11111111;  //Enable Internal Pull-up

  DDRK  = B11111111;  //Change Port K from input to output
  PORTK = B00000000;  //Output low (Sink)

  NOP;

  return PINC;
}

void buttonPress (unsigned int button, const char *value)
{
    switch (button)
    {
      //Row 0
      case 257:
        //Keyboard 1
        sendDcsBiosMessage("UFC_1", value);
      break;
      case 258:
        //Keyboard 2
        sendDcsBiosMessage("UFC_2", value);
      break;
      case 260:
        //Keyboard 3
        sendDcsBiosMessage("UFC_3", value);
      break;
      case 264:
        //NC
      break;
      case 272:
        //Option Select 1
        sendDcsBiosMessage("UFC_OS1", value);
      break;
      case 288:
        //Function A/P
        sendDcsBiosMessage("UFC_AP", value);
      break;
      case 320:
        //Function IFF
        sendDcsBiosMessage("UFC_IFF", value);
      break;
      case 384:
        //NC
      break;

      //Row 1
      case 513:
        //Keyboard 4
        sendDcsBiosMessage("UFC_4", value);
      break;
      case 514:
        //Keyboard 5
        sendDcsBiosMessage("UFC_5", value);
      break;
      case 516:
        //Keyboard 6
        sendDcsBiosMessage("UFC_6", value);
      break;
      case 520:
        //NC
      break;
      case 528:
        //Option Select 2
        sendDcsBiosMessage("UFC_OS2", value);
      break;
      case 544:
        //Function TCN
        sendDcsBiosMessage("UFC_TCN", value);
      break;
      case 576:
        //Function ILS
        sendDcsBiosMessage("UFC_ILS", value);
      break;
      case 640:
        //NC
      break;
      
      //Row 2
      case 1025:
        //Keyboard 7
        sendDcsBiosMessage("UFC_7", value);
      break;
      case 1026:
        //Keyboard 8
        sendDcsBiosMessage("UFC_8", value);
      break;
      case 1028:
        //Keyboard 9
        sendDcsBiosMessage("UFC_9", value);
      break;
      case 1032:
        //NC
      break;
      case 1040:
        //Option Select 3
        sendDcsBiosMessage("UFC_OS3", value);
      break;
      case 1056:
        //Function DL
        sendDcsBiosMessage("UFC_DL", value);
      break;
      case 1088:
        //Function BCN
        sendDcsBiosMessage("UFC_BCN", value);
      break;
      case 1152:
        //NC
      break;

      //Row 3
      case 2049:
        //Keyboard CLR
        sendDcsBiosMessage("UFC_CLR", value);
      break;
      case 2050:
        //Keyboard 0
        sendDcsBiosMessage("UFC_0", value);
      break;
      case 2052:
        //Keyboard ENT
        sendDcsBiosMessage("UFC_ENT", value);
      break;
      case 2056:
        //NC
      break;
      case 2064:
        //Option Select 4
        sendDcsBiosMessage("UFC_OS4", value);
      break;
      case 2080:
        //Function ON/OFF
        sendDcsBiosMessage("UFC_ONOFF", value);
      break;
      case 2112:
        //Emission Control
        sendDcsBiosMessage("UFC_EMCON", value);
      break;
      case 2176:
        //NC
      break;

      //Row 4
      case 4097:
        //NC
      break;
      case 4098:
        //NC
      break;
      case 4100:
        //NC
      break;
      case 4104:
        //NC
      break;
      case 4112:
        //Option Select 5
        sendDcsBiosMessage("UFC_OS5", value);
      break;
      case 4128:
        //NC
      break;
      case 4160:
        //I/P
        sendDcsBiosMessage("UFC_IP", value);
      break;
      case 4224:
        //NC
      break;
     
      default:
      break;
    }
}
