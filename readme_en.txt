MIDI Tools 2.3.2                                            July, 20th 2014
===========================================================================

(c) 2007-2021 by Martin Gäckler, Linz, Austria, https://www.gaeckler.at/



Introduction
------------

This program applies new features to your MIDI sound system. You can record
and playback your MIDI data and store them in MIDI files.


Usage
-----

Allthoug the usage is very simple, let us say some words:


Toolbar
-------

The tool bar is used the start the compontents of MIDI Tools:

Recorder:
Opens the MIDI recorder any playback window.

Drum Editor:
Opens the Drum Editor to create and play drum patterns.

Loop Editor:
Opens the Loop Editor to create and play loops.

MIDI Devices:
Opens the device configuration. This windows allows you to configure the
name of the instrument connected to a midi port and (for midi output
devices) the CSV files with the voices and drum kits. By default MIDI Tools
are using control codes for general MIDI devices. There are also CSV files
available for Yamaha PSR-275 and Yamaha SY-35.

Close:
Terminates all tools and exists.

Englisch:
When selected the MIDI Tools try to load the english GUI upon the next
start.


MIDI Recorder
-------------

MIDI-In:
Here you can select the input device.

Record:
Starts recording of all MIDI events.

MIDI-Out:
Here you can select the output device.

Play:
Starts playing the MIDI Data.

Auto Rhythm:
Adds a rhythm to the notes during play.

Tempo:
Here you can enter the beat.

Apply:
Applies the new beat to recorded midi data.

Metronome:
Starts playing the metronome you can select which clicks will be used.

Midi Trough:
When selected, all MIDI events will be sent to the output device.

Filter:
When selected, all filter settings will be applied before playing MIDI
events.

Channels:
Opens the Channel Settings (see above) for the selected MIDI output device.

Close:
Terminates the tool.

Load/Save:
You can save and load all MIDI Settings (channel and filter) and all MIDI
events.

Edit:
Opens the MIDI Editor.


Channel Settings
----------------

Here you can change common settings for all 16 MIDI channels.

Note: It depends on the MIDI output device how these settings will affect
your music.

Channel:
The channel you are changing.

Voice:
Select the voice you want to here. All voices are for Yamaha PSR 273/275.
Create a new list by changing the file voices.csv.

Stereo:
Select the stereo position.

Volume:
Change the volume.

Effect 1-5:
Change the value for the 5 MIDI effects available.

Sound 1-5:
Change the value for the 5 MIDI sound varitations available.

Expression:
Change the value of the expression pedal.

Close:
Closes the window.

Standard:
Changes all values to standard values.


Filter Settings
---------------

List:
Select one filter rule to edit.

New:
Creates a new filter rule. You must enter a unique name before creating a
new rule.

Apply:
Changes the selected filter rule.

Delete:
Deletes the selected filter rule.

Up:
Moves the selected filter rule up one position.

Name:
Enter a unique name for the filter rule.

Filter:
Select which midi events will be affected by the rule. Control will select
all channel and system messages.

Action:
Select how to change the MIDI event before playing:
You can select the midi output device, channel, transpone the note (+12 is
one octave higher,
-12 is one octave lower), change the volume (offset +/-number, percent
200%, an absolute value 0-127 or Auto).
The note offset and volume changes have no effect on system and channel
messages. The selected output channel has no effect on system messages. 
The checkbox "Stop" stops terminates the filter checking for all notes
matching this filter rule.

Close:
Closes the window.

Standards:
Creates a standard filter rule for each channel.


MIDI-Editor
-----------

The MIDI Editor allows you to edit all recored MIDI events.

The main area shows all midi events. To edit an event select it before
changing the values.

Channel:
Select the channel of the event.

Track:
Select the midi trac for the event.

Bar.Beat.Time:
Change the time of the event.

Voice:
Creates/changes a program change event.

Note/Volume/Length/Velocity:
Creates/changes a Note On/Off event pair.

Controler:
Creates/changes a controller event.

Other:
Creates/changes other events

System Exclusive:
This is for recorded system exclusive messages. These messages cannot be
created or changed.

Apply:
Changes the selected event with the selected values.

New:
Creates a new event with the selected values.

Delete:
Deletes the selected events.

Instrument:
Opens an instrument window showing a guitar and piano. Using this window
you can easily select a note, display a chord or a scale.

Edit Mulitple Entries:
The following commands change all selected entries.

Volume:
Changes the volume of all Note On Events for one or all channels.

Swap Channels:
Changes all events of two channels.

Repeat:
Duplicates all events to the end.

Transpone:
Transpones all midi note events. The value must be entered in the edit
field left to the button.

Move:
Moves all events to a new position. You can enter the number of bars, beats
and time in miliseconds in the edit control on the left. A minus sign at
the beginning moves the entries in direction to the begining of the file

Copy:
Copies all events to a new position. You can enter the number of bars,
beats and time in miliseconds in the edit control on the left. A minus sign
at the beginning moves the entries in direction to the begining of the
file.


Drum Editor
-----------

The Drum Editor allows you to create and edit drum patterns.

MIDI-Out:
Select the output device.

Play:
Starts playing the drum pattern until you click the button again.

Tempo/Length:
Here you can enter beats per minute and the length of the pattern.

Grid:
Here you can select the grid for new beats.

Length:
Here you can select the length for new beats.

Volume:
Here you can select the volume for new beats.

Velocity:
Here you can select the velocity for new beats.

Instrument:
Here you can select the instruments for your pattern.

Add:
Adds the selected instrument to your pattern.

Instruments:
Shows your pattern.
- A left click adds or removes a beat to or from the pattern.
- A shift left click selects a beat.
- A right click copies the volume from the beat to the volume selector.
- A control right click copies the volume from the volume selector to the
  beat.
- A left click on the position bar moves the position pointer for the
  copy and move operations.

With the select box left to the pattern you can define which instruments
play.

With the volume button you can change the volume of one or all instruments
of your pattern.

Delete:
Removes the selected instrument or all selected beats from your pattern.

Delete All:
Removes all instruments from your pattern.

Copy:
Copies all selected beats to the new position.

Move:
Moves all selected beats to the new position.

Close:
Exits the drum editor.

Load:
Loads a drum file

Save:
Saves a drum file.


Loop Editor
-----------

The Loop Editor allows to loops containg drum patterns or standard midi
files. By dragging drum files or midi files to the main area you can add
patterns to the loop.

MIDI-Out:
Select the output device.

Tempo:
Change the bpm (beats per minute) of the loop.

Play:
Starts plying the loop until you click the button again.

Longer:
Increases the number of loops playing the selected pattern.

Shorter:
Decreases the number of loops playing the selected pattern. When the number
is 0 the pattern will be removed before saving the loop file.

Up/Down:
Moves the selected pattern up and down.

Close:
Exits the loop editor

Load:
Loads a loop file.

Save:
Saves a loop file.
