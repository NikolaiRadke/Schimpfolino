# Setting up a serial connection and writing to EEPROM

You need:  
* Sketch **eeprom_writer.ino**.
* An Arduino Uno/Nano/Whatever.
* A serial terminal like *Tera Term* (Windows) or *CuteCom* (Linux).

### Connecting your Arduino with the EEPROM

1. Disconnect your Arduino.
2. Connect your Arduino with the EEPROM as seen below. It is an **I2C** connection. If you want to use any other board, connect **SDA** with **pin 5** and **SCL** with **pin 6** of your EEPROM.
   
![EEPROM with Arduino](https://www.nikolairadke.de/schimpfolino/eeprom_verbinden.jpg)

**Wait!** Where is the **pull up resistor**?  
Yes indeed, I2C needs a pull up resistor, **4,7 kOhm** between **SDA** and **VCC**. But: The Arduino has an internal **20 kOhm** pull up resistor at every I/O pin. These are pulled up with every I2C connections when the standard library **Wire.h** is used. This is not the correct value but works reliable with slow connections. This is a very slow connection. But if you are unsure, add an externel pull up resistor. Schimpfolino's display finally has the missing resistors on board.    

### Flashing the content

1. Connect the Arduino to your computer.  
2. Open the Schimpfolino writer sketch **eeprom_writer.ino** in the Arduino IDE and upload the sketch. 
3. Start your favorite serial terminal, set to **9600 Baud, 8-N-1** and a **5 ms delay** after each character, no Linefeed (LF) oder Carriage Return (CR). 
4. Open the connection to the port (COM? in Windows, see Device Manager or IDE | /dev/ttyACM0 for Arduino Uno or /dev/ttyUSB? other Arduinos in Linux)
5. The Arduino will tell you through the serial terminal, what eeprom_writer is doing. Now upload the files **eeprom1.txt** to **eeprom5.txt**.
6. Close the connection and remove the EEPROM.
  
## Structure of eeprom?.txt files

Each file contains a set of words. Each word has **10** characters, spaces are mandatory! Just write them one after another. The EEPROM writing sketch will calculate their number and store both number and words into the EEPROM. The last character of the file has to be a **!**. You need a text editor. An office word processor will not work.  

German **Umlaute** must be converted and only small letters are possible:
**ä** -> **#** | **ö** -> **$** | **ü** -> **%** | **ß** -> *  

There is an (beta) editor **eeprom.html** in the files folder. You can easily add new words or generate a new eeprom?.txt. Umlaute will be converted and spaces and the terminating ! added automatically.  



