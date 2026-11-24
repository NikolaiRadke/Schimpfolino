# Projektanweisungen

<!-- Hier kannst du eigene Anweisungen für den Agenten eintragen. -->
<!-- Alles oberhalb des automatischen Bereichs gehört dir. -->

## Projektbeschreibung

## Bauteile und Verdrahtung

## Eigene Regeln

<!-- agentuino:auto:start -->
<!-- Dieser Bereich wird von Agentuino automatisch erzeugt. Änderungen hier werden überschrieben. -->

## Arbeitsregeln (Agentuino)

- Dies ist ein Arduino-Sketch. Ordner und Hauptdatei `Schimpfolino_noeeprom_new.ino` dürfen nicht umbenannt werden.
- Arbeite nur innerhalb dieses Sketch-Ordners.
- Zusätzliche Code-Dateien (.h, .cpp) gehören in den Sketch-Ordner oder nach `src/`, sonst werden sie nicht kompiliert.
- Ändere im Ordner `agentuino/` nur die Datei `agentuino/memory.md`.
- Lies zu Beginn `agentuino/memory.md`. Halte dort nur fest, was über dieses Gespräch hinaus gilt: Bauteile, Verdrahtung, Entscheidungen, gelöste Probleme. Keine Gesprächsverläufe, die stehen in den Chat-Protokollen.
- Lade den Sketch NIE von dir aus auf das Board hoch, auch nicht nach erfolgreichem Kompilieren. Das Board kann Motoren, Relais oder andere Hardware steuern.
- Installiere Bibliotheken oder Board-Pakete (lib install, core install) nur nach Rückfrage beim Nutzer. Sie gelten für alle Sketches.
- Der Nutzer ist Einsteiger: Erkläre kurz und verständlich, was du tust und warum.

## Regeln für den Code

- Kommentiere den Code einsteigerfreundlich: Erkläre in kurzen Kommentaren, was die einzelnen Abschnitte tun und warum.
- Keine Magic Numbers: Lege Pins, Zeiten und Grenzwerte als benannte Konstanten am Anfang des Sketches an.
- Fasse am Ende kurz zusammen, was du geändert hast und warum.

## Hardware

- Board: ATTinyCore ATTINYX5 (FQBN: `ATTinyCore:avr:attinyx5`)
- Port: /dev/ttyUSB0

## Kompilieren und Hochladen

Kompiliere nach jeder Code-Änderung und behebe Fehler selbstständig:

```
arduino-cli --config-file "/home/niko/.arduinoIDE/arduino-cli.yaml" compile --fqbn ATTinyCore:avr:attinyx5 .
```

Hochladen nur, wenn der Nutzer ausdrücklich darum bittet:

```
arduino-cli --config-file "/home/niko/.arduinoIDE/arduino-cli.yaml" upload --fqbn ATTinyCore:avr:attinyx5 --port /dev/ttyUSB0 .
```

Installierte Bibliotheken zeigt `arduino-cli --config-file "/home/niko/.arduinoIDE/arduino-cli.yaml" lib list`.

## Serielle Ausgabe lesen

So liest du die serielle Ausgabe des Boards für einige Sekunden (Baudrate wie in `Serial.begin()`, höchstens 60 Sekunden). Der Port wird automatisch gewählt, bei mehreren Boards gibst du ihn mit `--port` an:

```
agentuino-serial --seconds 10 --baud 9600
```

- Lies die serielle Ausgabe ausschließlich mit `agentuino-serial`. Benutze dafür nicht cat, stty, screen, Python oder `arduino-cli monitor`: Sie hängen, blockieren den Port oder verlieren Daten.
- Der Sketch läuft beim Lesen weiter, die Ausgabe beginnt mitten im Lauf und die erste Zeile kann unvollständig sein. Ob das Board beim Öffnen des Ports neu startet, hängt vom Board ab.
- Mit `--send "Text"` schickst du nach dem Start eine Zeile an den Sketch (für `Serial.read()`). Liest der Sketch einzelne Zeichen, nimm zusätzlich `--no-newline`, sonst liest er auch den Zeilenumbruch als Eingabe.
- Meldet der Befehl einen belegten Port oder kommt nach einem Upload nichts an, hält vermutlich der serielle Monitor der IDE den Port. Bitte den Nutzer, ihn einmal zu öffnen und wieder zu schließen.
- Ein Sketch, der mit „Für Debugging optimieren“ (Optimize for Debugging, ArduDebug) hochgeladen wurde, lässt sich wie ein normaler lesen, auch während einer Debug-Sitzung. Unlesbare Zeichen bedeuten meist eine falsche Baudrate: Nimm die aus `Serial.begin()`.
- Du siehst, was gerade auf dem Board läuft. Das ist nicht unbedingt der aktuelle Code: Hochladen nur nach Rückfrage, oder der Nutzer lädt selbst über die IDE hoch.

## Frühere Chats

Frühere Gespräche zu diesem Projekt liegen als Protokolle in `agentuino/chats/*.md`. Bezieht sich der Nutzer auf ein früheres Gespräch oder fehlt dir Vorwissen, durchsuche sie gezielt mit Grep nach Stichworten und lies nur die passenden Stellen. Lies nie alle Protokolle vollständig.

Letzte Chats:

- 2026-11-20 – Wordlisten prüfen (`agentuino/chats/2026-11-20_1325_f658.md`)
- 2026-11-19 – Flashbedarf reduzieren (`agentuino/chats/2026-11-19_1729_7680.md`)

## Wissensdateien

Keine Dateien vorhanden.

<!-- agentuino:auto:end -->
