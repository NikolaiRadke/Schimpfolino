# Firmware and tools for Schimpfolino

* **Schimpfolino**: a sketch for use with EEPROM only. For ATtiny45/85.  
* **Schimpfolino_noreeprom**: a hybrid sketch for use with or without EEPROM with reduced internal word list. For ATtiny85 only.  
* **eeprom**: files with EEPROM content and writing tool.  

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
3. Add the 10uF capacitor.    
  
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
  
1. Open sketch **Schimpfolino.ino** (of **Schimpfolino_noeeprom.ino**).
2. Select ``` Sketch > Upload using programmer ``` (IDE 2.X) or press upload button (IDE 1.X) 
4. DONE.  

Now you can remove the microcontroller and plug it into the socket of your Schimpfolino.  
