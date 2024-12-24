// Wordlist arrays - a single array can hold only 4000 bytes | Used if no EEPROM present | 5 x 110 words = 5.500 bytes
// Umlaute have to be converted (UTF-8): ä -> # | ö -> $ | ü -> % | ß -> * | Ä -> & | Ö -> ' | Ü -> (
// Beat Gloor Wordlist

// First word - adjective
const char data1[] PROGMEM = {"&tzend    Billig    Diffus    Furchtbar Harzig    Komisch   Mickrig   Piefig    Sch#ndlichTierisch  "
                              "'lig      Bizarr    Doppelt   Furzend   Heillos   Korrupt   Miefend   Pimpelig  Schauder$sTorkelnd  "
                              "Aalglatt  Blank     Dr$g      GackeierndHeimlich  Krank     Mies      Platt     Schaurig  Total     "
                              "Abartig   Blasiert  Drall     Gammlig   Heulend   Krass     Minder    Plockig   Scheckig  Tr%b      "
                              "Abgefeimt Bleich    Dreist    Geh$rnt   Hibbelig  Krumm     Miserabl  Plump     Scheu*lichTranig    "
                              "Verklemmt Borniert  Dumm      Geifernd  Hinkend   Kuh#ugig  Missraten Zwickend  Schielend Tratschig "
                              "AbgetakeltBr#sig    Dumpf     Geistig   HinterzartL#cherlichModrig    Popelig   Schlammig Treudoof  "
                              "AbgewracktBr%sk     Dusselig  Geklaut   Hirnlos   L#stig    Morsch    Prek#rer  Schlunzig Triefend  "
                              "Absolut   Br$ckelig Dusslig   Gelblich  HochgradigL$chrig   Motzig    ProgressivSchmierig Trocken   "
                              "Absurd    Brennend  Echt      Gem#stet  Hohl      Labbrig   Muffig    Prollig   Schmutzig Wirr      "
                              "Angenehm  Rockig    Einarmig  Hohlk$pfigLahm      N$rgelnd  Zottelig  Schr#g    Unendlich Witzlos   "};

// Second word part 1 - noun
const char data2[] PROGMEM = {"4711      Doof      H%hner    Kratz     Mist      Plunsch   Reier     Schleck   Schweine  Trivial   "
                              "'ko       Dulli     H$llen    Krawatten Mittel    Pluntsch  Reserve   Schleich  Schwitz   Tropf     "
                              "(berkreuz Dumm      Hack      Kreisch   Moder     Polter    Riesen    Schleim   Schwurbel Turbo     "
                              "360-Grad  Dumpf     Halbwelt  Kreuz     Morast    Pomaden   Rotz      Schling   Socken    Ultra     "
                              "AllerweltsEdel      Hampel    Kriech    Mords     Pr%gel    Rumba     Schluff   Speck     Unsinns   "
                              "Alpen     Egal      Haupt     Kulissen  Nachttopf PrinzipienRumpel    Schmacht  Spei      Versch%tt "
                              "Alpha     Eiter     Hecken    Kuschel   Nackt     Problem   Rundum    SchmalspurSpielplatzViertel   "
                              "Alt$l     Ekel      Hilfs     L%gen     Narren    Profit    Rundweg   Schmalz   Sport     Voll      "
                              "Amateur   Ersatz    Himbeer   Labber    Nasen     Protz     Runzel    Schmei*   Sprudel   Vollbart  "
                              "Angst     Eunuchen  Hochkant  Laber     Neopren   Pseudo    S%sswasserSchmier   Spuck     Vollblut  "
                              "Asphalt   Ex        Holz      Lach      Niemands  Psycho    Sabbel    Schmodder Stachel   Vorgarten "};

 // Second word part 2 - female noun
const char data3[] PROGMEM = {"alge      brumme    geige     kanallie  larve     niete     pl$rre    ruine     so*e      trulla    "
                              "am$be     brut      gestalt   kanone    nudel     pl$tze    dramaqueens%lze     socke     unke      "
                              "amsel     bude      glatze    kippe     latte     nulpe     pocke     sammlung  spinne    uschi     "
                              "assel     creme     gr#fin    kiste     lawine    nuss      pomade    sau       spur      wachtel   "
                              "attrappe  distel    gr%tze    klatsche  leuchte   oma       pracht    schabe    strippe   wampe     "
                              "aushilfe  diva      graupe    klette    liese     omme      praline   schabrackestulle    wanne     "
                              "b%rste    dr%se     grazie    knolle    lotte     pampe     primel    scharte   suppe     warze     "
                              "backe     fackel    grube     kr#he     m%cke     pappe     pulle     schippe   t%te      wespe     "
                              "backpfeifeflosse    gurke     kr%cke    made      parade    qualle    schlampe  t$hle     windel    "
                              "beule     flunder   h%fte     kr$te     magd      pauke     queen     schleuder tablette  wolke     "
                              "birne     fluppe    haxe      krake     matrone   pelle     r%be      schrulle  tante     wunde     "};

// Second word part 2 - male noun
const char data4[] PROGMEM = {"%berzwerchblutsaugereimer     glotzer   horst     kobold    michel    pinsel    sauhund   stinkador "
                              "aal       bock      ersatz    gnom      husten    kodderteigmimoserichzwerg     schabzigerstinkfu*  "
                              "aasgeier  bolzen    eumel     gnubbel   iltis     kohl      mist      playboy   schaden   stumpf    "
                              "abklatsch br#gen    geruch    hornochs  infarkt   kolben    molch     politiker scheffdepptarzan    "
                              "abschaum  bruder    f#nger    gr%sel    infekt    kollege   mongo     pott      tentakel  zombie    "
                              "after     egel      fan       grasaff   j%nger    kompost   mop       pr%gel    schieber  terrier   "
                              "ajatollah brutalo   fascho    greis     jeck      konsument mops      produzent schimpanseteufel    "
                              "alptraum  bubi      fetzen    groschen  jogurt    koofmich  muffel    prolo     schlabi   tiger     "
                              "amateur   buckel    feudel    grosskotz k#se      kopf      nachttopf protz     schlaffi  totsch    "
                              "anfall    bummelant finger    grufti    k%bel     meier     nagel     psycho    schlawinertrieb     "
                              "apostel   busch     figurant  gulli     k$nig     kr%mel    narr      pudding   schleim   trottel   "};

// Second word part 2 - neutrum noun
const char data5[] PROGMEM = {"%bel      debakel   fass      gebr#u    gericht   granulat  m#rchen   patent    skelett   unheil    "
                              "abbild    deo       fell      gebrechen gerippe   gulasch   m$bel     pech      spielzeug unkraut   "
                              "abwasser  derivat   ferkel    gebrodel  ges#*     h#hnchen  mahl      pissoir   st%ck     untier    "
                              "angebot   desaster  fettpapiergef#*     geschmei* haupt     mittel    placebo   symptom   verbrechen"
                              "astloch   dilemma   fiasko    gejodel   geschnetz hindernis modell    plankton  syndrom   verderben "
                              "auge      ding      folikel   gel       geschw#tz hirn      molek%l   podest    synonym   verlie*   "
                              "baby      dingdong  fondue    gel$t     gesetz    hormon    parf%m    prinzip   tabu      versagen  "
                              "balg      dingsbums format    gelee     gesicht   imitat    monster   produkt   theater   versteck  "
                              "ballett   donutloch fossil    gelumpe   gespenst  inferno   moped     reptil    tier      vieh      "
                              "beispiel  drama     frettchen gem#cht   gespinnst insekt    mus       rinnsaal  toupet    virus     "
                              "bier      ei        furunkel  gem#lde   get$se    kamel     objekt    risotto   trauma    wachstum  "};
