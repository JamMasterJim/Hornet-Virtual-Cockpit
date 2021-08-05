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
const char True[] = "1";
const char False[] = "0";
uint16_t OptDisplay1Buf[8];
uint16_t OptDisplay2Buf[8];
uint16_t OptDisplay3Buf[8];
uint16_t OptDisplay4Buf[8];
uint16_t OptDisplay5Buf[8];
const uint8_t OptDisplay1Addr = 0x70;
const uint8_t OptDisplay2Addr = 0x71;
const uint8_t OptDisplay3Addr = 0x72;
const uint8_t OptDisplay4Addr = 0x73;
const uint8_t OptDisplay5Addr = 0x74;


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
#include <Wire.h>

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
void onUfcBrtChange(unsigned int UFCBrt) {
    int OptionDisBrt = 0;
    OptionDisBrt = map(UFCBrt, 0, 65535, 0, 15);
    setBrightness(OptDisplay1Addr, OptionDisBrt);
    setBrightness(OptDisplay2Addr, OptionDisBrt);
    setBrightness(OptDisplay3Addr, OptionDisBrt);
    setBrightness(OptDisplay4Addr, OptionDisBrt);
    setBrightness(OptDisplay5Addr, OptionDisBrt);
}
DcsBios::IntegerBuffer ufcBrtBuffer(0x741e, 0xffff, 0, onUfcBrtChange);


//Option Cueing
void onUfcOptionCueing1Change(char* OptionCue1) {
  
  switch(OptionCue1[0])
  {
    case ':':
      for (int i = 0; i <=3; i++)
        {
          OptDisplay1Buf[i] = OptDisplay1Buf[i] + 0b0100000000000000;
        }      
    break;    
    default:
      for (int i = 0; i <=3; i++)
        {
          OptDisplay1Buf[i] = OptDisplay1Buf[i] & 0b0011111111111111;
        }  
    break;
  }
  show(OptDisplay1Addr);
}
DcsBios::StringBuffer<1> ufcOptionCueing1Buffer(0x7428, onUfcOptionCueing1Change);

void onUfcOptionCueing2Change(char* OptionCue2) {

  switch(OptionCue2[0])
  {
    case ':':
      for (int i = 0; i <=3; i++)
        {
          OptDisplay2Buf[i] = OptDisplay2Buf[i] + 0b0100000000000000;
        }       
    break;    
    default:
      for (int i = 0; i <=3; i++)
        {
          OptDisplay2Buf[i] = OptDisplay2Buf[i] & 0b0011111111111111;
        }  
    break;
  }
  show(OptDisplay2Addr);
}
DcsBios::StringBuffer<1> ufcOptionCueing2Buffer(0x742a, onUfcOptionCueing2Change);

void onUfcOptionCueing3Change(char* OptionCue3) {

  switch(OptionCue3[0])
  {
    case ':':
      for (int i = 0; i <=3; i++)
        {
          OptDisplay3Buf[i] = OptDisplay3Buf[i] + 0b0100000000000000;
        }  
    break;    
    default:
      for (int i = 0; i <=3; i++)
        {
          OptDisplay3Buf[i] = OptDisplay3Buf[i] & 0b0011111111111111;
        }  
    break;
  }
  show(OptDisplay3Addr);
}
DcsBios::StringBuffer<1> ufcOptionCueing3Buffer(0x742c, onUfcOptionCueing3Change);

void onUfcOptionCueing4Change(char* OptionCue4) {

  switch(OptionCue4[0])
  {
    case ':':
      for (int i = 0; i <=3; i++)
        {
          OptDisplay4Buf[i] = OptDisplay4Buf[i] + 0b0100000000000000;
        }  
    break;    
    default:
      for (int i = 0; i <=3; i++)
        {
          OptDisplay4Buf[i] = OptDisplay4Buf[i] & 0b0011111111111111;
        }  
    break;
  }
  show(OptDisplay4Addr);
}
DcsBios::StringBuffer<1> ufcOptionCueing4Buffer(0x742e, onUfcOptionCueing4Change);

void onUfcOptionCueing5Change(char* OptionCue5) {

  switch(OptionCue5[0])
  {
    case ':':
      for (int i = 0; i <=3; i++)
        {
          OptDisplay5Buf[i] = OptDisplay5Buf[i] + 0b0100000000000000;
        }  
    break;    
    default:
      for (int i = 0; i <=3; i++)
        {
          OptDisplay5Buf[i] = OptDisplay5Buf[i] & 0b0011111111111111;
        }  
    break;
  }
  show(OptDisplay5Addr);
}
DcsBios::StringBuffer<1> ufcOptionCueing5Buffer(0x7430, onUfcOptionCueing5Change);


//Option Display
void onUfcOptionDisplay1Change(char* Optiondisplay1) {

    int cue = 0;
    //clear(OptDisplay1Addr);
    for (int i = 0; i < strlen(Optiondisplay1); i++)
      {
        cue = OptDisplay1Buf[4-strlen(Optiondisplay1) + i] & 0b0100000000000000;
        OptDisplay1Buf[4-strlen(Optiondisplay1) + i] = lookup(Optiondisplay1[i]) + cue;
      }  
    show(OptDisplay1Addr);
}
DcsBios::StringBuffer<4> ufcOptionDisplay1Buffer(0x7432, onUfcOptionDisplay1Change);

void onUfcOptionDisplay2Change(char* Optiondisplay2) {

    int cue = 0;
    //clear(OptDisplay2Addr);
    for (int i = 0; i < strlen(Optiondisplay2); i++)
      {
        cue = OptDisplay2Buf[4-strlen(Optiondisplay2) + i] & 0b0100000000000000;
        OptDisplay2Buf[4-strlen(Optiondisplay2) + i] = lookup(Optiondisplay2[i]) + cue;
      }  
    show(OptDisplay2Addr);
}
DcsBios::StringBuffer<4> ufcOptionDisplay2Buffer(0x7436, onUfcOptionDisplay2Change);

void onUfcOptionDisplay3Change(char* Optiondisplay3) {

    int cue = 0;
    //clear(OptDisplay3Addr);
    for (int i = 0; i < strlen(Optiondisplay3); i++)
      {
        cue = OptDisplay3Buf[4-strlen(Optiondisplay3) + i] & 0b0100000000000000;
        OptDisplay3Buf[4-strlen(Optiondisplay3) + i] = lookup(Optiondisplay3[i]) + cue;
      }  
    show(OptDisplay3Addr);
}
DcsBios::StringBuffer<4> ufcOptionDisplay3Buffer(0x743a, onUfcOptionDisplay3Change);

void onUfcOptionDisplay4Change(char* Optiondisplay4) {

    int cue = 0;
    //clear(OptDisplay4Addr);
    for (int i = 0; i < strlen(Optiondisplay4); i++)
      {
        cue = OptDisplay4Buf[4-strlen(Optiondisplay4) + i] & 0b0100000000000000;
        OptDisplay4Buf[4-strlen(Optiondisplay4) + i] = lookup(Optiondisplay4[i]) + cue;
      }  
    show(OptDisplay4Addr);
}
DcsBios::StringBuffer<4> ufcOptionDisplay4Buffer(0x743e, onUfcOptionDisplay4Change);

void onUfcOptionDisplay5Change(char* Optiondisplay5) {

    int cue = 0;
    //clear(OptDisplay5Addr);
    for (int i = 0; i < strlen(Optiondisplay5); i++)
      {
        cue = OptDisplay5Buf[4-strlen(Optiondisplay5) + i] & 0b0100000000000000;
        OptDisplay5Buf[4-strlen(Optiondisplay5) + i] = lookup(Optiondisplay5[i]) + cue;
      }  
    show(OptDisplay5Addr);
}
DcsBios::StringBuffer<4> ufcOptionDisplay5Buffer(0x7442, onUfcOptionDisplay5Change);


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
  PCMSK2 = B11111111;      

  NOP; // Delay 1 clock cycle for synchronization

  //Setup Selection Display (16 Segment Displays)
  for (int i = 0; i < 5; i++)
  {
  Wire.begin();
  Wire.beginTransmission(0x70+i);
  Wire.write(0x20 | 1); // turn on oscillator
  Wire.endTransmission();
  setBrightness(0x70+i, 15);
  blink(0x70+i, 0);
  }
  
  clear(OptDisplay1Addr);
  show(OptDisplay1Addr);
  clear(OptDisplay2Addr);
  show(OptDisplay2Addr);
  clear(OptDisplay3Addr);
  show(OptDisplay3Addr);
  clear(OptDisplay4Addr);
  show(OptDisplay4Addr);
  clear(OptDisplay5Addr);
  show(OptDisplay5Addr);  
}

void loop() {
  DcsBios::loop();
  //UFCBrtPWM = map(UFCBrt, 0, 65535, 0, 255);
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


//Functions for 16 Segment Displays using HT16K33

int lookup(char chr){
   switch (chr)
   {
    case 'A':
    return 0b0000000011110111;
    break;
    case 'B':
    return 0b0001001010001111;
    break;
    case 'C':
    return 0b0000000000111001;
    break;
    case 'D':
    return 0b0001001000001111;
    break;
    case 'E':
    return 0b0000000011111001;
    break;
    case 'F':
    return 0b0000000011110001;
    break;
    case 'G':
    return 0b0000000010111101;
    break;
    case 'H':
    return 0b0000000011110110;
    break;
    case 'I':
    return 0b0001001000001001;
    break;
    case 'J':
    return 0b0000000000011110;
    break;
    case 'K':
    return 0b0010010001110000;
    break;
    case 'L':
    return 0b0000000000111000;
    break;
    case 'M':
    return 0b0000010100110110;
    break;
    case 'N':
    return 0b0010000100110110;
    break;
    case 'O':
    return 0b0000000000111111;
    break;
    case 'P':
    return 0b0000000011110011;
    break;
    case 'Q':
    return 0b0010000000111111;
    break;
    case 'R':
    return 0b0010000011110011;
    break;
    case 'S':
    return 0b0000000011101101;
    break;
    case 'T':
    return 0b0001001000000001;
    break;
    case 'U':
    return 0b0000000000111110;
    break;
    case 'V':
    return 0b0000110000110000;
    break;
    case 'W':
    return 0b0010100000110110;
    break;
    case 'X':
    return 0b0010110100000000;
    break;
    case 'Y':
    return 0b0000000011101110;
    break;
    case 'Z':
    return 0b0000110000001001;
    break;
    case '0':
    return 0b0000110000111111;
    break;
    case '1':
    return 0b0000010000000110;
    break;
    case '2':
    return 0b0000000011011011;
    break;
    case '3':
    return 0b0000000010001111;
    break;
    case '4':
    return 0b0000000011100110;
    break;
    case '5':
    return 0b0010000001101001;
    break;
    case '6':
    return 0b0000000011111101;
    break;
    case '7':
    return 0b0000000000000111;
    break;
    case '8':
    return 0b0000000011111111;
    break;
    case '9':
    return 0b0000000011101111;
    break;
    case '/':
    return 0b0000110000000000;
    break;
    case '-':
    return 0b0000000011000000;
    break;
    default:
    return 0x0000; 
    break;
   }
  }
 
void show(uint8_t DisplayAddr){
  Wire.beginTransmission(DisplayAddr);
  Wire.write(0x00); // start at address 0x0
 
  switch(DisplayAddr)
  {
    case OptDisplay1Addr:
      for (int i = 0; i < 8; i++) 
      {
        Wire.write(OptDisplay1Buf[i] & 0xFF); 
        Wire.write(OptDisplay1Buf[i] >> 8);    
      } 
    break;
    case OptDisplay2Addr:
      for (int i = 0; i < 8; i++) 
      {
        Wire.write(OptDisplay2Buf[i] & 0xFF); 
        Wire.write(OptDisplay2Buf[i] >> 8);    
      }
    break;
    case OptDisplay3Addr:
      for (int i = 0; i < 8; i++) 
      {
        Wire.write(OptDisplay3Buf[i] & 0xFF); 
        Wire.write(OptDisplay3Buf[i] >> 8);    
      }
    break;
    case OptDisplay4Addr:
      for (int i = 0; i < 8; i++) 
      {
        Wire.write(OptDisplay4Buf[i] & 0xFF); 
        Wire.write(OptDisplay4Buf[i] >> 8);    
      }
    break;
    case OptDisplay5Addr:
      for (int i = 0; i < 8; i++) 
      {
        Wire.write(OptDisplay5Buf[i] & 0xFF); 
        Wire.write(OptDisplay5Buf[i] >> 8);    
      }
    break; 
    default:
    break;
  }
  Wire.endTransmission(); 
}
 
void clear(uint8_t DisplayAddr){

  switch(DisplayAddr)
  {
    case OptDisplay1Addr:
      for(int i = 0; i < 8; i++)
      {
        OptDisplay1Buf[i] = 0;
      }
    break;
    case OptDisplay2Addr:
      for(int i = 0; i < 8; i++)
      {
        OptDisplay2Buf[i] = 0;
      }
    break;
    case OptDisplay3Addr:
      for(int i = 0; i < 8; i++)
      {
        OptDisplay3Buf[i] = 0;
      }
    break;
    case OptDisplay4Addr:
      for(int i = 0; i < 8; i++)
      {
        OptDisplay4Buf[i] = 0;
      }
    break;
    case OptDisplay5Addr:
      for(int i = 0; i < 8; i++)
      {
        OptDisplay5Buf[i] = 0;
      }
    break; 
    default:
    break;
  }
}
 
void setBrightness(uint8_t addr, uint8_t b){
  if(b > 15) return;
 
  Wire.beginTransmission(addr);
  Wire.write(0xE0 | b); // Dimming command
  Wire.endTransmission();
}
 
void blank(uint8_t addr){
  static boolean blankOn;  
 
  Wire.beginTransmission(addr);
  Wire.write(0x80 | blankOn); // Blanking / blinking command
  Wire.endTransmission();
 
  blankOn = !blankOn;
}
 
void blink(uint8_t addr, uint8_t b){
  if(b > 3) return;
 
  Wire.beginTransmission(addr);
  Wire.write(0x80 | b << 1 | 1); // Blinking / blanking command
  Wire.endTransmission();
}
