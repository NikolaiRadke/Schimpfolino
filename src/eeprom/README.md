# Setting up a serial connection and writing to EEPROM

You need:  
* Sketch **eeprom_writer.ino**.  
* A serial terminal like *Tera Term* for Windows or *CuteCom* fo Linux.

1. Open the Schimpfolino writer sketch **eeprom_writer.ino** in the Arduino IDE and upload the sketch. 

2. Start your favorite serial terminal, set to **9600 Baud, 8-N-1** and a **10ms delay** after each character, no Linefeed (LF) oder Carrige Return (CR). 

3. Open the connection to the Port (COM? in Windows, see Device Manager | /dev/ttyACM0 for Arduino Uno or /dev/ttyUSBX other Arduinos for Linux)

4. The serial terminal will tell you, what eeprom_writer is doing. Now upload the files **eeprom1.txt** to **eeprom5.txt**.

5. Close the connection an insert the EEPROM into the socket.  
  
## Structure of eepromX.txt files

Each file contains a set of words. Each word has **10** characters, spaces are mandatory! Just write them one after another. The eeprom writing sketch will calculate their number and store both number an words into the eeprom.  


