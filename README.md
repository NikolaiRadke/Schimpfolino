# ![Schimpfolino](http://www.nikolairadke.de/schimpfolino/schimpfolino_back.jpg)

  
### "Klappriger Blubberfetzen" "Nervendes Riesenungemach" "Windige Pupswindel"  

    
Who doesn't like to be insulted? Schimpfolino is a gadget that always has a saying ready. Small, handy, energy-saving. And completely useless. You need a PCB and some parts listed below.      

*Huch, Englisch? Kann ich nicht. Schnell [hier hin](https://github.com/NikolaiRadke/Schimpfolino/wiki)*.  

  
This repo will be finished soon, stay tuned.  

What's new?  
**17.05.2024** Opened the new repository. Welcome!  
-- More news? See the [newsblog](https://github.com/NikolaiRadke/Schimpfolino/tree/main/NEWS.md).   
  

### Notes for non-German Nokolino makers  
  
The building wiki and the curse words are in German. You need english words? **Contribute!**  

### Prerequisites

Exept for the tools and some time, you need:
* Arduino IDE 1.6.6 or higher
* Serial terminal software like *Tera Term* (Windows) or *CuteCom* (Linux).
* Arduino UNO/Nano/Whatever with USB cable
* Some Dupont wires
* A 10uF capacitor

And the following hardware:
* Schimpfolino PCB 
* ATtiny45/85
* 24LC64 or larger EEPROM
* 0,96" SSD1306 oled display
* One button
* 2 Dip-8 sockets
* Batteryholder
* CR2032 battery

### Content

```
Schimpfolino/
Root directory with some explanation files.  
|
├── case/
|   STL Files to 3D print a case. Other kind of hulls will follow.
|
├── schematics/
|   The circuit diagram, PCB renderings and the KiCad source files.
|   ├── gerber/
|   |   The gerber files for the PCB.
|   └── Schimpfolino_EEPROM_pretty/
|       Footprints for KiCad
|
└── src/
    Firmware and word files.
    ├── eeprom/
    |   All you need to flash your EEPROM.
    |   ├── eeprom_writer/
    |   |   Arduino sketch to write to your EEPROM
    |   └── files/
    |       The EEPROM content word lists.
    |
    ├── Schimpfolino/
    |   Standard sketch with libraries.
    └── Schimpfolino_nopeeprom/
        Sketch for Schimpfolino without EEPROM.

```
### Let's get started!

Ready? Great! Let's start here: [German wiki](https://github.com/NikolaiRadke/Schimpfolino/wiki).  

