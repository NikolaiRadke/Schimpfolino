/*  
    Schimpfolino V1.2 30.10.2024 - Nikolai Radke
    https://www.monstermaker.de
    Next version for new improvements. Compatible with older versions.

    Sketch for the insulting gadget | With or without additional 24LAAXX EEPROM
    For ATtiny85 only - set to 8 MHz | B.O.D disabled | No bootloader | No millis()
    Remember to burn the "bootloader" (IDE is setting fuses) first!

    Flash usage: 7.964 bytes (IDE 2.3.3 | ATTinyCore 1.5.2 | Linux X86_64 | ATtiny85)
    Power:       1.6 mA (display on, no EEPROM) | ~ 200 nA (sleep)

    Umlaute have to be converted (UTF-8):
    ä -> # | ö -> $ | ü -> % | ß -> * | Ä -> & | Ö -> ' | Ü -> (
    Last character of a wordlist is "!" 

    Wiring:
                         +-\/-+
    RST          - PB5  1|    |8  VCC - Battery
    Free         - PB3  2|    |7  PB2 - SCL
    Devices VCC  - PB4  3|    |6  PB1 - Button -> GND
    GND          - GND  4|    |5  PB0 - SDA
                         +----+
*/

#include <util/delay.h>                          // Needs less flash memory than delay()
#include "TinyI2CMaster.h"                       // I2C communication with display and EEPROM. Very tight library!
#include "oled.h"                                // OLED display library for SSD1306 and SH1106

// Hardware
#define  BUTTON   PB1                            // Button pin
#define  DEVICES  PB4                            // External devices power pin

// Wordlist arrays - a single array can hold only 4000 bytes | Used if no EEPROM present | 5 x 108 words = 5.500 bytes
const char data1[] PROGMEM = {"Dumpfe    Staubige  Miefende  Stinkende Gammlige  Hinkende  Winzige   Popelige  Nasse     Furzende  Rostige   Hohle     Siffige   Miese     Krumme    Klapprige Trockene  Haarige   Uralte    Grunzende SchreiendeMeckernde Nervende  Sabbernde Triefende Modrige   Lumpige   Lausige   Sinnlose  Olle      Unn$tige  Dampfende Ledrige   Einarmige Leere     L#stige   Heulende  Pickelige Faule     Ranzige   Tr%be     Dralle    Blanke    Gierige   Tranige   Wackelnde Torkelnde W%ste     Fischige  Beknackte Modrige   VerkorksteHeimliche L$chrige  Brockige  Plumpe    Tattrige  Ratternde SchmutzigeLiderlicheD$sige    Prollige  Fiese     Dr$ge     Muffige   M%ffelnde Peinliche N$rgelnde Fettige   Zahnlose  Freche    Sch#bige  Piefige   Gummige   Labbrige  Patzige   Pelzige   Reudige   Pekige    M%rbe     Harzige   Lahme     Mickrige  Br#sige   Zottelige Gelbliche Knorrige  Salzige   Schrille  Dusselige Windige   Grausige  Gr#sslicheGrobe     Spackige  Kauzige   Flachsige Fransige  Motzige   Kahle     Niedrige  Keifende  Nichtige  Dr$ge     Fade      Weinende  Sch#bige  N$rgelnde "};
const char data2[] PROGMEM = {"Stampf    Wabbel    Pups      Schmalz   Schmier   Hack      Zement    Spuck     Stachel   Keller    Laber     Stock     Runzel    Schrumpf  Ekel      Schnodder Matsch    Wurm      Eiter     Speck     Mist      Klotz     W%rg      Lumpen    Schleim   Wurst     Doof      Brat      Schwamm   Kratz     Grotten   Kriech    Gift      Schlabber Reier     G$bel     Knatter   Kleb      Schmadder Grind     Labber    Luft      Massen    Schimmel  Mini      Ochsen    Problem   Quassel   Schnaps   Saft      Fummel    Friemel   Zappel    Tropf     Pluntsch  Sumpf     Hecken    Grab      Schwitz   Schnarch  Schleich  Schluff   Fl$ten    Holz      Kreisch   Dulli     Luschen   Gammel    Alt$l     R$chel    Glibber   Lach      Krach     Knick     Quetsch   Quatsch   Quietsch  Knautsch  T%mpel    Teich     Knatter   Sauf      Pipi      Struller  Gr#ten    Nasen     Pech      Leier     Reier     Bl$d      Schorf    Sabbel    Quengel   Bananen   Unsinns   Plunsch   Frust     Lotter    Fummel    Blubber   Wobbel    Vollbart  Lack      Klo       Moder     Knirsch   Zitter    Kalt      "};
const char data3[] PROGMEM = {"suppe     socke     bombe     boulette  schwarte  warze     beule     pest      pflaume   r%be      geige     ratte     krankheit wunde     oma       knolle    stulle    liese     brut      henne     zwiebel   bude      kiste     braut     leuchte   kr$te     nuss      spinne    grube     toilette  krake     pf%tze    backe     bratsche  klatsche  nudel     knolle    t%te      nase      made      tonne     krampe    b%rste    windel    semmel    haxe      gr#fin    schleuder zierde    kr#he     latte     niete     rassel    assel     torte     galle     latsche   schrulle  kanone    blase     pelle     trine     queen     zecke     praline   magt      pracht    fritte    so*e      larve     murmel    hexe      pampe     sirene    dr%se     klette    petze     brumme    glatze    qualle    natter    kralle    ziege     gr%tze    s%lze     nulpe     wampe     frikadelleflunder   trulla    zichte    uschi     kuh       pappe     hupe      tr$te     schabe    kanallie  scharte   rille     amsel     alge      l%cke     bremse    m%cke     b%rste    wanne     pocke     "};
const char data4[] PROGMEM = {"busch     fink      nagel     bammel    klopper   tentakel  br#gen    schlumpf  husten    ersatz    haufen    beutel    kn$del    r%ssel    hintern   eimer     pickel    stumpf    k#se      molch     kohl      gnubbel   sack      hansel    puller    alptraum  kasten    kopf      beutel    bewohner  kuchen    freund    nascher   opa       rotz      klumpen   peter     hansel    bengel    kollege   fleck     l$ffel    lurch     hobel     spaten    pudel     rettich   rinnstein unfall    lappen    k%bel     mops      pfosten   zwerg     pudding   nuckel    putzer    l%mmel    baron     mop       besen     feudel    br#gen    bolzen    pilz      stiefel   k$ter     gulli     pfropf    schrank   k$nig     pott      paddel    rinnstein zinken    haken     witz      buckel    knecht    fan       schmand   klops     gauner    lulli     graupe    pimpf     kasper    spross    teufel    hammel    bock      schmodder pr%gel    spie*er   aal       groschen  geist     rochen    knochen   horst     quark     keks      zausel    iltis     jeck      honk      spargel   nippel    "};
const char data5[] PROGMEM = {"sekret    balg      blag      monster   gel$t     imitat    skelett   ding      unding    auge      brot      deo       insekt    bier      mus       ende      futter    gew#chs   produkt   ger$ll    bonbon    furunkel  paket     virus     desaster  st%ck     fass      zeug      ferkel    ei        gewitter  hormon    experimentgulasch   schnitzel fell      theater   schauspielbaby      spielzeug gel       donutloch gelee     gelumpe   zeug      schaf     molek%l   gew%rz    gespenst  gespinnst mittel    geschnetz organ     risotto   vieh      ges#*     gez%cht   ekzem     moped     ger%mpel  hirn      gef#*     wachstum  moloch    rinnsaal  gemenge   opossum   frettchen h#hnchen  plankton  untier    unget%m   gebr#u    fondue    beispiel  elend     leid      gift      verderben ungl%ck   drama     trauma    versagen  fiasko    dilemma   debakel   tabu      ger%cht   hindernis dingdong  dingsbums gewicht   abwasser  abbild    modell    gem#lde   brett     geballer  gem#cht   toupet    geschw#tz gerippe   pech      leiden    verbrechenfossil    symptom   biest     "};

// Grafikdaten für Rahmen
const unsigned char rahmen_mini[] PROGMEM = {
  0b00000000, //  0 Part 1 - Page 0 und 7, Spalte 0 - 21 und 106 - 127
  0b00000000, //  1
  0b10000000, //  2
  0b10000000, //  3
  0b11100000, //  4
  0b11110000, //  5
  0b00110000, //  6
  0b10110000, //  7
  0b10100000, //  8
  0b11100000, //  9
  0b11100000, // 10
  0b01110000, // 11
  0b00111100, // 12
  0b00000011, // 13
  0b01101010, // 14
  0b01111110, // 15
  0b11001110, // 16
  0b11010110, // 17
  0b01110010, // 18
  0b00110101, // 19
  0b00100101, // 20
  0b00100101, // 21
  0b00000101, // 22 horizontaler Rand - Page 0 und 7, Spalte 22 - 83
  0b00000000, // 23 Part 2 - Page 1 und 6, Spalte 0 - 9 und 118 - 127
  0b00000011, // 24
  0b00110111, // 25
  0b11110110, // 26
  0b00011110, // 27
  0b11111101, // 28
  0b00000111, // 29
  0b00000001, // 30
  0b00000001, // 31
  0b00000001, // 32
  0b11110000, // 33 Part 3  - Page 2 und 5, Spalte 0 - 7 und 120 - 127
  0b00111100, // 34
  0b11001111, // 36
  0b00000111, // 37
  0b11110011, // 38
  0b10100101, // 39
  0b00111110, // 40
  0b00011100, // 41
  0b11111111, // 42 vertikaler Rand - Page 3 und 4, Spalte 0 - 2 und 125 - 127
  0b00000000, // 43
  0b11111111  // 44
};

// Variables
char     *field;                                 // Pointer to one of the character arrays
uint8_t  genus = 0;                              // Genus of the swearword
uint8_t  chars = 0;                              // Number of characters in the word | Gobal
uint16_t list;                                   // Variable for parsing word lists
uint16_t number;                                 // Variable for calculating addresses and selecting words
uint16_t addresses[5] = {108, 108, 108, 108, 108}; // Wordlists addresses array - overwritten if EEPROM is present
char     wordbuffer[20];                         // Buffer for read words
bool     eeprom = false;                         // No EEPROM used -> Auto detect

volatile bool awake = false;                     // Stay wake when button is pressed

int main(void) {                                 
  init(); {                                      // Setup
    // Power saving
    ADCSRA &= ~(1 << ADEN);                      // Switch ADC off | Saves 270 uA
    MCUCR = (1 << SM1) | (0 << SM0);             // Always deepest sleep mode (Power-down)

    // Port setup
    DDRB = (1 << DEVICES) | (1 << SDA) | (1 << SCL); // Set PB4 to OUTPUT to power up display and EEPROM | SDA and SCL for I2C
    PORTB = 0x3F;                                // Set output ports to HIGH and input ports to INPUT_PULLUP to prevent floating

    // Hardware pin and watchdog interrupt
    cli();                                       // Stop all interrupts. An interrupt can ruin the timed sequence
    GIMSK |= (1 << PCIE);                        // Turn on pin change interrupt
    PCMSK |= (1 << PCINT1);                      // Turn on interrupt on PB1 button
    MCUSR &= ~(1 << WDRF);                       // No watchdog reset 
    WDTCR |= (1 << WDCE) | (1 << WDE);           // Watchdog change enable
    WDTCR = (1 << WDP0) | (1 << WDP3);           // Set prescaler to 8 s
    sei();                                       // Start interrupts

    // Init I2C
    TinyI2C.init();                              // Start I2C

    // Look for EEPROM and read wordlist addresses if available | genus is a helping variable here 
    if (TinyI2C.start(0x50, 0)) {                // Look for 24LCXX EEPROM at 0x50
      eeprom = true;                             // if available, set EEPROM flag
      for (list = 0; list < 5; list ++) {        // Read numbers of 5 wordlists
        number = read_eeprom(0 + genus) * 255;   // Calculate number: 
        number += read_eeprom(1 + genus);        // First byte = high, second byte = low
        addresses[list] = number;                // Write word numbers to array 
        genus += 2;                              // Change number address
      }  
    }

    // Randomize number generator
    PORTB &= ~(1 << DEVICES);                    // Devices off
    sleep();                                     // Sleep until button is pressed to "turn on"
    TCCR0A = 0x00;                               // Set timer 0 to normal mode
    TCCR0B = (1 << CS00);                        // Set prescaler to 1 to start the timer
    while (!(PINB & (1 << BUTTON)));             // Wait until button is released
    randomSeed(TCNT0);                           // Get a time count as seed

    // Main routine | Runs after waking up
    while(1) {
      // Init Display
      PORTB |= (1 << DEVICES);                   // Devices on
      PRR |= (1 << PRTIM0) | (1 << PRTIM1);      // Both timers are not needed anymore | Saves 100 uA when active
      Oled_init();                               // Connect and start OLED via I2C

      // Display swearwords until timeout
      while (awake) {                            // Wait 8.5 seconds timeout
        Oled_clear(rahmen_mini);                 // Clear display buffer and fill with frame gfx

        // First word
        number = (random(0, addresses[0]));      // Select first word
        field = data1;                           // Pointer to first array
        get_swearword(number);                   // Read first word 
        genus = random(0, 3);                    // Set word genus
        if (genus != 0) {                        // If male, write "r", if neutrum, write "s"
          wordbuffer[chars] = 48 + genus;
          chars++;
        } 
        write_swearword(2);                      // Write first word in the first line

        // Second word first part
        list = 0;                                // Set start address for array
        if (eeprom) list = addresses[0];         // Set start address for EEPROM
        number = (random(list, addresses[1]));   // Select second part of second word
        field = data2;                           // Pointer to second array
        get_swearword(number);                   // Read first part of second word 
        
        // Second word second part
        if (eeprom) list = addresses[genus + 1]; //  Set start adress for EEPROM
        number = (random(list, addresses[genus + 2])); // Select second part of second word
        field = data3;                           // Pointer to female array
        if (genus == 1) field = data4;           // Pointer to male array
        if (genus == 2) field = data5;           // Pointer to neutrum array
        get_swearword(number);                   // Read second part of second word
        write_swearword(4);                      // Write second word in second line
        
         // Wait for button and sleep 8s
        _delay_ms(500);                          // Debounce button
        awake = false;                           // Set to sleep
        WDTCR |= (1 << WDIE);                    // Set watchdog interrupt
        sleep();                                 // Sleep 8 s or wake when button is pressed
        WDTCR &= ~(1 << WDIE);                   // Stop watchdog interrupt
      } 

      // Go to sleep after 8s seconds if button is not pressed before                           
      Oled_sendCommand(0xAE);                    // Display off and sleep (old boards)
      PORTB &= ~(1 << DEVICES);                  // Devices off
      sleep();                                   // Sleep until button is pressed
    }
  }
}

// Functions
void get_swearword(uint16_t address) {           // Fetch characters from EEPROM
  char c;                                        // Helping variable for fetched character
  uint16_t i;                                    // Helping variable for 10 readings
  address *= 10;                                 // Each address has 10 characters
  for (i = address; i < address + 10; i ++) {    // Read 10 characters...        
    c = pgm_read_byte(&field[i]);                // ...from wordlist...
    if (eeprom) c = read_eeprom(i + 10);         // ...or from EEPROM with address memory offset
    if (c != 32) {                               // Check for space
      switch (c) {                               // Set German Umlaute   
        case 35: wordbuffer[chars] = 27; break;  // # -> ä
        case 36: wordbuffer[chars] = 28; break;  // $ -> ö
        case 37: wordbuffer[chars] = 29; break;  // % -> ü
        case 38: wordbuffer[chars] = 58; break;  // & -> Ä
        case 39: wordbuffer[chars] = 59; break;  // ' -> Ö
        case 40: wordbuffer[chars] = 60; break;  // ' -> Ü
        case 42: wordbuffer[chars] = 30; break;  // * -> ß
        default: wordbuffer[chars] = c - 65;     // Set non-empty character
      }
    chars ++;                                    // Increase number of fetched characters
    }
  } 
}

void write_swearword(uint8_t line) {             // Write centered word
  uint8_t x = 7;                                 // Helping variable for the x position on display
  if (chars > 17) {                              // Modify for very long words
    x = 6; 
  }

  x = (128 - (chars * x) - (x - 5)) / 2;         // Calculate centering
  Oled_cursorTo(x, line);                        // Set cursor to selected line
  for (x = 0; x < chars; x ++)                   // Print the characters...
    Oled_printChar(wordbuffer[x]);               // ...from buffer
  chars = 0;                                     // Set number of characters back to 0 
}

uint8_t read_eeprom(uint16_t e_address) {        // Read from EEPROM
  TinyI2C.start(0x50, 0);                        // Open connection to I2C-address 0x50 in write mode
  TinyI2C.write(e_address >> 8);                 // Send the MSB (Most Significant Byte) of the memory address
  TinyI2C.write(e_address & 0xFF);               // Send the LSB (Least Significant Byte) of the memory address
  TinyI2C.start(0x50, 1);                        // Restart connection for reading one byte
  return TinyI2C.read();                         // Read and return byte
}

void sleep() {
  MCUCR |= (1 << SE);                            // Set SE (sleep Enable) bit
  __asm__ __volatile__ ( "sleep" "\n\t" :: );    // Sleep now!!
  MCUCR &= ~(1 << SE);                           // CLear SE bit
}

ISR(PCINT0_vect) {                               // Interrupt routine for pin change 
  awake = true;                                  // Set awake flag when button is pressed
}

ISR(WDT_vect) {}                                 // Interrupt routine for watchdog. Unused but mandatory                               