/* 
    TinyI2C v2.0.1 

    David Johnson-Davies - www.technoblogy.com - 5th June 2022
    Modified for ATtiny85 8 MHz only for Schimpfolino by Nikolai Radke 
                         - www.monstermaker.de - 29th October 2024
   
    CC BY 4.0
    Licensed under a Creative Commons Attribution 4.0 International license: 
    http://creativecommons.org/licenses/by/4.0/
*/

#include "TinyI2CMaster.h"

#define DELAY_T2TWI _delay_us(2)                 // > 1.3 us
#define DELAY_T4TWI _delay_us(1)                 // > 0.6 us
#define TWI_NACK_BIT 0                           // Bit position for (N)ACK bit

// Constants
// Prepare register value to: Clear flags, and set USI to shift 8 bits i.e. count 16 clock edges
uint8_t const USISR_8bit = 1 << USISIF | 1 << USIOIF | 1 << USIPF | 1 << USIDC | 0x0 << USICNT0;
// Prepare register value to: Clear flags, and set USI to shift 1 bit i.e. count 2 clock edges
uint8_t const USISR_1bit = 1 << USISIF | 1 << USIOIF | 1 << USIPF | 1 << USIDC | 0xE << USICNT0;

uint8_t TinyI2CMaster::transfer(uint8_t data) {
  USISR = data;                                  // Set USISR according to data
                                                 // Prepare clocking
  data = 0 << USISIE | 0 << USIOIE |             // Interrupts disabled
         1 << USIWM1 | 0 << USIWM0 |             // Set USI in Two-wire mode
         1 << USICS1 | 0 << USICS0 | 1 << USICLK | // Software clock strobe as source
         1 << USITC;                             // Toggle Clock Port
  do {
    DELAY_T2TWI;
    USICR = data;                                // Generate positive SCL edge
    while (!(PIN_USI_CL & 1 << PIN_USI_SCL));    // Wait for SCL to go high
    DELAY_T4TWI;
    USICR = data;                                // Generate negative SCL edge
  } while (!(USISR & 1 << USIOIF));              // Check for transfer complete
  DELAY_T2TWI;
  data = USIDR;                                  // Read out data
  USIDR = 0xFF;                                  // Release SDA
  DDR_USI |= (1 << PIN_USI_SDA);                 // Enable SDA as output
  return data;                                   // Return the data from the USIDR
}

void TinyI2CMaster::init() { // Ports were already defined in main sketch
  USIDR = 0xFF;                                  // Preload data register with data
  USICR = 0 << USISIE | 0 << USIOIE |            // Disable Interrupts
          1 << USIWM1 | 0 << USIWM0 |            // Set USI in Two-wire mode
          1 << USICS1 | 0 << USICS0 | 1 << USICLK | // Software stobe as counter clock source
          0 << USITC;
  USISR = 1 << USISIF | 1 << USIOIF | 1 << USIPF | 1 << USIDC | // Clear flags...
          0x0 << USICNT0;                        // ...and reset counter
}

uint8_t TinyI2CMaster::read(void) {  
  DDR_USI &= ~(1 << PIN_USI_SDA);                // Enable SDA as input
  uint8_t data = TinyI2CMaster::transfer(USISR_8bit);
  /* Prepare to generate ACK (or NACK in case of End Of Transmission) */
  TinyI2CMaster::transfer(0xFF);                 // Generate ACK
  return data;                                   // Read successfully completed
}

bool TinyI2CMaster::write(uint8_t data) {
  /* Write a byte */
  PORT_USI_CL &= ~(1 << PIN_USI_SCL);            // Pull SCL LOW
  USIDR = data;                                  // Setup data
  TinyI2CMaster::transfer(USISR_8bit);           // Send 8 bits on bus
  /* Clock and verify (N)ACK from slave */
  DDR_USI &= ~(1 << PIN_USI_SDA);                // Enable SDA as input.
  if (TinyI2CMaster::transfer(USISR_1bit) & 1 << TWI_NACK_BIT) return false;
  return true;                                   // Write successfully completed
}

// Start transmission by sending address
bool TinyI2CMaster::start(uint8_t address, uint8_t read) {
  /* Release SCL to ensure that (repeated) Start can be performed */
  PORT_USI_CL |= 1 << PIN_USI_SCL;               // Release SCL
  while (!(PIN_USI_CL & 1 << PIN_USI_SCL));      // Verify that SCL becomes high
  DELAY_T4TWI;
  /* Generate Start Condition */
  PORT_USI &= ~(1 << PIN_USI_SDA);               // Force SDA LOW
  DELAY_T4TWI;
  PORT_USI_CL &= ~(1 << PIN_USI_SCL);            // Pull SCL LOW.
  PORT_USI |= 1 << PIN_USI_SDA;                  // Release SDA
  if (!(USISR & 1 << USISIF)) return false; 
  /*Write address */
  PORT_USI_CL &= ~(1 << PIN_USI_SCL);            // Pull SCL LOW
  USIDR = address << 1 | read;                   // Setup data
  TinyI2CMaster::transfer(USISR_8bit);           // Send 8 bits on bus
  /* Clock and verify (N)ACK from slave */
  DDR_USI &= ~(1 << PIN_USI_SDA);                // Enable SDA as inp.
  if (TinyI2CMaster::transfer(USISR_1bit) & 1 << TWI_NACK_BIT) return false; // No ACK
  return true;                                   // Start successfully completed
}

void TinyI2CMaster::stop(void) {
  PORT_USI &= ~(1 << PIN_USI_SDA);               // Pull SDA low
  PORT_USI_CL |= 1 << PIN_USI_SCL;               // Release SCL
  while (!(PIN_USI_CL & 1 << PIN_USI_SCL));      // Wait for SCL to go high
  DELAY_T4TWI;
  PORT_USI |= 1 << PIN_USI_SDA;                  // Release SDA
  DELAY_T2TWI;
}
