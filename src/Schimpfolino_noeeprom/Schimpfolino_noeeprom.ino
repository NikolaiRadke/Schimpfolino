/*  
    Schimpfolino V1.0 05.05.2024 - Nikolai Radke
    https://www.monstermaker.de

    Sketch for the insulting gadget | With or without additional 24LCXX EEPROM
    For ATtiny85 only - set to 8 Mhz set to 8 Mhz and remember to flash your bootloader first!

    Flash usage: 8.104 (IDE 2.3.2 | AVR 1.8.6 | ATtiny 1.0.2 | Linux X86_64 | ATtiny85)
    Power:       5mA (idle) | 7μA (sleep) | 9μA (sleep, with EEPROM)
    Wordlist:    4.800 (120 batches of 10 bytes * 4 arrays | 2 batches free)

    Umlaute in wordlist and EEPROM file have to be converted (UTF-8):
    ä -> # | ö -> $ | ü -> % | ß -> * | Captials are not supported. 

    Wiring:
    1: RST | PB5  Free    
    2: D3  | PB3  Free
    3: A2  | PB4  Free 
    4: GND |      GND
    5: D0  | PB0  SDA    - SDA | SSD1806 
    6: D1  | PB1  Button - GND
    7: D2  | PB2  SCL    - SCL | SSD1806
    8: VCC        VCC

    Needs additional TinyWireM library!
*/

#include <EEPROM.h>
#include <avr/sleep.h>
#include "SSD1306_minimal.h"                     // Modified library!

// Hardware
#define Button   PB1                             // Button address          

// Wordlist arrays - a single arry can hold only 4kb | Used, if no EEPROM present
const char data1[] PROGMEM = {"Dumpfe    Staubige  Miefende  Stinkende Gammlige  Hinkende  Winzige   Popelige  Nasse     Furzende  Rostige   Hohle     Siffige   Miese     Krumme    Klapprige Trockene  Haarige   Uralte    Grunzende SchreiendeMeckernde Nervende  Sabbernde Triefende Modrige   Lumpige   Lausige   Sinnlose  Olle      Unn$tige  Dampfende Ledrige   Einarmige Leere     L#stige   Heulende  Pickelige Faule     Ranzige   Tr%be     Dralle    Blanke    Gierige   Tranige   Wackelnde Torkelnde W%ste     Fischige  Beknackte Modrige   VerkorksteHeimliche L$chrige  Brockige  Plumpe    Tattrige  Ratternde SchmutzigeLiderlicheD$sige    Prollige  Fiese     Dr$ge     Muffige   M%ffelnde Peinliche N$rgelnde Fettige   Zahnlose  Freche    Sch#bige  Piefige   Gummige   Labbrige  Patzige   Pelzige   Reudige   Pekige    M%rbe     Harzige   Lahme     Mickrige  Br#sige   Zottelige Gelbliche Knorrige  Salzige   Schrille  Dusselige Windige   Grausige  Gr#sslicheGrobe     Spackige  Kauzige   Flachsige Fransige  Motzige   Kahle     Niedrige  Keifende  Nichtige  Dr$ge     Fade      Weinende  Sch#bige  N$rgelnde Hibbelige Plockige  Brennende D%rre     Kochende  Knarzende Faltige   SchlammigeBr$ckeligeRissige   Verkeimte Kantige   "};
const char data2[] PROGMEM = {"Stampf    Wabbel    Pups      Schmalz   Schmier   Hack      Zement    Spuck     Stachel   Keller    Laber     Stock     Runzel    Schrumpf  Ekel      Schnodder Matsch    Wurm      Eiter     Speck     Mist      Klotz     W%rg      Lumpen    Schleim   Wurst     Doof      Brat      Schwamm   Kratz     Grotten   Kriech    Gift      Schlabber Reier     G$bel     Knatter   Kleb      Schmadder Grind     Labber    Luft      Massen    Schimmel  Mini      Ochsen    Problem   Quassel   Schnaps   Saft      Fummel    Friemel   Zappel    Tropf     Pluntsch  Sumpf     Hecken    Grab      Schwitz   Schnarch  Schleich  Schluff   Fl$ten    Holz      Kreisch   Dulli     Luschen   Gammel    Alt$l     R$chel    Glibber   Lach      Krach     Knick     Quetsch   Quatsch   Quietsch  Knautsch  T%mpel    Teich     Knatter   Sauf      Pipi      Struller  Gr#ten    Nasen     Pech      Leier     Reier     Bl$d      Schorf    Sabbel    Quengel   Bananen   Unsinns   Plunsch   Frust     Lotter    Fummel    Blubber   Wobbel    Vollbart  Lack      Klo       Moder     Knirsch   Zitter    Kalt      Schl%rf   Schnief   Klecker   Rumba     Schwurbel Schrabbel Schlauch  Schrumpel H%hner    Schlacker Brabbel   Krampf    "};
const char data3[] PROGMEM = {"busch     fink      nagel     bammel    klopper   tentakel  br#gen    schlumpf  husten    ersatz    haufen    beutel    kn$del    r%ssel    hintern   eimer     pickel    stumpf    k#se      molch     kohl      gnubbel   sack      hansel    puller    alptraum  kasten    kopf      beutel    bewohner  kuchen    freund    nascher   opa       rotz      klumpen   peter     hansel    bengel    kollege   fleck     l$ffel    lurch     hobel     spaten    pudel     rettich   rinnstein unfall    lappen    k%bel     mops      pfosten   zwerg     pudding   nuckel    putzer    l%mmel    baron     mop       besen     feudel    br#gen    bolzen    pilz      stiefel   k$ter     gulli     pfropf    schrank   k$nig     pott      fass      rinnstein zinken    haken     witz      buckel    knecht    fan       schmand   klops     gauner    lulli     graupe    pimpf     kasper    spross    teufel    hammel    bock      schmodder pr%gel    spie*er   aal       groschen  geist     rochen    knochen   horst     quark     keks      zausel    iltis     jeck      honk      spargel   nippel    atze      muffel    greis     pin$kel   gehilfe   halunke   lauch     th$le     onkel     klecks    furunkel  auswurf   "};
const char data4[] PROGMEM = {"birne     suppe     socke     bombe     boulette  schwarte  warze     beule     pest      pflaume   r%be      geige     ratte     krankheit wunde     oma       knolle    stulle    liese     brut      henne     zwiebel   bude      kiste     braut     leuchte   kr$te     nuss      spinne    grube     toilette  krake     pf%tze    backe     bimmel    klatsche  nudel     knolle    t%te      nase      made      tonne     krampe    b%rste    windel    semmel    haxe      gr#fin    schleuder zierde    kr#he     latte     niete     rassel    assel     torte     galle     latsche   schrulle  kanone    blase     pelle     trine     queen     zecke     praline   magt      pracht    fritte    so*e      larve     murmel    hexe      pampe     sirene    dr%se     klette    petze     brumme    glatze    qualle    natter    kralle    ziege     gr%tze    s%lze     nulpe     wampe     frikadelleflunder   trulla    zichte    uschi     kuh       pappe     hupe      tr$te     schabe    kanallie  scharte   rille     amsel     alge      l%cke     bremse    m%cke     b%rste    wanne     pocke     pl$rre    schabrackewuppe     kiste     tante     reuse     ratsche   pauke     fluppe    matrone   hummel    "};

// Variables
char     *field;                                 // Pointer to one of the character arrays
uint8_t  gender;                                 // Gender of the swearword
uint16_t batches = 120;                          // Number of wordlist quadruple batches | 120 in Wordlist
uint16_t number, seed;                           // Random seed and helping variable
uint32_t counter;                                // Timer begin for sleep timeout
bool     wake = true;                            // Stay wake when button is pressed
bool     eeprom = false;                         // EEPROM used -> Auto detect

SSD1306_Mini oled;                               // Set display

int main(void) {                                 
  init(); {                                      // Setup
    // Power saving
    ACSR |= _BV(ACD);                            // Disable analog comparator - default?
    ADCSRA &= ~_BV(ADEN);                        // Switch ADC off

    // Port setup
    DDRB &= ~(1 << Button);                      // PB1 button INPUT
    PORTB |= (1 << Button);                      // PB1 INPUT_PULLUP 

    // Hardware interrupt
    cli();                                       // Stop all interrupts
    GIMSK |= (1 << PCIE);                        // Turns on pin change interrupt
    PCMSK |= (1 << PCINT1);                      // Turn on interrupt on PB1 button
    sei();                                       // Start interrupts

    // Main routine - runs after waking up
    while(1) {
      // Init Display
      oled.init(0x3C);                           // Connect OLED via I2C
      oled.startScreen();                        // Display on

      // Look for EEPROM and count batches
      TinyWireM.beginTransmission(0x50);         // Look for 24LCXX Eeprom at 0x50
      if (TinyWireM.endTransmission() == 0) {    // 0x00 for used, 0xff for unused
        eeprom = true;                           // if used, set eeprom flag
        batches = read_eeprom(0) * 255;          // Calculate number batches:
        batches += read_eeprom(1);               // First byte = High, second bye = low
      }

      // Randomize number generator
      number = eeprom_read_word(0);              // Read EEPROM address
      if ((number < 2) || (number > (EEPROM.length() - 3))) {
        // Initialize EEPROM and size for first use or after end of cycle
        number = 2;                              // Starting address
        eeprom_write_word(0, number);            // Write starting address
        eeprom_write_word(number, 0);            // Write seed 0
      }
      seed = eeprom_read_word(number);           // Read seed
      if (seed > 900) {                          // After 900 write-cyles move to another address
        seed = 0;                                // to keep the EEPROM alive
        number += 2;                             // 2 places, adress is a word
        eeprom_write_word(0, number);            // Write address of seed
      }
      seed ++;                                   // New seed
      eeprom_write_word(number, seed);           // Save new seed for next startup
      randomSeed(seed);                          // Randomize number generator

      // Display swearwords until timeout
      while (wake) {                             // Wait 10 seconds timeout
        counter = millis();                      // Set starting time
        oled.clear();                            // Clear display buffer

        // Firt word
        oled.cursorTo(0, 10);                    // Set cursor
        gender = random(0, 2);                   // Set word gender
        number = (random(0, batches) * 10);      // Select first word
        if (eeprom) number += 2;
        field = data1;                           // Vector to first array
        write_swearword(number);                 // Write first word

        // Second word first part
        if (gender == 0) oled.printChar(49);     // If male, write "r"
        oled.cursorTo(0, 20);                    // Next line
        number = (random(0, batches) * 10);      // Select second word
        if (eeprom) number += batches * 10;
        field = data2;                           // Vector to second array
        write_swearword(number);                 // Write second word
        
        // Second word second part
        number = (random(0, batches) * 10);      // Select second part of second word
        if (eeprom) {
          number += batches * 20 +2;
          if (gender == 1) number += batches * 10;
        }
        field = ((gender == 0)? data3 : data4);  // Vector to male or female array
        write_swearword(number);                 // Write second part
        
        // Wait for button or sleep
        delay(500);                              // Debounce button
        wake = false;                            // Set to sleep
        while ((!wake) && (millis() - counter < 10000)); // Wait for button oder timeout
      } 

      // Go to sleep after 10 seconds if button is not pressed before                           
      oled.sendCommand(GOFi2cOLED_Display_Off_Cmd);// Display off
      set_sleep_mode(SLEEP_MODE_PWR_DOWN);       // Deepest sleep mode
      sleep_mode();                              // Good night, sleep until reset
    }
  }
}

void write_swearword(uint16_t address) {
  char c;
  uint16_t i;
  for (i = address; i < address + 10; i ++) {    // Read ten chars        
    if (eeprom) c = read_eeprom(i);              // from EEPROM
    else c = pgm_read_byte(&field[i]);           // Or from wordlist
    if (c != 32) {                               // Check for space
      switch (c) {                               // Print german Umlaute   
        case 35: oled.printChar(27); break;      // # -> ä
        case 36: oled.printChar(28); break;      // $ -> ö
        case 37: oled.printChar(29); break;      // % -> ü
        case 42: oled.printChar(30); break;      // * -> ß
        default: oled.printChar(c - 65);         // Print non-empty character
      }
    }
  } 
}

uint8_t read_eeprom(uint16_t e_address) {        // Read from EEPROM
  TinyWireM.beginTransmission(0x50);             // open transmission to I2C-address 0x50
  TinyWireM.write((uint16_t)(e_address >> 8));   // Send the MSB (Most Significant Byte) of the memory address
  TinyWireM.write((uint16_t)(e_address & 0xFF)); // Send the LSB (Least Significant Byte) of the memory address
  TinyWireM.endTransmission();                   // Close transmissiom
  TinyWireM.requestFrom(0x50,1);                 // Request one byte
  return TinyWireM.read();                       // Read and return byte
}

ISR(PCINT0_vect) {wake = true;}                  // Set wake flag if button is pressed