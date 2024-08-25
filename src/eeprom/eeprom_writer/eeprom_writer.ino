/*
    Schimpfolino EEPROM writer V1.0 27.06.2024 - Nikolai Radke
    EEPROMS bigger than 512 kBit (64 kB) are NOT supported.
    24AAXXX supports page writing, but is unused here.
    Works with 24LCXXX too, if no 24AAXXX are available.
  
    This sketch writes the wordlist into 24AAXXX (maximum 24AA512) EEPROM.
    No code and power optimizations were used for better readability.
    Umlaute must be converted for UTF-8, only first 128 characters are supported.
    # = ä, $ = ö, % = ü, * = ß. Capitals are not supported.
    Last character of each file must be !.
 
    Wordlist addresses in EEPROM:
    0 + 1: eeprom1.txt | 2 + 3: eeprom2.txt | 4 + 5: eeprom3.txt | 6 + 7: eeprom4.txt | 8 + 9: eeprom5.txt
    See README in folder src/eeprom/.

    Wiring:
            +-V--+
    GND - 1 |    | 8 - VCC (3.3 V)
    GND - 2 |    | 7 
    GND - 3 |    | 6 - SCL (A5)
    GND - 4 |    | 5 - SDA (A4) (- 4k7 Resistor - VCC)
            +----+
    The 4k7 resistor is optional, see src/eeprom/README.md
*/

#include <Wire.h>                                // I2C Library

#define eeprom  0x50                             // 24AAXXX address

uint16_t address = 10;                           // Starting adress
uint16_t words = 0;                              // Count words
uint8_t  low, c;                                 // Helping variables
uint8_t  file = 0;                               // Count EEPROM text files

// Open connection to terminal and EEPROM
void setup() {
  Serial.begin(9600);                            // Start serial connection to terminal
  Wire.begin();                                  // Start I2C connection
  Serial.print("Looking for EEPROM... ");
  while(1) {                                     // Wait for EEPROM connection
    Wire.beginTransmission(0x50);                // Look for 24AAXXX EEPROM at 0x50
    if (Wire.endTransmission() == 0x00) break;   // 0x00 for available, 0xFF for not found
    delay(100);
  }
  Serial.println("found!");
  Serial.println("Select file to send");         // Ready to flash
}

void loop() {
  // Main loop waits for one byte and perfoms checks before writing
  while(Serial.available() == 0);                // Wait for serial data
  c = Serial.read();                             // Read character from stream
  low = c & 0x00FF;                              // Take only LSB (Least Significant Byte)
  if (!((low == 0xC3) || (low == 0x0A) || (low == 0x21))) { // Write if not "!"
    write_byte(address, low);                    // Write byte to EEPROM
    address ++;                                  // Next address
  }

  // End of file found?
  if (c == 33) {                                 // "!" marks end of file
    words = (address - 10) / 10;                 // Count words of 10 bytes
    delay(10);
    write_byte(0 + file, words / 255);           // Write high byte of batch number
    delay(10);
    write_byte(1 + file, words % 255);           // Write low byte of batch number
    delay(10);
    Serial.print("File ");                       // Print number of file
    Serial.print(1 + file / 2);
    Serial.print("/5: ");                        // Number of 5 (eeprom1.txt to eeprom5.txt)
    Serial.print(address - 10);                  
    Serial.print(" characters in ");             // Print number of characters written
    Serial.print(words);
    Serial.println(" words written.");           // Print number of words written
    if (file == 8) {                             // After 5 files send, print finish message
      Serial.println("Done.");
      address = 10;
      file = 0;                                  // Reset counter variables for new writing process
    }
    else {
      Serial.println("Waiting for next file.");  // Ready for next file
      file += 2;                                 // Increase file counter
    }
  }
}

void write_byte(uint16_t address, uint8_t data) { // Writes one byte to an address
  Wire.beginTransmission(eeprom);                // Beginn transmission to EEPROM
  Wire.write((uint16_t)(address >> 8));          // Send the MSB (Most Significant Byte) of the memory address
  Wire.write((uint16_t)(address & 0xFF));        // Send the LSB (Least Significant Byte) of the memory address
  Wire.write(data);                              // Write character to EEPROM
  Wire.endTransmission();                        // Close transmission
  delay(1);                                      // Wait. EEPROMs are kind of slow
}

