/* 
    TinyI2C v2.0.1

    David Johnson-Davies - www.technoblogy.com - 5th June 2022
    Modified for ATtiny85 8 MHz only for Schimpfolino by Nikolai Radke 
                         - www-monstermaker.de - 29th October 2024
  
    CC BY 4.0
    Licensed under a Creative Commons Attribution 4.0 International license: 
    http://creativecommons.org/licenses/by/4.0/
*/

#ifndef TinyI2CMaster_h
#define TinyI2CMaster_h

#include <stdint.h>
#include <Arduino.h>
#include <avr/io.h>
#include <util/delay.h>

class TinyI2CMaster {

public:
  void init(void);
  uint8_t read(void);
  bool write(uint8_t data);
  bool start(uint8_t address, uint8_t readcount);
  void stop(void);

private:
  uint8_t transfer(uint8_t data);
};

extern TinyI2CMaster TinyI2C;

#endif

