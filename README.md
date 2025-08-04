![Schimpfolino](http://www.nikolairadke.de/schimpfolino/schimpfolino_back.jpg)

  
### "Klappriger Blubberfetzen" "Nervendes Riesenungemach" "Windige Pupswindel"  
    
Who doesn't like to be insulted? Schimpfolino is an evil gadget that always has a saying ready. Small, handy, power saving. And completely useless. You need a PCB and some parts listed below. It's easy to build and long lasting for everyday use. Insert battery, press the button and recieve your daily insult!        

*Huch, Englisch? Kann ich nicht. Schnell [hier hin](https://github.com/NikolaiRadke/Schimpfolino/wiki)*.  

![Schimpfolino](https://www.nikolairadke.de/schimpfolino/schimpfolino_case.png)
  
Schimpfolino is a random curse word generator with **12.288.000** possible combinations right now. That should be enough for a while. It's suitable for children (but still evil), maintenance-free and the battery lasts for years. And with the right case, it's an evil monster, like the **Nokolino shaped** case below! Just press the button, receive an insult, after eight seconds it turns off again. That's it. Evil. You can print the case in black! Black is an evil color. It is so evil. It is so evil that it hurts many people that are good!

What's new?  
* **15.05.2025** Firmware **1.X-150525** once again with smaller flash size. The **noeeprom** variants got 5 more words.  
    -- More news? See the [newsblog](https://github.com/NikolaiRadke/Schimpfolino/tree/main/NEWS.md).   
  

### Notes for non-German Schimpfolino makers  
  
The building wiki and the curse words are in German. You need English words? **Contribute!**  

### Prerequisites

Except for the tools and some time, you need:
* Arduino IDE 1.6.6 or higher
* Serial terminal software like *CuteCom* (Linux) or *CoolTerm* (Linux/Windows/Mac).  
* Arduino UNO/Nano/Whatever with USB cable
* A breadboard
* Some Dupont wires
* A 10 µF capacitor
* (Optional a 2 kOhm resistor)  
  
And the following hardware:
* Schimpfolino PCB 
* ATtiny25/45/85 (ATtiny25 only for >= V1.4)
* 24AA64 or larger EEPROM (up to 24AA512)
* 0,96" SDD1306 (V1.0) or 0,96"/1,3" SDD1306/SH1106 I2C display (V1.3)  
* Button 6 x 6 mm, 9,5 mm height
* 2 DIP-8 sockets
* Batteryholder, 20 mm pin spacing
* CR2032 battery
* Double-sided adhesive pad, 25 x 15 x 3 mm
* (Optional 2 100 nF capacitors)  
  
### Content

```
Schimpfolino/
Root directory with some explanation files.  
|
├── case/
|   STL files to 3D print a case. Other kinds of hulls will follow.
|   ├── Boring/
|   |   Plain but smooth case by Michael Kussmaul.
|   ├── CuteVader/
|   |   Cute and evil case by falsanu.
|   └── Nokolino/
|       Standard Nokolino-shaped case.
|
├── python/
|   Schimpfolino as python script.
|
├── schematics/
|   The circuit diagram, PCB renderings and gerber files.
|
└── src/
    Firmware and word files.
    ├── eeprom/
    |   All you need to flash your EEPROM.
    |   ├── eeprom_writer/
    |   |   Arduino sketch to write to your EEPROM.
    |   ├── files/
    |   |   The EEPROM content word lists and the editor.
    |   └── files_new/
    |       EEPROM content vor new version (>= 1.4)
    |
    ├── Schimpfolino/
    |   Standard sketch with libraries used in the Make article.
    ├── Schimpfolino_new/
    |   Version for new improvments. Compatible to old version.
    ├── Schimpfolino_nopeeprom/
    |   Sketch for Schimpfolino without EEPROM.
    └── Schimpfolino_nopeeprom_new/
        New version. Compatible to old version.
```

### Let's get started!

Ready? Great! Let's start here: [German wiki](https://github.com/NikolaiRadke/Schimpfolino/wiki).  

