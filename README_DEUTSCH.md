# cpp-fatedit
FATEDIT ist ein FAT12 und FAT16 Disketten-Editor, zum Editieren der File Allocation Table ( FAT ) und dem Inhaltsverzeichnis von MSDOS Disketten mit 12-bit FAT ( FAT12 ) und von Harddisks mit 16-bit FAT ( FAT16 ).
Es ist auf IBM-PC/AT Rechnern mit dem Betriebssystem MSDOS lauffähig. 
## Historische deutsche Dokumentation ( Brief an einen Verlag )

Rolf Hemmerling  
Im Moore 39  
3000 Hannover 1  

Hannover, 1988-01-18  

An den  
Vogel-Verlag  
CHIP-TOOLS  
Redaktionen München  
Schillerstraße 23a  
8000 München 2  


Sehr geehrte Damen und Herren !  

Anbei ein Testprogramm für den Microsoft Quick-C Compiler. Aufgrund mangelnder Bibliotheksfunktionen MUSSTE ein Assemblermodul mit beigefügt werden, das ich bei einem Bekannten assemblieren ließ.

Das Programm läßt sich sowohl mit Microsoft-C 5.X ,Microsoft QUICK-C 1.X, Borland Turbo-C 2.0 und Mix Power-C 1.2.0 kompilieren. Es zeigt, daß für Quick-C erstellter Sourcecode portabel ist. Der Bibliotheksumfang der anderen Compiler (Turbo,Mix) ist allerdings umfangreicher, so daß für diese Compiler die im Assemblermodul enthaltenen Routinen schon in
den Standardbibliotheken vorhanden sind.

Es ist ein "Power-User-Tool" mit einer zugegebenermaßen kryptischen Benutzeroberfläche zur Diskettenrestaurierung. Ich habe mir auf einer Diskette, auf der sich Briefe und selbst erstellter Sourcecode befand, einen Diskettenfehler auf Spur 0 eingehandelt. Falls man eine beschriebene Diskette fälschlicherweise anfängt zu formattieren, und dies noch bemerkt, ist man in derselben Situation: Spur 0 und damit
das Inhaltsverzeichnis samt FAT sind gelöscht, obgleich die
eigentlichen Dateien auf den höheren Spuren noch vollständig vorhanden sind. Mein interaktives Programm dient nun dazu, das Inhaltsverzeichnis und die FAT zu interaktiv restaurieren. Das kann natrlich nur klappen, wenn auf der Diskette nur TEXTDATEIEN restauriert werden sollen, und wenn der Inhalt der Dateien im Großen und Ganzen bekannt ist. Die Chancen zur Restaurierung steigen, wenn es sich bei der Diskette um
eine "Sicherheits"-Kopie handelt, auf die zur Datensicherung die Dateien nacheinander mit dem MsDos COPY Kommando heraufkopiert´wurden. Dann steht der überwiegende Teil der Dateien hintereinander auf aufeinanderfolgenden Clustern, und man spart viel Zeit.

Selbstverständlich muß eine auf Spur 0 defekte Diskette "irgendwie" auf eine frisch formattierte Diskette umkopiert werden, so daß auf der neuen Diskette eine lesbare Spur 0 mit Bootsektor, leerer FAT und leerem Inhaltsverzeichnis sowie der Disketteninhalt Spur 1-39/79 der defekten Diskette vorhanden sind. Dann kann man auf der neuen Diskette beginnen, alles zu restaurieren.

Mir ist bislang kein anderes kommerzielles oder Public Domain/Shareware-Programm bekannt, das dies SO in dieser Form leistet.

Die prinzipiellen Einschränkungen in Kürze:
- grundlegende Annahme: Sektorgröße maximal 1024 Bytes (kann geändert werden im Sourcecode).
- Da ich nur eine 10 Mbyte Harddisk habe, sind nur die 12-bit FAT Routinen getestet.
- Nicht lauffähig unter MSDOS 3.3 oder PCDOS 3.3 aufgrund von
  Betriebssystemfehlern.
- Aufgrund des Funktionsprinzips nicht für die Restaurierung von Binärdateien ( .COM .EXE, .BIN) oder archivierten Dateien (-> ARC, PK????,SQUEEZE,LUNT) geeignet.
- Nur 12- und 16- bit FATs, d.h. MSDOS 4.X Partitionen größer als 32Mbyte werden (noch) nicht unterstützt, mangels Kenntnissen über den FAT-Aufbau meinerseits. Eine Anpassung wäre aufwendig, es müßten überall "long" statt "int" Variablen verwendet werden. Aufgrund der riesigen Datenmenge kann man eine solche Restaurierung bei Festplatten (egal ob mit 12- oder 16-bit FAT) aber eh vergessen.

Voraussichtlich werde ich das Programm später einmal noch weiter verbessern, und zwar um folgende Einschränkungen nach und nach zu beseitigen:
- z.Z.keine Restaurierung von Dateien in Unterverzeichnissen. Wird später möglich sein, erfordert aber groáen Programmier-Aufwand. Da diese Verzeichnisse normalerweise noch intakt vorhanden sind, und auch ein Startcluster angegeben ist, ist eine etwas unkomfortable Restaurierung auch jetzt schon möglich.
- Bootsektor-Informationen müssen vollständig vorhanden sein.
  Beim Formattieren von 8-Sektor-Disketten mit den Formattern der MsDos 3.X Versionen wird u.U. nur das Media-Byte eingesetzt. Ähnliches gilt für ATARI-TOS Disketten. Zur Zeit ist eine Restaurierung dieser Disketten nicht möglich. Später soll eine spezielle Media-Byte Auswertroutine die bekannten Werte für ein Format eintragen.
- Zur Zeit kann bei der Namensgebung keine leere Extension oder ein leerer Dateiname eingegeben werden, da die zur Eingabe verwendete Funktion SCANF Leerzeichenüberliest. Nachbearbeitung des Inhaltsverzeichnisses mit einem handelsüblichen Disketteneditor nötig. Bei der jetzigen Benutzeroberfläche ist auch langfristig keine Abhilfe in Sicht.

Folgende Erweiterungen wären möglich, sind aber im Augenblick
nur fernes Planungsziel:
- Text-Suchfunktionen (im FAT-Menü)
- erweiterte FAT-Darstellung: Zuordung jeden Clusters einer Datei (was das heißt, da habe ich noch keine so genauen Vorstellungen)

Verwendete Literatur, von der Konkurrenz aus dem Heise-Verlag:
- C'T 12/1987: "CP/M goes MSDOS"
- C'T 11/1987: "Daten auf Scheiben"

Benutzeranleitung: Für "Power-User" sind die Menüs Erklärung genug. Die erwähnte Literatur und Ihr völliges Verständnis reichen zur Benutzung aus. Die Autoren haben ja alles bis ins Detail erklärt.

Zur Überprüfung der FAT-Struktur ist die Benutzung des DOS Programms CHKDSK empfohlen.

Hinweise:
- TURBOC: Einen Eingabe-Abbruch bei Zeichen/Zahleneingaben erreicht man durch CTRL-Z Eingabe. Der alte Wert bleibt dann immer erhalten.

- MSC,QUICKC: Einen Eingabe-Abbruch bei Zeichen/Zahleneingaben erreicht man durch CTRL-Z Eingabe. Bei Zahleneingaben bleibt dann der alte Wert immer erhalten. 

- MIX POWERC: Eingabeabbruch mit CTRL-Z führt zu Fehlfunktion, VERBOTEN !

- TURBO,MIX POWERC,MSC,QUICKC:
  - Abbruch der Auflistungen (Cluster als ASCII / Hexdump, FAT, Directory,
  - Directory mit FAT-Eintraegen) ist durch CTRL-C oder CTRL-BREAK möglich, danach Rückkehr ins jeweilige Menü.

Dies ist ein wirklicher Compiler-Test, bei dem die Microsoft
Produkte leider nicht so toll wie das Borland Produkt abschneidet.

Falls Sie tatsächlich an einer Veröffentlichung interessiert sind, bin ich bereit, das Programm etwas besser und einheitlich in Deutsch zu kommentieren, die Menüs in die deutsche Sprache zu fassen und die höchst überflüssige Anleitung zu erstellen. Ansonsten bevorzuge ich ein Mischmasch aus Englisch/Deutsch - Kommentaren, englischen Funktionsnamen, englische Mentexten, da dadurch sich ein Programm auch im nichtdeutschen Ausland verwenden läßt. Auáerdem sind die Fachausdrücke wie FAT,Sektor,.. ect. sowieso Fremdwörter.

Machen Sie doch so weiter mit den Verlosungsaktionen mit GROßEN Gewinnstückzahlen. Wenn nur ein Paar wenige Gewinne da sind, hat man ja doch keine Chance, bei 500 Compilern aber doch wohl. Ich warte auf die Verlosung von Quick-Basic, MASM, Microsoft Mouse, Multiplan (Junior), Word (Junior),... . Besteht irgendeine Chance, daß Sie Borland-Produkte
in Zukunft in ähnlicher Stückzahl verlosen ? Das wäre toll !!!

Es wäre toll, wenn wie bisher Abonenten NICHT bei der Verlosung einseitig bevorzugt würden, d.h. nicht die ersten 100,200,.. Einsender gewinnen, sondern Verlosung am frühen Stichtag. Komplizierte Preisfragen wie die zum Flugsimulator sind in Ordnung, "TEST-PROGRAMME" als Beweis der Einarbeitung in die entsprechenden Programme eine tolle Idee.

Ihr Magazin ist auch ansonsten recht ansprechend !
Schönen Dank für meinen Gewinn !

Mit freundlichen Grüßen
Rolf Hemmerling
