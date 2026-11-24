/*
    Schimpfolino V1.6 24.11.2026 - Nikolai Radke
    https://www.monstermaker.de
    Optimized version with point-terminated strings and complete word list

    Sketch for the insulting gadget | Wordlist in flash only (no EEPROM support)
    For ATtiny85 only - set to 8 MHz | B.O.D disabled | No bootloader | No millis()
    Remember to burn the "bootloader" (IDE is setting fuses) first!

    Flash usage: 8.072 bytes (IDE 2.3.11 | ATTinyCore 1.5.2 | Linux X86_64 | ATtiny85)
    Power:       1.2 mA (display on, 50% contrast) | ~ 200 nA (sleep)

    Umlaute have to be converted (UTF-8):
    ä -> [ | ö -> ] | ü -> ^ | ß -> _ | Ä -> { | Ö -> | | Ü -> }

    Wiring:
                         +-\/-+
    RST          - PB5  1|    |8  VCC - Battery
    Free         - PB3  2|    |7  PB2 - SCL
    Display VCC  - PB4  3|    |6  PB1 - Button -> GND
    GND          - GND  4|    |5  PB0 - SDA
                         +----+
*/

#include <util/delay.h>                          // Needs less flash memory than delay()
#include "TinyI2CMaster.h"                       // I2C communication with the display. Very tight library!
#include "oled.h"                                // OLED display library for SSD1306 and SH1106
#include "Schimpfolino_wordlist.h"               // Wordlist with point-terminated strings

// Hardware
#define  BUTTON         PB1                      // Button pin
#define  DEVICES        PB4                      // Display power pin

// Wordlist and display
#define  GENUS          3                        // Number of genera: female, male, neutrum
#define  TERMINATOR     '.'                      // Character marking the end of a word
#define  CHAR_OFFSET    'A'                      // Wordlist character minus offset = font index
#define  DIGIT_ZERO     '0'                      // '0' + 1 = font index of "r", '0' + 2 = "s"
#define  DISPLAY_WIDTH  128                      // Display width in pixels
#define  LONG_WORD      17                       // Longer words are drawn narrower | Must match "chars < 18" in oled.cpp!

// Timing
#define  DEBOUNCE_MS    500                      // Wait time against button bouncing
#define  AWAKE_BIT      0                        // Bit 0 in GPIOR0: stay awake when button is pressed

// Variables
const char *field;                               // Pointer to one of the character arrays
uint8_t  chars = 0;                              // Number of characters in the word | Global, used by oled.cpp
char     wordbuffer[20];                         // Buffer for read words
uint16_t rng_state;                              // State of the random number generator | Seeded from timer, no start value needed

int main(void) {
  // Power saving
  ADCSRA &= ~(1 << ADEN);                        // Switch ADC off | Saves 270 uA
  MCUCR = (1 << SM1) | (0 << SM0);               // Always deepest sleep mode (Power-down)

  // Port setup
  DDRB = (1 << DEVICES) | (1 << SDA) | (1 << SCL); // Set PB4 to OUTPUT to power up the display | SDA and SCL for I2C
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

  // Randomize number generator
  PORTB &= ~(1 << DEVICES);                      // Display off
  sleep();                                       // Sleep until button is pressed to "turn on"
  TCCR0A = 0x00;                                 // Set timer 0 to normal mode
  TCCR0B = (1 << CS00);                          // Set prescaler to 1 to start the timer
  while (!(PINB & (1 << BUTTON)));               // Wait until button is released
  rng_state = TCNT0 + 1;                         // Get a time count as seed (+1: must never be 0)

  // Main routine | Runs after waking up
  for(;;) {
    // Init Display
    PORTB |= (1 << DEVICES);                     // Display on
    PRR |= (1 << PRTIM0) | (1 << PRTIM1);        // Both timers are not needed anymore | Saves 100 uA when active
    Oled_init();                                 // Connect and start OLED via I2C

    // Display swearwords until timeout
    while (GPIOR0 & (1 << AWAKE_BIT)) {          // Stay awake as long as the button was pressed
      Oled_clear();                              // Clear display

      // First word
      field = data1;                             // Pointer to first array
      get_swearword(random16(WORDS));            // Read first word from wordlist
      uint8_t genus = random16(GENUS);           // Set word genus
      if (genus) {                               // Check if not female
        wordbuffer[chars] = DIGIT_ZERO + genus;  // If male, add "r", if neutrum, add "s" to buffer
        ++ chars;                                // Increase number of characters
      }
      write_swearword(2);                        // Write first word in the first line

      // Second word first part
      field = data2;                             // Pointer to second array
      get_swearword(random16(WORDS));            // Read first part of second word

      // Second word second part
      field = data3;                             // Pointer to female array
      if (genus == 1) field = data4;             // Pointer to male array
      if (genus == 2) field = data5;             // Pointer to neutrum array
      get_swearword(random16(WORDS));            // Read second part of second word
      write_swearword(4);                        // Write second word in second line

      // Wait for button and sleep 8s
      _delay_ms(DEBOUNCE_MS);                    // Debounce button
      GPIOR0 = 0;                                // Clear awake flag -> go to sleep
      WDTCR |= (1 << WDIE);                      // Set watchdog interrupt
      sleep();                                   // Sleep 8 s or wake when button is pressed
      WDTCR &= ~(1 << WDIE);                     // Stop watchdog interrupt
    }

    // Go to sleep after 8s seconds if button is not pressed before
    Oled_sendCommand(0xAE);                      // Display off and sleep (old boards)
    PORTB &= ~(1 << DEVICES);                    // Display power off
    sleep();                                     // Sleep until button is pressed
  }
}

// Functions
void get_swearword(uint16_t address) {           // Fetch characters from the wordlist in flash
  uint16_t pos = 0;                              // Start at the beginning of the array
  while (address) {                              // Skip words until the wanted one is reached
    if (pgm_read_byte(&field[pos++]) == TERMINATOR) address--; // Every point ends one word
  }
  char c;
  while ((c = pgm_read_byte(&field[pos++])) != TERMINATOR) // Copy characters until the next point
    wordbuffer[chars ++] = c - CHAR_OFFSET;      // Store font index of the character
}

void write_swearword(const uint8_t line) {       // Write centered word
  uint8_t x = (uint8_t)(DISPLAY_WIDTH - (uint8_t)((chars << 3) - chars)) >> 1; // Centering with 7 pixels per character
  if (chars > LONG_WORD)                         // Very long word: only 6 pixels per character
    x = (uint8_t)(DISPLAY_WIDTH - (uint8_t)((chars << 2) + (chars << 1))) >> 1;
  Oled_cursorTo(x, line);                        // Set cursor to selected line
  for (x = 0; x < chars; ++ x)                   // Print the characters...
    Oled_printChar(wordbuffer[x]);               // ...from buffer
  chars = 0;                                     // Set number of characters back to 0
}

uint8_t random16(const uint8_t high) {           // Small replacement for random() | Returns 0 to high - 1
  rng_state ^= rng_state << 7;                   // 16 bit xorshift: mix the bits...
  rng_state ^= rng_state >> 9;
  rng_state ^= rng_state << 8;
  return rng_state % high;                       // ...and limit the result to the wanted range
}

void sleep() {
  MCUCR |= (1 << SE);                            // Set SE (Sleep Enable) bit
  __asm__ __volatile__ ("sleep" "\n\t" ::);      // Sleep now!!
  MCUCR &= ~(1 << SE);                           // Clear SE bit
}

ISR(PCINT0_vect, ISR_NAKED) {
  GPIOR0 |= (1 << AWAKE_BIT);                    // Set awake flag when button is pressed
  reti();                                        // Return from interrupt (must be written by hand in naked ISRs)
}

EMPTY_INTERRUPT(WDT_vect);                       // Watchdog interrupt only wakes the ATtiny up. Just a "reti" instruction
