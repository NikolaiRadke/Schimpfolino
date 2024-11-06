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
                 -- Bold font by Sebastian Völke


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

// Config display
#define SlaveAddress 0x3c                        // SSD1306 I2C address
//#define bold                                   // Uncomment to use bold font

// Schimpfolino: count characters in main sketch, used to shrink font witdh if too long
extern uint8_t chars;                            

    // Call this function once to initialize the display
    void Oled_init(void);
  
    // Reset clipArea to maximum and clear the display
    void Oled_clear(void);
    
    // Move the cursor to a location (similar to clipArea)
    void Oled_cursorTo(uint8_t row, uint8_t col);
    
    // Print a single character
    void Oled_printChar(char ch);

    // Send a command | Schimpfolino needs to switch off the display (old boards)
    void Oled_sendCommand(uint8_t command);

    // Set command mode
    void Oled_commandMode(void);

    // Set data mode
    void Oled_dataMode(void);

    // Send data
    void Oled_sendData(uint8_t data);
#endif
