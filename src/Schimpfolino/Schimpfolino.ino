/*  
    Schimpfolino V1.0 05.07.2024 - Nikolai Radke
    https://www.monstermaker.de

    Sketch for the insulting gadget | Only with additional 24LCXX EEPROM
    For ATtiny45/85 - set to 8 Mhz | B.O.D disabled | No bootloader
    Remember to burn the "bootloader" first!

    Flash usage: 3.516 (IDE 2.3.2 | ATTinyCore 1.5.2 | Linux X86_64 | ATtiny85)
    Power:       5mA (idle) | 7μA (sleep)

    Umlaute have to be converted (UTF-8):
    ä -> # | ö -> $ | ü -> % | ß -> * | Captial letters are not supported
    Last charakter of a wordlist is '!'

    Wiring:
    1: RST | PB5  Free    
    2: D3  | PB3  Free
    3: A2  | PB4  Free 
    4: GND |      GND
    5: D0  | PB0  SDA   
    6: D1  | PB1  Button - GND
    7: D2  | PB2  SCL    
    8: VCC |      VCC
*/

#include <Wire.h>                                // I2C communication with display and EEPROM
#include <EEPROM.h>                              // Internal EEPROM saves random seed
#include <avr/sleep.h>                           // Used for deep sleep
#include <util/delay.h>                          // Less flash memory needed
#include "SSD1306_minimal.h"                     // Modified library!

// Hardware
#define  Button   PB1                            // Button address       

// Software
#define  Timeout  10000                          // 10 seconds before sleep
   
// Variables
uint8_t  gender;                                 // Gender of the swearword
uint8_t  chars = 0;                              // Number of characters in the word | Gobal
uint16_t number, seed;                           // Random seed and helping variable
uint16_t address[5];                             // Wordlists addresses array
uint32_t counter;                                // Timer begin for sleep timeout
char     wordbuffer[20];                         // Buffer for reading words

volatile bool wake = true;                       // Stay wake when button is pressed

SSD1306_Mini  oled;                              // Set display

int main(void) {                                 
  init(); {                                      // Setup
    // Power saving
    ACSR = (1 << ACD);                           // Disable analog comparator - anyway by default?
    ADCSRA &= ~(1 << ADEN);                      // Switch ADC off | saves 270uA

    // Port setup
    PORTB = 0x3f;                                // Set all Ports to INPUT_PULLUP to prevent floating

    // Hardware interrupt
    cli();                                       // Stop all interrupts
    GIMSK |= (1 << PCIE);                        // Turn on pin change interrupt
    PCMSK |= (1 << PCINT1);                      // Turn on interrupt on PB1 button
    sei();                                       // Start interrupts

    // Main routine - runs after waking up
    while(1) {
      // Init I2C
      Wire.setClock(400000L);                    // Fast mode
      Wire.begin();                              // Start I2C

      // Init Display
      oled.init();                               // Connect and start OLED via I2C

      // Read wordlist addresses
      gender = 0;                                // gender and seed are helping variables here
      for (seed = 0; seed < 5; seed ++) {        // Read numbers of 4 wordlists
        number = read_eeprom(0 + gender) * 255;  // Calculate number: 
        number += read_eeprom(1 + gender);       // First byte = high, second bye = low
        if (number == 0) wake = false;           // Sleep if no EEPROM or no wordlist present
        address[seed] = number;                  // Write word numbers to array 
        gender += 2;                             // Chance number address
      }

      // Randomize number generator
      number = eeprom_read_word(0);              // Read EEPROM address
      if ((number < 2) || (number > (EEPROM.length() - 3))) {
        // Initialize EEPROM and size for first use or after end of cycle
        number = 2;                              // Starting address
        eeprom_write_word(0, number);            // Write starting address
        eeprom_write_word(number, 0);            // Write seed 0
      }
      seed = eeprom_read_word(number);           // Read seed
      if (seed > 999) {                          // After 1000 write cyles move to another address
        seed = 0;                                // to keep the EEPROM alive
        number += 2;                             // 2 places, address is a word
        eeprom_write_word(0, number);            // Write address of seed
      }
      seed ++;                                   // New seed
      eeprom_write_word(number, seed);           // Save new seed for next startup
      randomSeed(seed);                          // Randomize number generator

      // Display swearwords until timeout
      while (wake) {                             // Wait 10 seconds timeout
        counter = millis();                      // Set starting time
        oled.clear();                            // Clear display buffer

        // First word
        number = (random(0, address[0]));        // Select first word
        get_swearword(number);                   // Read word from EEPROM
        write_swearword(2);                      // Write first word in the first line

        // Second word first part
        gender = random(0, 3);                   // Set word gender
        if (gender != 0) oled.printChar(48 + gender); // If male, write "r", if neutrum, write "s"
        number = (random(address[0], address[1])); // Select second word
        get_swearword(number);                   // Read second word from EEPROM
        
        // Second word second part
        number = (random(address[gender + 1], address[gender + 2])); // Select second part of second word
        get_swearword(number);                   // Read second part of second word
        write_swearword(4);                      // Write second word in second line
        
        // Wait for button or sleep
        _delay_ms(500);                          // Debounce button
        wake = false;                            // Set to sleep
        while ((!wake) && (millis() - counter < Timeout)); // Wait for button oder timeout
      } 

      // Go to sleep after 10 seconds if button is not pressed before                           
      oled.sendCommand(0xAE);                    // Display off and sleep
      set_sleep_mode(SLEEP_MODE_PWR_DOWN);       // Deepest sleep mode
      sleep_mode();                              // Good night, sleep until interrupt
    }
  }
}

// Functions
void get_swearword(uint16_t address) {           // Fetch characters from EEPROM
  char c;
  uint16_t i;
  address *= 10;
  for (i = address; i < address + 10; i ++) {    // Read 10 characters        
    c = read_eeprom(i+10);                       // from EEPROM with address memory offset
    if (c != 32) {                               // Check for space
      switch (c) {                               // Set german Umlaute   
        case 35: wordbuffer[chars] = 27; break;  // # -> ä
        case 36: wordbuffer[chars] = 28; break;  // $ -> ö
        case 37: wordbuffer[chars] = 29; break;  // % -> ü
        case 42: wordbuffer[chars] = 30; break;  // * -> ß
        default: wordbuffer[chars] = c - 65;     // Set non-empty character
      }
      chars ++;                                  // Increase number of fetched characters
    }
  } 
}

void write_swearword(uint8_t line) {             // Write centered word
  uint8_t x;
  x = (128 - (chars * 7)) / 2;                   // Calculate centering
  if (chars > 18)  x = (128 - (chars * 6)) / 2;  // or for very long words
  if ((gender != 0) && (line == 2)) x -= 4;      // If not female, set first one half block left for gender char
  oled.cursorTo(x, line);                        // Set cursor to selected line
  for (x = 0; x < chars; x ++)                   // Print the characters
    oled.printChar(wordbuffer[x]);               // from buffer
  chars = 0;                                     // Set number of character back to 0
}

uint8_t read_eeprom(uint16_t e_address) {        // Read from EEPROM
  Wire.beginTransmission(0x50);                  // open transmission to I2C-address 0x50
  Wire.write((uint16_t)(e_address >> 8));        // Send the MSB (Most Significant Byte) of the memory address
  Wire.write((uint16_t)(e_address & 0xFF));      // Send the LSB (Least Significant Byte) of the memory address
  Wire.endTransmission();                        // Close transmissiom
  Wire.requestFrom(0x50, 1);                     // Request one byte
  return Wire.read();                            // Read and return byte
}

ISR(PCINT0_vect) {wake = true;}                  // Interrupt routine. Set wake flag if button is pressed