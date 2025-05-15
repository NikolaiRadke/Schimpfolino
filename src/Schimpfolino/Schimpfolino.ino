/*  
    Schimpfolino V1.01 15.05.2025 - Nikolai Radke
    https://www.monstermaker.de

    Sketch for the insulting gadget | Only with additional 24AAXXX EEPROM
    For ATtiny45/85 - set to 8 MHz | B.O.D disabled | No bootloader
    Remember to burn the "bootloader" (IDE is setting fuses) first!

    Flash usage: 3.170 bytes (IDE 2.3.6 | ATTinyCore 1.5.2 | Linux X86_64 | ATtiny85)
    Power:       1.7 mA (display on, EEPROM on) | ~ 200 nA (sleep)

    Umlaute have to be converted (UTF-8):
    ä -> # | ö -> $ | ü -> % | ß -> * | Ä -> & | Ö -> ' | Ü -> (
    Last character of a wordlist is "!" 

    Wiring:
                         +-\/-+
    RST          - PB5  1|    |8  VCC - Battery
    Free         - PB3  2|    |7  PB2 - SCL
    Devices VCC  - PB4  3|    |6  PB1 - Button -> GND
    GND          - GND  4|    |5  PB0 - SDA
                         +----+
*/

#include <avr/sleep.h>                           // Used for deep sleep
#include <util/delay.h>                          // Needs less flash memory than delay()
#include "SSD1306_minimal.h"                     // Modified library!
#include <Wire.h>                                // I2C communication with display and EEPROM

// Hardware
#define  BUTTON   PB1                            // Button pin    
#define  DEVICES  PB4                            // External devices power pin

// Variables
uint8_t  chars = 0;                              // Number of characters in the word | Gobal
uint16_t addresses[5];                           // Wordlists addresses array
char     wordbuffer[20];                         // Buffer for reading words

volatile bool awake = false;                     // Stay awake when button is pressed

SSD1306_Mini  oled;                              // Set display

int main(void) {                                 
  // Power saving
  ADCSRA &= ~(1 << ADEN);                        // Switch ADC off | Saves 270 uA
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);           // Always deepest sleep mode

  // Port setup
  DDRB  |= (1 << DEVICES);                       // Set PB4 to OUTPUT to power up display and EEPROM
  PORTB = 0x3F;                                  // Set all ports to INPUT_PULLUP to prevent floating and start devices
    
  // Hardware pin and watchdog interrupt
  cli();                                         // Stop all interrupts. An interrupt can ruin the timed sequence
  GIMSK |= (1 << PCIE);                          // Turn on pin change interrupt
  PCMSK |= (1 << PCINT1);                        // Turn on interrupt on PB1 button
  MCUSR &= ~(1 << WDRF);                         // No watchdog reset 
  WDTCR |= (1 << WDCE) | (1 << WDE);             // Watchdog change enable
  WDTCR = (1 << WDP0) | (1 << WDP3);             // Set prescaler to 8 s
  sei();                                         // Start interrupts

  // Init I2C  
  Wire.setClock(400000L);                        // Fast mode (400 kHz)
  Wire.begin();                                  // Start I2C

  // Read wordlist addresses
  for (uint8_t list = 0; list < 5; ++ list)      // Read numbers of 5 wordlists
    addresses[list] = (read_eeprom(list * 2) * 255) + (read_eeprom((list * 2) +1)); // Write word numbers to array

  // Randomize number generator
  PORTB &= ~(1 << DEVICES);                      // Devices off
  sleep_mode();                                  // Sleep until button is pressed to "turn on"
  while (!(PINB & (1 << BUTTON)));               // Wait until button is released
  randomSeed(millis());                          // Time passed is used for random numbers

  // Main routine | Runs after waking up
  while(1) {
    // Init Display
    PORTB |= (1 << DEVICES);                     // Devices on
    PRR |= (1 << PRTIM0) | (1 << PRTIM1);        // Both timers are not needed anymore | Saves 100 uA when active
    oled.init();                                 // Connect and start OLED via I2C

    // Display swearwords until timeout
    while (awake) {                              // Wait 8.5 seconds timeout
      oled.clear();                              // Clear display buffer

      // First word
      get_swearword(random(0, addresses[0]));    // Read first word from EEPROM
      uint8_t genus = random(0, 3);              // Set word genus
      if (genus != 0) {                          // Check if not female
        wordbuffer[chars] = 48 + genus;          // If male, add "r", if neutrum, add "s" to buffer
        ++ chars;                                // Increase number of characters
      } 
      write_swearword(2);                        // Write first word in the first line

      // Second word first part
      get_swearword(random(addresses[0], addresses[1])); // Read second word from EEPROM
        
      // Second word second part
      get_swearword(random(addresses[genus + 1], addresses[genus + 2])); // Read second part of second word
      write_swearword(4);                        // Write second word in second line
        
      // Wait for button and sleep 8s
      _delay_ms(500);                            // Debounce button
      awake = false;                             // Set to sleep     
      WDTCR |= (1 << WDIE);                      // Set watchdog interrupt
      sleep_mode();                              // Sleep 8 s or wake when button is pressed
      WDTCR &= ~(1 << WDIE);                     // Stop watchdog interrupt
    } 

    // Go to sleep after 8 seconds if button is not pressed before                           
    oled.sendCommand(0xAE);                      // Display off and sleep (old boards)
    PORTB &= ~(1 << DEVICES);                    // Devices off   
    sleep_mode();                                // Sleep until button is pressed
  }
}

// Functions
void get_swearword(uint16_t address) {           // Fetch characters from EEPROM
  address *= 10;                                 // Each address has 10 characters
  for (uint8_t i = 0; i < 10; i++) {             // Read 10 characters
    char c = read_eeprom(address + i + 10);      // From EEPROM with offset
    if (c != 32) {                               // Skip space
      // Set German Umlaute or character
      switch (c) {
        case 35: c = 27; break;                  // # -> ä
        case 36: c = 28; break;                  // $ -> ö
        case 37: c = 29; break;                  // % -> ü
        case 38: c = 58; break;                  // & -> Ä
        case 39: c = 59; break;                  // ' -> Ö
        case 40: c = 60; break;                  // ( -> Ü
        case 42: c = 30; break;                  // * -> ß
        default: c -= 65;                        // Set non-empty character
      }
      wordbuffer[chars ++] = c;                  // Store and increment counter
    }
  } 
}

void write_swearword(uint8_t line) {             // Write centered word
  uint8_t x = (128 - (chars * 7)) / 2;           // Calculate centering
  if (chars > 17) x = (128 - (chars * 6)) / 2;   // Modify for very long words
  oled.cursorTo(x, line);                        // Set cursor to selected line
  for (x = 0; x < chars; ++ x)                   // Print the characters...
    oled.printChar(wordbuffer[x]);               // ...from buffer
  chars = 0;                                     // Set number of characters back to 0 
}

uint8_t read_eeprom(uint16_t e_address) {        // Read from EEPROM
  Wire.beginTransmission(0x50);                  // Open transmission to I2C-address 0x50
  Wire.write(e_address >> 8);                    // Send the MSB (Most Significant Byte) of the memory address
  Wire.write(e_address & 0xFF);                  // Send the LSB (Least Significant Byte) of the memory address
  Wire.endTransmission();                        // Close transmission
  Wire.requestFrom(0x50, 1);                     // Request one byte
  return Wire.read();                            // Read and return byte
}

ISR(PCINT0_vect) {                               // Interrupt routine for pin change 
  awake = true;                                  // Set awake flag when button is pressed
}

ISR(WDT_vect) {}                                 // Interrupt routine for watchdog. Unused but mandatory                               