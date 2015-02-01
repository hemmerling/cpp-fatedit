# FATEDIT - FAT12 und FAT16 Disketten-Editor f�r MSDOS. Historische deutsche Dokumentation ( Brief an einen Verlag )

Rolf Hemmerling                           Hannover, 1988-01-18
Im Moore 39
3000 Hannover 1


An den
Vogel-Verlag
CHIP-TOOLS
Redaktionen M�nchen
Schillerstra�e 23a
8000 M�nchen 2


Sehr geehrte Damen und Herren !
Anbei ein Testprogramm f�r den Microsoft Quick-C Compiler. Aufgrund mangelnder Bibliotheksfunktionen MUSSTE ein Assemblermodul mit beigef�gt werden, das ich bei einem Bekannten assemblieren lie�.

Das Programm l��t sich sowohl mit Microsoft-C 5.X ,Microsoft QUICK-C 1.X, Borland Turbo-C 2.0 und Mix Power-C 1.2.0 kompilieren. Es zeigt, da� f�r Quick-C erstellter Sourcecode portabel ist. Der Bibliotheksumfang der anderen Compiler (Turbo,Mix) ist allerdings umfangreicher, so da� f�r diese Compiler die im Assemblermodul enthaltenen Routinen schon in
den Standardbibliotheken vorhanden sind.

Es ist ein "Power-User-Tool" mit einer zugegebenerma�en kryptischen Benutzeroberfl�che zur Diskettenrestaurierung. Ich habe mir auf einer Diskette, auf der sich Briefe und selbst erstellter Sourcecode befand, einen Diskettenfehler auf Spur 0 eingehandelt. Falls man eine beschriebene Diskette f�lschlicherweise anf�ngt zu formattieren, und dies noch bemerkt, ist man in derselben Situation: Spur 0 und damit
das Inhaltsverzeichnis samt FAT sind gel�scht, obgleich die
eigentlichen Dateien auf den h�heren Spuren noch vollst�ndig vorhanden sind. Mein interaktives Programm dient nun dazu, das Inhaltsverzeichnis und die FAT zu interaktiv restaurieren. Das kann nat�rlich nur klappen, wenn auf der Diskette nur TEXTDATEIEN restauriert werden sollen, und wenn der Inhalt der Dateien im Gro�en und Ganzen bekannt ist. Die Chancen zur Restaurierung steigen, wenn es sich bei der Diskette um
eine "Sicherheits"-Kopie handelt, auf die zur Datensicherung die Dateien nacheinander mit dem MsDos COPY Kommando heraufkopiert�wurden. Dann steht der �berwiegende Teil der Dateien hintereinander auf aufeinanderfolgenden Clustern, und man spart viel Zeit.

Selbstverst�ndlich mu� eine auf Spur 0 defekte Diskette "irgendwie" auf eine frisch formattierte Diskette umkopiert werden, so da� auf der neuen Diskette eine lesbare Spur 0 mit Bootsektor, leerer FAT und leerem Inhaltsverzeichnis sowie der Disketteninhalt Spur 1-39/79 der defekten Diskette vorhanden sind. Dann kann man auf der neuen Diskette beginnen, alles zu restaurieren.

Mir ist bislang kein anderes kommerzielles oder Public Domain/Shareware-Programm bekannt, das dies SO in dieser Form leistet.

Die prinzipiellen Einschr�nkungen in K�rze:
- grundlegende Annahme: Sektorgr��e maximal 1024 Bytes (kann ge�ndert werden im Sourcecode).
- Da ich nur eine 10 Mbyte Harddisk habe, sind nur die 12-bit FAT Routinen getestet.
- Nicht lauff�hig unter MSDOS 3.3 oder PCDOS 3.3 aufgrund von
  Betriebssystemfehlern.
- Aufgrund des Funktionsprinzips nicht f�r die Restaurierung von Bin�rdateien ( .COM .EXE, .BIN) oder archivierten Dateien (-> ARC, PK????,SQUEEZE,LUNT) geeignet.
- Nur 12- und 16- bit FATs, d.h. MSDOS 4.X Partitionen gr��er als 32Mbyte werden (noch) nicht unterst�tzt, mangels Kenntnissen �ber den FAT-Aufbau meinerseits. Eine Anpassung w�re aufwendig, es m��ten �berall "long" statt "int" Variablen verwendet werden. Aufgrund der riesigen Datenmenge kann man eine solche Restaurierung bei Festplatten (egal ob mit 12- oder 16-bit FAT) aber eh vergessen.

Voraussichtlich werde ich das Programm sp�ter einmal noch weiter verbessern, und zwar um folgende Einschr�nkungen nach und nach zu beseitigen:
- z.Z.keine Restaurierung von Dateien in Unterverzeichnissen. Wird sp�ter m�glich sein, erfordert aber gro�en Programmier-Aufwand. Da diese Verzeichnisse normalerweise noch intakt vorhanden sind, und auch ein Startcluster angegeben ist, ist eine etwas unkomfortable Restaurierung auch jetzt schon m�glich.
- Bootsektor-Informationen m�ssen vollst�ndig vorhanden sein.
  Beim Formattieren von 8-Sektor-Disketten mit den Formattern der MsDos 3.X Versionen wird u.U. nur das Media-Byte eingesetzt. �hnliches gilt f�r ATARI-TOS Disketten. Zur Zeit ist eine Restaurierung dieser Disketten nicht m�glich. Sp�ter soll eine spezielle Media-Byte Auswertroutine die bekannten Werte f�r ein Format eintragen.
- Zur Zeit kann bei der Namensgebung keine leere Extension oder ein leerer Dateiname eingegeben werden, da die zur Eingabe verwendete Funktion SCANF Leerzeichen��berliest. Nachbearbeitung des Inhaltsverzeichnisses mit einem handels�blichen Disketteneditor n�tig. Bei der jetzigen Benutzeroberfl�che ist auch langfristig keine Abhilfe in Sicht.

Folgende Erweiterungen w�ren m�glich, sind aber im Augenblick
nur fernes Planungsziel:
- Text-Suchfunktionen (im FAT-Men�)
- erweiterte FAT-Darstellung: Zuordung jeden Clusters einer Datei (was das hei�t, da habe ich noch keine so genauen Vorstellungen)

Verwendete Literatur, von der Konkurrenz aus dem Heise-Verlag:
- C'T 12/1987: "CP/M goes MSDOS"
- C'T 11/1987: "Daten auf Scheiben"

Benutzeranleitung: F�r "Power-User" sind die Men�s Erkl�rung genug. Die erw�hnte Literatur und Ihr v�lliges Verst�ndnis reichen zur Benutzung aus. Die Autoren haben ja alles bis ins Detail erkl�rt.

Zur �berpr�fung der FAT-Struktur ist die Benutzung des DOS Programms CHKDSK empfohlen.

Hinweise:
- TURBOC: Einen Eingabe-Abbruch bei Zeichen/Zahleneingaben erreicht man durch CTRL-Z Eingabe. Der alte Wert bleibt dann immer erhalten.

- MSC,QUICKC: Einen Eingabe-Abbruch bei Zeichen/Zahleneingaben erreicht man durch CTRL-Z Eingabe. Bei Zahleneingaben bleibt dann der alte Wert immer erhalten. 

- MIX POWERC: Eingabeabbruch mit CTRL-Z f�hrt zu Fehlfunktion, VERBOTEN !

- TURBO,MIX POWERC,MSC,QUICKC:
  - Abbruch der Auflistungen (Cluster als ASCII / Hexdump, FAT, Directory,
  - Directory mit FAT-Eintraegen) ist durch CTRL-C oder CTRL-BREAK m�glich, danach R�ckkehr ins jeweilige Men�.

Dies ist ein wirklicher Compiler-Test, bei dem die Microsoft
Produkte leider nicht so toll wie das Borland Produkt abschneidet.

Falls Sie tats�chlich an einer Ver�ffentlichung interessiert sind, bin ich bereit, das Programm etwas besser und einheitlich in Deutsch zu kommentieren, die Men�s in die deutsche Sprache zu fassen und die h�chst �berfl�ssige Anleitung zu erstellen. Ansonsten bevorzuge ich ein Mischmasch aus Englisch/Deutsch - Kommentaren, englischen Funktionsnamen, englische Men�texten, da dadurch sich ein Programm auch im nichtdeutschen Ausland verwenden l��t. Au�erdem sind
die Fachausdr�cke wie FAT,Sektor,.. ect. sowieso Fremdw�rter.

Machen Sie doch so weiter mit den Verlosungsaktionen mit GRO�EN Gewinnst�ckzahlen. Wenn nur ein Paar wenige Gewinne da sind, hat man ja doch keine Chance, bei 500 Compilern aber doch wohl. Ich warte auf die Verlosung von Quick-Basic, MASM, Microsoft Mouse, Multiplan (Junior), Word (Junior),... . Besteht irgendeine Chance, da� Sie Borland-Produkte
in Zukunft in �hnlicher St�ckzahl verlosen ? Das w�re toll !!!

Es w�re toll, wenn wie bisher Abonenten NICHT bei der Verlosung einseitig bevorzugt w�rden, d.h. nicht die ersten 100,200,.. Einsender gewinnen, sondern Verlosung am fr�hen Stichtag. Komplizierte Preisfragen wie die zum Flugsimulator sind in Ordnung, "TEST-PROGRAMME" als Beweis der Einarbeitung in die entsprechenden Programme eine tolle Idee.

Ihr Magazin ist auch ansonsten recht ansprechend !
Sch�nen Dank f�r meinen Gewinn !

Mit freundlichen Gr��en
Rolf Hemmerling
