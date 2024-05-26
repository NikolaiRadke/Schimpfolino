# Setting up a serial connection and writing to EEPROM

You need:  
* Sketch **eeprom_writer.ino**.
* An Arduino Uno/Nano/Whatever.
* A serial terminal like *Tera Term* (Windows) or *CuteCom* (Linux).

### Connecting your Arduino with the EEPROM

1. Disconnect your Arduino.
2. Connect your Arduino with the EEPROM as seen below. it is an **I2C** connection. If you want to use any other board, connect **SDA** to **Pin 5** and **SCL** to **Pin 6**.
   
![EEPROM with Arduino](https://www.nikolairadke.de/schimpfolino/eeprom_verbinden.jpg)

1. Open the Schimpfolino writer sketch **eeprom_writer.ino** in the Arduino IDE and upload the sketch. 
2. Start your favorite serial terminal, set to **9600 Baud, 8-N-1** and a **10ms delay** after each character, no Linefeed (LF) oder Carrige Return (CR). 
3. Open the connection to the Port (COM? in Windows, see Device Manager | /dev/ttyACM0 for Arduino Uno or /dev/ttyUSBX other Arduinos for Linux)
4. The serial terminal will tell you, what eeprom_writer is doing. Now upload the files **eeprom1.txt** to **eeprom5.txt**.
5. Close the connection and remove the EEPROM.
  
## Structure of eepromX.txt files

Each file contains a set of words. Each word has **10** characters, spaces are mandatory! Just write them one after another. The EEPROM writing sketch will calculate their number and store both number and words into the EEPROM.  


