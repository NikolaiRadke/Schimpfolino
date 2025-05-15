/*  
    Schimpfolino V1.4 15.05.2025 - Nikolai Radke
    https://www.monstermaker.de
    Next version for new improvements. Compatible with older boards

    Sketch for the insulting gadget | Only with additional 24AAXXX EEPROM
    For ATtiny25/45/85 - set to 8 MHz | B.O.D disabled | No bootloader | No millis()
    Remember to burn the "bootloader" (IDE is setting fuses) first!

    Flash usage: 1.994 bytes (IDE 2.3.6 | ATTinyCore 1.5.2 | Linux X86_64 | ATtiny85)
    Power:       1.7 mA (display on, EEPROM on) | ~ 200 nA (sleep)

    Umlaute have to be converted (UTF-8):
    ä -> [ | ö -> ] | ü -> ^ | ß -> _ | Ä -> { | Ö -> | | Ü -> }
    Last character of a wordlist in the file is "!" 

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

// Hardware
#define  BUTTON   PB1                            // Button pin    
#define  DEVICES  PB4                            // External devices power pin

// Variables
uint8_t  chars = 0;                              // Number of characters in the word | Gobal
uint16_t addresses[5];                           // Wordlists addresses array
char     wordbuffer[20];                         // Buffer for reading words

volatile bool awake = false;                     // Stay awake when button is pressed

int main(void) {                
  // Power saving
  ADCSRA &= ~(1 << ADEN);                        // Switch ADC off | Saves 270 uA
  MCUCR = (1 << SM1) | (0 << SM0);               // Always deepest sleep mode (Power-down)

  // Port setup
  DDRB = (1 << DEVICES) | (1 << SDA) | (1 << SCL); // Set PB4 to OUTPUT to power up display and EEPROM | SDA and SCL for I2C
  PORTB = 0x3F;                                  // Set output ports to HIGH and input ports to INPUT_PULLUP to prevent floating
    
  // Hardware pin and watchdog interrupt
  cli();                                         // Stop all interrupts. An interrupt can ruin the timed sequence
  GIMSK |= (1 << PCIE);                          // Turn on pin change interrupt
  PCMSK |= (1 << PCINT1);                        // Turn on interrupt on PB1 button
  MCUSR &= ~(1 << WDRF);                         // No watchdog reset 
  WDTCR |= (1 << WDCE) | (1 << WDE);             // Watchdog change enable
  WDTCR = (1 << WDP0) | (1 << WDP3);             // Set prescaler to 8 s
  sei();                                         // Start interrupts

   // Init I2C
  TinyI2C.init();                                // Start I2C

  // Read wordlist addresses | genus is a helping variable here
  for (uint8_t list = 0; list < 5; ++ list)      // Read numbers of 5 wordlists
    addresses[list] = (read_eeprom(list * 2) * 255) + (read_eeprom((list * 2) +1)); // Write word numbers to array

   // Randomize number generator
  PORTB &= ~(1 << DEVICES);                      // Devices off
  sleep();                                       // Sleep until button is pressed to "turn on"
  TCCR0A = 0x00;                                 // Set timer 0 to normal mode
  TCCR0B = (1 << CS00);                          // Set prescaler to 1 to start the timer
  while (!(PINB & (1 << BUTTON)));               // Wait until button is released
  randomSeed(TCNT0);                             // Get a time count as seed

  // Main routine | Runs after waking up
  for(;;) {
    // Init Display
    PORTB |= (1 << DEVICES);                     // Devices on
    PRR |= (1 << PRTIM0) | (1 << PRTIM1);        // Both timers are not needed anymore | Saves 100 uA when active
    Oled_init();                                 // Connect and start OLED via I2C

    // Display swearwords until timeout
    while (awake) {                              // Wait 8.5 seconds timeout
      Oled_clear();                              // Clear display buffer
  
      // First word
      get_swearword(random(0, addresses[0]));    // first Read word from EEPROM
      uint8_t genus = random(0, 3);              // Set word genus
      if (genus) {                               // Check if not female
        wordbuffer[chars] = 48 + genus;          // If male, add "r", if neutrum, add "s" to buffer
        ++ chars;                                // Increase number of characters
      } 
      write_swearword(2);                        // Write first word in the first line
      
      // Second word first part
      get_swearword(random(addresses[0], addresses[1])); // Read first second word from EEPROM
      
      // Second word second part
      get_swearword(random(addresses[genus + 1], addresses[genus + 2])); // Read second part of second word
      write_swearword(4);                        // Write second word in second line
        
      // Wait for button and sleep 8s
      _delay_ms(500);                            // Debounce button
      awake = false;                             // Set to sleep     
      WDTCR |= (1 << WDIE);                      // Set watchdog interrupt
      sleep();                                   // Sleep 8 s or wake when button is pressed
      WDTCR &= ~(1 << WDIE);                     // Stop watchdog interrupt
    } 
    
    // Go to sleep after 8 seconds if button is not pressed before                           
    Oled_sendCommand(0xAE);                      // Display off and sleep (old boards)
    PORTB &= ~(1 << DEVICES);                    // Devices off   
    sleep();                                     // Sleep until button is pressed
  }
}

// Functions
void get_swearword(uint16_t address) {           // Fetch characters from EEPROM
  address *= 10;                                 // Each address has 10 characters
  for (uint8_t i = 0; i < 10; ++ i) {            // Read 10 characters
    char c = read_eeprom(address + i + 10);      // From EEPROM with offset
    if (c != 32) wordbuffer[chars ++] = c - 65;  // Convert and store character, increment counter
  } 
}

void write_swearword(uint8_t line) {             // Write centered word
  uint8_t x = (128 - (chars * 7)) / 2;           // Calculate centering
  if (chars > 17) x = (128 - (chars * 6)) / 2;   // Modify for very long words
  Oled_cursorTo(x, line);                        // Set cursor to selected line
  for (x = 0; x < chars; ++ x )                  // Print the characters...
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
  MCUCR |= (1 << SE);                            // Set SE (Sleep Enable) bit
  __asm__ __volatile__ ("sleep" "\n\t" ::);      // Sleep now!!
  MCUCR &= ~(1 << SE);                           // Clear SE bit
}

ISR(PCINT0_vect) {                               // Interrupt routine for pin change 
  awake = true;                                  // Set awake flag when button is pressed
}

ISR(WDT_vect) {}                                 // Interrupt routine for watchdog. Unused but mandatory                               