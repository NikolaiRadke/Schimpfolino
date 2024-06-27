/*  
    Schimpfolino V1.0 14.06.2024 - Nikolai Radke
    https://www.monstermaker.de

    Sketch for the insulting gadget | With or without additional 24LCXX EEPROM
    For ATtiny85 only - set to 8 Mhz | B.O.D disabled | No bootloader
    Remember to burn the "bootloader" first!

    Flash usage: 8.188 (IDE 2.3.2 | ATTinyCore 1.5.2 | Linux X86_64 | ATtiny85)
    Power:       5mA (idle) | 7μA (sleep)

    Umlaute have to be converted (UTF-8):
    ä -> # | ö -> $ | ü -> % | ß -> * | Captial letters are not supported
    Last charakter of a wordlist in EEPROM is '!'

    Wiring:
    1: RST | PB5  Free    
    2: D3  | PB3  Free
    3: A2  | PB4  Free 
    4: GND |      GND
    5: D0  | PB0  SDA    - SDA | SSD1306 
    6: D1  | PB1  Button - GND
    7: D2  | PB2  SCL    - SCL | SSD1306
    8: VCC |      VCC
*/

#include <Wire.h>                                // I2C communication with display and EEPROM
#include <EEPROM.h>                              // Internal EEPROM saves random seed
#include <avr/sleep.h>                           // Used for deep sleep
#include <util/delay.h>
#include "SSD1306_minimal.h"                     // Modified library!

// Hardware
#define Button   PB1                             // Button address   

// Software
#define Timeout  10000                           // 10 seconds before sleep

//Wordlist arrays - a single array can hold only 4000 bytes | Used, if no EEPROM present | 5 x 90 words = 4500 bytes
const char data1[] PROGMEM = {"Dumpfe    Staubige  Miefende  Stinkende Gammlige  Hinkende  Winzige   Popelige  Nasse     Furzende  Rostige   Hohle     Siffige   Miese     Krumme    Klapprige Trockene  Haarige   Uralte    Grunzende SchreiendeMeckernde Nervende  Sabbernde Triefende Modrige   Lumpige   Lausige   Sinnlose  Olle      Unn$tige  Dampfende Ledrige   Einarmige Leere     L#stige   Heulende  Pickelige Faule     Ranzige   Tr%be     Dralle    Blanke    Gierige   Tranige   Wackelnde Torkelnde W%ste     Fischige  Beknackte Modrige   VerkorksteHeimliche L$chrige  Brockige  Plumpe    Tattrige  Ratternde SchmutzigeLiderlicheD$sige    Prollige  Fiese     Dr$ge     Muffige   M%ffelnde Peinliche N$rgelnde Fettige   Zahnlose  Freche    Sch#bige  Piefige   Gummige   Labbrige  Patzige   Pelzige   Reudige   Pekige    M%rbe     Harzige   Lahme     Mickrige  Br#sige   Zottelige Gelbliche Knorrige  Salzige   Schrille  Dusselige "};
const char data2[] PROGMEM = {"Stampf    Wabbel    Pups      Schmalz   Schmier   Hack      Zement    Spuck     Stachel   Keller    Laber     Stock     Runzel    Schrumpf  Ekel      Schnodder Matsch    Wurm      Eiter     Speck     Mist      Klotz     W%rg      Lumpen    Schleim   Wurst     Doof      Brat      Schwamm   Kratz     Grotten   Kriech    Gift      Schlabber Reier     G$bel     Knatter   Kleb      Schmadder Grind     Labber    Luft      Massen    Schimmel  Mini      Ochsen    Problem   Quassel   Schnaps   Saft      Fummel    Friemel   Zappel    Tropf     Pluntsch  Sumpf     Hecken    Grab      Schwitz   Schnarch  Schleich  Schluff   Fl$ten    Holz      Kreisch   Dulli     Luschen   Gammel    Alt$l     R$chel    Glibber   Lach      Krach     Knick     Quetsch   Quatsch   Quietsch  Knautsch  T%mpel    Teich     Knatter   Sauf      Pipi      Struller  Gr#ten    Nasen     Pech      Leier     Reier     Bl$d      "};
const char data3[] PROGMEM = {"suppe     socke     bombe     boulette  schwarte  warze     beule     pest      pflaume   r%be      geige     ratte     krankheit wunde     oma       knolle    stulle    liese     brut      henne     zwiebel   bude      kiste     braut     leuchte   kr$te     nuss      spinne    grube     toilette  krake     pf%tze    backe     bratsche  klatsche  nudel     knolle    t%te      nase      made      tonne     krampe    b%rste    windel    semmel    haxe      gr#fin    schleuder zierde    kr#he     latte     niete     rassel    assel     torte     galle     latsche   schrulle  kanone    blase     pelle     trine     queen     zecke     praline   magt      pracht    fritte    so*e      larve     murmel    hexe      pampe     sirene    dr%se     klette    petze     brumme    glatze    qualle    natter    kralle    ziege     gr%tze    s%lze     nulpe     wampe     frikadelleflunder   trulla    "};
const char data4[] PROGMEM = {"busch     fink      nagel     bammel    klopper   tentakel  br#gen    schlumpf  husten    ersatz    haufen    beutel    kn$del    r%ssel    hintern   eimer     pickel    stumpf    k#se      molch     kohl      gnubbel   sack      hansel    puller    alptraum  kasten    kopf      beutel    bewohner  kuchen    freund    nascher   opa       rotz      klumpen   peter     hansel    bengel    kollege   fleck     l$ffel    lurch     hobel     spaten    pudel     rettich   rinnstein unfall    lappen    k%bel     mops      pfosten   zwerg     pudding   nuckel    putzer    l%mmel    baron     mop       besen     feudel    br#gen    bolzen    pilz      stiefel   k$ter     gulli     pfropf    schrank   k$nig     pott      paddel    rinnstein zinken    haken     witz      buckel    knecht    fan       schmand   klops     gauner    lulli     graupe    pimpf     kasper    spross    teufel    hammel    "};
const char data5[] PROGMEM = {"sekret    balg      blag      monster   gel$t     imitat    skelett   ding      unding    auge      brot      deo       insekt    bier      mus       ende      futter    gew#chs   produkt   ger$ll    bonbon    furunkel  paket     virus     desaster  st%ck     fass      zeug      ferkel    ei        gewitter  hormon    experimentgulasch   schnitzel fell      theater   schauspielbaby      spielzeug gel       donutloch gelee     gelumpe   zeug      schaf     molek%l   gew%rz    gespenst  gespinnst mittel    geschnetz organ     risotto   vieh      ges#*     gez%cht   ekzem     moped     ger%mpel  hirn      gef#*     wachstum  moloch    rinnsaal  gemenge   opossum   frettchen h#hnchen  plankton  untier    unget%m   gebr#u    fondue    beispiel  elend     leid      gift      verderben ungl%ck   drama     trauma    versagen  fiasko    dilemma   debakel   tabu      ger%cht   hindernis dingdong  "};

// Variables
char     *field;                                 // Pointer to one of the character arrays
uint8_t  gender;                                 // Gender of the swearword
uint8_t  chars = 0;                              // Number of charakters in the word | Gobal
uint16_t number, seed;                           // Random seed and helping variable
uint16_t address[5] = {90, 90, 90, 90, 90};      // Wordlists addresses array - overwritten if EEPROM present
uint32_t counter;                                // Timer begin for sleep timeout
char     wordbuffer[20];                         // Buffer for read words
bool     eeprom = false;                         // EEPROM used -> Auto detect

volatile bool wake = true;                       // Stay wake when button is pressed

SSD1306_Mini  oled;                              // Set display

int main(void) {                                 
  init(); {                                      // Setup
    // Power saving
    ACSR |= _BV(ACD);                            // Disable analog comparator - anyway by default?
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
      // Init I2C
      Wire.setClock(400000L);                    // Fast mode
      Wire.begin();                              // Start I2C

      // Init Display
      oled.init();                               // Connect and start OLED via I2C

      // Look for EEPROM and read wordlist addresses if available
      Wire.beginTransmission(0x50);              // Look for 24LCXX EEPROM at 0x50
      if (Wire.endTransmission() == 0) {         // 0x00 for used, 0xff for unused
        eeprom = true;                           // if used, set EEPROM flag
        gender = 0;                              // gender and seed are helping variables here
        for (seed = 0; seed < 5; seed ++) {      // Read numbers of 4 wordlists
          number = read_eeprom(0 + gender) * 255; // Calculate number: 
          number += read_eeprom(1 + gender);     // First byte = High, second bye = low
          address[seed] = number;                // Write word numbers to array 
          gender += 2;                           // Chance number address
        }  
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
      if (seed > 999) {                          // After 1000 write-cyles move to another address
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

        // First word
        number = (random(0, address[0]));        // Select first word
        field = data1;                           // Vector to first array
        get_swearword(number);                   // Read first word 
        write_swearword(2);                      // Write first word

        // Second word first part
        seed = 0;                                // Set start address for array
        gender = random(0, 3);                   // Set word gender
        if (gender != 0) oled.printChar(48 + gender); // If male, write "r", if neutrum, write "s"
        if (eeprom) seed = address[0];           // Set start adress for EEPROM
        number = (random(seed, address[1]));     // Select second part of second word
        field = data2;                           // Vector to second array
        get_swearword(number);                   // Read first part of second word 
        
        // Second word second part
        if (eeprom) seed = address[gender + 1];  // Set start address for EEPROM
        number = (random(seed, address[gender + 2])); // Select second part of second word
        field = data3;                           // Female
        if (gender == 1) field = data4;          // Male
        if (gender == 2) field = data5;          // Neutrum
        get_swearword(number);                   // Read second part of second word
        write_swearword(4);                      // Write second word in second line
        
        // Wait for button or sleep
        _delay_ms(500);                          // Debounce button
        wake = false;                            // Set to sleep
        while ((!wake) && (millis() - counter < Timeout)); // Wait for button oder timeout
      } 

      // Go to sleep after 10 seconds if button is not pressed before                           
      oled.sendCommand(0xAE);                    // Display off and sleep
      set_sleep_mode(SLEEP_MODE_PWR_DOWN);       // Deepest sleep mode
      sleep_mode();                              // Good night, sleep until interrupt
    }
  }
}

// Functions
void get_swearword(uint16_t address) {           // Fetch characters from EEPROM array
  char c;
  uint16_t i;
  address *= 10;
  for (i = address; i < address + 10; i ++) {    // Read 10 characters        
    c = pgm_read_byte(&field[i]);                // From wordlist
    if (eeprom) c = read_eeprom(i + 10);         // Or from EEPROM with address memory offset
    if (c != 32) {                               // Check for space
      switch (c) {                               // Print german Umlaute   
        case 35: wordbuffer[chars] = 27; break;  // # -> ä
        case 36: wordbuffer[chars] = 28; break;  // $ -> ö
        case 37: wordbuffer[chars] = 29; break;  // % -> ü
        case 42: wordbuffer[chars] = 30; break;  // * -> ß
        default: wordbuffer[chars] = c - 65;     // Set non-empty character
      }
    chars ++;                                    // Increase number of fetched characters
    }
  } 
}

void write_swearword(uint8_t line) {             // Write centered word
  uint8_t x;
  x = (128 - (chars * 7)) / 2;                   // Calculate centering
  if (chars > 18)  x = (128 - (chars * 6)) / 2;  // or for very long words
  if ((gender != 0) && (line == 2)) x -= 4;      // If not female, set first one half block left for gender char
  oled.cursorTo(x, line);                        // Set cursor to selected line
  for (x = 0; x < chars; x ++)                   // Print the characters
    oled.printChar(wordbuffer[x]);               // from buffer
  chars = 0;                                     // Set number of character back to 0
}

uint8_t read_eeprom(uint16_t e_address) {        // Read from EEPROM
  Wire.beginTransmission(0x50);                  // open transmission to I2C-address 0x50
  Wire.write((uint16_t)(e_address >> 8));        // Send the MSB (Most Significant Byte) of the memory address
  Wire.write((uint16_t)(e_address & 0xFF));      // Send the LSB (Least Significant Byte) of the memory address
  Wire.endTransmission();                        // Close transmissiom
  Wire.requestFrom(0x50,1);                      // Request one byte
  return Wire.read();                            // Read and return byte
}

ISR(PCINT0_vect) {wake = true;}                  // Interrupt routine. Set wake flag if button is pressed