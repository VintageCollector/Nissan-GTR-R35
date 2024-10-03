#ifndef __SHCUSTOMPROTOCOL_H__
#define __SHCUSTOMPROTOCOL_H__
#include "CRC8.h"
#include "stdint.h"

int RPM = 0;
int Speed = 0;
int Fuel;
int EngineTemp;
int Ignition;
int Ignition2;
int distanceCounter;
byte distanceCounterL;
byte distanceCounterH;
uint32_t timestamp100ms = 0;
uint32_t timestamp200ms = 0;
String gear;
byte AT_Gear = 0x26;
String Light;
int left;
int right;
int escontrol;
int ESC;
int pows = 240;
int val1;
CRC8 crc8Calculator;
uint8_t rpmCounter = 0;
uint8_t gearSelectorMessageCounter = 0;
 unsigned long dashboardUpdateTime10 = 10;
    unsigned long lastDashboardUpdateTime = 0;
static uint8_t count = 0x00;

typedef enum {
   L_BRAKE = 0b00000001,
  L_FOG = 0b00000100,
  L_BACKLIGHT = 0b00000001,
  L_MAIN = 0b00000100,
  L_DIP = 0b00000001,
} CAN_LIGHTS;

extern bool s_light_parking;
extern bool s_light_dip;
extern bool s_light_main;
extern bool s_light_fog;
extern bool s_handbrake;
extern bool s_esc;
bool s_handbrake = false;
bool s_light_main = false;
bool s_esc = false;
#define pinIgnition 5

unsigned short previousMillis = 0;

const short intervall = 20;
int S1, S2, S3, S4, S5, S6, S7;

#include <mcp_can.h>
#include <SPI.h>
#include <Wire.h>  
#include "stdint.h"
int turn;
int BREK;
int highbeam;

#define lo8(x) ((int)(x)&0xff)
#define hi8(x) ((int)(x)>>8)
#define CAN_INT 2 
MCP_CAN CAN(9);

bool s_ignition = true;
extern bool s_ignition;

class SHCustomProtocol {
private:

public:

 
  void setup() {
crc8Calculator.begin();
    if(CAN.begin(MCP_ANY, CAN_500KBPS, MCP_16MHZ) == CAN_OK) 
      CAN.setMode(MCP_NORMAL);

      pinMode(CAN_INT, INPUT);

      timestamp100ms = millis();
      timestamp200ms = millis();

       pinMode(pinIgnition, OUTPUT);
       digitalWrite(pinIgnition, LOW);
}

void CanSend3B(short address, byte a, byte b, byte c)
{
  unsigned char DataToSend[3] = {a, b, c};
  CAN.sendMsgBuf(address, 0, 3, DataToSend);
}

void CanSend(short address, byte a, byte b, byte c, byte d, byte e, byte f, byte g, byte h)
{
  unsigned char DataToSend[8] = {a, b, c, d, e, f, g, h};
  CAN.sendMsgBuf(address, 0, 8, DataToSend);
}

void CanSend9B(short address, byte a, byte b, byte c, byte d, byte e, byte f, byte g, byte h, byte i)
{
  unsigned char DataToSend[9] = {a, b, c, d, e, f};
  CAN.sendMsgBuf(address, 0, 9, DataToSend);
}

void CanSend4B(short address, byte a, byte b, byte c, byte d)
{
  unsigned char DataToSend[4] = {a, b, c};
  CAN.sendMsgBuf(address, 0, 9, DataToSend);

  

  
      
}


void CanSend5b(short address, byte a, byte b, byte c, byte d, byte e)
{
  unsigned char DataToSend[5] = {a, b, c, d, e};
  CAN.sendMsgBuf(address, 0, 5, DataToSend);
}


  // Called when new data is coming from computer
  void read() {
    // EXAMPLE 1 - read the whole message and sent it back to simhub as debug message
    // Protocol formula can be set in simhub to anything, it will just echo it
    // -------------------------------------------------------
    RPM = FlowSerialReadStringUntil(';').toInt();
    Speed = FlowSerialReadStringUntil(';').toInt();
    Ignition2 = FlowSerialReadStringUntil(';').toInt();
    left = FlowSerialReadStringUntil(';').toInt();
    right = FlowSerialReadStringUntil(';').toInt();
    gear = FlowSerialReadStringUntil(';');
    Light = FlowSerialReadStringUntil(';');
    EngineTemp = FlowSerialReadStringUntil('\n').toInt();
    
    
  
if (Ignition2 == 1)
        {
          Ignition = 1;
        }else{
          Ignition = 0;
        }

 if (Light.substring(0, 11) == "DL_FULLBEAM")
        {
          S1 = 13;
          s_light_main = true;
        }
        else
        {
          S1 = 0;
          s_light_main = false;
        } 

        if (Light.substring(S1, S1 + 12) == "DL_HANDBRAKE")
        {
          S2 = 14;
          s_handbrake = true;
        }
        else
        {
          S2 = 0;
          s_handbrake = false;
        }

if (Light.substring(S1 + S2, S1 + S2 + 5 ) == "DL_TC") //DIDNT WORK DONT UNSLASH MIGHT NEED LATER!!! 
        {
          ESC = 1;
          S3 = 7;
          s_esc = true;
        }
        else
        {
          ESC = 0;
          S3 = 0;
          s_esc = false;
        }


 if (left == 1 && right == 0) {
            turn = 0x28;
        }

        else if (left == 0 && right == 1)
        {
            turn = 0x48;
        }
        else if (left == 1 && right == 1)
        {
            turn = 0x68;
        }
        else
        {
            turn = 0x08;
            turn = 0x08;
        }

   
  }

   bool s_light_parking = true;
  bool s_light_dip = false;
  bool s_light_fog = false;
  uint8_t s_engine_temp = 0;
  uint8_t s_engine_oil_temp = 0;

  // Called once per arduino loop, timing can't be predicted, 
  // but it's called between each command sent to the arduino

void setDistance(){
  distanceCounter == 0 ? distanceCounter = Speed * 18 / 72 : distanceCounter = 0;
  distanceCounterL = distanceCounter & 0xff;
  distanceCounterH = distanceCounter >> 8;
}
  
  void loop() {


unsigned short currentMillis = millis();
  
      uint16_t value11 =  Speed*100;
   //Speedometer                     byte 5 change speed
  CanSend(0x284, count, count, value11, 0x00, (value11 >> 8), 0x00, 0x00, 0x00);
  
   //  uint16_t value = RPM * 4;

//CanSend(0x0f3, 0x00, (int(RPM * 1.557) & 0xff), (int(RPM * 1.557) >> 8), 0x00, 0x00, 0x00, 0x00, 0x00);

 uint16_t value = RPM *8;

CanSend(0x180, (value >>8), 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00); 


// Gear indicator for numbers
if (gear.equalsIgnoreCase("1")) {
  AT_Gear = 0x80;
} else if (gear.equalsIgnoreCase("2")) {
  AT_Gear = 0x8A;
} else if (gear.equalsIgnoreCase("3")) {
  AT_Gear = 0x90;
} else if (gear.equalsIgnoreCase("4")) {
  AT_Gear = 0x9A;
} else if (gear.equalsIgnoreCase("5")) {
  AT_Gear = 0xA0;
} else if (gear.equalsIgnoreCase("6")) {
  AT_Gear = 0xAA;
} else if (gear.equalsIgnoreCase("7")) {
  AT_Gear = 0xB0;
} else if (gear.equalsIgnoreCase("8")) {
  AT_Gear = 0xBA;
} else if (gear.equalsIgnoreCase("R")) {
  AT_Gear = 0x10;
} else if (gear.equalsIgnoreCase("N")) {
  AT_Gear = 0x1A;
  } else if (gear.equalsIgnoreCase("P")) {
  AT_Gear = 0x0A;
}



   if(Ignition == 1){
  digitalWrite(pinIgnition, HIGH); 
 //Ignition
  CanSend(0x12F, 0xFB, count, 0x8a, 0x1C, 0xF1, 0x05, 0x30, 0x86);
  }else{
  digitalWrite(pinIgnition, LOW); 
  //Ignition
  CanSend(0x12F, 0xFB, count, 0x00, 0x1C, 0xF1, 0x05, 0x30, 0x86);
  }


  
   count++;
  if (count == 0x00)
  {
    count = 0xFF;
    count++;
  }

 //////////////////////////////////

 //Power Steering
CanSend5b(0x2A7, val1, pows, 0xFE, 0xFF, 0x14); 

pows++;
  if (pows == 255)
  {
    val1 = 
    pows = 240;
  }
  if (pows == 254)
  {
    val1 = 122;
  }
  if (pows == 253)
  {
    val1 = 93;
  }
  if (pows == 252)
  {
    val1 = 169;
  }
  if (pows == 251)
  {
    val1 = 52;
  }
  if (pows == 250)
  {
    val1 = 142;
  }
  if (pows == 249)
  {
    val1 = 19;
  }
  if (pows == 248)
  {
    val1 = 123;
  }
  if (pows == 247)
  {
    val1 = 122;
  }
  if (pows == 246)
  {
    val1 = 230;
  }
  if (pows == 245)
  {
    val1 = 92;
  }
  if (pows == 244)
  {
    val1 = 193;
  }
  if (pows == 243)
  {
    val1 = 53;
  }
  if (pows == 242)
  {
    val1 = 168;
  }
  if (pows == 241)
  {
    val1 = 18;
  }
  if (pows == 240)
  {
    val1 = 143;
  }
 

         /// oil temp
    //CanSend(0x288, EngineTemp+78, EngineTemp+78, 00, 0x00, 00, 0x00, 0x00, 0x00);

     //oiltemp
  CanSend(0x3f9, 0x02, count, count, 0x00, 0x00, 148, count, count);

  
//backlight
  CanSend(0x635, 0x64 ,  0x64, 0x64 , 0x64 , 0x64 , 0x64 , 0x64 , 0x64 );


// AT Display
  CanSend(0x421, AT_Gear, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0X00); 
          
//drive modes        byte 5
 // CanSend(0x00, 0x00, count, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00);
  
 //Fuel
 // CanSend(0x000, 0x00, 0x01, 0xBE, 0x0A, 0x00, 0x00, 0x00, 0x00);

  int turns;
 turns = turn;

 
 uint16_t lights = 0;
  if(s_light_main) lights |= L_MAIN;

  //lights
  CanSend(0x60d, 0x00, turns, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
  //byte 2 turn signals & Hi beam: 

   //Temp gauge + Service engine + Cruise Control
  CanSend(0x551, EngineTemp+42, 0x00, 0x00, Speed, 0x10, 0x00, 0x00, 0x00);

   //ABS 1
  CanSend(0x36E, 0x00, count, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);

   //ABS 2
  CanSend(0xB6E, count, count, count, count, count, count, count, count);

  //airbag
  CanSend(0xD7, count, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);

   //Lights
  CanSend(0x21A, 0x05, 0x12, 0xF7, 0x00, 0x00, 0x00, 0x00, 0x00);
  

 //Transmission
  uint8_t transmissionBuff[8] = { 0x00, gearSelectorMessageCounter, AT_Gear, 0x00, 0x00, 0x00, 0x00, 0x00 };
  CanSend(0x3fd, crc8Calculator.get_transmission_crc8(0, &transmissionBuff[1], 4), gearSelectorMessageCounter, AT_Gear, 0x00, 0x00, 0x00, 0x00, 0x00); //0x20= P, 0x40= R, 0x60= N, 0x80= D, 0x81= DS
  
 gearSelectorMessageCounter++;
  if (gearSelectorMessageCounter >= 15) { gearSelectorMessageCounter = 0; }
  
 rpmCounter++;
  if (rpmCounter >= 0x15) { rpmCounter = 0x00; }


    
  }
  
  
  void idle() {
  }
};


#endif
