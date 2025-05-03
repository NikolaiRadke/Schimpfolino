/*  
    Schimpfolino V1.3 15.04.2025 - Nikolai Radke
    https://www.monstermaker.de
    Optimiert für minimalen Flash-Verbrauch

    Sketch for the insulting gadget | Only with additional 24AAXXX EEPROM
    For ATtiny45/85 - set to 8 MHz | B.O.D disabled | No bootloader | No millis()
    
    Umlaute have to be converted (UTF-8):
    ä -> # | ö -> $ | ü -> % | ß -> * | Ä -> & | Ö -> ' | Ü -> (
    Last character of a wordlist is "!" 
*/

#include <util/delay.h>                          // Needs less flash memory than delay()
#include "TinyI2CMaster.h"                       // I2C communication with display and EEPROM
#include "oled.h"                                // OLED display library for SSD1306 and SH1106

// Hardware
#define BUTTON   PB1                             // Button pin    
#define DEVICES  PB4                             // External devices power pin

// EEPROM constants
#define EEPROM_ADDR 0x50                         // EEPROM I2C address

// Variables
uint8_t genus;                                   // Genus of the swearword
uint8_t chars;                                   // Number of characters in the word
uint16_t addresses[5];                           // Wordlists addresses array
char wordbuffer[20];                             // Buffer for reading words

volatile bool awake;                             // Stay awake when button is pressed

// Function prototypes
void get_swearword(uint16_t address);
void write_swearword(uint8_t line);
uint8_t read_eeprom(uint16_t e_address);
void sleep(void);

ISR(PCINT0_vect) {                               // Interrupt routine for pin change 
  awake = true;                                  // Set awake flag when button is pressed
}

ISR(WDT_vect) {}                                 // Empty interrupt routine for watchdog

int main(void) {                                 
  // Power saving
  ADCSRA &= ~(1 << ADEN);                        // Switch ADC off | Saves 270 uA
  MCUCR = (1 << SM1);                            // Power-down sleep mode

  // Port setup
  DDRB = (1 << DEVICES) | (1 << SDA) | (1 << SCL); // Set outputs
  PORTB = 0x3F;                                  // Set pullups on inputs
  
  // Hardware pin and watchdog interrupt
  cli();                                         // Stop all interrupts
  GIMSK |= (1 << PCIE);                          // Turn on pin change interrupt
  PCMSK |= (1 << PCINT1);                        // Turn on interrupt on PB1 button
  MCUSR &= ~(1 << WDRF);                         // No watchdog reset 
  WDTCR |= (1 << WDCE) | (1 << WDE);             // Watchdog change enable
  WDTCR = (1 << WDP0) | (1 << WDP3);             // Set prescaler to 8 s
  sei();                                         // Start interrupts

  // Init I2C
  TinyI2C.init();                                // Start I2C

  // Read wordlist addresses
  for (uint8_t list = 0; list < 5; list++)       // Read numbers of 5 wordlists
    addresses[list] = (read_eeprom(list * 2) * 255) + read_eeprom((list * 2) + 1);

  // Randomize number generator
  PORTB &= ~(1 << DEVICES);                      // Devices off
  sleep();                                       // Sleep until button is pressed to "turn on"
  TCCR0A = 0;                                    // Set timer 0 to normal mode
  TCCR0B = (1 << CS00);                          // Set prescaler to 1 to start the timer
  while (!(PINB & (1 << BUTTON)));               // Wait until button is released
  randomSeed(TCNT0);                             // Get a time count as seed

  // Main routine
  for(;;) {
    // Init Display
    PORTB |= (1 << DEVICES);                     // Devices on
    PRR |= (1 << PRTIM0) | (1 << PRTIM1);        // Disable timers to save power
    Oled_init();                                 // Init OLED display

    // Display swearwords until timeout
    while (awake) {                              // Wait 8.5 seconds timeout
      Oled_clear();                              // Clear display buffer

      // First word
      get_swearword(random(0, addresses[0]));    // Read word from EEPROM
      genus = random(0, 3);                      // Set word genus
      if (genus) {                               // Check if not female (0)
        wordbuffer[chars] = 48 + genus;          // Add "r" or "s" to buffer
        chars++;
      } 
      write_swearword(2);                        // Write first word in the first line
      
      // Second word
      get_swearword(random(addresses[0], addresses[1])); // First part
      get_swearword(random(addresses[genus + 1], addresses[genus + 2])); // Second part
      write_swearword(4);                        // Write second word in second line
      
      // Wait for button and sleep 8s
      _delay_ms(500);                            // Debounce button
      awake = false;                             // Set to sleep     
      WDTCR |= (1 << WDIE);                      // Set watchdog interrupt
      sleep();                                   // Sleep 8s or wake when button is pressed
      WDTCR &= ~(1 << WDIE);                     // Stop watchdog interrupt
    } 

    // Go to sleep after timeout                           
    Oled_sendCommand(0xAE);                      // Display off
    PORTB &= ~(1 << DEVICES);                    // Devices off   
    sleep();                                     // Sleep until button is pressed
  }
}

void get_swearword(uint16_t address) {           // Fetch characters from EEPROM
  char c;
  address *= 10;                                 // Each address has 10 characters
  
  for (uint8_t i = 0; i < 10; i++) {             // Read 10 characters
    c = read_eeprom(address + i + 10);           // From EEPROM with offset
    
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
      wordbuffer[chars++] = c;                   // Store and increment counter
    }
  } 
}

void write_swearword(uint8_t line) {             // Write centered word
  uint8_t x = (128 - (chars * 7)) / 2;           // Calculate centering
  
  if (chars > 17)                                // Modify for very long words
    x = (128 - (chars * 6)) / 2;
    
  Oled_cursorTo(x, line);                        // Set cursor
  
  for (uint8_t i = 0; i < chars; i++)            // Print characters from buffer
    Oled_printChar(wordbuffer[i]);
    
  chars = 0;                                     // Reset counter
}

uint8_t read_eeprom(uint16_t e_address) {        // Read from EEPROM
  TinyI2C.start(EEPROM_ADDR, 0);                 // Open connection to write mode
  TinyI2C.write(e_address >> 8);                 // Send MSB of address
  TinyI2C.write(e_address & 0xFF);               // Send LSB of address
  TinyI2C.start(EEPROM_ADDR, 1);                 // Restart for reading
  return TinyI2C.read();                         // Return byte
}

void sleep(void) {
  MCUCR |= (1 << SE);                            // Set Sleep Enable bit
  __asm__ __volatile__ ("sleep" "\n\t" ::);      // Sleep instruction
  MCUCR &= ~(1 << SE);                           // Clear SE bit
}