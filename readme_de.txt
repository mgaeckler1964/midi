MIDI Tools 2.3.2                                              20. Juli 2014
===========================================================================

(c) 2007-2021 by Martin Gäckler, Linz, Austria, https://www.gaeckler.at/



Einleitung
----------

Mit diesem Programm ist es möglich MIDI-Daten von einem MIDI-Controler,
einem MIDI-Keyboard oder einem MIDI-Synthesizer aufzunehmen und auf einem
beliebigen MIDI-fähigen Klangerzeuger wieder abzuspielen.

Aufgenommene MIDI-Daten können in einer MIDI-Datei gespeichert werden und
später wieder geladen werden oder mit einen MIDI-fähigen Sequenzer weiter
bearbeitet werden.


Installation
------------

Kopieren Sie die Dateien midi.exe, midi_en.dll, *_drums.csv und
*_voices.csv in ein beliebiges Verzeichnis. Anschließend können Sie eine
Verknüpfung in Ihrem Explorer erzeugen. Achten Sie aber darauf, daß als
Startpfad (Feld "Ausführen in") das Verzeichnis angegeben ist, in das Sie
die Dateien kopiert haben, da das Programm sonst die Dateien *_drums.csv
und *_voices.csv nicht findet.

Anschließend sollten Sie einmal mit Administrationsrechten das Programm
starten und Ihre MIDI-Geräte einrichten. Diese Einstellungen gelten dann
für alle Benutzer.


Anwendung
---------

Die Bedienung des Programmes sollte eigentlich selbsterklärend sein,
dennoch geben wir ein paar Hinweise für jede Option:


Das Toolbarfenster
------------------

Die Toolbar wird beim Start automatisch angezeigt, von hier aus können Sie
die einzelnen Programme aufrufen.

Recorder:
Beim Klick auf diesen Schalter wird das Aufnahmefenster (MIDI Recorder)
geöffnet.

Schlagzeugeditor:
Beim Klick auf diesen Schalter wird der Schlagzeugeditor geöffnet.

Loop Editor:
Beim Klick auf diesen Schalter wird der Loop Editor geöffnet.

MIUDI-Geräte:
Mit diesem Fenster können Sie Ihre MIDI-Geräte konfigurieren. Es werden
alle MIDI-Ports Ihres Systems angezeigt und Sie können jeweils den Namen
des Instruments eingeben und bei Klangerzeugern die Namen der CSV-Dateien
für die Stimmen und die Drumsets eingeben. Standardmässig werden die
Klangdateien für General MIDI (gm_voices.csv und gm_drums.csv) verwendet.
Im Lieferumfang befinden sich ausßerdem die Dateien für Yamaha PSR-275 und
Yamaha SY-35.

Schließen:
Beendet alle Programme.

Englisch:
Wenn Sie diese Auswahl betätigen, wird beim nächsten Programmstart
versucht, die englische Oberfläche zu starten.


Das Aufnahmefenster
-------------------

MIDI-In:
Hier können Sie das Eingabegerät auswählen. Wenn Sie "Alle" auswählen,
werden alle vorhanden MIDI-Eingabereäte geöffnet.

Aufnahme:
Ein Klick auf diesen Schalter startet die Aufnahme vom ausgewählten Ein-
gabegerät. Ein erneuter Klick beendet die Aufnahme.Wenn die Filter
aktiviert sind, wird für jedes Ausgabegerät eine eigene MIDI-Spur erzeugt.

MIDI-Out:
Hier können Sie das Ausgabegerät auswählen. Wenn die Filter aktiviert
wurden, werden alle MIDI-Ausgaberäte geöffnet.

Abspielen:
Ein Klick auf diesen Schalter startet die Wiedergabe der MIDI-Daten auf
das zuvor ausgewählte Ausgabegerät. Ein erneuter Klick beendet die Wieder-
gabe.

Auto Rhythmus:
Wenn dieser Schalter vor dem Abspielen aktiviert wurde, werden Töne, die
länger als eine Viertelnote klingen und alleine gespiel werden, mit einem
Rhythmus unterlegt.

Tempo:
In diesen drei Feldern können Sie das Tempo der MIDI-Daten und die Taktart
festlegen.

Übernehmen:
Ein Klick auf diesem Schalter rechnet gespeicherte MIDI-Daten auf das neue
Tempo um. Wurde das Tempo erhöht, werden die Daten schneller abgespielt.
Verringern Sie das Tempo, werden die Daten langsamer abgespielt.

Metronom:
Startet die Wiedergabe des Metronoms auf dem MIDI-Kanal 16 des ausgewählten
Ausgabegeräts. Ein erneuter Klick beendet das Metronom. Im Popupfenster des
Metronoms können Sie auswählen, welche Schlaginstrumente verwendet werden.
Die Effekteinstellungen des Kanal 16 werden bis auf die Stimme ebenfalls
verwendet, so daß Sie die Lautstärke, Stereoposition und andere
Einstellungen bei den Kanaleinstellungen festlegen können.

Midi Trough:
Wenn dieser Schalter aktiv ist, werden eingehende MIDI-Daten sofort
abgespielt. Das Ausgabegerät wird bei aktiven Schalter in den
Local-Off-modus (bei Yamaha heißt dies PC-Modus) gesetzt. Wenn der Schalter
deaktiviert wird, so wird das Ausgabegerät in den Local-On-Modus gesetzt
und geschlossen.

Filter:
Wenn dieser Schalter (die Checkbox) aktiviert ist, werden beim Abspielen
der MIDI-Daten und bei der Aufnahme die eingestellten Filter verwendet.
Der Pushbutton Filter öffnet den Dialog für die Filtereinstellungen (siehe
weiter unten).

Kanäle:
Ein Klick auf diesen Schalter öffnet die Kanaleinstellungen (siehe weiter
unten) für das aktive MIDI-Ausgabegerät.

Schließen:
Beendet das Fenster.

Laden/Speichern:
Die beiden Pushbuttons im rechten Bereich des Dialogs erlauben Ihnen, die
Kanal- und Filtereinstellungen zu laden bzw. zu speichern.
Die beiden Pushbutton im unteren Bereich des Dialogs erlauben Ihnen die
MIDI-Daten zu laden bzw. zu speichern.
Beim Speichern der MIDI-Daten können Sie zu Testzwecken auch das CSV-Format
angeben, um dann die Daten mit einem Tabellenprogramm wie z.B. StarCalc
oder Excel anschauen zu können. Die Spalten der Tabelle enthalten in dieser
Reihenfolge den Timecode, den MIDI-Befehl, den MIDI-Kanal und die beiden
Datenbytes.
Beim Laden der MIDI-Dateien ist zu beachten, daß Tempowechsel innerhalb
der Mididatei nicht unterstützt werden. Beim Speichern von MIDI-Dateien
werden grundsätzlich alle Tracks zu einem zusammengefaßt (Format 0)

Bearbeiten:
Öffnet den MIDI-Editor.


Kanaleinstellungen
------------------

In diesem Fenster können Sie für jeden einzelnen der 16 MIDI-Kanäle Ihre
gewünschten Einstellungen vornehmen. Beachten Sie jedoch, daß nicht alle
Klangerzeuger alle Einstellungen beachten.

Kanal:
Hier wählen Sie den gewünschten Kanal aus.

Stimme:
Hier wählen Sie die Stimme des Kanals aus. Die möglichen Stimmen werden
aus der Datei voices.csv geladen. Die mitgelieferte Datei enthält die
Bedienfeldstimmen des Yamaha PSR 273/275. Sie können die Datei beliebig
nach Ihren Anfordernissen anpassen.

Stereo:
Hier geben Sie die Stereoposition des Kanals an.

Lautstärke:
Hier können Sie Lautstärke des Kanals festlegen.

Effekt 1-5:
Hier geben Sie die Intensität jeweils eines der 5 möglichen Effekte an. Die
Wirkung eines Effekts auf den Klang ist vom Klangerzeuger abhängig.

Sound 1-5:
Hier geben Sie die Wirkung jeweils eines der 5 möglichen Soundvariationen
an. Die Wirkung einer Variante hängt auch hier wieder von Klangerzeuger ab.

Expression:
Hier geben sie den Wert des Expressionpedals an. Die Wirkung des Pedals
hängt auch hier wieder von Klangerzeuger ab.

Schließen:
Beendet das Fenster.

Standard:
Setzt die Stereoposition und die Soundvarianten in die Mittelposition, alle
Effekte auf 0 und die Lautstärke und das Expressionpedal auf 100%.


Filtereinstellungen
-------------------

Hier können Sie festlegen, wie die Eingangsdaten bearbeitet werden sollen
bevor sie wieder ausgegeben bzw. gespeichert werden. Die Filter sind nur
dann aktiv, wenn der Schalter Filter im Hauptfenster aktiv ist. Wenn trotz
aktiven Schalter kein Filter exisitert, werden die Daten, so wie sie
ankommen wieder abgespielt bzw. gespeichert.

Liste der Filter:
Hier können Sie einen der definierten Filter auswählen.

Neu:
Nach Klick auf diesen Schalter wir ein neuer Filter erstellt. Sie müssen
jedoch einen Namen eingegeben haben, der noch nicht verwendet wurde.

Übernehmen:
Definiert kopiert die Parameter im Bereich Filter und Aktion in den
ausgewählten Filter.

Löschen:
Entfernt des ausgewählten Filter.

Oben:
Verschiebt den ausgewählten Filter um eine Position nach oben.

Name:
Hier geben Sie den Namen eines neuen Filters an.

Filter:
Hier legen Sie fest welche Noten von diesem Filter behandelt werden sollen.
Sie können einen bestimmten Eingangskanal bzw. ein bestmmtes Eingabegerät
oder alle auswählen und festlegen, welche Notenwerte berücksichtigt werden
sollen. Wenn Sie als Filter Control ausgewählt haben, werden alle System-
und Kanalnachrichten berücksichtigt. Der eingegebene Notenwerden wird dann
allerdings ignoriert.

Aktion:
Hier legen Sie fest, wie die Note ausgegeben werden soll. Sie können einen
bestimmten Kanal auswählen oder die Note an den gleichen Kanal schicken,
die der Controler verwendet hat. Ebenso können Sie auswählen, auf welchem
Ausgaberät die Note geschickt werden soll. Darüber hinaus, können Sie auch
einen Offset in Halbtonschritten für die Transformation angeben. +12 erhöht
die Note um eine Oktave. -12 verringert die Note um eine Oktave. Desweitern
können Sie die Lautstärke anpassen. Hierbei gibt es die Möglichkeit einen
Offset einzugeben (+/-Zahl), einen absoluten Wert (Zahl) oder einen
Prozentwert (Zahl%) einzutragen. Wenn Sie "Auto" eintragen, so wird die
Lautstärke automatisch auf den größtmöglichen Wert gesetzt. Es werden aber
nur solche Werte geändert, die nicht 0 sind, da manche MIDI-Controler statt
MIDI-Note-Off einfach die Lautstärke auf 0 setzen.
Bei System- und Kanalnachrichten werden Noten- und Lautstärkewerte
ignoriert. Bei Systemnachrichten wird zusätzlich der Kanal ignoriert.
Wenn die Checkbox "Stop" ausgewählt ist, werden bei einem Treffer die
nächsten Filter nicht mehr berücksichtigt.

Schließen:
Beendet das Fenster.

Standards:
Erzeugt für jeden Eingangskanal einen Filter, der bewirkt, daß die Note auf
den gleichen Kanal wieder ausgegeben wird.


MIDI-Editor
-----------

Zeigt die Liste aller MIDI-Ereignisse an. MIDI-Noten und Stimmwechsel
werden dabei zu einem Ereigniss zusammengefasst.

Im rechten Teil sehen Sie die Art des ausgewählten Ereignisses.

Kanal:
Hiermit legen Sie den MIDI-Kanal (1-16) fest.

Spur:
Hiermit legen sie die MIDI-Spur fest.

Takt.Beat.Zeit:
Hiermit legen Sie den Zeitpunkt fest.

Stimme:
Wenn dies ausgewählt ist, bearbeiten Sie einen Stimmwechsel. Sie können
dann die Stimme auswählen.

Note / Lautstärke / Dauer / Ausklingzeit:
Hiermit bearbeiten Sie ein Notenereigniss. Mit den Schaltern für die
ganzen, halben, viertel und achtel Noten können Sie die Dauer leicht
eingeben ohne sie ausrechnen zu müssen.

Controller:
Hiermit bearbeiten Sie ein Controllerereigniss.

Andere:
Dies ist für alle anderen unterstützen MIDI-Ereignisse vorgesehen.

System Exclusive:
Dies wird für MIDI System Exclusive Messages benutzt. Beachten Sie, es ist
nicht mögchlich,SysEx Messages zu erzeugen noch zu ändern.

Übernehmen:
Überträgt die Einstellungen zum ausgewählen MIDI-Ereigniss.

Neu:
Erzeugt einen neues MIDI-Ereigniss.

Löschen:
Entfernt ausgewählte MIDI-Ereignisse.

Instrument:
Öffnet eine virtuelle Klaviatur und Gitarrengriffbrett. Nach einem Klick
auf eine Note, wird diese im Editor übernommen. Darüber hinaushaben Sie im
Instrumentenfenster die Möglichkeit. sich wichtige Akkorde und Skalen
anzeigen zu lassen. Ein Rechtsklick auf eine Note fixiert dies.

Mehrere Einträge bearbeiten:
Bearbeitet alle markierten Einträge.

Lautstärke:
Hiermit können Sie die Lautstärke eines einzelnen Kanals oder die
Gesamtlautstärke einstellen.

Kan. tauschen:
Hiermit können Sie die MIDI-Ereignisse zweier Kanäle vertauschen.

Wdh.:
Hiermit können Sie alle markierten Daten kopieren und die Kopie ans Ende
der Datei stellen.

Transp.:
Hiermit können Sie alle Noten der Datei um einen bestimmten Wert höher
(positive Werte) oder tiefer machen (negative Werte).

Versch.:
Verschiebt alle markierten Ereignisse um die den eingetragenen Zeitraum.
Im Eingabefeld links tragen Sie den Takt, Beat und Zeit in Milisekunden
ein, um den die Ereignisse verschoben werden sollen. Ein Vorangestelltes
Minuszeichen, verschiebt die Einträge in Richtung Anfang der Datei.

Kopieren:
Kopiert alle markierten Ereignisse um die den eingetragenen Zeitraum.
Im Eingabefeld links tragen Sie den Takt, Beat und Zeit in Milisekunden
ein, um den die Ereignisse verschoben werden sollen. Ein Vorangestelltes
Minuszeichen, verschiebt die Einträge in Richtung Anfang der Datei.

OK:
Schließt das Fenster und überträgt die Daten zum Rekorderfenster.

Abbruch:
Schließt das Fenster und verwirft alle Änderungen.


Tips
----

Die Notenwerte werden in Halbtonschritten von 0 bis 127 durchnummeriert.
Das normale c hat den Notenwert 60.

Um für jede Note automatisch den korrekten Durakkord zu spielen, benötigen
Sie drei Filter:
- Filter: Kanal Alle, Note >= 0; Aktion: Kanal Controller, Note 0
- Filter: Kanal Alle, Note >= 0; Aktion: Kanal Controller, Note 4
- Filter: Kanal Alle, Note >= 0; Aktion: Kanal Controller, Note 7

Für den Splitmodus (z.B. ab dem c) benötigen Sie zwei Filter:
- Filter: Kanal Alle, Note >= 60; Aktion: Kanal 0, Note 0
- Filter: Kanal Alle, Note < 60; Aktion: Kanal 1, Note 0
Bei den Kanaleinstellungen wählen Sie für die beiden Kanäle 0 und 1 die
gewünschten Stimmen.

Für den Dualmodus benötigen Sie zwei Filter:
- Filter: Kanal Alle, Note >= 0; Aktion: Kanal 0, Note 0
- Filter: Kanal Alle, Note >= 0; Aktion: Kanal 1, Note 0
Bei den Kanaleinstellungen wählen Sie für die beiden Kanäle 0 und 1 die
gewünschten Stimmen.

Wenn Sie gleichzeitig den Drumcomputer im Keyboard laufen lassen wollen,
sollten Sie die Filter auf einen Kanal beschränken, da sonst auch der
Drumcomputer verändert wird. Um dann den Drumcomputer hören zu können,
müssen Sie die Standardfilter generieren lassen und den Standardfilter, der
die Klaviatur Ihres Keyboards durchreichen würde (das ist üblicherweise
Kanal 1, bei besseren Keyboards ist das einstellbar), wieder löschen.


Schlagzeugeditor
----------------

MIDI-Out:
Hier können Sie das Ausgabegerät auswählen.

Abspielen:
Spielt das Drumloop solange bis Sie den Schalter erneut betätigen ab.

Tempo/Länge:
Hier geben Sie das Tempo in Beats Per Minute und die Anzahl der Takte des
Drumloops ein. Sie können bis zu 99 Takte bearbeiten.

Raster:
Hier legen Sie das Raster für die Eingabe fest. Der eingestellte Wert
wirkt sich nur auf neue Anschläge aus.

Länge:
Hier legen Sie die Länge eines Anschlags fest. Der eingestellte Wert wirkt
sich nur auf neue Anschläge aus.

Laustärke:
Hier legen Sie die Lautstärke eines Anschlags fest. Der eingestellte Wert
wirkt sich nur auf neue Anschläge aus.

Zeit:
Hier legen Sie die Ausklingzeit des Anschlags fest. Der eingestellte Wert
wirkt sich nur auf neue Anschläge aus.

Instrument:
Hier können Sie eines der verfügbaren Instrumente laden.  Die möglichehn
Instrumente werden aus der Datei drums.csv geladen. Die mitgelieferte Datei
enthält die Instrumente des Yamaha PSR 273/275. Sie können die Datei
beliebig nach Ihren Anfordernissen anpassen.

Hinzufügen:
Erzeugt einen neuen Pattern.

Instrumente:
Hier wird die Liste der Pattern angezeigt. Dies dient auch zum Bearbeiten
der Patterns. Jeder Schlag wird mit einem Balken entsprechend der Länge und
Position angezeigt. Je dunkler der Balken ist desto lauter wird der Schlag
ausgeführt.

Mit einen Klick auf den Instrumentnamen können Sie diesen auswählen.
Mit gedrückter Maustaste können Sie das Instrument auch verschieben. Ein
Linksklick im Schlagbereich fügt einen Schlag hinzu bzw. entfernt diesen.
Zusammen mit der Shifttaste wird ein Schlag ausgewählt.
Ein Rechtsklick auf einen Schlag überträgt dessen Lautstärke auf den
Lautstärkeregler. Zusammen mit der Steuerungstaste bewirkt ein Rechtsklick
die Übertragung der Lautstärke vom Regler zum Schlag. Ein Linksklick auf
den Positionsbereich verschiebt den Positionszeiger für Verschiebe- und
Kopieroperation an die entsprechende Stelle.

Mit der Combobox links des Instrumenteneditors können Sie festlegen, welche
Instrumente gespielt werden sollen.

Mit dem Schalter Lautstärke können Sie die Lautstärke eines einzelnen
Instruments oder die Gesamtlautstärke verändern.

Stereo:
Dieser Schalter erlaubt es einzelnen Patternseine Position im Stereobild
des Schlagzeugs zuzuweisen.

Löschen:
Löscht den ausgewählten Pattern oder die ausgewählten Beats. Falls nichts
selektiert ist, werden alle Beats ab der aktuellen Position bis zu einge-
stellten Länge gelöscht und die nachfolgenden Beats entsprechend
verschoben.

Kopieren:
Kopiert alle ausgewählten Beats an die aktuelle Position.

Verschieben:
Verschiebt alle ausgewählten Beats an die aktuelle Position.

Pause:
Fügt an der aktuellen Position eine Pause ein und verschiebt alle rechts
davon befindlichen Beats entsprechend nach rechts. Falls notwendig, wird
auch ein weiterer Takt hinzugefügt.

Alles Löschen:
Löscht alle Patterns.

Schließen:
Beendet den Schlagzeugeditor.

Laden...:
Lädt einen Drumloop in den Speicher.

Speichern...:
Speichert einen Drumloop. Sie können eines der folgenden Formate auswählen:
- Drum Dateien: Eigenes Format. Nur dieses Format kann geladen werden.
- Kommasepariert: CSV-Format um die Rohdaten anzeigen zu können.
- MIDI Dateien: Das MIDI-Format erlaubt die Weiterverarbeitung mit einem
  Sequenzer.


Loop Editor
-----------

Der Loop Editor können Sie zum Bearbeiten eines ganzen Songs verwenden.

MIDI-Out:
Hier können Sie das Ausgabegerät auswählen.

Tempo:
Hiermit legen Sie das Tempo des Loops fest. Alle Mididateien und Drum-
dateien werden automatisch umgerechnet.

Abspielen:
Spielt das Loop solange bis Sie den Schalter erneut betätigen ab. Wenn ein
Element ausgewählt wurde, so wird jede Wiederholung ab dem ausgewählten
Element gespielt. Beim Abspielen wird das gerade gespielte Element
angezeigt.

Liste:
Hier werden alle Elemente angezeigt. Sie können einfach mit Drag und Drop
Drumdateien oder Mididateien hinzufügen.

Länger/Kürzer:
Mit diesen beiden Schaltern können Sie festlegen, wie oft das ausgewählte
Element abgespielt werden soll. Wenn Sie hier den Wert 0 auswählen, wird
der Eintrag beim Speichern entfernt.

Oben/Unten:
Mit diesen beiden Schaltern, können Sie die Position des ausgewählten
Elements verändern.

Schließen:
Beendet den Loop Editor.

Laden...:
Lädt einen Loop in den Speicher.

Speichern...:
Speichert einen Drumloop. Sie können eines der folgenden Formate auswählen:
- Loop Dateien: Eigenes Format. Nur dieses Format kann geladen werden.
- Kommasepariert: CSV-Format um die Rohdaten anzeigen zu können.
- MIDI Dateien: Das MIDI-Format erlaubt die Weiterverarbeitung mit einem
  Sequenzer.






Anregungen, Wünsche, Kritik
---------------------------

Dieses Programm wurde erstellt, weil die Konfiguration des Splitmodus oder
des Dualmodus des Yamaha PSR 273/275 über das Keyboard sehr umständlich ist
und beim Ausschalten auch verloren geht. 



Haftungsausschluß
-----------------

Kaum ein Softwareprogramm ist frei von Fehlern. Erst recht dann nicht, wenn
es sich wie hier um eine Betaversion handelt. Wir können daher keine
Gewährleistung übernehmen, daß alles zu Ihrer Zufriedenheit funktioniert
und können auch nicht, obwohl sehr viel Mühe bei der Entwicklung
aufgewendet wurde, keine Haftung für eventuell auftretende Schäden, die
durch die Benutzung der Software entstehen könnten, übernehmen.



Versionshinweise
----------------

Folgende Fehler wurden behoben, folgende Neuerungen wurden eingeführt:

1.0ß2:
- Nach dem Laden neuer Einstellungen wurde das Ausgabegerät geöffnet und so
  der MIDI-Through-Modus aktiviert.
- Nach dem Beenden des Metronoms wurde das Ausgabegerät nicht geschlossen.

1.1ß1:
- Neuer Drumeditor.
- Die CSV Dateien enthalten nun MIDI-Nachrichten und Controller Changes im
  Klartext.
- Beim Beenden wurde das MIDI-Gerät nicht immer geschloßen, was bei manchen
  Treibern zum Systemcrash führen konnte.

1.2ß1:
- Neuer MIDI-Editor zum Bearbeiten der MIDI-Dateien.
- Beim Metronom kann nun ausgewählt werden, welche Schlaginstrumente
  verwendet werden.
- Beim Schließen der Fenster werden Sie nun gefragt, ob evtl. ungesicherte
  Daten gespeichert werden sollen.

1.2ß2:
- Drumpatterns, die keine Daten enthalten, werden automatisch gelöscht.
- Drumpatterns, bei denen das Schlaginstrument geändert wurde, werden nun
  schon beim Editieren automatisch umbenannt.
- Das Abspielen von Drumpatterns kann nun auch dann gestartet werden, wenn
  noch keine Drumpatterns vorhanden sind.
- MIDI-Dateien, Drumpatterns und Einstellungen können nun direkt aus dem
  Explorer geöffnet werden. Hierzu müssen Sie aber mindestens einmal das
  Programm als Hauptbenutzer oder Administrator aufrufen, damit es die
  entsprechenden Verknüpfungen in der Registrierungsdatenbank eintragen
  kann.
- Der Midieditor zeigt nun beim Programchangeeintrag auch den Namen des
  ausgewählten Instruments.
- Beim Laden neuer Dateien werden Sie nun gefragt, ob evtl. ungesicherte
  Daten gespeichert werden sollen.
- Es konnte vorkommen, daß der Patterneditor mit seinen Schlaginsrumenten
  durcheinander kam und daher entweder Pattern ganz verloren hat oder die
  Instrumente nicht geändert werden konnten.
- Neuer Schalter Alles Löschen im Schlagzeugeditor.
- Neuer Schalter Lautstärke im MIDI-Editor.
- Neuer Schalter Kanäle tauschen im MIDI-Editor.
- Kanäle werden nun von 1-16 durchnummeriert.
- Fehlerbehandlung beim MIDI-Editor verbessert.

1.3ß1:
- Schlagzeugeditor überarbeitet.
- Neuer Loop Editor.

1.3ß2:
- Im Loop Editor kann nun das Tempo eingestellt werden.
- Recorder, Schlagzeugeditor und Loop Editor können nun mehrfach geöffnet werden.
- Der Loop Editor speichert Pfade nun relativ zur Loopdatei.
- Im Midi-Editor wurden neue Schalter zum Wiederholen und Transponieren
  eingefügt.
- Wenn Recordereinstellungen per Doppelklick aus dem Explorer geladen
  werden sollen, werden diese nun korrekt geladen.
- Wenn ein Filtereintrag nach oben verschoben werden soll, so bleibt dieser
  nun selektiert.
- Beim Speichern der Settings im Recorder Window werden auch Filter Enabled
  und MIDI Through gespeichert.
- Beim Wechsel des Instruments werden nun die Filtereinstellungen und MIDI
  Through übertragen.
- Der Recorder kann nun alle MIDI-Quellen gleichzeitig öffnen.

1.4:
- Neue englische Oberfläche
- Die Anzeige der gespielten Note verbessert.
- Im MIDI-Filter kann nun die Lautstärke angepasst werden.

1.5:
- Im MIDI-Editor können nun mehrere Einträge ausgewählt werden. Alle
  Bearbeitungsfunktionen beziehen sich nun auf die ausgewählten Einträge.
- Neue Bearbeitungsfunktionen Kopieren und Verschieben im MIDI-Editor.
- Im Schlagzeugeditor wurden die Verschiebe- und Kopieroperationen
  hinzugefügt.

2.0:
Es können nun mehrere Klangerzeuger gleichzeitig verwendet werden und
unterschiedliche Klangdateien für jedes einzelne Instrument konfiguriert
werden.

2.1:
- Fehler beim Speichern von Schlageugdateien für General MIDI Geräte
  behoben.
- Neues Instrumentenfenster zum Anzeigen wichtiger Akkorde und Skalen
- Bei der Aufnahme von MIDI-Dateien wird nun für jedes Instrument eine
  eigene MIDI-Spur erzeugt.
- Beim Abspielen einer MIDI-Datei kann nun ein Rhythmus hinzugefügt werden.
- MIDI System Exclusive Meldungen werden nun aufgenommen und abgespielt.
- Stabilitätsprobleme unter Windows XP behoben.