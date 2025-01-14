/*  
    Schimpfolino V1.2 24.12.2024 - Nikolai Radke
    https://www.monstermaker.de
    Next version for new improvements. Compatible with older versions.

    Sketch for the insulting gadget | With or without additional 24LAAXX EEPROM
    For ATtiny85 only - set to 8 MHz | B.O.D disabled | No bootloader | No millis()
    Remember to burn the "bootloader" (IDE is setting fuses) first!

    Flash usage: 7.952 bytes (IDE 2.3.4 | ATTinyCore 1.5.2 | Linux X86_64 | ATtiny85)
    Power:       1.6 mA (display on, no EEPROM) | ~ 200 nA (sleep)

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

#include <util/delay.h>                          // Needs less flash memory than delay()
#include "TinyI2CMaster.h"                       // I2C communication with display and EEPROM. Very tight library!
#include "oled.h"                                // OLED display library for SSD1306 and SH1106
#include "Schimpfolino_wordlist.h"               // Schimpfolino wordlist

// Hardware
#define  BUTTON   PB1                            // Button pin
#define  DEVICES  PB4                            // External devices power pin

// Variables
const char *field;                               // Pointer to one of the character arrays
uint8_t  genus = 0;                              // Genus of the swearword
uint8_t  chars = 0;                              // Number of characters in the word | Gobal
uint16_t list;                                   // Variable for parsing word lists
uint16_t number;                                 // Variable for calculating addresses and selecting words
uint16_t addresses[5] = {                        // Wordlists addresses array - overwritten if EEPROM is present
  sizeof(data1) / 10,                            // Wordcount in array for first word - adjective
  sizeof(data2) / 10,                            // Wordcount in array for second word part 1 - noun
  sizeof(data3) / 10,                            // Wordcount in array for second word part 2 - female noun
  sizeof(data4) / 10,                            // Wordcount in array for second word part 2 - male noun
  sizeof(data5) / 10                             // Wordcount in array for second word part 2 - neutrum noun
};
char     wordbuffer[20];                         // Buffer for read words
bool     eeprom = false;                         // No EEPROM used -> auto detect

volatile bool awake = false;                     // Stay awake when button is pressed

int main(void) {                                 
  init(); {                                      // Setup
    // Power saving
    ADCSRA &= ~(1 << ADEN);                      // Switch ADC off | Saves 270 uA
    MCUCR = (1 << SM1) | (0 << SM0);             // Always deepest sleep mode (Power-down)

    // Port setup
    DDRB = (1 << DEVICES) | (1 << SDA) | (1 << SCL); // Set PB4 to OUTPUT to power up display and EEPROM | SDA and SCL for I2C
    PORTB = 0x3F;                                // Set output ports to HIGH and input ports to INPUT_PULLUP to prevent floating

    // Hardware pin and watchdog interrupt
    cli();                                       // Stop all interrupts. An interrupt can ruin the timed sequence
    GIMSK |= (1 << PCIE);                        // Turn on pin change interrupt
    PCMSK |= (1 << PCINT1);                      // Turn on interrupt on PB1 button
    MCUSR &= ~(1 << WDRF);                       // No watchdog reset 
    WDTCR |= (1 << WDCE) | (1 << WDE);           // Watchdog change enable
    WDTCR = (1 << WDP0) | (1 << WDP3);           // Set prescaler to 8 s
    sei();                                       // Start interrupts

    // Init I2C
    TinyI2C.init();                              // Start I2C

    // Look for EEPROM and read wordlist addresses if available | genus is a helping variable here 
    if (TinyI2C.start(0x50, 0)) {                // Look for 24LCXX EEPROM at 0x50
      eeprom = true;                             // if available, set EEPROM flag
      for (list = 0; list < 5; list ++) {        // Read numbers of 5 wordlists
        number = read_eeprom(0 + genus) * 255;   // Calculate number: 
        number += read_eeprom(1 + genus);        // First byte = high, second byte = low
        addresses[list] = number;                // Write word numbers to array 
        genus += 2;                              // Change number address
      }  
    }

    // Randomize number generator
    PORTB &= ~(1 << DEVICES);                    // Devices off
    sleep();                                     // Sleep until button is pressed to "turn on"
    TCCR0A = 0x00;                               // Set timer 0 to normal mode
    TCCR0B = (1 << CS00);                        // Set prescaler to 1 to start the timer
    while (!(PINB & (1 << BUTTON)));             // Wait until button is released
    randomSeed(TCNT0);                           // Get a time count as seed

    // Main routine | Runs after waking up
    while(1) {
      // Init Display
      PORTB |= (1 << DEVICES);                   // Devices on
      PRR |= (1 << PRTIM0) | (1 << PRTIM1);      // Both timers are not needed anymore | Saves 100 uA when active
      Oled_init();                               // Connect and start OLED via I2C

      // Display swearwords until timeout
      while(awake) {                             // Wait 8.5 seconds timeout
        Oled_clear();                            // Clear display buffer

        // First word
        number = (random(0, addresses[0]));      // Select first word
        field = data1;                           // Pointer to first array
        get_swearword(number);                   // Read word from EEPROM or wordlist
        genus = random(0, 3);                    // Set word genus
        if (genus != 0) {                        // Check if not female
          wordbuffer[chars] = 48 + genus;        // If male, add "r", if neutrum, add "s" to buffer
          chars++;                               // Increase number of characters
        } 
        write_swearword(2);                      // Write first word in the first line

        // Second word first part
        list = 0;                                // Set start address for array
        if (eeprom) list = addresses[0];         // Set start address for EEPROM
        number = (random(list, addresses[1]));   // Select second part of second word
        field = data2;                           // Pointer to second array
        get_swearword(number);                   // Read first part of second word 
        
        // Second word second part
        if (eeprom) list = addresses[genus + 1]; //  Set start adress for EEPROM
        number = (random(list, addresses[genus + 2])); // Select second part of second word
        field = data3;                           // Pointer to female array
        if (genus == 1) field = data4;           // Pointer to male array
        if (genus == 2) field = data5;           // Pointer to neutrum array
        get_swearword(number);                   // Read second part of second word
        write_swearword(4);                      // Write second word in second line
        
         // Wait for button and sleep 8s
        _delay_ms(500);                          // Debounce button
        awake = false;                           // Set to sleep
        WDTCR |= (1 << WDIE);                    // Set watchdog interrupt
        sleep();                                 // Sleep 8 s or wake when button is pressed
        WDTCR &= ~(1 << WDIE);                   // Stop watchdog interrupt
      } 

      // Go to sleep after 8s seconds if button is not pressed before                           
      Oled_sendCommand(0xAE);                    // Display off and sleep (old boards)
      PORTB &= ~(1 << DEVICES);                  // Devices off
      sleep();                                   // Sleep until button is pressed
    }
  }
}

// Functions
void get_swearword(uint16_t address) {           // Fetch characters from EEPROM
  char c;                                        // Helping variable for fetched character
  uint16_t i;                                    // Helping variable for 10 readings
  address *= 10;                                 // Each address has 10 characters
  for (i = address; i < address + 10; i ++) {    // Read 10 characters...        
    c = pgm_read_byte(&field[i]);                // ...from wordlist...
    if (eeprom) c = read_eeprom(i + 10);         // ...or from EEPROM with address memory offset
    if (c != 32) {                               // Check for space
      switch (c) {                               // Set German Umlaute   
        case 35: wordbuffer[chars] = 27; break;  // # -> ä
        case 36: wordbuffer[chars] = 28; break;  // $ -> ö
        case 37: wordbuffer[chars] = 29; break;  // % -> ü
        case 38: wordbuffer[chars] = 58; break;  // & -> Ä
        case 39: wordbuffer[chars] = 59; break;  // ' -> Ö
        case 40: wordbuffer[chars] = 60; break;  // ' -> Ü
        case 42: wordbuffer[chars] = 30; break;  // * -> ß
        default: wordbuffer[chars] = c - 65;     // Set non-empty character
      }
    chars ++;                                    // Increase number of fetched characters
    }
  } 
}

void write_swearword(uint8_t line) {             // Write centered word
  uint8_t x;                                     // Helping variable for the x position on display
  x = (128 - (chars * 6)) / 2 - 6;               // Calculate centering
  if (chars > 17) (128 - (chars * 7)) / 2 - 7;   // Modify for very long words
  Oled_cursorTo(x, line);                        // Set cursor to selected line
  for (x = 0; x < chars; x ++)                   // Print the characters...
    Oled_printChar(wordbuffer[x]);               // ...from buffer
  chars = 0;                                     // Set number of characters back to 0 
}

uint8_t read_eeprom(uint16_t e_address) {        // Read from EEPROM
  TinyI2C.start(0x50, 0);                        // Open connection to I2C-address 0x50 in write mode
  TinyI2C.write(e_address >> 8);                 // Send the MSB (Most Significant Byte) of the memory address
  TinyI2C.write(e_address & 0xFF);               // Send the LSB (Least Significant Byte) of the memory address
  TinyI2C.start(0x50, 1);                        // Restart connection for reading one byte
  return TinyI2C.read();                         // Read and return byte
}

void sleep() {
  MCUCR |= (1 << SE);                            // Set SE (sleep Enable) bit
  __asm__ __volatile__ ("sleep" "\n\t" ::);      // Sleep now!!
  MCUCR &= ~(1 << SE);                           // CLear SE bit
}

ISR(PCINT0_vect) {                               // Interrupt routine for pin change 
  awake = true;                                  // Set awake flag when button is pressed
}

ISR(WDT_vect) {}                                 // Interrupt routine for watchdog. Unused but mandatory                               