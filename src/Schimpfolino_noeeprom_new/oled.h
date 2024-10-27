/*
    oled.h - SSD1306 and SH1106 OLED driver library
    based on SSD1306_minimal.h

    Copyright (c) 2012 Adafruit Industries. All rights reserved
    Copyright (c) 2012 GOF Electronics Co. Ltd (http://www.geekonfire.com)
    Copyright (c) 2015 CoPiino Electronics. All rights reserved
    Copyright (c) 2016 Kirk Northrop (github.com/kirknorthrop)
    Copyright (c) 2024 Nikolai Radke (http://www.monstermaker.de)
  
    Original Author: Limor Fried/Ladyada Adafruit Industries
    Modified by: Jimbo.we (www.geekonfire.com)
    Modified by: CoPiino Electronics (http://copiino.cc)
    Modified by: Kirk Northrop (github.com/kirknorthrop)
    Modified by: Nikolai Radke (www.monstermaker.de)
                 -- Reworked formatting and spelling
                 -- Removed unused code
                 -- Modified for TinyI2C.h instead of TinyWireM.h
                 -- Optimized init sequence and functions
                 -- SH1106 support by Sebastian Völke

    What is it?
      This library is derived from GOFi2cOLED library, only for SSD1306 in I2C Mode.
      As the original library only supports frame buffered mode which requires to have
      at least 1024 bytes of free RAM for a 128 x 64 px display it is too big for smaller devices.

      So this a SSD1306/SH1106 library that works great with ATtiny45/85 devices :)

    It is a free software; you can redistribute it and/or modify it under the terms of 
    BSD license, check LICENSE for more information. All text above must be included in 
    any redistribution.
*/

#ifndef __OLED_H__
#define __OLED_H__

#include <Arduino.h>                             // Arduino and ATtiny specific library

// Config display | For SH1106 comment OLED_CS_SSD!06 and uncomment OLED_CS_SH1106
#define SlaveAddress 0x3c                        // SSD1306 I2C address
#define OLED_CS_SSD1306                          // Most 0.96" OLED displays 
//#define OLED_CS_SH1106                         // Most 1.30" OLED displays

// Schimpfolino: count characters in main sketch, used to shrink font witdh if too long
extern uint8_t chars;                            

class Oled {
  public:
    // Call this function once to initialize the display
    void init();
  
    // Reset clipArea to maximum and clear the display
    void clear();
    
    // Set the clipArea, by default (0, 0, 128, 8)
    void clipArea(uint8_t col, uint8_t row, uint8_t w, uint8_t h);
    
    // Move the cursor to a location (similar to clipArea)
    void cursorTo(uint8_t row, uint8_t col);
    
    // Print a single character
    void printChar(char ch);

    // Send a command | Schimpfolino needs to switch off the display (old boards)
    void sendCommand(uint8_t command);

  private:
    // Set command mode
    void commandMode();

    // Set data mode
    void dataMode();

    // Send data
    void sendData(uint8_t data);
};
#endif
