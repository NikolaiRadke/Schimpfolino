/*
 * Schimpfolino EEPROM writer V1.0 09.05.2024 - Nikolai Radke
 * EEPROMS bigger than 512 kBit are NOT supported.
 * 24LCXX supports page writing, but is unused here.
 * 
 * This sketch writes the wordlist into 24LCXXX eeprom.
 * Umlaute must be converted for UTF-8.
 * #=ä, $=ö, %=ü, *=ß. Capitals are not supported.
 * Last character of each file must be !.
 *
 * Wordlist addresses:
 * 0+1: eeprom1.txt | 2+3: eeprom2.txt | 4+5: eeprom3.txt | 6+7: eeprom4.txt
 * 
 * See README in folder eeprom.
*/

#include <Wire.h>

#define eeprom  0x50                             // 24LC256 address

uint16_t address = 10;                            // Starting adress
uint16_t words = 0;
uint8_t  low, c;                                 // Helping variables
uint8_t  file = 0;

void setup() {
  Serial.begin(9600);                            // Start serial connection
  Wire.begin();                                  // Connect ot EEPROM via I2C
  Serial.println("Select file to send");
  delay(100);
}

void loop() {
  while(Serial.available() ==0);                 // Wait for serial data
  c = Serial.read();                             // Read charakter from stream
  low = c & 0x00FF;                              // Take only LSB (Least Significant Byte)
  if (!((low == 0xC3) || (low == 0x0A) || (low == 0x21))) {
    write_byte(address, low);                    // Write byte to EEPROM
    address ++;                                  // Next adress
  }
  if (c == 33) {
    words = (address - 10) / 10;
    delay(10);
    write_byte(0 + file, words / 255);                // Write high byte of batch number
    delay(10);
    write_byte(1 + file, words % 255);                // Write low byte of batch number
    delay(10);
    Serial.print("File ");
    Serial.print(1 + file / 2);
    Serial.print("/5: ");
    Serial.print(address - 10);
    Serial.print(" characters in ");
    Serial.print(words);
    Serial.println(" words written.");
    if (file == 8) {
      Serial.println("Done.");
      address = 10;
      file = 0;
    }
    else {
      Serial.println("Waiting for next file...");
      file += 2;
    }
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

