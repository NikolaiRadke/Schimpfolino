/*
  SSD1306_minimal.h - SSD1306 OLED Driver Library
  
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
               -- Reworked horrible formating and spelling
               -- Removed unused code
               -- Modified for Wire.h instead of TinyWireM.h
               -- Optimized Init-sequence and functions

      What is it?
        This library is derived from GOFi2cOLED library, only for SSD1306 in I2C Mode.
        As the original library only supports Frame Buffered mode which requires to have
        at least 1024bytes of free RAM for a 128x64px display it is too big for smaller devices.

        So this a SSD1306 library that works great with ATTiny45/85 devices :)

  It is a free software; you can redistribute it and/or modify it 
  under the terms of BSD license, check LICENSE for more information.
  All text above must be included in any redistribution.
*/

#ifndef __SSD1306_MINIMAL_H__
#define __SSD1306_MINIMAL_H__

#include <Arduino.h>
#include <Wire.h>

// Config
#define SlaveAddress 0x3c                        // SSD1306 I2C adress

// Schimpfolino: Count chars in main sketch, used to shrink font witdh if too long
extern uint8_t chars;                            

class SSD1306_Mini {
  public:
    // Call this function once to initiallize the display
    void init();
  
    // Reset clipArea to maximum and clear the display
    void clear();
    
    // Set the clipArea, by default (0, 0, 128, 8)
    void clipArea(unsigned char col, unsigned char row, unsigned char w, unsigned char h);
    
    // Move the cursor to a location (similar to clipArea)
    void cursorTo( unsigned char row, unsigned char col);
    
    // Print a single character
    void printChar(char ch);

    // Send a command - Schimpfolino needs to switch off the display
    void sendCommand(unsigned char command);

  private:

    // Set command mode
    void commandMode();

    // Set data mode
    void dataMode();

    // Send data
    void sendData(unsigned char Data);
};
#endif
