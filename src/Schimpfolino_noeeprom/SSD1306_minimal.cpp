/*
  SSD1306_minimal.cpp - SSD1306 OLED Driver Library
  
  Copyright (c) 2012, Adafruit Industries. All rights reserved.
  Copyright (c) 2012 GOF Electronics Co. Ltd ( http://www.geekonfire.com )
  Copyright (c) 2015 CoPiino Electronics All right reserved.
  Copyright (c) 2016 Kirk Northrop (github.com/kirknorthrop)
  Copytight (c) 2024 Nikolai Radke (http://www.monstermaker.de)
  
  Original Author: Limor Fried/Ladyada Adafruit Industries
  Modified by: Jimbo.we (www.geekonfire.com)
  Modified by: CoPiino Electronics (http://copiino.cc)
  Modified by: Kirk Northrop (github.com/kirknorthrop)
  Modified by: Nikolai Radke (www.monstermaker.de)

      CoPiino Electronics invests time and resources providing this open source code,
      please support CoPiino Electronics and open-source hardware by purchasing
      products from CoPiino Electronics!

      What is it?
        This library is derived from GOFi2cOLED library, only for SSD1306 in I2C Mode.
        As the original library only supports Frame Buffered mode which requires to have
        at least 1024bytes of free RAM for a 128x64px display it is too big for smaller devices.

        So this a SSD1306 library that works great with ATTiny85 devices :)

  It is a free software; you can redistribute it and/or modify it 
  under the terms of BSD license, check LICENSE for more information.
	All text above must be included in any redistribution.
*/

#include <TinyWireM.h>
#include <USI_TWI_Master.h>
#include "SSD1306_minimal.h"

// Reworked 5x8 font table with only used charakters
const unsigned char  BasicFont[] PROGMEM = {
  
  0x7C, 0x12, 0x11, 0x12, 0x7C, // A 0
  0x7F, 0x49, 0x49, 0x49, 0x36, // B 1
  0x3E, 0x41, 0x41, 0x41, 0x22, // C 2
  0x7F, 0x41, 0x41, 0x22, 0x1C, // D 3
  0x7F, 0x49, 0x49, 0x49, 0x41, // E 4
  0x7F, 0x09, 0x09, 0x09, 0x01, // F 5
  0x3E, 0x41, 0x49, 0x49, 0x7A, // G 6
  0x7F, 0x08, 0x08, 0x08, 0x7F, // H 7
  0x00, 0x41, 0x7F, 0x41, 0x00, // I 8
  0x20, 0x40, 0x41, 0x3F, 0x01, // J 9
  0x7F, 0x08, 0x14, 0x22, 0x41, // K 10
  0x7F, 0x40, 0x40, 0x40, 0x40, // L 11
  0x7F, 0x02, 0x0C, 0x02, 0x7F, // M 12
  0x7F, 0x04, 0x08, 0x10, 0x7F, // N 13
  0x3E, 0x41, 0x41, 0x41, 0x3E, // O 14
  0x7F, 0x09, 0x09, 0x09, 0x06, // P 15
  0x3E, 0x41, 0x51, 0x21, 0x5E, // Q 16
  0x7F, 0x09, 0x19, 0x29, 0x46, // R 17
  0x46, 0x49, 0x49, 0x49, 0x31, // S 18
  0x01, 0x01, 0x7F, 0x01, 0x01, // T 19
  0x3F, 0x40, 0x40, 0x40, 0x3F, // U 20
  0x1F, 0x20, 0x40, 0x20, 0x1F, // V 21
  0x3F, 0x40, 0x38, 0x40, 0x3F, // W 22
  0x63, 0x14, 0x08, 0x14, 0x63, // X 23
  0x07, 0x08, 0x70, 0x08, 0x07, // Y 24
  0x61, 0x51, 0x49, 0x45, 0x43, // Z 25
  0x1C, 0x3E, 0x7C, 0x3E, 0x1C, // ♥ 26
  0x20, 0x55, 0x54, 0x55, 0x78, // ä 27 
  0x38, 0x45, 0x44, 0x45, 0x38, // ö 28
  0x3C, 0x41, 0x40, 0x21, 0x7C, // ü 29
  0xFC, 0x02, 0x2A, 0x2A, 0x28, // ß 30
  0x00, 0x00, 0x00, 0x00, 0x00, //   31 unused
  0x20, 0x54, 0x54, 0x54, 0x78, // a 32 
  0x7F, 0x48, 0x44, 0x44, 0x38, // b 33
  0x38, 0x44, 0x44, 0x44, 0x20, // c 34
  0x38, 0x44, 0x44, 0x48, 0x7F, // d 35
  0x38, 0x54, 0x54, 0x54, 0x18, // e 36
  0x08, 0x7E, 0x09, 0x01, 0x02, // f 37
  0x18, 0xA4, 0xA4, 0xA4, 0x7C, // g 38
  0x7F, 0x08, 0x04, 0x04, 0x78, // h 39
  0x00, 0x44, 0x7D, 0x40, 0x00, // i 40
  0x40, 0x80, 0x84, 0x7D, 0x00, // j 41
  0x7F, 0x10, 0x28, 0x44, 0x00, // k 42
  0x00, 0x41, 0x7F, 0x40, 0x00, // l 43
  0x7C, 0x04, 0x18, 0x04, 0x78, // m 44
  0x7C, 0x08, 0x04, 0x04, 0x78, // n 45
  0x38, 0x44, 0x44, 0x44, 0x38, // o 46
  0xFC, 0x24, 0x24, 0x24, 0x18, // p 47
  0x18, 0x24, 0x24, 0x18, 0xFC, // q 48
  0x7C, 0x08, 0x04, 0x04, 0x08, // r 49
  0x48, 0x54, 0x54, 0x54, 0x20, // s 50
  0x04, 0x3F, 0x44, 0x40, 0x20, // t 51
  0x3C, 0x40, 0x40, 0x20, 0x7C, // u 52
  0x1C, 0x20, 0x40, 0x20, 0x1C, // v 53
  0x3C, 0x40, 0x30, 0x40, 0x3C, // w 54
  0x44, 0x28, 0x10, 0x28, 0x44, // x 55
  0x1C, 0xA0, 0xA0, 0xA0, 0x7C, // y 56
  0x44, 0x64, 0x54, 0x4C, 0x44, // z 57
};


unsigned char SSD1306_Mini::getFlash( const unsigned char * mem, unsigned int idx  ){
  unsigned char data= pgm_read_byte( &(mem[idx]) );
  return data;

}
 
void SSD1306_Mini::sendCommand(unsigned char command)
{
  TinyWireM.begin();                       //initialize I2C
  TinyWireM.beginTransmission(SlaveAddress); // begin I2C communication

  TinyWireM.send(GOFi2cOLED_Command_Mode);	     // Set OLED Command mode
  TinyWireM.send(command);

  TinyWireM.endTransmission();    		     // End I2C communication
}

void SSD1306_Mini::sendData(unsigned char Data)
{
  TinyWireM.begin();                    //initialize I2C
  TinyWireM.beginTransmission(SlaveAddress); // begin I2C transmission
  TinyWireM.send(GOFi2cOLED_Data_Mode);            // data mode
  TinyWireM.send(Data);
  TinyWireM.endTransmission();                    // stop I2C transmission
}

void SSD1306_Mini::init(uint8_t address)
{
  
 TinyWireM.begin();
 
 delay(5);	//wait for OLED hardware init
// constructor(128, 64);
 //SlaveAddress = address;

 sendCommand(GOFi2cOLED_Display_Off_Cmd);    /*display off*/

 sendCommand(Set_Multiplex_Ratio_Cmd);    /*multiplex ratio*/
 sendCommand(0x3F);    /*duty = 1/64*/

 sendCommand(Set_Display_Offset_Cmd);    /*set display offset*/
 sendCommand(0x00);


    sendCommand(Set_Memory_Addressing_Mode_Cmd); 	//set addressing mode
    sendCommand(HORIZONTAL_MODE); 			//set horizontal addressing mode

 sendCommand(0xB0); 			//set page address
 sendCommand(0x00); 	//set column lower address
 sendCommand(0x10); 	//set column higher address

    

 sendCommand(0x40);    /*set display starconstructort line*/

 sendCommand(Set_Contrast_Cmd);    /*contract control*/
 sendCommand(0xcf);    /*128*/

 sendCommand(Segment_Remap_Cmd);    /*set segment remap*/

 sendCommand(COM_Output_Remap_Scan_Cmd);    /*Com scan direction*/

 sendCommand(GOFi2cOLED_Normal_Display_Cmd);    /*normal / reverse*/

 sendCommand(Set_Display_Clock_Divide_Ratio_Cmd);    /*set osc division*/
 sendCommand(0x80);

 sendCommand(Set_Precharge_Period_Cmd);    /*set pre-charge period*/
 sendCommand(0xf1);

 sendCommand(Set_COM_Pins_Hardware_Config_Cmd);    /*set COM pins*/
 sendCommand(0x12);

 sendCommand(Set_VCOMH_Deselect_Level_Cmd);    /*set vcomh*/
 sendCommand(0x30);

 sendCommand(Deactivate_Scroll_Cmd);

 sendCommand(Charge_Pump_Setting_Cmd);    /*set charge pump enable*/
 sendCommand(Charge_Pump_Enable_Cmd);

 sendCommand(GOFi2cOLED_Display_On_Cmd);    /*display ON*/
}

void SSD1306_Mini::clipArea(unsigned char col, unsigned char row, unsigned char w, unsigned char h){
  
  TinyWireM.begin();                    //initialize I2C
  TinyWireM.beginTransmission(SlaveAddress); // begin I2C transmission
  TinyWireM.send(GOFi2cOLED_Command_Mode);            // data mode
  TinyWireM.send(Set_Column_Address_Cmd);
  TinyWireM.send(0);

  TinyWireM.send(col);
  TinyWireM.send(col+w-1);

  TinyWireM.endTransmission();                    // stop I2C transmission

  TinyWireM.begin();                    //initialize I2C
  TinyWireM.beginTransmission(SlaveAddress); // begin I2C transmission
  TinyWireM.send(GOFi2cOLED_Command_Mode);            // data mode
  TinyWireM.send(Set_Page_Address_Cmd);
  TinyWireM.send(0);

  TinyWireM.send(row); 
  TinyWireM.send(row+h-1);

  TinyWireM.endTransmission();                    // stop I2C transmission

}

void SSD1306_Mini::cursorTo(unsigned char col, unsigned char row){
  clipArea(col, row, 128-col, 8-row);
}

void SSD1306_Mini::startScreen(){
  
  sendCommand(0x00 | 0x0);  // low col = 0
  sendCommand(0x10 | 0x0);  // hi col = 0
  sendCommand(0x40 | 0x0); // line #0

}

void SSD1306_Mini::clear() {
  
  sendCommand(0x00 | 0x0);  // low col = 0
  sendCommand(0x10 | 0x0);  // hi col = 0
  sendCommand(0x40 | 0x0); // line #0
    
  clipArea(0,0,128,8);
  
    for (uint16_t i=0; i<=((128*64/8)/16); i++) 
    {
      // send a bunch of data in one xmission
      TinyWireM.beginTransmission(SlaveAddress);
      TinyWireM.send(GOFi2cOLED_Data_Mode);            // data mode
      for (uint8_t k=0;k<16;k++){
        TinyWireM.send( 0 );
      }
      TinyWireM.endTransmission();
    }
}


void SSD1306_Mini::displayX(int off) {
  sendCommand(0x00 | 0x0);  // low col = 0
  sendCommand(0x10 | 0x0);  // hi col = 0
  sendCommand(0x40 | 0x0); // line #0
    
    for (uint16_t i=0; i<=((128*64/8)/16); i++) 
    {
      // send a bunch of data in one xmission
      TinyWireM.beginTransmission(SlaveAddress);
      TinyWireM.send(GOFi2cOLED_Data_Mode);            // data mode
      for (uint8_t k=0;k<16;k++){
        TinyWireM.send((uint8_t) i*16 + k + off);
      }
      TinyWireM.endTransmission();
    }
}

void SSD1306_Mini::printChar( char ch ){
    
    char data[5];
    unsigned char i= ch;
    
    data[0]= getFlash(BasicFont, i*5 );
    data[1]= getFlash(BasicFont, i*5 + 1);
    data[2]= getFlash(BasicFont, i*5 + 2);
    data[3]= getFlash(BasicFont, i*5 + 3);
    data[4]= getFlash(BasicFont, i*5 + 4);    
    
    TinyWireM.beginTransmission(SlaveAddress);
    TinyWireM.send(GOFi2cOLED_Data_Mode);            // data mode
    
    TinyWireM.send( 0x00 );
    TinyWireM.send( data[0] );
    TinyWireM.send( data[1] );
    TinyWireM.send( data[2] );
    TinyWireM.send( data[3] );
    TinyWireM.send( data[4] );
    TinyWireM.send( 0x00 );
      
    TinyWireM.endTransmission();
  
}

void SSD1306_Mini::printString( char * pText ){
  unsigned char i;
  unsigned char len = strlen( pText );
  
  for (i=0;i<len;i++){
     printChar( pText[i] ); 
  }
  
}
 
 
void SSD1306_Mini::drawImage( const unsigned char * img, unsigned char col, unsigned char row, unsigned char w, unsigned char h ){
  unsigned int i,k,data;
  
  clipArea( col, row, w, h);
  
  for (i=0;i< (w*h);i++){

      data= getFlash( img, i);
              
      TinyWireM.beginTransmission(SlaveAddress);
      TinyWireM.send(GOFi2cOLED_Data_Mode);            // data mode

        TinyWireM.send((uint8_t) data );
      TinyWireM.endTransmission();
      
  }
  
}

