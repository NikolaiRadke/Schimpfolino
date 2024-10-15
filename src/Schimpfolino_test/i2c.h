/*  
  Von ChatGPT erstellte alternative I2C-Bibliothek.
  Wird getestet.
*/

#ifndef i2c_h
#define i2c_h

// I2C Pin Definitionen (SDA = PB0, SCL = PB2)
#define SDA_PIN PB0
#define SCL_PIN PB2

class i2c {
    public:
    // I2C Functions for Direct Register Manipulation
    void init() {}

    // Start condition: SDA goes low while SCL is high
    void start() {}

    // Stop condition: SDA goes high while SCL is high
    void stop() {}

    // Write Data to address
    void write_byte(uint8_t data) {}

    // Send command to display
    void sendCommand(uint8_t command) {}

    // Send data to display
    void sendData(uint8_t data) {}
};
