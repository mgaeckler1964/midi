MIDI Tools 2.3.2                                              20. Juli 2014
===========================================================================

(c) 2007-2021 by Martin G�ckler, Linz, Austria, https://www.gaeckler.at/



Einleitung
----------

Mit diesem Programm ist es m�glich MIDI-Daten von einem MIDI-Controler,
einem MIDI-Keyboard oder einem MIDI-Synthesizer aufzunehmen und auf einem
beliebigen MIDI-f�higen Klangerzeuger wieder abzuspielen.

Aufgenommene MIDI-Daten k�nnen in einer MIDI-Datei gespeichert werden und
sp�ter wieder geladen werden oder mit einen MIDI-f�higen Sequenzer weiter
bearbeitet werden.


Installation
------------

Kopieren Sie die Dateien midi.exe, midi_en.dll, *_drums.csv und
*_voices.csv in ein beliebiges Verzeichnis. Anschlie�end k�nnen Sie eine
Verkn�pfung in Ihrem Explorer erzeugen. Achten Sie aber darauf, da� als
Startpfad (Feld "Ausf�hren in") das Verzeichnis angegeben ist, in das Sie
die Dateien kopiert haben, da das Programm sonst die Dateien *_drums.csv
und *_voices.csv nicht findet.

Anschlie�end sollten Sie einmal mit Administrationsrechten das Programm
starten und Ihre MIDI-Ger�te einrichten. Diese Einstellungen gelten dann
f�r alle Benutzer.


Anwendung
---------

Die Bedienung des Programmes sollte eigentlich selbsterkl�rend sein,
dennoch geben wir ein paar Hinweise f�r jede Option:


Das Toolbarfenster
------------------

Die Toolbar wird beim Start automatisch angezeigt, von hier aus k�nnen Sie
die einzelnen Programme aufrufen.

Recorder:
Beim Klick auf diesen Schalter wird das Aufnahmefenster (MIDI Recorder)
ge�ffnet.

Schlagzeugeditor:
Beim Klick auf diesen Schalter wird der Schlagzeugeditor ge�ffnet.

Loop Editor:
Beim Klick auf diesen Schalter wird der Loop Editor ge�ffnet.

MIUDI-Ger�te:
Mit diesem Fenster k�nnen Sie Ihre MIDI-Ger�te konfigurieren. Es werden
alle MIDI-Ports Ihres Systems angezeigt und Sie k�nnen jeweils den Namen
des Instruments eingeben und bei Klangerzeugern die Namen der CSV-Dateien
f�r die Stimmen und die Drumsets eingeben. Standardm�ssig werden die
Klangdateien f�r General MIDI (gm_voices.csv und gm_drums.csv) verwendet.
Im Lieferumfang befinden sich aus�erdem die Dateien f�r Yamaha PSR-275 und
Yamaha SY-35.

Schlie�en:
Beendet alle Programme.

Englisch:
Wenn Sie diese Auswahl bet�tigen, wird beim n�chsten Programmstart
versucht, die englische Oberfl�che zu starten.


Das Aufnahmefenster
-------------------

MIDI-In:
Hier k�nnen Sie das Eingabeger�t ausw�hlen. Wenn Sie "Alle" ausw�hlen,
werden alle vorhanden MIDI-Eingabere�te ge�ffnet.

Aufnahme:
Ein Klick auf diesen Schalter startet die Aufnahme vom ausgew�hlten Ein-
gabeger�t. Ein erneuter Klick beendet die Aufnahme.Wenn die Filter
aktiviert sind, wird f�r jedes Ausgabeger�t eine eigene MIDI-Spur erzeugt.

MIDI-Out:
Hier k�nnen Sie das Ausgabeger�t ausw�hlen. Wenn die Filter aktiviert
wurden, werden alle MIDI-Ausgaber�te ge�ffnet.

Abspielen:
Ein Klick auf diesen Schalter startet die Wiedergabe der MIDI-Daten auf
das zuvor ausgew�hlte Ausgabeger�t. Ein erneuter Klick beendet die Wieder-
gabe.

Auto Rhythmus:
Wenn dieser Schalter vor dem Abspielen aktiviert wurde, werden T�ne, die
l�nger als eine Viertelnote klingen und alleine gespiel werden, mit einem
Rhythmus unterlegt.

Tempo:
In diesen drei Feldern k�nnen Sie das Tempo der MIDI-Daten und die Taktart
festlegen.

�bernehmen:
Ein Klick auf diesem Schalter rechnet gespeicherte MIDI-Daten auf das neue
Tempo um. Wurde das Tempo erh�ht, werden die Daten schneller abgespielt.
Verringern Sie das Tempo, werden die Daten langsamer abgespielt.

Metronom:
Startet die Wiedergabe des Metronoms auf dem MIDI-Kanal 16 des ausgew�hlten
Ausgabeger�ts. Ein erneuter Klick beendet das Metronom. Im Popupfenster des
Metronoms k�nnen Sie ausw�hlen, welche Schlaginstrumente verwendet werden.
Die Effekteinstellungen des Kanal 16 werden bis auf die Stimme ebenfalls
verwendet, so da� Sie die Lautst�rke, Stereoposition und andere
Einstellungen bei den Kanaleinstellungen festlegen k�nnen.

Midi Trough:
Wenn dieser Schalter aktiv ist, werden eingehende MIDI-Daten sofort
abgespielt. Das Ausgabeger�t wird bei aktiven Schalter in den
Local-Off-modus (bei Yamaha hei�t dies PC-Modus) gesetzt. Wenn der Schalter
deaktiviert wird, so wird das Ausgabeger�t in den Local-On-Modus gesetzt
und geschlossen.

Filter:
Wenn dieser Schalter (die Checkbox) aktiviert ist, werden beim Abspielen
der MIDI-Daten und bei der Aufnahme die eingestellten Filter verwendet.
Der Pushbutton Filter �ffnet den Dialog f�r die Filtereinstellungen (siehe
weiter unten).

Kan�le:
Ein Klick auf diesen Schalter �ffnet die Kanaleinstellungen (siehe weiter
unten) f�r das aktive MIDI-Ausgabeger�t.

Schlie�en:
Beendet das Fenster.

Laden/Speichern:
Die beiden Pushbuttons im rechten Bereich des Dialogs erlauben Ihnen, die
Kanal- und Filtereinstellungen zu laden bzw. zu speichern.
Die beiden Pushbutton im unteren Bereich des Dialogs erlauben Ihnen die
MIDI-Daten zu laden bzw. zu speichern.
Beim Speichern der MIDI-Daten k�nnen Sie zu Testzwecken auch das CSV-Format
angeben, um dann die Daten mit einem Tabellenprogramm wie z.B. StarCalc
oder Excel anschauen zu k�nnen. Die Spalten der Tabelle enthalten in dieser
Reihenfolge den Timecode, den MIDI-Befehl, den MIDI-Kanal und die beiden
Datenbytes.
Beim Laden der MIDI-Dateien ist zu beachten, da� Tempowechsel innerhalb
der Mididatei nicht unterst�tzt werden. Beim Speichern von MIDI-Dateien
werden grunds�tzlich alle Tracks zu einem zusammengefa�t (Format 0)

Bearbeiten:
�ffnet den MIDI-Editor.


Kanaleinstellungen
------------------

In diesem Fenster k�nnen Sie f�r jeden einzelnen der 16 MIDI-Kan�le Ihre
gew�nschten Einstellungen vornehmen. Beachten Sie jedoch, da� nicht alle
Klangerzeuger alle Einstellungen beachten.

Kanal:
Hier w�hlen Sie den gew�nschten Kanal aus.

Stimme:
Hier w�hlen Sie die Stimme des Kanals aus. Die m�glichen Stimmen werden
aus der Datei voices.csv geladen. Die mitgelieferte Datei enth�lt die
Bedienfeldstimmen des Yamaha PSR 273/275. Sie k�nnen die Datei beliebig
nach Ihren Anfordernissen anpassen.

Stereo:
Hier geben Sie die Stereoposition des Kanals an.

Lautst�rke:
Hier k�nnen Sie Lautst�rke des Kanals festlegen.

Effekt 1-5:
Hier geben Sie die Intensit�t jeweils eines der 5 m�glichen Effekte an. Die
Wirkung eines Effekts auf den Klang ist vom Klangerzeuger abh�ngig.

Sound 1-5:
Hier geben Sie die Wirkung jeweils eines der 5 m�glichen Soundvariationen
an. Die Wirkung einer Variante h�ngt auch hier wieder von Klangerzeuger ab.

Expression:
Hier geben sie den Wert des Expressionpedals an. Die Wirkung des Pedals
h�ngt auch hier wieder von Klangerzeuger ab.

Schlie�en:
Beendet das Fenster.

Standard:
Setzt die Stereoposition und die Soundvarianten in die Mittelposition, alle
Effekte auf 0 und die Lautst�rke und das Expressionpedal auf 100%.


Filtereinstellungen
-------------------

Hier k�nnen Sie festlegen, wie die Eingangsdaten bearbeitet werden sollen
bevor sie wieder ausgegeben bzw. gespeichert werden. Die Filter sind nur
dann aktiv, wenn der Schalter Filter im Hauptfenster aktiv ist. Wenn trotz
aktiven Schalter kein Filter exisitert, werden die Daten, so wie sie
ankommen wieder abgespielt bzw. gespeichert.

Liste der Filter:
Hier k�nnen Sie einen der definierten Filter ausw�hlen.

Neu:
Nach Klick auf diesen Schalter wir ein neuer Filter erstellt. Sie m�ssen
jedoch einen Namen eingegeben haben, der noch nicht verwendet wurde.

�bernehmen:
Definiert kopiert die Parameter im Bereich Filter und Aktion in den
ausgew�hlten Filter.

L�schen:
Entfernt des ausgew�hlten Filter.

Oben:
Verschiebt den ausgew�hlten Filter um eine Position nach oben.

Name:
Hier geben Sie den Namen eines neuen Filters an.

Filter:
Hier legen Sie fest welche Noten von diesem Filter behandelt werden sollen.
Sie k�nnen einen bestimmten Eingangskanal bzw. ein bestmmtes Eingabeger�t
oder alle ausw�hlen und festlegen, welche Notenwerte ber�cksichtigt werden
sollen. Wenn Sie als Filter Control ausgew�hlt haben, werden alle System-
und Kanalnachrichten ber�cksichtigt. Der eingegebene Notenwerden wird dann
allerdings ignoriert.

Aktion:
Hier legen Sie fest, wie die Note ausgegeben werden soll. Sie k�nnen einen
bestimmten Kanal ausw�hlen oder die Note an den gleichen Kanal schicken,
die der Controler verwendet hat. Ebenso k�nnen Sie ausw�hlen, auf welchem
Ausgaber�t die Note geschickt werden soll. Dar�ber hinaus, k�nnen Sie auch
einen Offset in Halbtonschritten f�r die Transformation angeben. +12 erh�ht
die Note um eine Oktave. -12 verringert die Note um eine Oktave. Desweitern
k�nnen Sie die Lautst�rke anpassen. Hierbei gibt es die M�glichkeit einen
Offset einzugeben (+/-Zahl), einen absoluten Wert (Zahl) oder einen
Prozentwert (Zahl%) einzutragen. Wenn Sie "Auto" eintragen, so wird die
Lautst�rke automatisch auf den gr��tm�glichen Wert gesetzt. Es werden aber
nur solche Werte ge�ndert, die nicht 0 sind, da manche MIDI-Controler statt
MIDI-Note-Off einfach die Lautst�rke auf 0 setzen.
Bei System- und Kanalnachrichten werden Noten- und Lautst�rkewerte
ignoriert. Bei Systemnachrichten wird zus�tzlich der Kanal ignoriert.
Wenn die Checkbox "Stop" ausgew�hlt ist, werden bei einem Treffer die
n�chsten Filter nicht mehr ber�cksichtigt.

Schlie�en:
Beendet das Fenster.

Standards:
Erzeugt f�r jeden Eingangskanal einen Filter, der bewirkt, da� die Note auf
den gleichen Kanal wieder ausgegeben wird.


MIDI-Editor
-----------

Zeigt die Liste aller MIDI-Ereignisse an. MIDI-Noten und Stimmwechsel
werden dabei zu einem Ereigniss zusammengefasst.

Im rechten Teil sehen Sie die Art des ausgew�hlten Ereignisses.

Kanal:
Hiermit legen Sie den MIDI-Kanal (1-16) fest.

Spur:
Hiermit legen sie die MIDI-Spur fest.

Takt.Beat.Zeit:
Hiermit legen Sie den Zeitpunkt fest.

Stimme:
Wenn dies ausgew�hlt ist, bearbeiten Sie einen Stimmwechsel. Sie k�nnen
dann die Stimme ausw�hlen.

Note / Lautst�rke / Dauer / Ausklingzeit:
Hiermit bearbeiten Sie ein Notenereigniss. Mit den Schaltern f�r die
ganzen, halben, viertel und achtel Noten k�nnen Sie die Dauer leicht
eingeben ohne sie ausrechnen zu m�ssen.

Controller:
Hiermit bearbeiten Sie ein Controllerereigniss.

Andere:
Dies ist f�r alle anderen unterst�tzen MIDI-Ereignisse vorgesehen.

System Exclusive:
Dies wird f�r MIDI System Exclusive Messages benutzt. Beachten Sie, es ist
nicht m�gchlich,SysEx Messages zu erzeugen noch zu �ndern.

�bernehmen:
�bertr�gt die Einstellungen zum ausgew�hlen MIDI-Ereigniss.

Neu:
Erzeugt einen neues MIDI-Ereigniss.

L�schen:
Entfernt ausgew�hlte MIDI-Ereignisse.

Instrument:
�ffnet eine virtuelle Klaviatur und Gitarrengriffbrett. Nach einem Klick
auf eine Note, wird diese im Editor �bernommen. Dar�ber hinaushaben Sie im
Instrumentenfenster die M�glichkeit. sich wichtige Akkorde und Skalen
anzeigen zu lassen. Ein Rechtsklick auf eine Note fixiert dies.

Mehrere Eintr�ge bearbeiten:
Bearbeitet alle markierten Eintr�ge.

Lautst�rke:
Hiermit k�nnen Sie die Lautst�rke eines einzelnen Kanals oder die
Gesamtlautst�rke einstellen.

Kan. tauschen:
Hiermit k�nnen Sie die MIDI-Ereignisse zweier Kan�le vertauschen.

Wdh.:
Hiermit k�nnen Sie alle markierten Daten kopieren und die Kopie ans Ende
der Datei stellen.

Transp.:
Hiermit k�nnen Sie alle Noten der Datei um einen bestimmten Wert h�her
(positive Werte) oder tiefer machen (negative Werte).

Versch.:
Verschiebt alle markierten Ereignisse um die den eingetragenen Zeitraum.
Im Eingabefeld links tragen Sie den Takt, Beat und Zeit in Milisekunden
ein, um den die Ereignisse verschoben werden sollen. Ein Vorangestelltes
Minuszeichen, verschiebt die Eintr�ge in Richtung Anfang der Datei.

Kopieren:
Kopiert alle markierten Ereignisse um die den eingetragenen Zeitraum.
Im Eingabefeld links tragen Sie den Takt, Beat und Zeit in Milisekunden
ein, um den die Ereignisse verschoben werden sollen. Ein Vorangestelltes
Minuszeichen, verschiebt die Eintr�ge in Richtung Anfang der Datei.

OK:
Schlie�t das Fenster und �bertr�gt die Daten zum Rekorderfenster.

Abbruch:
Schlie�t das Fenster und verwirft alle �nderungen.


Tips
----

Die Notenwerte werden in Halbtonschritten von 0 bis 127 durchnummeriert.
Das normale c hat den Notenwert 60.

Um f�r jede Note automatisch den korrekten Durakkord zu spielen, ben�tigen
Sie drei Filter:
- Filter: Kanal Alle, Note >= 0; Aktion: Kanal Controller, Note 0
- Filter: Kanal Alle, Note >= 0; Aktion: Kanal Controller, Note 4
- Filter: Kanal Alle, Note >= 0; Aktion: Kanal Controller, Note 7

F�r den Splitmodus (z.B. ab dem c) ben�tigen Sie zwei Filter:
- Filter: Kanal Alle, Note >= 60; Aktion: Kanal 0, Note 0
- Filter: Kanal Alle, Note < 60; Aktion: Kanal 1, Note 0
Bei den Kanaleinstellungen w�hlen Sie f�r die beiden Kan�le 0 und 1 die
gew�nschten Stimmen.

F�r den Dualmodus ben�tigen Sie zwei Filter:
- Filter: Kanal Alle, Note >= 0; Aktion: Kanal 0, Note 0
- Filter: Kanal Alle, Note >= 0; Aktion: Kanal 1, Note 0
Bei den Kanaleinstellungen w�hlen Sie f�r die beiden Kan�le 0 und 1 die
gew�nschten Stimmen.

Wenn Sie gleichzeitig den Drumcomputer im Keyboard laufen lassen wollen,
sollten Sie die Filter auf einen Kanal beschr�nken, da sonst auch der
Drumcomputer ver�ndert wird. Um dann den Drumcomputer h�ren zu k�nnen,
m�ssen Sie die Standardfilter generieren lassen und den Standardfilter, der
die Klaviatur Ihres Keyboards durchreichen w�rde (das ist �blicherweise
Kanal 1, bei besseren Keyboards ist das einstellbar), wieder l�schen.


Schlagzeugeditor
----------------

MIDI-Out:
Hier k�nnen Sie das Ausgabeger�t ausw�hlen.

Abspielen:
Spielt das Drumloop solange bis Sie den Schalter erneut bet�tigen ab.

Tempo/L�nge:
Hier geben Sie das Tempo in Beats Per Minute und die Anzahl der Takte des
Drumloops ein. Sie k�nnen bis zu 99 Takte bearbeiten.

Raster:
Hier legen Sie das Raster f�r die Eingabe fest. Der eingestellte Wert
wirkt sich nur auf neue Anschl�ge aus.

L�nge:
Hier legen Sie die L�nge eines Anschlags fest. Der eingestellte Wert wirkt
sich nur auf neue Anschl�ge aus.

Laust�rke:
Hier legen Sie die Lautst�rke eines Anschlags fest. Der eingestellte Wert
wirkt sich nur auf neue Anschl�ge aus.

Zeit:
Hier legen Sie die Ausklingzeit des Anschlags fest. Der eingestellte Wert
wirkt sich nur auf neue Anschl�ge aus.

Instrument:
Hier k�nnen Sie eines der verf�gbaren Instrumente laden.  Die m�glichehn
Instrumente werden aus der Datei drums.csv geladen. Die mitgelieferte Datei
enth�lt die Instrumente des Yamaha PSR 273/275. Sie k�nnen die Datei
beliebig nach Ihren Anfordernissen anpassen.

Hinzuf�gen:
Erzeugt einen neuen Pattern.

Instrumente:
Hier wird die Liste der Pattern angezeigt. Dies dient auch zum Bearbeiten
der Patterns. Jeder Schlag wird mit einem Balken entsprechend der L�nge und
Position angezeigt. Je dunkler der Balken ist desto lauter wird der Schlag
ausgef�hrt.

Mit einen Klick auf den Instrumentnamen k�nnen Sie diesen ausw�hlen.
Mit gedr�ckter Maustaste k�nnen Sie das Instrument auch verschieben. Ein
Linksklick im Schlagbereich f�gt einen Schlag hinzu bzw. entfernt diesen.
Zusammen mit der Shifttaste wird ein Schlag ausgew�hlt.
Ein Rechtsklick auf einen Schlag �bertr�gt dessen Lautst�rke auf den
Lautst�rkeregler. Zusammen mit der Steuerungstaste bewirkt ein Rechtsklick
die �bertragung der Lautst�rke vom Regler zum Schlag. Ein Linksklick auf
den Positionsbereich verschiebt den Positionszeiger f�r Verschiebe- und
Kopieroperation an die entsprechende Stelle.

Mit der Combobox links des Instrumenteneditors k�nnen Sie festlegen, welche
Instrumente gespielt werden sollen.

Mit dem Schalter Lautst�rke k�nnen Sie die Lautst�rke eines einzelnen
Instruments oder die Gesamtlautst�rke ver�ndern.

Stereo:
Dieser Schalter erlaubt es einzelnen Patternseine Position im Stereobild
des Schlagzeugs zuzuweisen.

L�schen:
L�scht den ausgew�hlten Pattern oder die ausgew�hlten Beats. Falls nichts
selektiert ist, werden alle Beats ab der aktuellen Position bis zu einge-
stellten L�nge gel�scht und die nachfolgenden Beats entsprechend
verschoben.

Kopieren:
Kopiert alle ausgew�hlten Beats an die aktuelle Position.

Verschieben:
Verschiebt alle ausgew�hlten Beats an die aktuelle Position.

Pause:
F�gt an der aktuellen Position eine Pause ein und verschiebt alle rechts
davon befindlichen Beats entsprechend nach rechts. Falls notwendig, wird
auch ein weiterer Takt hinzugef�gt.

Alles L�schen:
L�scht alle Patterns.

Schlie�en:
Beendet den Schlagzeugeditor.

Laden...:
L�dt einen Drumloop in den Speicher.

Speichern...:
Speichert einen Drumloop. Sie k�nnen eines der folgenden Formate ausw�hlen:
- Drum Dateien: Eigenes Format. Nur dieses Format kann geladen werden.
- Kommasepariert: CSV-Format um die Rohdaten anzeigen zu k�nnen.
- MIDI Dateien: Das MIDI-Format erlaubt die Weiterverarbeitung mit einem
  Sequenzer.


Loop Editor
-----------

Der Loop Editor k�nnen Sie zum Bearbeiten eines ganzen Songs verwenden.

MIDI-Out:
Hier k�nnen Sie das Ausgabeger�t ausw�hlen.

Tempo:
Hiermit legen Sie das Tempo des Loops fest. Alle Mididateien und Drum-
dateien werden automatisch umgerechnet.

Abspielen:
Spielt das Loop solange bis Sie den Schalter erneut bet�tigen ab. Wenn ein
Element ausgew�hlt wurde, so wird jede Wiederholung ab dem ausgew�hlten
Element gespielt. Beim Abspielen wird das gerade gespielte Element
angezeigt.

Liste:
Hier werden alle Elemente angezeigt. Sie k�nnen einfach mit Drag und Drop
Drumdateien oder Mididateien hinzuf�gen.

L�nger/K�rzer:
Mit diesen beiden Schaltern k�nnen Sie festlegen, wie oft das ausgew�hlte
Element abgespielt werden soll. Wenn Sie hier den Wert 0 ausw�hlen, wird
der Eintrag beim Speichern entfernt.

Oben/Unten:
Mit diesen beiden Schaltern, k�nnen Sie die Position des ausgew�hlten
Elements ver�ndern.

Schlie�en:
Beendet den Loop Editor.

Laden...:
L�dt einen Loop in den Speicher.

Speichern...:
Speichert einen Drumloop. Sie k�nnen eines der folgenden Formate ausw�hlen:
- Loop Dateien: Eigenes Format. Nur dieses Format kann geladen werden.
- Kommasepariert: CSV-Format um die Rohdaten anzeigen zu k�nnen.
- MIDI Dateien: Das MIDI-Format erlaubt die Weiterverarbeitung mit einem
  Sequenzer.






Anregungen, W�nsche, Kritik
---------------------------

Dieses Programm wurde erstellt, weil die Konfiguration des Splitmodus oder
des Dualmodus des Yamaha PSR 273/275 �ber das Keyboard sehr umst�ndlich ist
und beim Ausschalten auch verloren geht. 



Haftungsausschlu�
-----------------

Kaum ein Softwareprogramm ist frei von Fehlern. Erst recht dann nicht, wenn
es sich wie hier um eine Betaversion handelt. Wir k�nnen daher keine
Gew�hrleistung �bernehmen, da� alles zu Ihrer Zufriedenheit funktioniert
und k�nnen auch nicht, obwohl sehr viel M�he bei der Entwicklung
aufgewendet wurde, keine Haftung f�r eventuell auftretende Sch�den, die
durch die Benutzung der Software entstehen k�nnten, �bernehmen.



Versionshinweise
----------------

Folgende Fehler wurden behoben, folgende Neuerungen wurden eingef�hrt:

1.0�2:
- Nach dem Laden neuer Einstellungen wurde das Ausgabeger�t ge�ffnet und so
  der MIDI-Through-Modus aktiviert.
- Nach dem Beenden des Metronoms wurde das Ausgabeger�t nicht geschlossen.

1.1�1:
- Neuer Drumeditor.
- Die CSV Dateien enthalten nun MIDI-Nachrichten und Controller Changes im
  Klartext.
- Beim Beenden wurde das MIDI-Ger�t nicht immer geschlo�en, was bei manchen
  Treibern zum Systemcrash f�hren konnte.

1.2�1:
- Neuer MIDI-Editor zum Bearbeiten der MIDI-Dateien.
- Beim Metronom kann nun ausgew�hlt werden, welche Schlaginstrumente
  verwendet werden.
- Beim Schlie�en der Fenster werden Sie nun gefragt, ob evtl. ungesicherte
  Daten gespeichert werden sollen.

1.2�2:
- Drumpatterns, die keine Daten enthalten, werden automatisch gel�scht.
- Drumpatterns, bei denen das Schlaginstrument ge�ndert wurde, werden nun
  schon beim Editieren automatisch umbenannt.
- Das Abspielen von Drumpatterns kann nun auch dann gestartet werden, wenn
  noch keine Drumpatterns vorhanden sind.
- MIDI-Dateien, Drumpatterns und Einstellungen k�nnen nun direkt aus dem
  Explorer ge�ffnet werden. Hierzu m�ssen Sie aber mindestens einmal das
  Programm als Hauptbenutzer oder Administrator aufrufen, damit es die
  entsprechenden Verkn�pfungen in der Registrierungsdatenbank eintragen
  kann.
- Der Midieditor zeigt nun beim Programchangeeintrag auch den Namen des
  ausgew�hlten Instruments.
- Beim Laden neuer Dateien werden Sie nun gefragt, ob evtl. ungesicherte
  Daten gespeichert werden sollen.
- Es konnte vorkommen, da� der Patterneditor mit seinen Schlaginsrumenten
  durcheinander kam und daher entweder Pattern ganz verloren hat oder die
  Instrumente nicht ge�ndert werden konnten.
- Neuer Schalter Alles L�schen im Schlagzeugeditor.
- Neuer Schalter Lautst�rke im MIDI-Editor.
- Neuer Schalter Kan�le tauschen im MIDI-Editor.
- Kan�le werden nun von 1-16 durchnummeriert.
- Fehlerbehandlung beim MIDI-Editor verbessert.

1.3�1:
- Schlagzeugeditor �berarbeitet.
- Neuer Loop Editor.

1.3�2:
- Im Loop Editor kann nun das Tempo eingestellt werden.
- Recorder, Schlagzeugeditor und Loop Editor k�nnen nun mehrfach ge�ffnet werden.
- Der Loop Editor speichert Pfade nun relativ zur Loopdatei.
- Im Midi-Editor wurden neue Schalter zum Wiederholen und Transponieren
  eingef�gt.
- Wenn Recordereinstellungen per Doppelklick aus dem Explorer geladen
  werden sollen, werden diese nun korrekt geladen.
- Wenn ein Filtereintrag nach oben verschoben werden soll, so bleibt dieser
  nun selektiert.
- Beim Speichern der Settings im Recorder Window werden auch Filter Enabled
  und MIDI Through gespeichert.
- Beim Wechsel des Instruments werden nun die Filtereinstellungen und MIDI
  Through �bertragen.
- Der Recorder kann nun alle MIDI-Quellen gleichzeitig �ffnen.

1.4:
- Neue englische Oberfl�che
- Die Anzeige der gespielten Note verbessert.
- Im MIDI-Filter kann nun die Lautst�rke angepasst werden.

1.5:
- Im MIDI-Editor k�nnen nun mehrere Eintr�ge ausgew�hlt werden. Alle
  Bearbeitungsfunktionen beziehen sich nun auf die ausgew�hlten Eintr�ge.
- Neue Bearbeitungsfunktionen Kopieren und Verschieben im MIDI-Editor.
- Im Schlagzeugeditor wurden die Verschiebe- und Kopieroperationen
  hinzugef�gt.

2.0:
Es k�nnen nun mehrere Klangerzeuger gleichzeitig verwendet werden und
unterschiedliche Klangdateien f�r jedes einzelne Instrument konfiguriert
werden.

2.1:
- Fehler beim Speichern von Schlageugdateien f�r General MIDI Ger�te
  behoben.
- Neues Instrumentenfenster zum Anzeigen wichtiger Akkorde und Skalen
- Bei der Aufnahme von MIDI-Dateien wird nun f�r jedes Instrument eine
  eigene MIDI-Spur erzeugt.
- Beim Abspielen einer MIDI-Datei kann nun ein Rhythmus hinzugef�gt werden.
- MIDI System Exclusive Meldungen werden nun aufgenommen und abgespielt.
- Stabilit�tsprobleme unter Windows XP behoben.