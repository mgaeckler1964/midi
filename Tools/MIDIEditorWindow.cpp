/*
		Project:		MIDI Tools
		Module:			MIDIEditorWindow.cpp
		Description:	A MIDI Editor (used by recorder)
		Author:			Martin Gäckler
		Address:		Hopfengasse 15. A-4020 Linz
		Web:			https://www.gaeckler.at/

		Copyright:		(c) 2005-2018 Martin Gäckler

		This program is free software: you can redistribute it and/or modify  
		it under the terms of the GNU General Public License as published by  
		the Free Software Foundation, version 3.

		You should have received a copy of the GNU General Public License 
		along with this program. If not, see <http://www.gnu.org/licenses/>.

		THIS SOFTWARE IS PROVIDED BY Martin Gäckler, Germany, Munich ``AS IS''
		AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
		TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
		PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR
		CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
		SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
		LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
		USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
		ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
		OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
		OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
		SUCH DAMAGE.
*/

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <math.h>
#include <time.h>

#include <gak/fmtNumber.h>

#include <winlib/popup.h>
#include <winlib/stddlg.h>
#include <winlib/winlib.h>
#include <winlib/controlw.h>

#include "MIDIeditorWindow.h"
#include "MIDIrecorderWindow.h"
#include "MidiChannelVolume.h"
#include "MidiSwapChannels.h"
#include "PianoPainter.h"
#include "InstrumentWindow.h"

#include "midi_rc.h"

// --------------------------------------------------------------------- //
// ----- imported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module switches ----------------------------------------------- //
// --------------------------------------------------------------------- //

using namespace winlib;
using namespace gak;

// --------------------------------------------------------------------- //
// ----- constants ----------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- macros -------------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- type definitions ---------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- exported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

static unsigned char otherMessages[] =
{
	MIDI_POLYPHONE,
	MIDI_CHANNEL,
	MIDI_PITCH
};

// --------------------------------------------------------------------- //
// ----- class static data --------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- prototypes ---------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module functions ---------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class inlines ------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class constructors/destructors -------------------------------- //
// --------------------------------------------------------------------- //

MIDIeditorWindow::~MIDIeditorWindow( void )
{
	if( midiPlayer && midiPlayer->isRunning )
	{
		midiPlayer->StopThread();
		while( !midiPlayer->isRunning )
			idleLoop();
	}
}

// --------------------------------------------------------------------- //
// ----- class static functions ---------------------------------------- //
// --------------------------------------------------------------------- //

int MIDIeditorWindow::eventCompare( const MidiEditorEvent &e1, const MidiEditorEvent &e2 )
{
	int compareVal = e1.timeCode - e2.timeCode;

	if( !compareVal )
		compareVal = e1.track - e2.track;

	if( !compareVal )
		compareVal = e1.index - e2.index;

	if( !compareVal )
	{
		unsigned char msg1 = e1.message;
		unsigned char msg2 = e2.message;

		if( msg1 != msg2 )
		{
			if( msg1 == MIDI_NOTE_ON && msg2 != MIDI_NOTE_ON )
				compareVal = 1;
			else if( msg1 != MIDI_NOTE_ON && msg2 == MIDI_NOTE_ON )
				compareVal = -1;
			else
				compareVal = msg1 - msg2;
		}
		else if( msg1 == MIDI_NOTE_ON )
		{
			compareVal = e1.midiEvent.sound.note - e2.midiEvent.sound.note;
			if( !compareVal )
				compareVal = e1.midiEvent.sound.volume - e2.midiEvent.sound.volume;
			if( !compareVal )
				compareVal = e1.midiEvent.sound.length - e2.midiEvent.sound.length;
			if( !compareVal )
				compareVal = e1.midiEvent.sound.time - e2.midiEvent.sound.time;
		}
		else
		{
			compareVal = e1.midiEvent.midiData.data1 - e2.midiEvent.midiData.data1;
			if( !compareVal )
				compareVal = e1.midiEvent.midiData.data2 - e2.midiEvent.midiData.data2;
		}
	}

	if( !compareVal )
	{
		compareVal = e1.channel - e2.channel;
	}
	return compareVal;
}
// --------------------------------------------------------------------- //
// ----- class privates ------------------------------------------------ //
// --------------------------------------------------------------------- //

void MIDIeditorWindow::handleMessageType( int messageRadio )
{
	if( messageRadio == winlibGUI::voiceRadio_id )
	{
		theVoices.enable();
	}
	else
	{
		theVoices.disable();
	}

	if( messageRadio == winlibGUI::noteRadio_id )
	{
		noteSelect->enable();
		noteVolumeEdit->enable();
		noteLengthEdit->enable();
		noteTimeEdit->enable();
		full->enable();
		half->enable();
		quarter->enable();
		eighth->enable();
	}
	else
	{
		noteSelect->disable();
		noteVolumeEdit->disable();
		noteLengthEdit->disable();
		noteTimeEdit->disable();
		full->disable();
		half->disable();
		quarter->disable();
		eighth->disable();
	}

	if( messageRadio == winlibGUI::controllerRadio_id )
	{
		controllerSelect->enable();
		controllerEdit->enable();
	}
	else
	{
		controllerSelect->disable();
		controllerEdit->disable();
	}

	if( messageRadio == winlibGUI::otherRadio_id )
	{
		messageSelect->enable();
		data1Edit->enable();
		data2Edit->enable();
	}
	else
	{
		messageSelect->disable();
		data1Edit->disable();
		data2Edit->disable();
	}

	if( messageRadio == winlibGUI::sysExRadio_id )
	{
		channelSelect->disable();
	}
	else
	{
		channelSelect->enable();
	}
}

void MIDIeditorWindow::loadMidi( void )
{
	doEnterFunction("MIDIeditorWindow::loadMidi");

	MidiEditorEvent		theEditorEvent;
	unsigned long		programTimeCode = 0;
	unsigned char		curChannel, message, statusByte, controlMessage;

	bool				programChange[16], bankMSBchange[16], bankLSBchange[16];
	unsigned short		curTrack, eventTrack, numTracks;
	int					curIndex;
	STRING				name;
	VoiceCodes			voiceCodes[16];
	size_t				voiceIdx, numMidiEvents;

	editorEvents.clear();

	//
	// Convert MIDI events to editable events:
	// =======================================
	//
	numTracks = midiData->getNumTracks();
	numMidiEvents = midiData->size();

	for( curTrack = 0; curTrack < numTracks; curTrack++ )
	{
		// add an enrty for the current track
		name = midiData->getInstrument( curTrack );
		if( name.isEmpty() )
			name = midiData->getTrackName( curTrack );
		if( name.isEmpty() )
		{
			name = "Track#";
			name += formatNumber( curTrack );
			midiData->setTrackName( curTrack, name );
			name = "Intrument#";
			name += formatNumber( curTrack );
			midiData->setInstrument( curTrack, name );
		}
		trackSelect->addEntry( name );

		// clear voice change flags
		for( curChannel = 0; curChannel<16; curChannel++ )
		{
			programChange[curChannel] =
			bankMSBchange[curChannel] =
			bankLSBchange[curChannel] = false;

			voiceCodes[curChannel] = theVoices[0].voiceCodes;
		}

		// walk through midi events
		for( std::size_t i=0; i<numMidiEvents; i++ )
		{
			const MIDIevent	&theEvent = (*midiData)[i];
			eventTrack = theEvent.getTrack();
			if( eventTrack == curTrack )	// only current track
			{
				statusByte = theEvent.getStatusByte();
				message = theEvent.getMessage();
				curChannel = theEvent.getChannel();
				curIndex = theEvent.getIndex();
				if( message == MIDI_PROGRAM )
				{
					programChange[curChannel] = true;
					programTimeCode = theEvent.getTimeCode();
					voiceCodes[curChannel].program = theEvent.getData1();
				}
				else if( message == MIDI_CONTROLLER )
				{
					controlMessage = theEvent.getData1();
					if( controlMessage == MIDI_BANK_MSB )
					{
						bankMSBchange[curChannel] = true;
						programTimeCode = theEvent.getTimeCode();
						voiceCodes[curChannel].bankMSB = theEvent.getData2();
					}
					else if( controlMessage == MIDI_BANK_LSB )
					{
						bankLSBchange[curChannel] = true;
						programTimeCode = theEvent.getTimeCode();
						voiceCodes[curChannel].bankLSB = theEvent.getData2();
					}
					else	// any other controller message:
					{
						theEditorEvent.title = theEvent.getControllerText();
						theEditorEvent.track = curTrack;
						theEditorEvent.index = curIndex;
						theEditorEvent.message = message;
						theEditorEvent.channel = curChannel;
						theEditorEvent.timeCode = theEvent.getTimeCode();
						theEditorEvent.midiEvent.midiData.data1 = theEvent.getData1();
						theEditorEvent.midiEvent.midiData.data2 = theEvent.getData2();

						newEntry( theEditorEvent );
					}
				}
				else if( message == MIDI_NOTE_ON )
				{
					for( size_t j=i+1; j<numMidiEvents; j++ )
					{
						const MIDIevent &theEvent2 = (*midiData)[j];
						if( theEvent2.getMessage() == MIDI_NOTE_OFF
						&&	theEvent2.getTrack() == curTrack
						&&  theEvent2.getChannel() == curChannel
						&&	theEvent2.getData1() == theEvent.getData1() )
						{
							if( programChange[curChannel]
							||	bankMSBchange[curChannel]
							||	bankLSBchange[curChannel] )
							{
								programChange[curChannel] =
								bankMSBchange[curChannel] =
								bankLSBchange[curChannel] = false;

								theEditorEvent.title = "Voice";
								theEditorEvent.track = curTrack;
								theEditorEvent.index = curIndex;
								theEditorEvent.message = MIDI_PROGRAM;
								theEditorEvent.channel = curChannel;
								theEditorEvent.timeCode = programTimeCode;
								theEditorEvent.midiEvent.voiceCodes = voiceCodes[curChannel];

								voiceIdx = theVoices.findVoice( voiceCodes[curChannel] );
								if( voiceIdx != (size_t)-1 )
								{
									theEditorEvent.title += ' ';
									theEditorEvent.title += theVoices[voiceIdx].voice;
								}
								newEntry( theEditorEvent );
							}


							theEditorEvent.title = "Note ";
							theEditorEvent.track = curTrack;
							theEditorEvent.index = curIndex;
							theEditorEvent.message = message;
							theEditorEvent.channel = curChannel;
							theEditorEvent.timeCode = theEvent.getTimeCode();

							theEditorEvent.midiEvent.sound.note = theEvent.getData1();
							theEditorEvent.midiEvent.sound.volume = theEvent.getData2();
							theEditorEvent.midiEvent.sound.time = theEvent2.getData2();
							theEditorEvent.midiEvent.sound.length = theEvent2.getTimeCode() - theEditorEvent.timeCode;

							theEditorEvent.title += MIDIevent::getNoteText(
								theEditorEvent.midiEvent.sound.note
							);
							newEntry( theEditorEvent );
							break;
						}
					}
				}
				else if( statusByte == MIDI_SYS_EX_START )
				{
					theEditorEvent.title = theEvent.getMessageText();
					theEditorEvent.track = curTrack;
					theEditorEvent.index = curIndex;
					theEditorEvent.message = MIDI_SYS_EX_START;
					theEditorEvent.channel = 255;
					theEditorEvent.timeCode = theEvent.getTimeCode();

					theEditorEvent.sysExEvent = theEvent;
					newEntry( theEditorEvent );
				}
				else if( message != MIDI_NOTE_OFF )
				{
					theEditorEvent.title = theEvent.getMessageText();
					theEditorEvent.track = curTrack;
					theEditorEvent.index = curIndex;
					theEditorEvent.message = message;
					theEditorEvent.channel = curChannel;
					theEditorEvent.timeCode = theEvent.getTimeCode();
					theEditorEvent.midiEvent.midiData.data1 = theEvent.getData1();
					theEditorEvent.midiEvent.midiData.data2 = theEvent.getData2();
					newEntry( theEditorEvent );
				}
			}
		}
	}
	trackInfo = midiData->getTrackInfo();
	trackSelect->selectEntry( 0 );
}

void MIDIeditorWindow::copyValues2Editor( void )
{
	char	tmpValue[32];
	int		selectedEntry = eventList->getSelection();

	if( selectedEntry >= 0 )
	{
		const MidiEditorEvent	&theEditorEvent = editorEvents[selectedEntry];

		channelSelect->selectEntry( theEditorEvent.channel );
		trackSelect->selectEntry( theEditorEvent.track );
		timeCodeEdit->setText(
			midiData->getTimeCodeStr( theEditorEvent.timeCode )
		);

		if( theEditorEvent.message == MIDI_PROGRAM )
		{
			voiceRadio->setActive();
			noteRadio->clrActive();
			controllerRadio->clrActive();
			otherRadio->clrActive();
			sysExRadio->clrActive();
			handleMessageType( winlibGUI::voiceRadio_id );
			theVoices.selectVoice( theEditorEvent.midiEvent.voiceCodes );
		}
		else if( theEditorEvent.message == MIDI_NOTE_ON )
		{
			voiceRadio->clrActive();
			noteRadio->setActive();
			controllerRadio->clrActive();
			otherRadio->clrActive();
			sysExRadio->clrActive();

			handleMessageType( winlibGUI::noteRadio_id );
			noteSelect->selectEntry( theEditorEvent.midiEvent.sound.note );

			sprintf( tmpValue, "%d",
				(int)theEditorEvent.midiEvent.sound.volume
			);
			noteVolumeEdit->setText( tmpValue );
			sprintf( tmpValue, "%d",
				(int)theEditorEvent.midiEvent.sound.length
			);
			noteLengthEdit->setText( tmpValue );
			sprintf( tmpValue, "%d",
				(int)theEditorEvent.midiEvent.sound.time
			);
			noteTimeEdit->setText( tmpValue );

		}
		else if( theEditorEvent.message == MIDI_CONTROLLER )
		{
			voiceRadio->clrActive();
			noteRadio->clrActive();
			controllerRadio->setActive();
			otherRadio->clrActive();
			sysExRadio->clrActive();

			handleMessageType( winlibGUI::controllerRadio_id );
			controllerSelect->selectEntry( theEditorEvent.midiEvent.midiData.data1 );
			sprintf( tmpValue, "%d",
				(int)theEditorEvent.midiEvent.midiData.data2
			);
			controllerEdit->setText( tmpValue );
		}
		else if( theEditorEvent.message != MIDI_SYSTEM )
		{
			voiceRadio->clrActive();
			noteRadio->clrActive();
			controllerRadio->clrActive();
			otherRadio->setActive();
			sysExRadio->clrActive();

			handleMessageType( winlibGUI::otherRadio_id );
			for( size_t i=0; i<arraySize(otherMessages); i++ )
			{
				if( theEditorEvent.message == otherMessages[i] )
				{
					messageSelect->selectEntry( int(i) );
					break;
				}
			}

			sprintf( tmpValue, "%d",
				(int)theEditorEvent.midiEvent.midiData.data1
			);
			data1Edit->setText( tmpValue );
			sprintf( tmpValue, "%d",
				(int)theEditorEvent.midiEvent.midiData.data2
			);
			data2Edit->setText( tmpValue );
		}
		else
		{
			voiceRadio->clrActive();
			noteRadio->clrActive();
			controllerRadio->clrActive();
			otherRadio->clrActive();
			sysExRadio->setActive();
			handleMessageType( winlibGUI::sysExRadio_id );
		}
	}
}

void MIDIeditorWindow::deleteEntry( bool selectNext )
{
	ArrayOfInts	selectedEntries;
	size_t	numSelected = eventList->getSelectedItems( &selectedEntries );
	int		selectedEntry = 0;

	if( numSelected )
	{
		for( int i=(int)numSelected-1; i>=0; i-- )
		{
			selectedEntry = selectedEntries[i];

			editorEvents.removeElementAt( selectedEntry );
			eventList->deleteEntry( selectedEntry );
		}
		if( selectNext )
		{
			if( selectedEntry >= (int)editorEvents.size() )
			{
				selectedEntry--;
			}

			eventList->selectEntry( selectedEntry );
			copyValues2Editor();
		}
	}
}

size_t MIDIeditorWindow::newEntry( const MidiEditorEvent &newEvent )
{
	char	tmpBuffer[1024];
	size_t	numElements = editorEvents.size();

	sprintf(
		tmpBuffer, "%2d\t%s\t%s",
		newEvent.channel+1,
		(const char*)midiData->getTimeCodeStr(
			newEvent.timeCode
		),
		(const char *)newEvent.title
	);

	for( size_t i=0; i<numElements; i++ )
	{
		if( eventCompare( editorEvents[i], newEvent ) > 0 )
		{
			editorEvents.insertElement( newEvent, i );
			eventList->insertEntry( int(i), tmpBuffer );

/***/		return i;
		}
	}

	editorEvents.addElement( newEvent );
	eventList->addEntry( tmpBuffer );

	return numElements;
}

void MIDIeditorWindow::newEntry( void )
{
	if( !sysExRadio->isActive() )		// do not create a system exclusive event
	{
		unsigned long		newTimeCode;
		MidiEditorEvent		newEvent;
		const char			*errText = NULL;

		newEvent.channel = static_cast<unsigned char>(channelSelect->getSelection());
		newEvent.track = static_cast<unsigned short>(trackSelect->getSelection());
		newEvent.timeCode = midiData->parseTimeCode( timeCodeEdit->getText() );
		if( voiceRadio->isActive() )
		{
			newEvent.title = "Voice ";
			newEvent.message = MIDI_PROGRAM;
			const VoiceEntry &theVoice = theVoices.getSelectedVoice();
			newEvent.midiEvent.voiceCodes = theVoice.voiceCodes;
			newEvent.title += theVoice.voice;
		}
		else if( noteRadio->isActive() )
		{
			newEvent.message = MIDI_NOTE_ON;
			newEvent.midiEvent.sound.note = static_cast<unsigned char>(noteSelect->getSelection());
			newEvent.midiEvent.sound.volume = noteVolumeEdit->getText().getValueN<unsigned char>();
			newEvent.midiEvent.sound.length = noteLengthEdit->getText().getValueN<unsigned long>();
			newEvent.midiEvent.sound.time = noteTimeEdit->getText().getValueN<unsigned char>();
			newEvent.title = "Note ";
			newEvent.title += MIDIevent::getNoteText(
				newEvent.midiEvent.sound.note
			);

			newTimeCode = newEvent.timeCode + newEvent.midiEvent.sound.length;
			timeCodeEdit->setText(
				midiData->getTimeCodeStr( newTimeCode )
			);

			if( newEvent.midiEvent.sound.note > 127 )
				errText = "Note muß zwischen 0 und 127 liegen";
			else if( newEvent.midiEvent.sound.volume > 127 )
				errText = "Lautstärke muß zwischen 0 und 127 liegen";
			else if( newEvent.midiEvent.sound.time > 127 )
				errText = "Ausklingzeit muß zwischen 0 und 127 liegen";
		}
		else if( controllerRadio->isActive() )
		{
			newEvent.message = MIDI_CONTROLLER;
			newEvent.midiEvent.midiData.data1 = static_cast<unsigned char>(controllerSelect->getSelection());
			newEvent.midiEvent.midiData.data2 = controllerEdit->getText().getValueN<unsigned char>();
			newEvent.title = MIDIevent::getControllerText( newEvent.midiEvent.midiData.data1 );
			if( newEvent.midiEvent.midiData.data2 > 127 )
				errText = "Wert muß zwischen 0 und 127 liegen";
		}
		else
		{
			newEvent.message = otherMessages[messageSelect->getSelection()];
			newEvent.midiEvent.midiData.data1 = data1Edit->getText().getValueN<unsigned char>();
			newEvent.midiEvent.midiData.data2 = data2Edit->getText().getValueN<unsigned char>();
			newEvent.title = MIDIevent::getChannelMessageText( newEvent.message );
			if( newEvent.midiEvent.midiData.data1 > 127 )
				errText = "Wert#1 muß zwischen 0 und 127 liegen";
			else if( newEvent.midiEvent.midiData.data2 > 127 )
				errText = "Wert#2 muß zwischen 0 und 127 liegen";
		}

		if( errText )
		{
			messageBox( errText, "Fehler", MB_ICONERROR );
		}
		else
		{
			eventList->clearSelection();
			newEvent.index = 0;
			eventList->selectEntry( int(newEntry( newEvent )) );
		}
	}
}

void MIDIeditorWindow::changeVolume( void )
{
	size_t					curEvent;
	unsigned char			curChannel, maxVolumes[17];
	ChannelVolumeDialog		dialog;
	ArrayOfInts				selectedEntries;
	size_t					numSelected = eventList->getSelectedItems( &selectedEntries );

	if( numSelected )
	{
		for( curChannel=0; curChannel<17; curChannel++ )
		{
			maxVolumes[curChannel] = 0;
		}
		for( int i=(int)numSelected-1; i>=0; i-- )
		{
			curEvent = selectedEntries[i];
			const MidiEditorEvent &theEditorEvent = editorEvents[curEvent];
			if( theEditorEvent.message == MIDI_NOTE_ON )
			{
				if( maxVolumes[theEditorEvent.channel] <
					theEditorEvent.midiEvent.sound.volume )
				{
					maxVolumes[theEditorEvent.channel] =
					theEditorEvent.midiEvent.sound.volume;
				}
				if( maxVolumes[16] <
					theEditorEvent.midiEvent.sound.volume )
				{
					maxVolumes[16] =
					theEditorEvent.midiEvent.sound.volume;
				}
			}
		}
		dialog.create( this, maxVolumes );
		if( dialog.getModalResult() == IDOK )
		{
			unsigned char newVolume = dialog.getNewVolume();
			unsigned char selectedChannel = dialog.getSelectedChannel();

			if( maxVolumes[selectedChannel] != newVolume )
			{
				double factor;

				if( maxVolumes[selectedChannel] )
				{
					factor = (double)newVolume /
						(double)maxVolumes[selectedChannel];
				}

				for( int i=(int)numSelected-1; i>=0; i-- )
				{

					curEvent = selectedEntries[i];
					MidiEditorEvent &theEditorEvent =
						editorEvents[curEvent]
					;
					if( theEditorEvent.message == MIDI_NOTE_ON
					&&  (
							theEditorEvent.channel == selectedChannel
							|| selectedChannel == 16
						)
					)
					{
						if( maxVolumes[selectedChannel] )
						{
							theEditorEvent.midiEvent.sound.volume =
							static_cast<unsigned char>((
								double(theEditorEvent.midiEvent.sound.volume) *	factor + 0.5
							));
						}
						else
							theEditorEvent.midiEvent.sound.volume = newVolume;
					}
				}
			}
		}

		copyValues2Editor();
	}
}

void MIDIeditorWindow::swapChannels( void )
{
	SwapChannelDialog	dialog;
	char				tmpBuffer[128];
	ArrayOfInts			selectedEntries;
	size_t				numSelected = eventList->getSelectedItems( &selectedEntries );

	if( numSelected )
	{
		dialog.create( this );
		if( dialog.getModalResult() == IDOK )
		{
			unsigned char	srcChannel = dialog.getSrcChannel();
			unsigned char	destChannel = dialog.getDestChannel();
			if( srcChannel != destChannel )
			{
				for( size_t i=0; i<numSelected; i++ )
				{
					MidiEditorEvent &theEditorEvent = editorEvents[selectedEntries[i]];

					if( theEditorEvent.channel == srcChannel
					||  theEditorEvent.channel == destChannel )
					{
						if( theEditorEvent.channel == srcChannel )
							theEditorEvent.channel = destChannel;
						else
							theEditorEvent.channel = srcChannel;

						sprintf(
							tmpBuffer, "%2d\t%s\t%s",
							theEditorEvent.channel+1,
							(const char*)midiData->getTimeCodeStr(
								theEditorEvent.timeCode
							),
							(const char *)theEditorEvent.title
						);
						eventList->replaceEntry( selectedEntries[i], tmpBuffer );
					}
				}
			}
		}

		eventList->setSelectedItems( selectedEntries );
	}
}

void MIDIeditorWindow::repeatEntries( void )
{
	long			newTimeCode, timeCodeOffset;
	char			tmpBuffer[1024];
	double			timePerTakt, factor;
	size_t			numEntries = editorEvents.size();
	ArrayOfInts		selectedEntries;
	size_t			numSelected = eventList->getSelectedItems( &selectedEntries );

	if( numSelected )
	{
		const MidiEditorEvent &theLastEvent = editorEvents[numEntries-1];
		newTimeCode = theLastEvent.timeCode;
		if( theLastEvent.message == MIDI_NOTE_ON )
			newTimeCode += theLastEvent.midiEvent.sound.length;
		newTimeCode += 1;

		timePerTakt = midiData->getTimePerBarDbl();

		factor = newTimeCode / timePerTakt;
		factor = floor( factor + 0.5 );
		newTimeCode = long(factor * timePerTakt);

		const MidiEditorEvent &theFirstEvent = editorEvents[selectedEntries[0]];
		timeCodeOffset = newTimeCode-theFirstEvent.timeCode;

		for( std::size_t i=0 ; i<numSelected; i++ )
		{
			MidiEditorEvent theEvent = editorEvents[selectedEntries[i]];
			theEvent.timeCode += timeCodeOffset;

			sprintf(
				tmpBuffer, "%2d\t%s\t%s",
				theEvent.channel+1,
				(const char*)midiData->getTimeCodeStr(
					theEvent.timeCode
				),
				(const char *)theEvent.title
			);

			editorEvents.addElement( theEvent );
			eventList->addEntry( tmpBuffer );
		}

	}
}

void MIDIeditorWindow::transposeEntries( void )
{
	char			tmpBuffer[1024];
	unsigned char 	noteOffset = transposeEdit->getText().getValueN<unsigned char>();

	ArrayOfInts		selectedEntries;
	size_t			numSelected = eventList->getSelectedItems( &selectedEntries );

	if( numSelected )
	{
		if( noteOffset )
		{
			for( size_t i=0; i<numSelected; i++ )
			{
				MidiEditorEvent &theEditorEvent =
					editorEvents[selectedEntries[i]]
				;
				if( theEditorEvent.message == MIDI_NOTE_ON )
				{
					theEditorEvent.midiEvent.sound.note += noteOffset;
					theEditorEvent.title = "Note ";
					theEditorEvent.title += MIDIevent::getNoteText(
						theEditorEvent.midiEvent.sound.note
					);
				}

				sprintf(
					tmpBuffer, "%2d\t%s\t%s",
					theEditorEvent.channel+1,
					(const char*)midiData->getTimeCodeStr(
						theEditorEvent.timeCode
					),
					(const char *)theEditorEvent.title
				);
				eventList->replaceEntry( selectedEntries[i], tmpBuffer );
			}
		}

		eventList->setSelectedItems( selectedEntries );
	}
}

void MIDIeditorWindow::copyMoveEntries( bool copy )
{
	Array<MidiEditorEvent>		selectedEvents;

	long	timeCodeOffset = midiData->parseRelativeTimeCode(
		moveEdit->getText()
	);

	unsigned char 	noteOffset = transposeEdit->getText().getValueN<unsigned char>();
	ArrayOfInts		selectedEntries;
	size_t			numSelected = eventList->getSelectedItems( &selectedEntries );

	if( numSelected )
	{
		if( timeCodeOffset || noteOffset )
		{
			// update time codes and remove selected entries
			for( int i=int(numSelected)-1; i>=0; i-- )
			{
				int selected = selectedEntries[i];
				MidiEditorEvent theEditorEvent =
					editorEvents[selected]
				;

				if( !copy )
				{
					editorEvents.removeElementAt( selected );
					eventList->deleteEntry( selected );
				}

				if( timeCodeOffset )
					theEditorEvent.timeCode += timeCodeOffset;
				if( theEditorEvent.message == MIDI_NOTE_ON && noteOffset )
				{
					theEditorEvent.midiEvent.sound.note += noteOffset;
					theEditorEvent.title = "Note ";
					theEditorEvent.title += MIDIevent::getNoteText(
						theEditorEvent.midiEvent.sound.note
					);
				}

				selectedEvents.addElement( theEditorEvent );
			}

			// insort selected entries
			for( int i=int(numSelected)-1; i>=0; i-- )
			{
				int selected = int(newEntry( selectedEvents[i] ));
				selectedEntries[i] = selected;
			}
		}

		if( !copy )
		{
			eventList->setSelectedItems( selectedEntries );
		}
	}
}

void MIDIeditorWindow::convertEditorEvents( MIDIdata *midiData, bool selected )
{
	MIDIevent		newEvent;
	size_t			startPos, numSelected;

	ArrayOfInts		selectedEntries;

	if( selected )
	{
		numSelected = eventList->getSelectedItems( &selectedEntries );
		if( numSelected == 0 )
		{
			// play all
			numSelected = editorEvents.size();
			startPos = 0;
			selected = false;
		}
		else if( numSelected == 1 )
		{
			// play from selection
			numSelected = editorEvents.size();
			startPos = selectedEntries[0];
			selected = false;
		}
		else
			startPos = 0;
	}
	else
	{
		// play all
		numSelected = editorEvents.size();
		startPos = 0;
	}

	midiData->clear();
	for( size_t i=startPos; i<numSelected; i++ )
	{
		MidiEditorEvent &theEditorEvent = editorEvents[ selected ? selectedEntries[i] : i ];

		newEvent.setTimeCode( theEditorEvent.timeCode );
		newEvent.setTrack( theEditorEvent.track );
		newEvent.setIndex( theEditorEvent.index );
		if( theEditorEvent.message == MIDI_PROGRAM )
		{
			newEvent.setProgramChange(
				theEditorEvent.channel, theEditorEvent.midiEvent.voiceCodes.program
			);
			midiData->addElement( newEvent );
			if( theEditorEvent.midiEvent.voiceCodes.bankMSB < 128 )
			{
				newEvent.setBankMSB(
					theEditorEvent.channel, theEditorEvent.midiEvent.voiceCodes.bankMSB
				);
				midiData->addElement( newEvent );
			}
			if( theEditorEvent.midiEvent.voiceCodes.bankLSB < 128 )
			{
				newEvent.setBankLSB(
					theEditorEvent.channel, theEditorEvent.midiEvent.voiceCodes.bankLSB
				);
				midiData->addElement( newEvent );
			}
		}
		else if( theEditorEvent.message == MIDI_NOTE_ON )
		{
			newEvent.setNoteOn(
				theEditorEvent.channel,
				theEditorEvent.midiEvent.sound.note,
				theEditorEvent.midiEvent.sound.volume
			);
			midiData->addElement( newEvent );
			newEvent.setTimeCode(
				theEditorEvent.timeCode + theEditorEvent.midiEvent.sound.length
			);
			newEvent.setNoteOff(
				theEditorEvent.channel,
				theEditorEvent.midiEvent.sound.note,
				theEditorEvent.midiEvent.sound.time
			);
			midiData->addElement( newEvent );
		}
		else if( theEditorEvent.message == MIDI_SYS_EX_START )
		{
			midiData->addElement( theEditorEvent.sysExEvent );
		}
		else
		{
			newEvent.setMessageCode(
				theEditorEvent.message,
				theEditorEvent.channel,
				theEditorEvent.midiEvent.midiData.data1,
				theEditorEvent.midiEvent.midiData.data2
			);
			midiData->addElement( newEvent );
		}
	}

	midiData->setTrackInfo( trackInfo );
}

// --------------------------------------------------------------------- //
// ----- class protected ----------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class virtuals ------------------------------------------------ //
// --------------------------------------------------------------------- //

void MidiEditPlayerThread::ExecuteThread( void )
{
	clock_t		actTime, nextTime, startTime;

	if( playerHandles.openOutMidi( midiDev ) )
	{
		startTime = clock();
		for(
			MIDIdata::const_iterator it = midiData->cbegin(),
				endIT = midiData->cend();
			!terminated && it != endIT;
			++it
		)
		{
			const MIDIevent	&midiMsg = *it;

			actTime = clock();
			nextTime = startTime + midiMsg.getTimeCode();
			if( nextTime > actTime )
				Sleep((nextTime-actTime)*1000/CLK_TCK);

			playerHandles.playMidiEvent( midiDev, midiMsg );
		}

		if( !terminated )
		{
			int 	barTime = midiData->getTimePerBar();
			int		quarterTime = barTime / 4;

			// wait three bars for the release time
			for( size_t i=0; i<12 && !terminated; i++ )
			{
				Sleep( quarterTime );
			}
		}
		playerHandles.stopOutMidi( midiDev );
	}

	const_cast<MIDIeditorWindow*>(owner)->showPlayLabel();
}

ProcessStatus MIDIeditorWindow::handleCreate( void )
{
	size_t			i;

	stopLabel = appObject->loadString( winlibGUI::STOP_LABEL_id );
	playLabel = appObject->loadString( winlibGUI::PLAY_LABEL_id );

	ChannelSelect::fill( channelSelect );

	int	tabs[] = { 25, 75 };
	eventList->setTabStops( 2, tabs );

	channelSelect->selectEntry( 0 );

	theVoices.loadVoices( voiceFile, groupSelect, voiceSelect );
	voiceRadio->setActive();
	handleMessageType( winlibGUI::voiceRadio_id );

	for( unsigned char note=0; note<128; note++ )
	{
		noteSelect->addEntry( MIDIevent::getNoteText( note ) );
	}
	noteSelect->selectEntry( 0 );

	for( unsigned char controllerMsg=0; controllerMsg<128; controllerMsg++ )
	{
		controllerSelect->addEntry( MIDIevent::getControllerText( controllerMsg ) );
	}
	controllerSelect->selectEntry( 0 );

	for( i=0; i<arraySize(otherMessages); i++ )
	{
		messageSelect->addEntry(
			MIDIevent::getChannelMessageText( otherMessages[i] )
		);
	}
	messageSelect->selectEntry( 0 );

	loadMidi();

	return psDO_DEFAULT;
}

ProcessStatus MIDIeditorWindow::handleOk( void )
{
	convertEditorEvents( midiData, false );
	return psDO_DEFAULT;
}

ProcessStatus MIDIeditorWindow::handleButtonClick( int btn )
{
	doEnterFunction("MIDIeditorWindow::handleButtonClick");
	static PushButton *lastHighlight = NULL;

	switch( btn )
	{
		case winlibGUI::voiceRadio_id:
		case winlibGUI::noteRadio_id:
		case winlibGUI::controllerRadio_id:
		case winlibGUI::otherRadio_id:
		case winlibGUI::sysExRadio_id:
			handleMessageType( btn );
			break;
		case winlibGUI::full_id:
		{
			int theTime = midiData->getTimePerFulNote();
			STRING value = formatNumber( theTime );
			noteLengthEdit->setText( value );

			if( lastHighlight )
			{
				lastHighlight->clrHighlight();
			}
			full->setHighlight();
			lastHighlight = full;
			break;
		}
		case winlibGUI::half_id:
		{
			int theTime = midiData->getTimePerFulNote() / 2;
			STRING value = formatNumber( theTime );
			noteLengthEdit->setText( value );

			if( lastHighlight )
			{
				lastHighlight->clrHighlight();
			}
			half->setHighlight();
			lastHighlight = half;
			break;
		}
		case winlibGUI::quarter_id:
		{
			int theTime = midiData->getTimePerFulNote() / 4;
			STRING value = formatNumber( theTime );
			noteLengthEdit->setText( value );

			if( lastHighlight )
			{
				lastHighlight->clrHighlight();
			}
			quarter->setHighlight();
			lastHighlight = quarter;
			break;
		}
		case winlibGUI::eighth_id:
		{
			int theTime = midiData->getTimePerFulNote() / 8;
			STRING value = formatNumber( theTime );
			noteLengthEdit->setText( value );

			if( lastHighlight )
			{
				lastHighlight->clrHighlight();
			}
			eighth->setHighlight();
			lastHighlight = eighth;
			break;
		}
		case winlibGUI::deleteEntryButton_id:
			deleteEntry( true );
			break;
		case winlibGUI::saveEntryButton_id:
			if( !sysExRadio->isActive() )	// do not overwrite system
			{								// exclusive messages
				deleteEntry( false );
			}
			else
			{
				break;
			}
		case winlibGUI::newEntryButton_id:
			newEntry();
			break;

		case winlibGUI::volumeButton_id:
			changeVolume();
			break;

		case winlibGUI::swapChannelsButton_id:
			swapChannels();
			break;

		case winlibGUI::repeatButton_id:
			repeatEntries();
			break;

		case winlibGUI::transposeButton_id:
			transposeEntries();
			break;

		case winlibGUI::moveButton_id:
			copyMoveEntries( false );
			break;

		case winlibGUI::copyButton_id:
			copyMoveEntries( true );
			break;

		case winlibGUI::instrumentButton_id:
		{
			InstrumentWindow *thePopup = new InstrumentWindow( this );
			thePopup->create( this );
			break;
		}
		default:
/***/		return MIDIeditorWindow_form::handleButtonClick( btn );
	}
	return psPROCESSED;
}

ProcessStatus MIDIeditorWindow::handleSelectionChange( int control )
{
	doEnterFunction("MIDIeditorWindow::handleSelectionChange");

	switch( control )
	{
		case winlibGUI::groupSelect_id:
			theVoices.handleGroupSelection();
			break;
		case winlibGUI::eventList_id:
			copyValues2Editor();
			break;
		default:
/***/		return MIDIeditorWindow_form::handleSelectionChange( control );
	}
	return psPROCESSED;
}

ProcessStatus MIDIeditorWindow::handleCommand( int cmd )
{
	doEnterFunction("MIDIeditorWindow::handleCommand");
	switch( cmd )
	{
		case winlibGUI::playButton_id:
			if( !midiPlayer || !midiPlayer->isRunning )
			{
				convertEditorEvents( &playerData, true );
				playerData.alignTimeCodes();

				midiPlayer = new MidiEditPlayerThread( &playerData, midiDev, this );
				midiPlayer->StartThread();
				showStopLabel();
			}
			else if( midiPlayer )
			{
				midiPlayer->StopThread();
				while( midiPlayer->isRunning )
					idleLoop();
				playerHandles.stopAllSoundMidi();
				showPlayLabel();
			}
			break;
		default:
/***/		return MIDIeditorWindow_form::handleCommand( cmd );
	}
	return psPROCESSED;
}

// --------------------------------------------------------------------- //
// ----- class publics ------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- entry points -------------------------------------------------- //
// --------------------------------------------------------------------- //


