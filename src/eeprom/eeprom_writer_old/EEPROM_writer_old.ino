/*
 * Schimpfolino EEPROM writer V1.0 06.05.2024 - Nikolai Radke
 * EEPROMS bigger than 256kBit are NOT supported.
 * 24LCXX supports page writing, but is unused here.
 * 
 * This sketch writes wordlist into 24LCXXX eeprom.
 * Umlaute must be converted for UTF-8.
 * #=ä, $=ö, %=ü, *=ß. Capitals are not supported.
 * 
 * See README in folder write_eeprom.
*/

#include <Wire.h>

#define batches 140                              // Number of 4 words x 10 bytes batches
#define eeprom  0x50                             // 24LC256 address
//define nerdmode                                // Use own serial writer

uint16_t addr = 2;                               // Starting adress
uint8_t  low, c;                                 // Helping variables

void setup() {
  Serial.begin(9600);                            // Start serial connection
  Wire.begin();                                  // Connect ot EEPROM via I2C
  delay(100);
  #ifdef nerdmode
    addr = 0;
  #else
    write_byte(0, batches / 255);                // Write high byte of batch number
    write_byte(1, batches % 255);                // Write low byte of batch number
  #endif
}

void loop() {
  while(Serial.available() ==0 );                // Wait for serial data
  c = Serial.read();                             // Read charakter from stream
  low = c & 0x00FF;                              // Take only LSB (Least Significant Byte)
  if (low != 0xC3) {
    write_byte(addr, low);                       // Write byte to EEPROM
    addr ++;                                     // Next adress
  }
}

void write_byte(uint16_t address, uint8_t data) {
  Wire.beginTransmission(eeprom);                // Beginn transmission to EEPROM
  Wire.write((uint16_t)(address >> 8));          // Send the MSB (Most Significant Byte) of the memory address
  Wire.write((uint16_t)(address & 0xFF));        // Send the LSB (Least Significant Byte) of the memory address
  Wire.write(data);                              // Write charakter to EEPROM
  Wire.endTransmission();                        // Close transmission
  delay(1); // EEPROMs are kind of slow
}

