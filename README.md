![Schimpfolino](http://www.nikolairadke.de/schimpfolino/schimpfolino_back.jpg)

  
### "Klappriger Blubberfetzen" "Nervendes Riesenungemach" "Windige Pupswindel"  

    
Who doesn't like to be insulted? Schimpfolino is an evil gadget that always has a saying ready. Small, handy, power saving. And completely useless. You need a PCB and some parts listed below. It's easy to build and long lasting for everyday use. Insert battery, press the button and recieve your daily insult!        

*Huch, Englisch? Kann ich nicht. Schnell [hier hin](https://github.com/NikolaiRadke/Schimpfolino/wiki)*.  

![Schimpfolino](https://www.nikolairadke.de/schimpfolino/schimpfolino_case.png)
  
Schimpfolino is a random curse word generator with **12.288.000** possible combinations right now. That should be enough for a while. It's suitable for children (but still evil), maintenance-free and the battery lasts for years. And with the right case, it's an evil monster, like the **Nokolino shaped** case below! Just press the button, receive an insult, after 10 seconds it turns off again. That's it. Evil. You can print the case in black! Black is an evil color. It is so evil. It is so evil that it hurts many people that are good!

What's new?  
**24.08.2024** Slightly flash usage improvements. **1.0-280824**.  
-- More news? See the [newsblog](https://github.com/NikolaiRadke/Schimpfolino/tree/main/NEWS.md).   
  

### Notes for non-German Nokolino makers  
  
The building wiki and the curse words are in German. You need English words? **Contribute!**  

### Prerequisites

Exept for the tools and some time, you need:
* Arduino IDE 1.6.6 or higher
* Serial terminal software like *Tera Term* (Windows) or *CuteCom* (Linux).
* Arduino UNO/Nano/Whatever with USB cable
* A breadboard
* Some Dupont wires
* A 10 µF capacitor
  
And the following hardware:
* Schimpfolino PCB 
* ATtiny45/85
* 24AA64 or larger EEPROM (up to 24AA512)
* 0,96" SDD1306 display I2C
* Button 6 x 6 mm, 9,5 mm height
* 2 Dip8 sockets
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
|   STL files to 3D print a case. Other kind of hulls will follow.
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
    |   └── files/
    |       The EEPROM content word lists and the editor.
    |
    ├── Schimpfolino/
    |   Standard sketch with libraries.
    └── Schimpfolino_nopeeprom/
        Sketch for Schimpfolino without EEPROM.
```

### Let's get started!

Ready? Great! Let's start here: [German wiki](https://github.com/NikolaiRadke/Schimpfolino/wiki).  

