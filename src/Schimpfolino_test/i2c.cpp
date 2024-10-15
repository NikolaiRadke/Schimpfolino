/*  
  Von ChatGPT erstellte alternative I2C-Bibliothek.
  Wird getestet.
*/

#include "i2c.h"
#include <util/delay.h>   

// I2C Functions for Direct Register Manipulation
void i2c::init() {
    // Set SDA and SCL as output
    DDRB |= (1 << SDA_PIN) | (1 << SCL_PIN);
    // Pull SDA and SCL high
    PORTB |= (1 << SDA_PIN) | (1 << SCL_PIN);
}

void i2c::start() {
    // Start condition: SDA goes low while SCL is high
    PORTB &= ~(1 << SDA_PIN);
    _delay_us(5);  // Small delay for stability
    PORTB &= ~(1 << SCL_PIN);
}

void i2c::stop() {
    // Stop condition: SDA goes high while SCL is high
    PORTB |= (1 << SCL_PIN);
    _delay_us(5);  // Small delay
    PORTB |= (1 << SDA_PIN);
}

void i2c::write_byte(uint8_t data) {
    for (uint8_t i = 0; i < 8; i++) {
        if (data & 0x80) {
            PORTB |= (1 << SDA_PIN);  // Send '1'
        } else {
            PORTB &= ~(1 << SDA_PIN);  // Send '0'
        }
        _delay_us(5);  // Short delay
        PORTB |= (1 << SCL_PIN);  // Clock pulse
        _delay_us(5);
        PORTB &= ~(1 << SCL_PIN);  // End clock pulse
        data <<= 1;  // Shift data left for the next bit
    }
    // Acknowledge bit (can be ignored or handled as needed)
    DDRB &= ~(1 << SDA_PIN);  // Set SDA as input
    PORTB |= (1 << SCL_PIN);  // Clock for ACK
    _delay_us(5);
    PORTB &= ~(1 << SCL_PIN);
    DDRB |= (1 << SDA_PIN);  // Set SDA back as output
}

// Replacing Wire.h functions with direct I2C
void i2c::sendCommand(uint8_t command) {
    i2c_start();
    i2c_write_byte(0x3C << 1);  // Address of the OLED display
    i2c_write_byte(0x00);       // Command mode
    i2c_write_byte(command);
    i2c_stop();
}

void i2c::sendData(uint8_t data) {
    i2c_start();
    i2c_write_byte(0x3C << 1);  // Address of the OLED display
    i2c_write_byte(0x40);       // Data mode
    i2c_write_byte(data);
    i2c_stop();
}
