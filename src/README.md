# Firmware and tools for Schimpfolino

* **Schimpfolino**: a sketch for use with EEPROM only. For ATtiny45/85. **Recommended**.
* **Schimpfolino_noreeprom**: a hybrid sketch for use with or without EEPROM with reduced internal word list. For **ATtiny85 only**.
* **eeprom/**: folder for files with EEPROM content and writing tool.  

# Compiling instructions for Schimpfolino

In order to flash your ATtiny with the Schimpfolino sketch, you need the **Arduino-IDE** (1.6.6 or higher), with installed **ATtiny-support** and an **Arduino** connected to your microcontroller.

### Preparing the IDE  
 
1. Open ``` File > Preferences ```.  
2. Find the field ``` Additional Boards Manager URLs ```.  
3. Enter ``` http://drazzy.com/package_drazzy.com_index.json ```.  
4. Press ``` OK ```.  
5. Open ``` Tools > Board > Boards Manager ```.  
6. Select ``` ATTinyCore by Spence Konde ```.  
7. Press ``` Install ```.

### Peparing your Arduino  

1. Connect your Arduino to your system.  
2. Open ``` File > Examples > ArduinoISP > ArduinoISP ```.  
3. Upload the sketch.

### Connecting your ATtiny with the Arduino  

1. Disconnect your Arduino.
2. Connect your ATtiny as seen below.
3. Add the 10 uF capacitor.    
  
![ATTINY-ARDUINO](http://www.nikolairadke.de/NOKOlino_2/attiny_steckplatine.png)  
  
### Burning an ATtiny bootloader  
  
Technically, this process will flash no bootloader, the IDE just sets the fuses. 
  
1. Connect your Arduino to your system again.  
2. Select ``` Tools > Programmer > Arduino as ISP ```.  
3. Select ``` Tools > Board > ATTinyCore > ATtiny25/45/85 (No bootloader) ```.
4. Select ``` Tools > Port ``` for the matching port.  
5. Select ``` Tools > B.O.D. Level [...] > B.O.D. Disabled (saves power) ```.
6. Select ``` Tools > Chip > ATtiny85 ``` or ``` Tools > Chip > ATtiny45 ```.  
8. Start burning process with ``` Tools > Burn bootloader ```.  

Now you can flash your ATtiny with any sketch you want! But this time, we focus on Schimpfolino.   

### Flashing Schimpfolino firmware
  
1. Open **Schimpfolino.ino**, **Schimpfolino_wire.ino**.
2. Select ``` Sketch > Upload using programmer ``` (IDE 2.X) or press upload button (IDE 1.X) 
3. DONE.  

Now you can remove the microcontroller and plug it into the socket of your Schimpfolino.  

## Hints for Linux users  

With some Debain-based distributions like Ubuntu or Mint you might recieve an error while trying to upload your sketch to the ATtiny like  
``` avrdude: ser_open(): can't open device "/dev/ttyUSB0": Permission denied ```.  
This issue can be solved by adding your user to the dialout-group. Open a terminal and type  
``` sudo usermod -a -G dialout $USERNAME ```.  
Enter your password and restart. Now it should work.  

If not: Maybe you are using an Arduino Nano clone with CH340 chip, the IDE may not find your port. This is because of a conflict with preinstalled Braille driver. If you don't need it, deinstall the driver with  
``` sudo apt-get remove brltty ```  
and the IDE should find the port now.  
