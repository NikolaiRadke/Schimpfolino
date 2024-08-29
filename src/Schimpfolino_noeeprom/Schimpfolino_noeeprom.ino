/*  
    Schimpfolino V1.0 29.09.2024 - Nikolai Radke
    https://www.monstermaker.de

    Sketch for the insulting gadget | With or without additional 24LAAXX EEPROM
    For ATtiny85 only - set to 8 MHz | B.O.D disabled | No bootloader
    Remember to burn the "bootloader" (IDE is setting fuses) first!

    Flash usage: 7.940 Bytes (IDE 2.3.2 | ATTinyCore 1.5.2 | Linux X86_64 | ATtiny85)
    Power:       1.6 mA (active) | ~ 200 nA (sleep)

    Umlaute have to be converted (UTF-8):
    ä -> # | ö -> $ | ü -> % | ß -> * | Captial letters are not supported
    Last character of a wordlist is "!" 

    Wiring:
                         +-\/-+
    RST          - PB5  1|    |8  VCC - Battery
    Free         - PB3  2|    |7  PB2 - SCL
    Devices VCC  - PB4  3|    |6  PB1 - Button -> GND
    GND          - GND  4|    |5  PB0 - SDA
                         +----+
*/

#include <avr/sleep.h>                           // Used for deep sleep
#include <util/delay.h>                          // Needs less flash memory than delay()
#include "SSD1306_minimal.h"                     // Modified library!
#include <Wire.h>                                // I2C communication with display and EEPROM

// Hardware
#define  BUTTON   PB1                            // Button pin
#define  DEVICES  PB4                            // External devices power pin

// Wordlist arrays - a single array can hold only 4000 bytes | Used if no EEPROM present | 5 x 90 words = 4500 bytes
const char data1[] PROGMEM = {"Dumpfe    Staubige  Miefende  Stinkende Gammlige  Hinkende  Winzige   Popelige  Nasse     Furzende  Rostige   Hohle     Siffige   Miese     Krumme    Klapprige Trockene  Haarige   Uralte    Grunzende SchreiendeMeckernde Nervende  Sabbernde Triefende Modrige   Lumpige   Lausige   Sinnlose  Olle      Unn$tige  Dampfende Ledrige   Einarmige Leere     L#stige   Heulende  Pickelige Faule     Ranzige   Tr%be     Dralle    Blanke    Gierige   Tranige   Wackelnde Torkelnde W%ste     Fischige  Beknackte Modrige   VerkorksteHeimliche L$chrige  Brockige  Plumpe    Tattrige  Ratternde SchmutzigeLiderlicheD$sige    Prollige  Fiese     Dr$ge     Muffige   M%ffelnde Peinliche N$rgelnde Fettige   Zahnlose  Freche    Sch#bige  Piefige   Gummige   Labbrige  Patzige   Pelzige   Reudige   Pekige    M%rbe     Harzige   Lahme     Mickrige  Br#sige   Zottelige Gelbliche Knorrige  Salzige   Schrille  Dusselige "};
const char data2[] PROGMEM = {"Stampf    Wabbel    Pups      Schmalz   Schmier   Hack      Zement    Spuck     Stachel   Keller    Laber     Stock     Runzel    Schrumpf  Ekel      Schnodder Matsch    Wurm      Eiter     Speck     Mist      Klotz     W%rg      Lumpen    Schleim   Wurst     Doof      Brat      Schwamm   Kratz     Grotten   Kriech    Gift      Schlabber Reier     G$bel     Knatter   Kleb      Schmadder Grind     Labber    Luft      Massen    Schimmel  Mini      Ochsen    Problem   Quassel   Schnaps   Saft      Fummel    Friemel   Zappel    Tropf     Pluntsch  Sumpf     Hecken    Grab      Schwitz   Schnarch  Schleich  Schluff   Fl$ten    Holz      Kreisch   Dulli     Luschen   Gammel    Alt$l     R$chel    Glibber   Lach      Krach     Knick     Quetsch   Quatsch   Quietsch  Knautsch  T%mpel    Teich     Knatter   Sauf      Pipi      Struller  Gr#ten    Nasen     Pech      Leier     Reier     Bl$d      "};
const char data3[] PROGMEM = {"suppe     socke     bombe     boulette  schwarte  warze     beule     pest      pflaume   r%be      geige     ratte     krankheit wunde     oma       knolle    stulle    liese     brut      henne     zwiebel   bude      kiste     braut     leuchte   kr$te     nuss      spinne    grube     toilette  krake     pf%tze    backe     bratsche  klatsche  nudel     knolle    t%te      nase      made      tonne     krampe    b%rste    windel    semmel    haxe      gr#fin    schleuder zierde    kr#he     latte     niete     rassel    assel     torte     galle     latsche   schrulle  kanone    blase     pelle     trine     queen     zecke     praline   magt      pracht    fritte    so*e      larve     murmel    hexe      pampe     sirene    dr%se     klette    petze     brumme    glatze    qualle    natter    kralle    ziege     gr%tze    s%lze     nulpe     wampe     frikadelleflunder   trulla    "};
const char data4[] PROGMEM = {"busch     fink      nagel     bammel    klopper   tentakel  br#gen    schlumpf  husten    ersatz    haufen    beutel    kn$del    r%ssel    hintern   eimer     pickel    stumpf    k#se      molch     kohl      gnubbel   sack      hansel    puller    alptraum  kasten    kopf      beutel    bewohner  kuchen    freund    nascher   opa       rotz      klumpen   peter     hansel    bengel    kollege   fleck     l$ffel    lurch     hobel     spaten    pudel     rettich   rinnstein unfall    lappen    k%bel     mops      pfosten   zwerg     pudding   nuckel    putzer    l%mmel    baron     mop       besen     feudel    br#gen    bolzen    pilz      stiefel   k$ter     gulli     pfropf    schrank   k$nig     pott      paddel    rinnstein zinken    haken     witz      buckel    knecht    fan       schmand   klops     gauner    lulli     graupe    pimpf     kasper    spross    teufel    hammel    "};
const char data5[] PROGMEM = {"sekret    balg      blag      monster   gel$t     imitat    skelett   ding      unding    auge      brot      deo       insekt    bier      mus       ende      futter    gew#chs   produkt   ger$ll    bonbon    furunkel  paket     virus     desaster  st%ck     fass      zeug      ferkel    ei        gewitter  hormon    experimentgulasch   schnitzel fell      theater   schauspielbaby      spielzeug gel       donutloch gelee     gelumpe   zeug      schaf     molek%l   gew%rz    gespenst  gespinnst mittel    geschnetz organ     risotto   vieh      ges#*     gez%cht   ekzem     moped     ger%mpel  hirn      gef#*     wachstum  moloch    rinnsaal  gemenge   opossum   frettchen h#hnchen  plankton  untier    unget%m   gebr#u    fondue    beispiel  elend     leid      gift      verderben ungl%ck   drama     trauma    versagen  fiasko    dilemma   debakel   tabu      ger%cht   hindernis dingdong  "};

// Variables
char     *field;                                 // Pointer to one of the character arrays
uint8_t  genus;                                  // Genus of the swearword
uint8_t  chars = 0;                              // Number of characters in the word | Gobal
uint8_t  list;                                   // Helping variable for parsing word lists
uint16_t number;                                 // Helping variable for calculating addresses and selecting words
uint16_t address[5] = {90, 90, 90, 90, 90};      // Wordlists addresses array - overwritten if EEPROM is present
char     wordbuffer[20];                         // Buffer for read words
bool     eeprom = false;                         // No EEPROM used -> Auto detect

volatile bool awake = false;                     // Stay wake when button is pressed

SSD1306_Mini  oled;                              // Set display

int main(void) {                                 
  init(); {                                      // Setup
    // Power saving
    ADCSRA = 0;                                  // Switch ADC off | saves 270 uA
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);         // Always deepest sleep mode

    // Port setup
    DDRB  |= (1 << DEVICES);                     // Set PB4 to OUTPUT to power up display and EEPROM
    PORTB = 0x3F;                                // Set all ports to INPUT_PULLUP to prevent floating and start devices

    // Hardware and watchdog interrupt
    cli();                                       // Stop all interrupts. An interrupt can ruin the timed sequence
    GIMSK |= (1 << PCIE);                        // Turn on pin change interrupt
    PCMSK |= (1 << PCINT1);                      // Turn on interrupt on PB1 button
    MCUSR &= ~(1 << WDRF);                       // No watchdog reset 
    WDTCR |= (1 << WDCE) | (1 << WDE);           // Watchdog change enable
    WDTCR = (1 << WDP0) | (1 << WDP3);           // Set prescaler to  8s
    sei();                                       // Start interrupts

    // Init I2C
    Wire.setClock(400000L);                      // Fast mode (400 kHz)
    Wire.begin();                                // Start I2C

    // Look for EEPROM and read wordlist addresses if available
    Wire.beginTransmission(0x50);                // Look for 24LCXX EEPROM at 0x50
    if (Wire.endTransmission() == 0) {           // 0x00 for available, 0xFF for not found
      eeprom = true;                             // if available, set EEPROM flag
      genus = 0;                                 // genus and list are helping variables here
      for (list = 0; list < 5; list ++) {        // Read numbers of 4 wordlists
        number = read_eeprom(0 + genus) * 255;   // Calculate number: 
        number += read_eeprom(1 + genus);        // First byte = High, second byte = low
        address[list] = number;                  // Write word numbers to array 
        genus += 2;                              // Change number address
      }  
    }

    // Randomize number generator
    PORTB &= ~(1 << DEVICES);                    // Devices off
    sleep_mode();                                // Sleep until button is pressed to "turn on"
    _delay_ms(5);                                // Wait to settle ports
    while (!(PINB & (1 << BUTTON)));             // Wait until button is released
    randomSeed(millis());                        // Time passed is used for random numbers
    PRR |= (1 << PRTIM0) | (1 << PRTIM1);        // Both timers are not needed anymore | saves 100 uA when active


    // Main routine - runs after waking up
    while(1) {
      // Init Display
      PORTB |= (1 << DEVICES);                   // Devices on
      oled.init();                               // Connect and start OLED via I2C

      // Display swearwords until timeout
      while (awake) {                            // Wait 8.5 seconds timeout
        oled.clear();                            // Clear display buffer

        // First word
        number = (random(0, address[0]));        // Select first word
        field = data1;                           // Pointer to first array
        get_swearword(number);                   // Read first word 
        write_swearword(2);                      // Write first word
        genus = random(0, 3);                    // Set word genus
        if (genus != 0) oled.printChar(48 + genus); // If male, write "r", if neutrum, write "s"

        // Second word first part
        list = 0;                                // Set start address for array
        if (eeprom) list = address[0];           // Set start adress for EEPROM
        number = (random(list, address[1]));     // Select second part of second word
        field = data2;                           // Pointer to second array
        get_swearword(number);                   // Read first part of second word 
        
        // Second word second part
        if (eeprom) list = address[genus + 1];   // Set start address for EEPROM
        number = (random(list, address[genus + 2])); // Select second part of second word
        field = data3;                           // Pointer to female array
        if (genus == 1) field = data4;           // Pointer to male array
        if (genus == 2) field = data5;           // Pointer to neutrum array
        get_swearword(number);                   // Read second part of second word
        write_swearword(4);                      // Write second word in second line
        
         // Wait for button and sleep 8s
        _delay_ms(500);                          // Debounce button
        awake = false;                           // Set to sleep
        WDTCR |= (1 << WDIE);                    // Set watchdog interrupt
        sleep_mode();                            // Sleep 8 s or wake when button is pressed
        WDTCR &= ~(1 << WDIE);                   // Stop watchdog interrupt
      } 

      // Go to sleep after 8s seconds if button is not pressed before                           
      oled.sendCommand(0xAE);                    // Display off and sleep (old boards)
      PORTB &= ~(1 << DEVICES);                  // Devices off
      sleep_mode();                              // Sleep until button is pressed
    }
  }
}

// Functions
void get_swearword(uint16_t address) {           // Fetch characters from EEPROM array
  char c;                                        // Helping variable for fetched character
  uint16_t i;                                    // Helping variable for 10 readings
  address *= 10;                                 // Each address has 10 characters
  for (i = address; i < address + 10; i ++) {    // Read 10 characters...        
    c = pgm_read_byte(&field[i]);                // ...from wordlist...
    if (eeprom) c = read_eeprom(i + 10);         // ...or from EEPROM with address memory offset
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
  uint8_t x;                                     // Helping variable for the x position on display
  x = (128 - (chars * 7)) / 2;                   // Calculate centering
  if (chars > 18)  x = (128 - (chars * 6)) / 2;  // or for very long words
  if ((genus != 0) && (line == 2)) x -= 4;       // If not female, set first one half block left for genus character
  oled.cursorTo(x, line);                        // Set cursor to selected line
  for (x = 0; x < chars; x ++)                   // Print the characters
    oled.printChar(wordbuffer[x]);               // from buffer
  chars = 0;                                     // Set number of characters back to 0 
}

uint8_t read_eeprom(uint16_t e_address) {        // Read from EEPROM
  Wire.beginTransmission(0x50);                  // Open transmission to I2C-address 0x50
  Wire.write(e_address >> 8);                    // Send the MSB (Most Significant Byte) of the memory address
  Wire.write(e_address & 0xFF);                  // Send the LSB (Least Significant Byte) of the memory address
  Wire.endTransmission();                        // Close transmissiom
  Wire.requestFrom(0x50,1);                      // Request one byte
  return Wire.read();                            // Read and return byte
}

ISR(PCINT0_vect) {                               // Interrupt routine for pin change 
  awake = true;                                  // Set awake flag when button is pressed
}

ISR(WDT_vect) {}                                 // Interrupt routine for watchdog. Unused but mandatory                               