/*
  Optimized Schimpfolino wordlist - point-terminated strings

  Wordlist arrays - a single array can hold only 4000 bytes | Used if no EEPROM present | 5 x 160 words
  Umlaute have to be converted (UTF-8): 
  ä -> [ | ö -> ] | ü -> ^ | ß -> _ | Ä -> { | Ö -> | | Ü -> }*/

// First word - adjective 
const char data1[] PROGMEM = {"Dumpfe.Staubige.Miefende.Stinkende.Gammlige.Hinkende.Winzige.Popelige.Nasse.Furzende."
                              "Rostige.Hohle.Siffige.Miese.Krumme.Klapprige.Trockene.Haarige.Uralte.Grunzende."
                              "Schreiende.Meckernde.Nervende.Sabbernde.Triefende.Modrige.Lumpige.Lausige.Sinnlose.Olle."
                              "Unn]tige.Dampfende.Ledrige.Einarmige.Leere.L[stige.Heulende.Pickelige.Faule.Ranzige."
                              "Tr^be.Dralle.Blanke.Gierige.Tranige.Wackelnde.Torkelnde.W^ste.Fischige.Beknackte."
                              "Modrige.Verkorkste.Heimliche.L]chrige.Brockige.Plumpe.Tattrige.Ratternde.Schmutzige."
                              "Liderliche.D]sige.Prollige.Fiese.Dr]ge.Muffige.M^ffelnde.Peinliche.N]rgelnde.Fettige."
                              "Zahnlose.Freche.Sch[bige.Piefige.Gummige.Labbrige.Patzige.Pelzige.Reudige.Pekige.M^rbe."
                              "Harzige.Lahme.Mickrige.Br[sige.Zottelige.Gelbliche.Knorrige.Salzige.Schrille.Dusselige."
                              "Windige.Grausige.Gr[ssliche.Grobe.Spackige.Kauzige.Flachsige.Fransige.Motzige.Kahle."
                              "Niedrige.Keifende.Nichtige.Dr]ge.Fade.Weinende.Sch[bige.N]rgelnde.Hibbelige.Plockige."
                              "Brennende.D^rre.Kochende.Knarzende.Faltige.Schlammige.Br]ckelige.Rissige.Verkeimte."
                              "Kantige.Geklaute.Quieckende.Faselnde.Bei_ende.Geh]rnte.Vergessene.Bleiche.Zweickende."
                              "Frostige.Nackige.Gruselige.Mindere.Hagere.Magere.Schuppige.Belegte.St[nkernde.B]sartige."
                              "Rollende.Scheckige.Rubbelnde.Schielende.Tratschige.Mickrige.Groteske.Absurde.Mehlige."
                              "Platte.M^de.Totale.Bekloppte.Schaurige.Taube.Bet[ubte.Beh[mmerte.Belanglose.Beleidigte."
                              "Betrunkene.Bizarre.Diffuse"};

// Second word part 1 - noun 
const char data2[] PROGMEM = {"Stampf.Wabbel.Pups.Schmalz.Schmier.Hack.Zement.Spuck.Stachel.Keller.Laber.Stock.Runzel."
                              "Schrumpf.Ekel.Schnodder.Matsch.Wurm.Eiter.Speck.Mist.Klotz.W^rg.Lumpen.Schleim.Wurst."
                              "Doof.Brat.Schwamm.Kratz.Grotten.Kriech.Gift.Schlabber.Reier.G]bel.Knatter.Kleb."
                              "Schmadder.Grind.Labber.Luft.Massen.Schimmel.Mini.Ochsen.Problem.Quassel.Schnaps.Saft."
                              "Fummel.Friemel.Zappel.Tropf.Pluntsch.Sumpf.Hecken.Grab.Schwitz.Schnarch.Schleich."
                              "Schluff.Fl]ten.Holz.Kreisch.Dulli.Luschen.Gammel.Alt]l.R]chel.Glibber.Lach.Krach.Knick."
                              "Quetsch.Quatsch.Quietsch.Knautsch.T^mpel.Teich.Knatter.Sauf.Pipi.Struller.Gr[ten.Nasen."
                              "Pech.Leier.Reier.Bl]d.Schorf.Sabbel.Quengel.Bananen.Unsinns.Plunsch.Frust.Lotter.Fummel."
                              "Blubber.Wobbel.Vollbart.Lack.Klo.Moder.Knirsch.Zitter.Kalt.Schl^rf.Schnief.Klecker."
                              "Rumba.Schwurbel.Schrabbel.Schlauch.Schrumpel.H^hner.Schlacker.Brabbel.Krampf.Pr^gel."
                              "Rappel.Zuppel.Plunder.Donner.Riesen.Butter.Wischwasch.Polter.Trampel.Sauer.Hampel."
                              "Bitter.Massel.Flitz.Warm.Schling.Plumps.Qu[l.Strampel.Schleck.Recycling.Egal.Blech."
                              "Horror.Rumpel.Schnuller.Scherz.Nackt.Pampel.Morast.Flach.Angst.Spei.Pumpel.Ausschlag."
                              "Qualm.Rambazamba.Klein.Sprudel"};

// Second word part 2 - female noun 
const char data3[] PROGMEM = {"suppe.socke.bombe.boulette.schwarte.warze.beule.pest.pflaume.r^be.geige.ratte.krankheit."
                              "wunde.oma.knolle.stulle.liese.brut.henne.zwiebel.bude.kiste.braut.leuchte.kr]te.nuss."
                              "spinne.grube.toilette.krake.pf^tze.backe.bratsche.klatsche.nudel.knolle.t^te.nase.made."
                              "tonne.krampe.b^rste.windel.semmel.haxe.gr[fin.schleuder.zierde.kr[he.latte.niete.rassel."
                              "assel.torte.galle.latsche.schrulle.kanone.blase.pelle.trine.queen.zecke.praline.magt."
                              "pracht.fritte.so_e.larve.murmel.hexe.pampe.sirene.dr^se.klette.petze.brumme.glatze."
                              "qualle.natter.kralle.ziege.gr^tze.s^lze.nulpe.wampe.frikadelle.flunder.trulla.zichte."
                              "uschi.kuh.pappe.hupe.tr]te.schabe.kanallie.scharte.rille.amsel.alge.l^cke.bremse.m^cke."
                              "b^rste.wanne.pocke.pl]rre.schabracke.wuppe.sichel.tante.reuse.ratsche.pauke.fluppe."
                              "matrone.hummel.parade.attrappe.l^ge.flosse.funzel.gurke.piepe.g]re.kolben.sammlung."
                              "primel.omme.lotte.unke.strippe.seife.pl]tze.wespe.lawine.tablette.kr^cke.grazie.diva."
                              "pulle.nessel.kakerlake.distel.am]be.fackel.h^fte.ruine.wachtel.seuche.kippe.schippe."
                              "gestalt.wolke.mumie.spur.creme.motte"};

// Second word part 2 - male noun 
const char data4[] PROGMEM = {"busch.fink.nagel.bammel.klopper.tentakel.br[gen.schlumpf.husten.ersatz.haufen.beutel."
                              "kn]del.r^ssel.hintern.eimer.pickel.stumpf.k[se.molch.kohl.gnubbel.sack.hansel.puller."
                              "alptraum.kasten.kopf.beutel.bewohner.kuchen.freund.nascher.opa.rotz.klumpen.peter."
                              "hupsi.bengel.kollege.fleck.l]ffel.lurch.hobel.spaten.pudel.rettich.rinnstein.unfall."
                              "lappen.k^bel.mops.pfosten.zwerg.pudding.nuckel.putzer.l^mmel.baron.mop.besen.feudel."
                              "br[gen.bolzen.pilz.stiefel.k]ter.gulli.pfropf.schrank.k]nig.pott.paddel.rinnstein."
                              "zinken.haken.witz.buckel.knecht.fan.schmand.klops.gauner.lulli.graupe.pimpf.kasper."
                              "spross.teufel.hammel.bock.schmodder.pr^gel.spie_er.aal.groschen.geist.rochen.knochen."
                              "horst.quark.keks.zausel.iltis.jeck.honk.spargel.nippel.atze.muffel.greis.pin]kel."
                              "gehilfe.halunke.lauch.bazillus.onkel.klecks.schaden.auswurf.herpes.unrat.abklatsch."
                              "flegel.glotzer.st]psel.rest.versuch.kompost.fluch.jogurt.p]mpel.stiel.fetzen.duscher."
                              "gnom.schluck.schnupfen.infekt.infarkt.geruch.rambo.dackel.schwingel.dieb.fladen."
                              "flatschen.fussel.knilch.frosch.wombat.anfall.hohlsaum.bimbam.wodka.duft.kadaver.befall."
                              "egel.f[nger"};

// Second word part 2 - neutrum noun 
const char data5[] PROGMEM = {"sekret.balg.blag.monster.gel]t.imitat.skelett.ding.unding.auge.brot.deo.insekt.bier.mus."
                              "ende.futter.gew[chs.produkt.ger]ll.bonbon.furunkel.paket.virus.desaster.st^ck.fass.zeug."
                              "ferkel.ei.gewitter.hormon.experiment.gulasch.schnitzel.fell.theater.schauspiel.baby."
                              "spielzeug.gel.donutloch.gelee.gelumpe.zeug.schaf.molek^l.gew^rz.gespenst.gespinnst."
                              "mittel.geschnetz.organ.risotto.vieh.ges[_.gez^cht.ekzem.moped.ger^mpel.hirn.gef[_."
                              "wachstum.moloch.rinnsaal.gemenge.opossum.frettchen.h[hnchen.plankton.untier.unget^m."
                              "gebr[u.fondue.beispiel.elend.leid.gift.verderben.ungl^ck.drama.trauma.versagen.fiasko."
                              "dilemma.debakel.tabu.ger^cht.hindernis.dingdong.dingsbums.gewicht.abwasser.abbild."
                              "modell.gem[lde.brett.geballer.gem[cht.toupet.geschw[tz.gerippe.pech.leiden.verbrechen."
                              "fossil.symptom.biest.wrack.geb[ck.unheil.ungemach.objekt.gesicht.konfekt.gebrechen."
                              "m[rchen.ger[t.verlust.syndrom.synonym.wasser.tier.follikel.unkraut.ungeziefer.get]se."
                              "geschmei_.gebrodel.gejodel.inferno.gericht.mahl.kamel.gebiss.reptil.verlie_.paddel."
                              "gebot.lager.gemisch.sausen.angebot.zimmer.m]bel.parf^m.podest.ungeheuer.zeichen."
                              "versteck.^bel.scrotum.eisen.ballett.lego.gesetz.format.buffet.granulat.derivat"};