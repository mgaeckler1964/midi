/*
		Project:		MIDI Tools
		Module:			MIDIrecorderWindow.h
		Description:	MIDI recording and controlling
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
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <gak/xmlParser.h>
#include <gak/numericString.h>

#include <winlib/winlib.h>
#include <winlib/winapp.h>

#include "MIDIrecorderWindow.h"
#include "MIDIeditorWindow.h"
#include "PianoPainter.h"
#include "InstrumentWindow.h"

// --------------------------------------------------------------------- //
// ----- constants ----------------------------------------------------- //
// --------------------------------------------------------------------- //

using namespace winlib;
using namespace gak;

#define	PIANO_SIZE		40

// --------------------------------------------------------------------- //
// ----- type definitions ---------------------------------------------- //
// --------------------------------------------------------------------- //

struct MIDInotePresentation
{
	size_t			midiDev;
	unsigned char	channel, note;
	STRING			presentation;
};
typedef Array<MIDInotePresentation>	MIDInotePresentations;

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

#ifdef __BORLANDC__
#	pragma option -RT-
#endif

class PianoArea : public ChildWindow
{
	private:
	int						lineHeight;
	Size					size;

	MIDInotePresentations	notesReceived;
	MIDInotePresentations	notesPlaying;
	PianoPainter			thePainter;

	private:
	int getLineHeight()
	{
		if( !lineHeight )
		{
			size = getClientSize();
			lineHeight = size.height - 40;
			lineHeight /= 20;
		}

		return lineHeight;
	}

	bool isNoteOnLine( unsigned char midi_note )
	{
		if( midi_note == MIDI_NOTE_E3
		||  midi_note == MIDI_NOTE_G3
		||  midi_note == MIDI_NOTE_H3
		||  midi_note == MIDI_NOTE_D4
		||  midi_note == MIDI_NOTE_F4
		||  midi_note == MIDI_NOTE_A2
		||  midi_note == MIDI_NOTE_F2
		||  midi_note == MIDI_NOTE_D2
		||  midi_note == MIDI_NOTE_H1
		||  midi_note == MIDI_NOTE_G1 )
			return true;
		else
			return false;
	}

	int getNoteCenter( unsigned char midi_note, bool *onLine );

	void drawFrameBorder( Device &context );
	void drawNoteLines( Device &context );

	virtual ProcessStatus handleRepaint( Device &hDC );

	private:
	static const char className[];
	public:
	static void registerClass( void );

	virtual STRING getWindowClassName( void ) const;
	PianoArea( BasicWindow *owner ) : ChildWindow( owner )
	{
		registerClass();
		lineHeight = 0;
	};

	void drawNoteOnLine( Device &context, unsigned char note, bool noteOn, bool halfTone, bool isSent, int xPos );
	void showMIDIevent( const MIDIevent &msg, size_t midiDev, bool isSent );
	void clearEvents( void )
	{
		if( notesReceived.size() || notesPlaying.size() )
		{
			MIDIevent	theEvent;
			notesReceived.clear();
			for(
				MIDInotePresentations::const_iterator it = notesPlaying.cbegin(),
					endIT = notesPlaying.cend();
				it != endIT;
				++it
			)
			{
				const MIDInotePresentation	&theNote = *it;

				theEvent.setNoteOn( theNote.channel, theNote.note, 0 );
				playerHandles.playMidiEvent( theNote.midiDev, theEvent );
				theEvent.setNoteOff( theNote.channel, theNote.note, 0 );
				playerHandles.playMidiEvent( theNote.midiDev, theEvent );
			}
			notesPlaying.clear();
			invalidateWindow();
		}
	}
};

// --------------------------------------------------------------------- //
// ----- exported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class static data --------------------------------------------- //
// --------------------------------------------------------------------- //

const char PianoArea::className[] = "PianoArea";

// --------------------------------------------------------------------- //
// ----- prototypes ---------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class constructors/destructors -------------------------------- //
// --------------------------------------------------------------------- //

MIDIrecorderWindow::MIDIrecorderWindow( BasicWindow *owner ) : MIDIrecorderWindow_form( owner )
{
	midiPlayer = NULL;
	metronomThread = NULL;
	metronomWindow = NULL;
	theMidiFilterWindow = NULL;
	theMidiChannelsWindow = NULL;
	filterEnabled = false;
	metronomWindowXpos = metronomWindowYpos = 100;

	settingsChangedFlag = midiChangedFlag = false;
	midiStatus = MIDI_WAITING;
}

// --------------------------------------------------------------------- //
// ----- class static functions ---------------------------------------- //
// --------------------------------------------------------------------- //

void PianoArea::registerClass( void )
{
	static bool registered = false;

	if( !registered )
	{
		WNDCLASS	wc;

		fillDefaultClass( &wc );
		wc.lpszClassName	 = className;

		registered = ChildWindow::registerClass( &wc );
	}
}

// --------------------------------------------------------------------- //
// ----- class privates ------------------------------------------------ //
// --------------------------------------------------------------------- //

int PianoArea::getNoteCenter( unsigned char midi_note, bool *onLine )
{
	double	noteCenter;
	int		lineHeight = getLineHeight();

	unsigned char	note	= (unsigned char)(midi_note-LEFT_OFFSET);
	unsigned char	octave	= (unsigned char)(note / 12);

	note = (unsigned char)(note%12);
	noteCenter = lineHeight*(3.5*octave +PianoPainter::noteInfo[note].lineFactor);

	if( PianoPainter::isWhiteKey( midi_note ) )
		*onLine = isNoteOnLine( midi_note );
	else
		*onLine = isNoteOnLine( (unsigned char)(midi_note-1) );

	return int(18*lineHeight-noteCenter);
}

void PianoArea::drawFrameBorder( Device &context )
{
	context.getPen().selectPen( Pen::spBlack );
	context.getBrush().selectBrush( Brush::sbNull );

	context.rectangle( 0, 0, size.width, size.height );
}

void PianoArea::drawNoteLines( Device &context )
{
	bool	dummy;
	int		i, yPos;

	static int noteLinePos[10];
	static bool first = true;

	if( first )
	{
		// violin schlüssel
		i = 0;
		noteLinePos[i++] = getNoteCenter( MIDI_NOTE_E3, &dummy );
		noteLinePos[i++] = getNoteCenter( MIDI_NOTE_G3, &dummy );
		noteLinePos[i++] = getNoteCenter( MIDI_NOTE_H3, &dummy );
		noteLinePos[i++] = getNoteCenter( MIDI_NOTE_D4, &dummy );
		noteLinePos[i++] = getNoteCenter( MIDI_NOTE_F4, &dummy );

		noteLinePos[i++] = getNoteCenter( MIDI_NOTE_A2, &dummy );
		noteLinePos[i++] = getNoteCenter( MIDI_NOTE_F2, &dummy );
		noteLinePos[i++] = getNoteCenter( MIDI_NOTE_D2, &dummy );
		noteLinePos[i++] = getNoteCenter( MIDI_NOTE_H1, &dummy );
		noteLinePos[i]   = getNoteCenter( MIDI_NOTE_G1, &dummy );
	}

	context.getPen().selectPen( Pen::spBlack );

	for( i=9; i>=0; i-- )
	{
		yPos = noteLinePos[i];
		context.horizontalLine( 0, size.width, yPos );
	}
}

void MIDIrecorderWindow::checkMidiThrough( void )
{
	doEnterFunction("MIDIrecorderWindow::checkMidiThrough");

	if( midiStatus == MIDI_WAITING  )
	{
		if( MIDIthrough->isActive() )
		{
			if( openInMidi() )
			{
				MIDIevent	theEvent;

				for( size_t midiDev = 0; midiDev < playerHandles.size(); midiDev++ )
				{
					if( (filterEnabled || midiDev == getDefaultPlayer())
					&&  playerHandles.openOutMidi( midiDev ) )
					{
						for( unsigned char channel=0; channel<=15; channel++ )
						{
							theEvent.setLocalOff( channel );
							playerHandles.playMidiEvent( midiDev, theEvent );
							sendChannelSettings( midiDev, channel );
						}
					}
				}
			}
			else
			{
				messageBox( winlibGUI::OPEN_IN_FAILED_id, "MIDI Recorder" );
			}
		}
		else
		{
			recorderHandles.stopInMidi( this );
			thePianoArea->clearEvents();
			playerHandles.stopAllSoundMidi();
			playerHandles.stopAllOutMidi();
		}
	}
}

size_t MIDIrecorderWindow::openInMidi( void )
{
	doEnterFunction("MIDIrecorderWindow::openInMidi");

	size_t	numOpenDevices;

	size_t	midiDev = MIDIrecSelect->getSelection();
	size_t	midiCount = recorderHandles.size();
	for( size_t i=0; i<midiCount; i++ )
	{
		if( midiDev == midiCount || midiDev == i )
			recorderHandles.openInMidi( i, this );
		else
			recorderHandles.stopInMidi( i, this );
	}
	numOpenDevices = recorderHandles.getNumOpenDevices( this );
	if( numOpenDevices )
	{
		MIDIthrough->enable();
		recordButton->enable();
		MIDIrecSelect->enable();
	}
	else
	{
		MIDIthrough->disable();
		recordButton->disable();
		MIDIrecSelect->disable();
	}
	return numOpenDevices;
}

void MIDIrecorderWindow::saveMidi( void )
{
	doEnterFunction("MIDIrecorderWindow::saveMidi");

	SaveFileAsDialog		dlg;

	if( midiStatus == MIDI_PLAYING  )
		stopPlayMidi();
	else if( midiStatus == MIDI_RECORDING  )
		stopRecMidi();

	dlg.setFilename( lastMidiFile );
	if( dlg.getDirectory().isEmpty() )
		dlg.setPersonalMusic();

	dlg.addDefaultExtension( "mid" );
	dlg.addDefaultExtension( "csv" );
	if( dlg.create( this, winlibGUI::SAVE_MIDI_id, winlibGUI::RecorderFiles_ids, winlibGUI::RecorderFiles_count ) )
	{
		lastMidiFile = dlg.getFilename();
		if( dlg.getFilterIndex() == 1 )
		{
			midiData.removeUnusedTracks();
			midiData.saveMidiFile( lastMidiFile, false );
		}
		else
			saveMidi2CSV( lastMidiFile );

		midiChangedFlag = false;
	}
}


void MIDIrecorderWindow::saveSettings( void )
{
	doEnterFunction("MIDIrecorderWindow::saveSettings");

	unsigned char		effect, sound;
	STRING				name;
	SaveFileAsDialog	dlg;

	dlg.setFilename( lastOptionFile );
	if( dlg.getDirectory().isEmpty() )
		dlg.setPersonalMusic();
	if( dlg.create( this, winlibGUI::SAVE_SETTINGS_id, winlibGUI::SettingsFiles_ids, winlibGUI::SettingsFiles_count ) )
	{
		lastOptionFile = dlg.getFilename();

		xml::Any	*xmlMidiSettings = new xml::Any( "MIDI_SETTINGS" );

		xmlMidiSettings->setBoolAttribute( "FILTER_ENABLED", filterEnabled );
		xmlMidiSettings->setBoolAttribute( "MIDI_THROUGH", MIDIthrough->isActive() );
		xmlMidiSettings->setStringAttribute( "MIDI_PLAYER", getSelectedMidiPlayer() );
		STRING selectedMidiDevice = MIDIrecSelect->getSelectedText();
		if( selectedMidiDevice == allRecordLabel )
			selectedMidiDevice = ALL_LABEL;
		xmlMidiSettings->setStringAttribute( "MIDI_RECORDER", selectedMidiDevice );

		size_t	numPlayer = playerHandles.size();
		for( size_t midiDev=0; midiDev<numPlayer; midiDev++ )
		{
			xml::Any	*xmlChannelSettings = static_cast<xml::Any*>(xmlMidiSettings->addObject( new xml::Any( "CHANNEL_SETTINGS" ) ));

			MIDIplayerHandle &playerHandle = playerHandles[midiDev];
			ChannelSettings *channelSettings = playerHandle.getChannelSettings();

			xmlChannelSettings->setStringAttribute( "instrument", playerHandle.getInstrument() );
			xmlChannelSettings->setStringAttribute( "device", playerHandle.getName() );

			for( unsigned char channel=0; channel<16; channel++ )
			{
				if( channelSettings[channel].group[0U] && channelSettings[channel].voice[0U] )
				{
					xml::Any *xmlChannel = static_cast<xml::Any*>(xmlChannelSettings->addObject( new xml::Any( "CHANNEL" ) ));

					xmlChannel->setIntegerAttribute( "CHANNEL", channel );
					xmlChannel->setBoolAttribute( "ACTIVE", channelSettings[channel].active );
					xmlChannel->setStringAttribute( "GROUP", channelSettings[channel].group );
					xmlChannel->setStringAttribute( "VOICE", channelSettings[channel].voice );
					xmlChannel->setIntegerAttribute( "PROGRAM", channelSettings[channel].voiceCodes.program );
					xmlChannel->setIntegerAttribute( "BANK_MSB", channelSettings[channel].voiceCodes.bankMSB );
					xmlChannel->setIntegerAttribute( "BANK_LSB", channelSettings[channel].voiceCodes.bankLSB );
					xmlChannel->setIntegerAttribute( "STEREO_POS", channelSettings[channel].stereoPos );
					xmlChannel->setIntegerAttribute( "VOLUME", channelSettings[channel].volume );
					xmlChannel->setIntegerAttribute( "EXPRESSION", channelSettings[channel].expression );
					for( effect = 0; effect<5; effect++ )
					{
						name = "EFFECT_";
						name += formatNumber(effect);
						xmlChannel->setIntegerAttribute( name, channelSettings[channel].effects[effect] );
					}
					for( sound = 0; sound<5; sound++ )
					{
						name = "SOUND_";
						name += formatNumber(sound);
						xmlChannel->setIntegerAttribute( name, channelSettings[channel].sounds[sound] );
					}
				}
			}
		}
		xml::Any	*xmlFilterSettings = static_cast<xml::Any*>(xmlMidiSettings->addObject( new xml::Any( "FILTER_SETTINGS" ) ));
		for( 
			FilterList::const_iterator it = theFilterList.cbegin(),
				endIT = theFilterList.cend();
			it != endIT;
			++it
		)
		{
			const FilterSettings &theSettings = *it;

			xml::Any *xmlFilter = static_cast<xml::Any*>(xmlFilterSettings->addObject( new xml::Any( "FILTER" ) ));

			xmlFilter->setStringAttribute( "NAME", theSettings.name );

			selectedMidiDevice = theSettings.midiInName;
			if( selectedMidiDevice == allRecordLabel )
				selectedMidiDevice = ALL_LABEL;
			xmlFilter->setStringAttribute( "FILTER_MIDI_IN", selectedMidiDevice );

			xmlFilter->setIntegerAttribute( "FILTER_CHANNEL", theSettings.filterChannel );
			xmlFilter->setIntegerAttribute( "CONDITION", theSettings.condition );
			xmlFilter->setIntegerAttribute( "NOTE", theSettings.note );

			selectedMidiDevice = theSettings.midiOutName;
			if( selectedMidiDevice == defaultPlayerLabel )
				selectedMidiDevice = DEFAULT_LABEL;
			xmlFilter->setStringAttribute( "FILTER_MIDI_OUT", selectedMidiDevice );

			xmlFilter->setIntegerAttribute( "ACTION_CHANNEL", theSettings.actionChannel );
			xmlFilter->setIntegerAttribute( "NOTE_OFFSET", theSettings.noteOffset );
			xmlFilter->setStringAttribute( "NEW_VOLUME", theSettings.newVolume );
			xmlFilter->setIntegerAttribute( "STOP_FLAG", theSettings.stopFlag );
		}
		xml::Any	*xmlMetronomSettings = static_cast<xml::Any*>(xmlMidiSettings->addObject( new xml::Any( "METRONOM_SETTINGS" ) ));
		xmlMetronomSettings->setStringAttribute( "FIRST_METRONOM", firstMetronom );
		xmlMetronomSettings->setStringAttribute( "OTHER_METRONOM", otherMetronom );


		STRING	xmlCode = xmlMidiSettings->generateDoc();
		std::ofstream	fStream( lastOptionFile );
		if( fStream.good() )
		{
			fStream << xmlCode;
		}
		settingsChangedFlag = false;
	}
}

void MIDIrecorderWindow::playMidi( void )
{
	doEnterFunction("MIDIrecorderWindow::playMidi");

	// when we are currently recording
	// stop recording
	if( midiStatus == MIDI_RECORDING )
		stopRecMidi();

	if( metronomThread )
		stopMetronom();

	/*
		if our current status is playing
		whe change the status to stop
	*/
	if( midiStatus == MIDI_PLAYING  )
		stopPlayMidi();
	/*
		else if we got a midiDevice
	*/
	else if( midiData.size() )
	{
		bool	success = false;

		// recorderHandles.stopInMidi( this );

		if( filterEnabled || midiData.getNumTracks() > 1 )
		{
			MIDIevent	theEvent;

			for( size_t midiDev = 0; midiDev < playerHandles.size(); midiDev++ )
			{
				if( playerHandles.openOutMidi( midiDev ) )
				{
					success = true;
					for( unsigned char channel=0; channel<=15; channel++ )
					{
						theEvent.setLocalOff( channel );
						playerHandles.playMidiEvent( midiDev, theEvent );
						sendChannelSettings( midiDev, channel );
					}
				}
			}
		}
		else
			success = openOutMidi();

		if( success )
		{
			// now we can start playing
			midiStatus = MIDI_PLAYING;
			midiPlayer = new MidiPlayerThread(
				this,
				&midiData,
				getDefaultPlayer(),
				authoRhytmCheck->isActive()
			);
			midiPlayer->StartThread();
			showStopLabel();
		}
		else
		{
			messageBox( winlibGUI::OPEN_OUT_FAILED_id, "MIDI Recorder" );
		}
	}
}

void MIDIrecorderWindow::stopPlayMidi( bool dontKill )
{
	doEnterFunction("MIDIrecorderWindow::stopPlayMidi");

	if( midiPlayer && !dontKill )
	{
		midiPlayer->StopThread();
		while( midiPlayer->isRunning )
			idleLoop();
		midiPlayer = NULL;
	}

	midiStatus = MIDI_WAITING;
	showPlayLabel();
	playerHandles.stopAllSoundMidi();
	checkMidiThrough();

	thePianoArea->invalidateWindow();
}

void MIDIrecorderWindow::stopRecMidi( void )
{
	doEnterFunction("MIDIrecorderWindow::stopRecMidi");

	if( midiStatus == MIDI_RECORDING )
	{
		midiStatus = MIDI_WAITING;

		playerHandles.stopAllSoundMidi();
		enableMidiPlaySelect();
		MIDIrecSelect->enable();

		recordButton->setText( recordLabel );

		midiData.alignTimeCodes();
	}
	checkMidiThrough();
}

void MIDIrecorderWindow::recMidi( void )
{
	doEnterFunction("MIDIrecorderWindow::recMidi");

	// when we are playing, stop playing first
	if( midiStatus == MIDI_PLAYING )
		stopPlayMidi();

	// if we are currently recording events
	// stop recording
	if( midiStatus == MIDI_RECORDING )
	{
		stopRecMidi();
	}
	else
	{
		playerHandles.stopAllSoundMidi();
		recorderHandles.stopInMidi( this );
		if( openInMidi() )
		{
			midiStatus = MIDI_RECORDING;

			disableMidiPlaySelect();
			MIDIrecSelect->disable();

			/*
				now we can start recording
			*/

			midiChangedFlag = true;
			midiData.clear();
			for( size_t i=0; i<playerHandles.size(); i++ )
			{
				const MIDIplayerHandle &playerHandle = playerHandles[i];
				midiData.setMidiOutDev( (unsigned short)i, i );
				midiData.setInstrument( (unsigned short)i, playerHandle.getInstrument() );
				midiData.setTrackName( (unsigned short)i, playerHandle.getName() );
				midiData.setDeviceName( (unsigned short)i, playerHandle.getName() );
			}

			midiData.setBPM(atoi(bpmEdit->getText()), false, 0);

			recordButton->setText( getStopLabel() );
		}
		else
		{
			messageBox( winlibGUI::OPEN_IN_FAILED_id, "MIDI Recorder" );
		}
	}
}

void MIDIrecorderWindow::stopMetronom( void )
{
	doEnterFunction("MIDIrecorderWindow::stopMetronom");

	if( metronomThread )
	{
		unsigned char channel = (*metronomThread).getChannel();

		metronomThread->StopThread();
		while( metronomThread->isRunning )
			idleLoop();
		metronomThread = NULL;

		sendChannelSettings( getDefaultPlayer(), channel );		// restore channel for normal usage

		playerHandles.stopAllSoundMidi();
		checkMidiThrough();
	}
	if( metronomWindow )
	{
		metronomWindow->getPosition( &metronomWindowXpos, &metronomWindowYpos );
		metronomWindow->close();
		metronomWindow = NULL;
	}
}

void MIDIrecorderWindow::startMetronom( void )
{
	doEnterFunction("MIDIrecorderWindow::startMetronom");

	if( midiStatus == MIDI_PLAYING || midiPlayer )
	{
		stopPlayMidi();
	}

	if( metronomThread )
	{
		stopMetronom();
	}
	else
	{
		unsigned bpm = bpmEdit->getText().getValueE<unsigned>();
		unsigned numerator = numeratorEdit->getText().getValueE<unsigned>();
		if( bpm && numerator )
		{
			if( openOutMidi() )
			{
				metronomWindow = new MetronomWindow( this );
				MIDIplayerHandle	&playerHandle = playerHandles[getDefaultPlayer()];
				STRING				drumsFile = playerHandle.getDrumsCSV();
				metronomWindow->create( this, firstMetronom, otherMetronom, drumsFile );
				metronomWindow->move( metronomWindowXpos, metronomWindowYpos );

				metronomThread = new MetronomThread(
					this,
					bpm,
					numerator,
					denominatorSelect->getText().getValueE<unsigned>()
				);
				// why is this neccessary?
				// sendChannelSettings( getDefaultPlayer(), metronomThread->getChannel() );
				metronomThread->StartThread();
			}
			else
			{
				messageBox( winlibGUI::OPEN_OUT_FAILED_id, "MIDI Recorder" );
			}
		}
	}
}

void MIDIrecorderWindow::playUnfilteredMidiEvent( size_t midiOutDev, MIDIevent &msg )
{
	doEnterFunction("MIDIrecorderWindow::playUnfilteredMidiEvent");

	unsigned char message = msg.getMessage();
	unsigned char volume = msg.getData2();

	if( doNoteHold )
	{
		if( (message == MIDI_NOTE_ON && volume == 0) || message == MIDI_NOTE_OFF )
		{
/***/		return;
		}
		
	}
	thePianoArea->showMIDIevent( msg, midiOutDev, true );

	if( (message == MIDI_NOTE_ON && volume == 0) || message == MIDI_NOTE_OFF )
		msg.setMessage( MIDI_NOTE_OFF );

	if( midiStatus == MIDI_RECORDING )
	{
		msg.setTrack( (unsigned short)midiOutDev );
		midiData.addElement( msg );
	}

	playerHandles.playMidiEvent( midiOutDev, msg );
}


// --------------------------------------------------------------------- //
// ----- class virtuals ------------------------------------------------ //
// --------------------------------------------------------------------- //

STRING PianoArea::getWindowClassName( void ) const
{
	return className;
}
ProcessStatus PianoArea::handleRepaint( Device &hDC )
{
	drawFrameBorder( hDC );
	drawNoteLines( hDC );

	size = getClientSize();

	thePainter.drawPiano( hDC, size, PIANO_SIZE );

	return psPROCESSED;
}

void MetronomThread::ExecuteThread( void )
{
	MIDIevent		midiMsg;

	const unsigned	timePerBar = (60000*4)/m_bpm;
	const unsigned	timePerTick = timePerBar/m_denominator;
	unsigned char	lastProgram = 0xFF;
	unsigned char	lastBankMSB = 0xFF;
	unsigned char	lastBankLSB = 0xFF;

	const clock_t	startTime = clock();

	for( clock_t i=0;!terminated; i++ )
	{
		const clock_t	nextTime = startTime + i*timePerTick;

		const clock_t actTime = clock();
		if( nextTime > actTime )
		{
			Sleep((nextTime-actTime)*1000/CLK_TCK);
		}

		const DrumVoice &theVoice = (!(i%m_numerator))
			? m_recWindow->getFirstMetronomVoice()
			: m_recWindow->getOtherMetronomVoice();

		const unsigned char volume = !(i%m_numerator) ? 127 : 64;

		if( lastProgram != theVoice.voiceCodes.program )
		{
			lastProgram = theVoice.voiceCodes.program;
			if( lastProgram < 128 )
			{
				channel = 15;
				midiMsg.setProgramChange( channel,  lastProgram );
				m_recWindow->playMidiEvent( midiMsg );
			}
		}
		if( lastBankMSB != theVoice.voiceCodes.bankMSB )
		{
			lastBankMSB = theVoice.voiceCodes.bankMSB;
			if( lastBankMSB < 128 )
			{
				channel = 15;
				midiMsg.setBankMSB( channel,  lastBankMSB );
				m_recWindow->playMidiEvent( midiMsg );
			}
		}
		if( lastBankLSB != theVoice.voiceCodes.bankLSB )
		{
			lastBankLSB = theVoice.voiceCodes.bankLSB;
			if( lastBankLSB < 128 )
			{
				channel = 15;
				midiMsg.setBankLSB( channel,  lastBankLSB );
				m_recWindow->playMidiEvent( midiMsg );
			}
		}

		midiMsg.setNoteOn( channel, theVoice.note, volume );
		m_recWindow->playMidiEvent( midiMsg );
		Sleep( 50 );
		midiMsg.setNoteOff( channel, theVoice.note, 50 );
		m_recWindow->playMidiEvent( midiMsg );
	}
}

void MidiPlayerThread::ExecuteThread( void )
{
	doEnterFunction("MidiPlayerThread::ExecuteThread");

	size_t			i, midiOutDev, numPlayers, numEvents;
	int 			barTime = midiData->getTimePerBar();
	int				quarterTime = barTime / 4;
	int				eighthTime = barTime / 8;
	MIDIevent		midiMsg;
	unsigned short	track;
	int				rtIdx, rhythmTimes[] =
	{
		quarterTime, eighthTime,
		quarterTime, eighthTime,
		eighthTime, eighthTime,
		0
	};

	clock_t			rhythmTime, rhythmEnd, actTime, nextTime, startTime;

	numEvents = midiData->size();
	numPlayers = playerHandles.size();
	startTime = clock();
	for( i=0;!terminated && i<numEvents; i++ )
	{
		midiMsg = (*midiData)[i];
		track = midiMsg.getTrack();
		midiOutDev = midiData->getMidiOutDev( track );
		if( midiOutDev >= numPlayers )
		{
			midiOutDev = midiDev;
		}

		if( playerHandles.isOutOpen( midiOutDev ) )
		{
			actTime = clock();
			nextTime = startTime + midiMsg.getTimeCode();
			if( nextTime > actTime )
				Sleep((nextTime-actTime)*1000/CLK_TCK);

			recWindow->playFilteredMidiEvent( ALL_DEVICE, midiOutDev, midiMsg );
			if( autoRhythm && midiMsg.getMessage() == MIDI_NOTE_ON && i<numEvents-1 )
			{
				nextTime = startTime + (*midiData)[i+1].getTimeCode();
				rtIdx = 0;
				unsigned char note, channel, volume;
				channel = midiMsg.getChannel();
				note = midiMsg.getData1();
				volume = midiMsg.getData2();
				rhythmEnd = clock();
				while( !terminated )
				{
					rhythmTime = rhythmTimes[rtIdx];
					if( !rhythmTime )
					{
						rtIdx = 0;
						rhythmTime = rhythmTimes[rtIdx];
					}
					else
						rtIdx++;
					rhythmEnd += rhythmTime;

					if( rhythmEnd >= nextTime )
/*v*/					break;

					Sleep((rhythmEnd-2-clock())*1000/CLK_TCK);
					midiMsg.setNoteOff( channel, note, 2 );
					recWindow->playFilteredMidiEvent( ALL_DEVICE, midiOutDev, midiMsg );
					Sleep( 2*1000/CLK_TCK );
					midiMsg.setNoteOn( channel, note, volume );
					recWindow->playFilteredMidiEvent( ALL_DEVICE, midiOutDev, midiMsg );
				}
			}
		}
	}

	if( !terminated )
	{
		// wait three bars for the release time
		for( size_t i=0; i<12 && !terminated; i++ )
			Sleep( quarterTime );

		recWindow->playFinished();
	}
}

ProcessStatus MIDIrecorderWindow::handleCreate( void )
{
	doEnterFunction("MIDIrecorderWindow::handleCreate");

	recordLabel = appObject->loadString( winlibGUI::RECORD_LABEL_id );
	allRecordLabel = appObject->loadString( winlibGUI::ALL_MIDI_RECORD_LABEL_id );
	defaultPlayerLabel = appObject->loadString( winlibGUI::DEFAULT_MIDI_PLAY_LABEL_id );

	initMidiPlaySelect( MIDIplaySelect, playButton );
	doNoteHold = false;
	MIDIholdNote->clrActive();

	denominatorSelect->selectEntry( 0 );

	thePianoArea->create( pianoFrame );
	pianoFrame->doLayout();

	size_t midiCount = recorderHandles.size();
	for( size_t i=0; i<midiCount; i++ )
	{
		MIDIrecSelect->addEntry( recorderHandles[i].getInstrument() );
	}
	MIDIrecSelect->addEntry( allRecordLabel );
	MIDIrecSelect->selectEntry( int(midiCount) );

	// check whether we can open any device
//	openInMidi();
//	recorderHandles.stopInMidi( this );
	if( midiCount )
	{
		MIDIthrough->enable();
		recordButton->enable();
		MIDIrecSelect->enable();
	}
	else
	{
		MIDIthrough->disable();
		recordButton->disable();
		MIDIrecSelect->disable();
	}

	midiCount = playerHandles.size();
	for( size_t i=0; i<midiCount; i++ )
	{
		const MIDIplayerHandle &playerHandle = playerHandles[i];
		midiData.setMidiOutDev( (short)i, i );
		midiData.setTrackName( (short)i, playerHandle.getName() );
		midiData.setDeviceName( (short)i, playerHandle.getName() );
		midiData.setInstrument( (short)i, playerHandle.getInstrument() );
	}
	return psDO_DEFAULT;
}

SuccessCode MIDIrecorderWindow::close( void )
{
	saveWindowRect();
	if( theMidiChannelsWindow )
	{
		theMidiChannelsWindow->close();
	}
	if( theMidiFilterWindow )
	{
		theMidiFilterWindow->close();
	}

	return MIDIrecorderWindow_form::close();
}

ProcessStatus MIDIrecorderWindow::handleDestroy( void )
{
	assert( !metronomThread );

	assert( midiStatus != MIDI_PLAYING );
	assert( midiStatus != MIDI_RECORDING );

	recorderHandles.stopInMidi( this );
	playerHandles.stopAllOutMidi();

	return psDO_DEFAULT;
}

bool MIDIrecorderWindow::handleChildClose( BasicWindow *child, bool deleted )
{
	if( child == theMidiChannelsWindow )
	{
		theMidiChannelsWindow = NULL;
	}

	if( child == theMidiFilterWindow )
	{
		theMidiFilterWindow = NULL;
	}

	if( child == metronomWindow )
	{
		metronomWindow = NULL;
	}

	return MIDIrecorderWindow_form::handleChildClose( child, deleted );
}

bool MIDIrecorderWindow::canClose( void )
{
	doEnterFunction("MIDIrecorderWindow::canClose");
	int		button = IDYES;

	if( metronomThread )
	{
		stopMetronom();
	}

	if( midiStatus == MIDI_PLAYING )
	{
		stopPlayMidi();
	}
	else if( midiStatus == MIDI_RECORDING )
	{
		stopRecMidi();
	}

	if( midiChangedFlag )
	{
		button = messageBox( winlibGUI::MIDI_FILE_CHANGED_id, "MIDI Recorder", MB_ICONQUESTION|MB_YESNOCANCEL );
		if( button == IDYES	)
		{
			saveMidi();
		}
	}
	if( settingsChangedFlag && button != IDCANCEL )
	{
		button = messageBox( winlibGUI::SETTINGS_CHANGED_id, "MIDI Recorder", MB_ICONQUESTION|MB_YESNOCANCEL );
		if( button == IDYES	)
		{
			saveSettings();
		}
	}
	return button != IDCANCEL;
}

ProcessStatus MIDIrecorderWindow::handleButtonClick( int btn )
{
	doEnterFunction("MIDIrecorderWindow::handleButtonClick");
	switch( btn )
	{
		case winlibGUI::filterEnabledCheckBox_id:
			filterEnabled = filterEnabledCheckBox->isActive();

		case winlibGUI::MIDIthrough_id:
			checkMidiThrough();
			settingsChangedFlag = true;
			break;

		case winlibGUI::MIDIholdNote_id:
			doNoteHold = MIDIholdNote->isActive();
			break;

		case winlibGUI::playButton_id:
			playMidi();
			break;

		case winlibGUI::recordButton_id:
			recMidi();
			break;

		case winlibGUI::channelsButton_id:
		{
			if( !theMidiChannelsWindow )
			{

				theMidiChannelsWindow = new MIDIchannelsWindow( this );
				theMidiChannelsWindow->create( this, getDefaultPlayer() );
			}
			else
				theMidiChannelsWindow->setChannelSettings( getDefaultPlayer() );

			theMidiChannelsWindow->show();
			theMidiChannelsWindow->focus();
			break;
		}
		case winlibGUI::filterButton_id:
			if( !theMidiFilterWindow )
			{
				theMidiFilterWindow = new MIDIfilterWindow( this );
				theMidiFilterWindow->create( this, &theFilterList );
			}
			theMidiFilterWindow->show();
			theMidiFilterWindow->focus();
			break;

		case winlibGUI::editButton_id:
		{
			size_t				midiDev = getDefaultPlayer();
			MIDIplayerHandle	&playerHandle = playerHandles[midiDev];
			STRING				voiceFile = playerHandle.getVoicesCSV();
			MIDIeditorWindow	theMidiEditorWindow;

			if( midiStatus == MIDI_RECORDING )
			{
				stopRecMidi();
			}
			if( metronomThread )
			{
				stopMetronom();
			}
			if( midiStatus == MIDI_PLAYING  )
			{
				stopPlayMidi();
			}

			recorderHandles.stopInMidi( this );
			theMidiEditorWindow.create( this, &midiData, voiceFile, midiDev );
			if( theMidiEditorWindow.getModalResult() == IDOK )
			{
				midiChangedFlag = true;
			}
			checkMidiThrough();
			break;
		}
		case winlibGUI::loadSettingButton_id:
			loadMidiSettings();
			break;
		case winlibGUI::saveSettingButton_id:
			saveSettings();
			break;

		case winlibGUI::convertBPMbutton_id:
			midiData.setTimeSignature(
				numeratorEdit->getText().getValueE<unsigned>(),
				denominatorSelect->getText().getValueE<unsigned>()
			);
			midiData.setBPM( bpmEdit->getText().getValueE<unsigned>(), true, 64 );
			midiChangedFlag = true;
			break;

		case winlibGUI::metronomButton_id:
			startMetronom();
			break;

		case winlibGUI::saveMidiButton_id:
			saveMidi();
			break;

		case winlibGUI::loadMidiButton_id:
			loadMidiFile();
			break;

		case winlibGUI::instrumentButton_id:
		{
			InstrumentWindow *thePopup = new InstrumentWindow( this );
			thePopup->create();
			break;
		}

		default:
/***/		return MIDIrecorderWindow_form::handleButtonClick( btn );
	}
	return psPROCESSED;
}

ProcessStatus MIDIrecorderWindow::handleSelectionChange( int control )
{
	doEnterFunction("MIDIrecorderWindow::handleSelectionChange");
	switch( control )
	{
		case winlibGUI::MIDIplaySelect_id:
			settingsChangedFlag = true;
			stopPlayMidi();
			stopMetronom();
			changeDefaultPlayer();
			checkMidiThrough();
			break;

		case winlibGUI::MIDIrecSelect_id:
			settingsChangedFlag = true;
			break;

		default:
/***/		return MIDIrecorderWindow_form::handleSelectionChange( control );
	}
	return psPROCESSED;
}

ProcessStatus MIDIrecorderWindow::handleCommand( int cmd )
{
	doEnterFunction("MIDIrecorderWindow::handleCommand");
	switch( cmd )
	{
		case winlibGUI::playButton_id:
			playMidi();
			break;
		default:
/***/		return MIDIrecorderWindow_form::handleCommand( cmd );
	}
	return psPROCESSED;
}

ProcessStatus MIDIrecorderWindow::handleMessage( UINT message, WPARAM wParam, LPARAM lParam )
{
	doEnterFunction("MIDIrecorderWindow::handleMessage");
	size_t	midiInDev = message-WM_MIDI_INPUT;
	size_t	numDevices = recorderHandles.size();

	/*
		Check for short messages
	*/
	if( midiInDev < numDevices )
	{
		MIDIevent	newMsg( static_cast<uint32>(lParam), static_cast<uint32>(wParam) );
		if( newMsg.getMessage() != MIDI_SYSTEM )	// ignore all system messages
		{
			playFilteredMidiEvent( int(midiInDev), getDefaultPlayer(), newMsg );
		}
/***/	return psPROCESSED;
	}

	/*
		check for long (SysEx) messages
	*/
	midiInDev -= numDevices;
	if( midiInDev < numDevices )
	{
		if( midiStatus == MIDI_RECORDING )
		{
			size_t len, midiOutDev;
			const unsigned char *buffer = recorderHandles.getSysExBuffer( midiInDev, &len );
			if( len && buffer[0] == 0xF0 && buffer[len-1] == 0xF7 )
			{
				// accept complete messages only
				const MIDIrecorderHandle &inDev = recorderHandles[midiInDev];
				midiOutDev = playerHandles.getPlayer( inDev.getInstrument() );
				if( midiOutDev == playerHandles.no_index )
					midiOutDev = getDefaultPlayer();

				MIDIevent	newMsg( static_cast<uint32>(lParam), MIDI_SYS_EX_START );
				newMsg.setTrack( short(midiOutDev) );

				newMsg.setSysExData( buffer, len, true );
				midiData.addElement( newMsg );

				recorderHandles.prepareSysExHeader( midiInDev );
			}
		}
/***/	return psPROCESSED;
	}
	return psDO_DEFAULT;
}

// --------------------------------------------------------------------- //
// ----- class publics ------------------------------------------------- //
// --------------------------------------------------------------------- //

void PianoArea::drawNoteOnLine( Device &context, unsigned char note, bool noteOn, bool halfTone, bool isSent, int xPos )
{
	if( noteOn )
	{
		if( isSent )
		{
			context.getBrush().selectBrush( Brush::sbBlack );
			context.getPen().selectPen( Pen::spBlack );
		}
		else
		{
			context.getBrush().create( 0, 0, 255 );
			context.getPen().create( Pen::psSolid, 1, 0, 0, 255 );
		}
	}
	else
	{
		context.getBrush().selectBrush( Brush::sbWhite );
		context.getPen().selectPen( Pen::spWhite );
	}

	/*
		Draw the dot on the note lines
	*/
	bool	dummy, onLine;

	int centerPos = getNoteCenter( note, &onLine );
	int yPos = centerPos -lineHeight/2;
	context.ellipse( xPos+1, yPos+1, xPos+lineHeight, yPos+lineHeight );

	// draw the helper lines
	for( yPos = getNoteCenter( MIDI_NOTE_F4, &dummy ) - lineHeight; yPos >= centerPos; yPos -= lineHeight )
		context.horizontalLine( xPos-1, xPos+lineHeight+1, yPos );

	for( yPos = getNoteCenter( MIDI_NOTE_G1, &dummy ) + lineHeight; yPos <= centerPos; yPos += lineHeight )
		context.horizontalLine( xPos-1, xPos+lineHeight+1, yPos );

	if( centerPos == getNoteCenter( MIDI_NOTE_C3, &dummy ) )
		context.horizontalLine( xPos-1, xPos+lineHeight+1, centerPos );

	// Draw the sharp (#) symbol
	if( halfTone )
	{
		context.verticalLine( xPos -9, centerPos+lineHeight, centerPos-lineHeight );
		context.verticalLine( xPos -5, centerPos+lineHeight, centerPos-lineHeight );

		context.line( xPos -11, centerPos+4, xPos -2, centerPos+1 );
		context.line( xPos -11, centerPos, xPos -2, centerPos-3 );
	}
}

void PianoArea::showMIDIevent( const MIDIevent &msg, size_t midiDev, bool isSent )
{
	unsigned char	message = msg.getMessage();

	if( message == MIDI_NOTE_OFF || message == MIDI_NOTE_ON )
	{
		unsigned char	channel = msg.getChannel();
		unsigned char	note = msg.getData1();
		unsigned char	volume = msg.getData2();
		DrawDevice		context( this );

		int				xPos;
		bool			halfTone;
		bool			noteOn = ((message == MIDI_NOTE_ON) && volume);

		/*
			Draw the dot on the keyboard
		*/
		xPos = thePainter.drawNoteOnKey( context, note, noteOn, isSent, &halfTone );
		drawNoteOnLine( context, note, noteOn, halfTone, isSent, xPos );

		// when note is off, restore all note lines and the border
		if( !noteOn )
		{
			drawFrameBorder( context );

			context.getPen().selectPen( Pen::spWhite );
			context.getBrush().selectBrush( Brush::sbWhite );
			context.rectangle( 1, 1, size.width-2, 20 );

			drawNoteLines( context );

			size_t	numNotesPlaying = notesPlaying.size();
			size_t	numNotesReceived = notesReceived.size();

			if( isSent )
			{
				// remove actual tone from list of playing tones and restore all other
				for( size_t i=0; i<numNotesPlaying; )
				{
					const MIDInotePresentation &notePresentation = notesPlaying[i];
					if( notePresentation.channel == channel
					&& notePresentation.midiDev == midiDev
					&& notePresentation.note == note )
					{
						notesPlaying.removeElementAt( i );
						numNotesPlaying--;
					}
					else
					{
						xPos = thePainter.drawNoteOnKey( context, notePresentation.note, true, true, &halfTone );
						drawNoteOnLine( context, notePresentation.note, true, halfTone, true, xPos );
						i++;
					}
				}
			}
			else
			{
				// remove actual tone from list of received tones and restore all other
				for( size_t i=0; i<numNotesReceived; )
				{
					const MIDInotePresentation &notePresentation = notesReceived[i];
					if( notePresentation.channel == channel
					&& notePresentation.midiDev == midiDev
					&& notePresentation.note == note )
					{
						notesReceived.removeElementAt( i );
						numNotesReceived--;
					}
					else
					{
						xPos = thePainter.drawNoteOnKey( context, notePresentation.note, true, false, &halfTone );
						drawNoteOnLine( context, notePresentation.note, true, halfTone, true, xPos );
						i++;
					}
				}
			}

			// show the last tone stil playing
			if( numNotesPlaying )
			{
				const MIDInotePresentation &notePresentation =
					notesPlaying[numNotesPlaying-1]
				;

				context.getBrush().selectBrush( Brush::sbBlack );
				context.getPen().selectPen( Pen::spBlack );
				context.setTextColor( 0, 0, 0 );
				context.textOut(
					2, 2,
					notePresentation.presentation
				);
			}

			// show the last received key still playing
			if( numNotesReceived )
			{
				const MIDInotePresentation &notePresentation =
					notesReceived[numNotesReceived-1]
				;

				context.getBrush().selectBrush( Brush::sbBlack );
				context.getPen().selectPen( Pen::spBlack );
				context.setTextColor( 0, 0, 255 );
				context.textOut(
					size.width/2 +1, 2,
					notePresentation.presentation
				);
			}
		}
		else	// if( noteOn )
		{
			char					buffer[128];
			MIDInotePresentation	notePresentation;

			notePresentation.midiDev = midiDev;
			notePresentation.channel = channel;
			notePresentation.note = note;
			channel++;

			sprintf( buffer, "%u/%02u: %s(%03u) %03u   ",
				(unsigned)midiDev, (unsigned)channel,
				MIDIevent::getNoteText(note), (unsigned)note,
				(unsigned)volume
			);
			notePresentation.presentation = buffer;

			if( isSent )
			{
				context.setTextColor( 0, 0, 0 );

				context.textOut( 2, 2, notePresentation.presentation );
				notesPlaying.addElement( notePresentation );
			}
			else
			{
				context.setTextColor( 0, 0, 255 );

				context.textOut( size.width/2+2, 2, notePresentation.presentation );
				notesReceived.addElement( notePresentation );
			}
		}
	}
}

void MIDIrecorderWindow::create( void )
{
	doEnterFunction("MIDIrecorderWindow::create");
	thePianoArea = new PianoArea( this );
	SuccessCode error = MIDIrecorderWindow_form::create( NULL );
	if( error == scSUCCESS )
	{
		restoreWindowPos();
	}
}

void MIDIrecorderWindow::sendChannelSettings( size_t midiDev, unsigned char channel )
{
	doEnterFunction("MIDIrecorderWindow::sendChannelSettings");

	unsigned char	effect, sound;
	MIDIevent		theEvent;

	MIDIplayerHandle &playerHandle = playerHandles[midiDev];
	ChannelSettings *channelSettings = playerHandle.getChannelSettings();

	if( channelSettings[channel].active && playerHandles.isOutOpen( midiDev ) )
	{
		if( channelSettings[channel].group[0U]
		&&  channelSettings[channel].voice[0U] )
		{
			// do not change voice of metronom
			if( !metronomThread || metronomThread->getChannel() != channel )
			{
				if( channelSettings[channel].voiceCodes.bankMSB < 128 )
				{
					theEvent.setBankMSB(
						channel, channelSettings[channel].voiceCodes.bankMSB
					);
					playUnfilteredMidiEvent( midiDev, theEvent );
				}
				if( channelSettings[channel].voiceCodes.bankLSB < 128 )
				{
					theEvent.setBankLSB(
						channel, channelSettings[channel].voiceCodes.bankLSB
					);
					playUnfilteredMidiEvent( midiDev, theEvent );
				}
				if( channelSettings[channel].voiceCodes.program < 128 )
				{
					theEvent.setProgramChange(
						channel, channelSettings[channel].voiceCodes.program
					);
					playUnfilteredMidiEvent( midiDev, theEvent );
				}
			}

			theEvent.setVolume(
				channel, channelSettings[channel].volume
			);
			playUnfilteredMidiEvent( midiDev, theEvent );
			theEvent.setPanPosition(
				channel, channelSettings[channel].stereoPos
			);
			playUnfilteredMidiEvent( midiDev, theEvent );
			theEvent.setBalancePosition(
				channel, channelSettings[channel].stereoPos
			);
			playUnfilteredMidiEvent( midiDev, theEvent );

			for( effect=0; effect<5; effect++ )
			{
				theEvent.setEffectValue(
					channel,
					effect,
					channelSettings[channel].effects[effect]
				);
				playUnfilteredMidiEvent( midiDev, theEvent );
			}
			for( sound=0; sound<5; sound++ )
			{
				theEvent.setSoundValue(
					channel,
					sound,
					channelSettings[channel].sounds[sound]
				);
				playUnfilteredMidiEvent( midiDev, theEvent );
			}

			theEvent.setExpression(
				channel, channelSettings[channel].expression
			);
			playUnfilteredMidiEvent( midiDev, theEvent );
		}
	}
}

void MIDIrecorderWindow::playFilteredMidiEvent( int midiInDev, size_t midiOutDev, MIDIevent &msg )
{
	doEnterFunction("MIDIrecorderWindow::playFilteredMidiEvent");

	size_t			midiOut = midiOutDev;

	size_t			i;
	unsigned char 	theMessageCode = msg.getMessage();
	size_t			numFilter = theFilterList.size();

	bool playDefault  = true;

	unsigned char volume = msg.getData2();

	if( doNoteHold )
	{
		if( (theMessageCode == MIDI_NOTE_ON && volume == 0) || theMessageCode == MIDI_NOTE_OFF )
		{
/***/		return;
		}
	}

	if( midiInDev >= 0 )
	{
		thePianoArea->showMIDIevent( msg, midiInDev, false );
	}

	if( numFilter && filterEnabled )
	{
		unsigned char	channel = msg.getChannel();
		unsigned char	note = msg.getData1();
		bool			isNoteEvent = (theMessageCode == MIDI_NOTE_OFF || theMessageCode == MIDI_NOTE_ON);
		bool			isSystemEvent = (theMessageCode >= MIDI_SYS_EX_START);
		bool			filterOK;
		MIDIevent		newEvent;

		if( midiInDev != ALL_DEVICE )
			playDefault  = false;

		for( i=0; i<numFilter; i++ )
		{
			FilterSettings	&theFilter = theFilterList[i];
			filterOK = false;

			if( theFilter.midiInDev == ALL_DEVICE
			||  theFilter.midiInDev == midiInDev )
			{
				if( theFilter.filterChannel == 16
				||  theFilter.filterChannel == channel )
				{
					newEvent = msg;

					if(theFilter.actionChannel < 16 && !isSystemEvent )
						newEvent.setChannel( theFilter.actionChannel );

					if( isNoteEvent )
					{
						if( (theFilter.condition == LESS_THAN		&& note <theFilter.note)
						||  (theFilter.condition == LESS_EQUAL		&& note<=theFilter.note)
						||  (theFilter.condition == EQUAL			&& note==theFilter.note)
						||  (theFilter.condition == GREATER_EQUAL	&& note>=theFilter.note)
						||  (theFilter.condition == GREATER_THAN	&& note >theFilter.note) )
						{
							filterOK = true;
							newEvent.setData1( (unsigned char )(note + theFilter.noteOffset) );

							if( !theFilter.newVolume.isEmpty() )
							{
								int volume = msg.getData2();

								if( volume )
								{
									char sign = theFilter.newVolume[0U];
									if( sign == '+' || sign == '-' )
									{
										int volumeOffset = atoi(
											theFilter.newVolume
										);

										volume += volumeOffset;
									}
									else if( theFilter.newVolume.endsWith( "%" ) )
									{
										double percent = atof(
											theFilter.newVolume
										);

										volume = int(double(volume) * percent / 100.0 + 0.5);
									}
									else if( theFilter.newVolume.beginsWith( "Auto" ) )
									{
										if( !theFilter.autoLevel )
										{
											theFilter.autoLevel = 12700.0/(double)volume;
											theFilter.newVolume = "Auto ";
											theFilter.newVolume += formatFloat(theFilter.autoLevel);
										}

										int newVolume = int(double(volume) * theFilter.autoLevel / 100.0 + 0.5);
										if( newVolume > 127 )
										{
											theFilter.autoLevel = 12700.0/(double)volume;
											theFilter.newVolume = "Auto ";
											theFilter.newVolume += formatFloat(theFilter.autoLevel);
											volume = 127;
										}
										else
											volume = newVolume;
									}
									else
										volume = atoi(
											theFilter.newVolume
										);

									if( volume < 0 )
										volume = 0;
									else if( volume > 127 )
										volume = 127;

									newEvent.setData2( (unsigned char)volume );
								}
							}
						}
					}
					else if( theFilter.condition == CONTROL )
						filterOK = true;

					if( filterOK )
					{
						if( theFilter.midiOutDev >= 0 )
							midiOut = theFilter.midiOutDev;
						else if( theFilter.midiOutDev == DEFAULT_DEVICE
						|| midiInDev == ALL_DEVICE )
						{
							midiOut = midiOutDev;
						}
						else
						{
							const MIDIrecorderHandle &inDev = recorderHandles[midiInDev];
							midiOut = playerHandles.getPlayer( inDev.getInstrument() );
							if( midiOut == playerHandles.no_index )
								midiOut = midiOutDev;
						}

						playUnfilteredMidiEvent( midiOut, newEvent );
						playDefault = false;

						if( theFilter.stopFlag )
							break;
					}
				}
			}
		}
	}

	if( playDefault )
	{
		if( midiInDev != ALL_DEVICE )
		{
			const MIDIrecorderHandle &inDev = recorderHandles[midiInDev];
			midiOut = playerHandles.getPlayer( inDev.getInstrument() );
			if( midiOut == playerHandles.no_index )
				midiOut = midiOutDev;
		}
		playUnfilteredMidiEvent( midiOut, msg );
	}
}

void MIDIrecorderWindow::loadMidiFile( const char *cmdLine )
{
	doEnterFunction("MIDIrecorderWindow::loadMidiFile");

	if( midiStatus == MIDI_PLAYING  )
		stopPlayMidi();
	else if( midiStatus == MIDI_RECORDING  )
		stopRecMidi();

	if( midiChangedFlag )
	{
		if(
			messageBox(
				winlibGUI::MIDI_FILE_CHANGED_id,
				"MIDI Recorder",
				MB_ICONQUESTION|MB_YESNO
			) == IDYES
		)
		{
			saveMidi();
		}
	}

	if( PlayerWindow::loadMidi( this, bpmEdit, winlibGUI::MidiFiles_id, cmdLine ) )
	{
		midiChangedFlag = false;
		numeratorEdit->setText( formatNumber( midiData.getNumerator() ) );
		denominatorSelect->selectEntry( formatNumber( midiData.getDenominator() ) );
	}
}

void MIDIrecorderWindow::loadMidiSettings( const char *cmdLine )
{
	doEnterFunction("MIDIrecorderWindow::loadMidiSettings");
	unsigned char	channel, effect, sound;
	STRING			name, value;
	int				valueI;
	OpenFileDialog	dlg;

	if( settingsChangedFlag )
	{
		if(
			messageBox(
				winlibGUI::SETTINGS_CHANGED_id,
				"MIDI Recorder",
				MB_ICONQUESTION|MB_YESNO
			) == IDYES
		)
		{
			saveSettings();
		}
	}

	dlg.setFilename( lastOptionFile );
	if( dlg.getDirectory().isEmpty() )
		dlg.setPersonalMusic();
	if( cmdLine || dlg.create( this, winlibGUI::OPEN_SETTINGS_id, winlibGUI::SettingsFiles_ids, winlibGUI::SettingsFiles_count ) )
	{
		lastOptionFile = cmdLine ? cmdLine : dlg.getFilename();

		xml::Parser		theParser( lastOptionFile );

		xml::Document *xmlMidiSettingsDoc = theParser.readFile( false );
		if( xmlMidiSettingsDoc )
		{
			xml::Element *xmlMidiSettings = xmlMidiSettingsDoc->getElement( "MIDI_SETTINGS" );
			if( xmlMidiSettings )
			{
				value = xmlMidiSettings->getAttribute( "MIDI_RECORDER" );
				if( value[0U] )
				{
					if( value == ALL_LABEL )
					{
						value = allRecordLabel;
						MIDIrecSelect->selectEntry( value );
					}
					else
					{
						size_t midiDev = recorderHandles.getRecorder( value );
						if( midiDev != recorderHandles.no_index )
						{
							MIDIrecSelect->selectEntry( int(midiDev) );
						}
						else
						{
							MIDIrecSelect->selectEntry( 0 );
						}
					}
					openInMidi();
				}

				value = xmlMidiSettings->getAttribute( "MIDI_PLAYER" );
				if( !value.isEmpty() )
					selectMidiPlayer( value );

				value = xmlMidiSettings->getAttribute( "FILTER_ENABLED" );
				if( !value.isEmpty() )
				{
					filterEnabled = value.getValueN<bool>();
					filterEnabledCheckBox->setActive( filterEnabled );
				}

				value = xmlMidiSettings->getAttribute( "MIDI_THROUGH" );
				if( value[0U] )
				{
					bool midiThroughEnabled;
					midiThroughEnabled = value.getValueN<bool>();
					MIDIthrough->setActive( midiThroughEnabled );
					checkMidiThrough();
				}

				xml::XmlArray	children;
				xmlMidiSettings->getChildElements( &children, "CHANNEL_SETTINGS" );
				for( 
					xml::XmlArray::const_iterator it = children.cbegin(),
						endIT = children.cend();
					it != endIT;
					++it
				)
				{
					ChannelSettings *channelSettings;
					xml::Element *xmlChannelSettings = *it;
					STRING instrument = xmlChannelSettings->getAttribute( "instrument" );
					STRING device = xmlChannelSettings->getAttribute( "device" );
					size_t midiDev = playerHandles.no_index;
					if( !instrument.isEmpty() )
						midiDev = playerHandles.getPlayer( instrument );
					if( midiDev == playerHandles.no_index && !device.isEmpty() )
						midiDev = playerHandles.getPlayer( device );
					if( midiDev == playerHandles.no_index )
						midiDev = getDefaultPlayer();

					if( midiDev != playerHandles.no_index )
					{
						channelSettings = playerHandles[midiDev].getChannelSettings();
						for( channel = 0; channel <= 16; channel++ )
							channelSettings[channel].active = false;

						for( size_t i=0; i<xmlChannelSettings->getNumObjects(); i++ )
						{
							xml::Element *xmlChannel = xmlChannelSettings->getElement( i );

							if( xmlChannel && xmlChannel->getTag() == "CHANNEL" )
							{
								value = xmlChannel->getAttribute( "CHANNEL" );
								channel = value.getValueE<unsigned char>();
								value = xmlChannel->getAttribute( "ACTIVE" );
								if( !value.isEmpty() )
									channelSettings[channel].active = value.getValueN<bool>();
								else
									channelSettings[channel].active = true;
								
								channelSettings[channel].group = xmlChannel->getAttribute( "GROUP" );
								channelSettings[channel].voice = xmlChannel->getAttribute( "VOICE" );
								value = xmlChannel->getAttribute( "PROGRAM" );
								channelSettings[channel].voiceCodes.program = value.getValueE<unsigned char>();
								value = xmlChannel->getAttribute( "BANK_MSB" );
								channelSettings[channel].voiceCodes.bankMSB = value.getValueE<unsigned char>();
								value = xmlChannel->getAttribute( "BANK_LSB" );
								channelSettings[channel].voiceCodes.bankLSB = value.getValueE<unsigned char>();
								value = xmlChannel->getAttribute( "STEREO_POS" );
								channelSettings[channel].stereoPos = value.getValueE<char>();
								value = xmlChannel->getAttribute( "VOLUME" );
								channelSettings[channel].volume = value.getValueE<unsigned char>();
								value = xmlChannel->getAttribute( "EXPRESSION" );
								channelSettings[channel].expression = value.getValueE<unsigned char>();

								for( effect=0; effect<5; effect++ )
								{
									name = "EFFECT_";
									name += formatNumber( effect );
									value = xmlChannel->getAttribute( name );
									channelSettings[channel].effects[effect] = value.getValueE<unsigned char>();
								}

								for( sound=0; sound<5; sound++ )
								{
									name = "SOUND_";
									name += formatNumber( sound );
									value = xmlChannel->getAttribute( name );
									channelSettings[channel].sounds[sound] = value.getValueE<unsigned char>();
								}
							}
							if( playerHandles.isOutOpen( midiDev ) )
							{
								for( channel=0; channel <= 15; channel++ )
								{
									sendChannelSettings( midiDev, channel );
								}
							}
						}
					}

					if( theMidiChannelsWindow )
						theMidiChannelsWindow->refreshWindow();

				}
				xml::Element *xmlFilterSettings = xmlMidiSettings->getElement( "FILTER_SETTINGS" );
				if( xmlFilterSettings )
				{
					theFilterList.clear();

					for( size_t i=0; i<xmlFilterSettings->getNumObjects(); i++ )
					{
						xml::Element *xmlFilter = xmlFilterSettings->getElement( i );

						if( xmlFilter && xmlFilter->getTag() == "FILTER" )
						{
							FilterSettings theFilter;

							theFilter.name = xmlFilter->getAttribute( "NAME" );
							theFilter.midiInName = xmlFilter->getAttribute( "FILTER_MIDI_IN" );
							if( theFilter.midiInName == ALL_LABEL )
							{
								theFilter.midiInName = allRecordLabel;
								theFilter.midiInDev = ALL_DEVICE;
							}
							else
							{
								theFilter.midiInDev = int(recorderHandles.getRecorder( theFilter.midiInName ));
							}

							value = xmlFilter->getAttribute( "FILTER_CHANNEL" );
							theFilter.filterChannel = value.getValueE<unsigned char>();
							value = xmlFilter->getAttribute( "CONDITION" );
							valueI = value.getValueE<unsigned>();
							theFilter.condition = (FilterCondition) valueI;
							value = xmlFilter->getAttribute( "NOTE" );
							theFilter.note = value.getValueE<unsigned char>();
							theFilter.midiOutName = xmlFilter->getAttribute( "FILTER_MIDI_OUT" );
							if( theFilter.midiOutName == CONTROLLER_LABEL )
								theFilter.midiOutDev = CONTROLLER_DEVICE;
							else if( theFilter.midiOutName == DEFAULT_LABEL )
							{
								theFilter.midiOutName = defaultPlayerLabel;
								theFilter.midiOutDev = DEFAULT_DEVICE;
							}
							else
								theFilter.midiOutDev = int(playerHandles.getPlayer( theFilter.midiOutName ));

							value = xmlFilter->getAttribute( "ACTION_CHANNEL" );
							theFilter.actionChannel = value.getValueE<unsigned char>();
							value = xmlFilter->getAttribute( "NOTE_OFFSET" );
							theFilter.noteOffset = value.getValueE<char>();
							theFilter.newVolume = xmlFilter->getAttribute( "NEW_VOLUME" );
							theFilter.autoLevel = 0.0;

							value = xmlFilter->getAttribute( "STOP_FLAG" );
							if( !value.isEmpty() )
							{
								valueI = value.getValueE<unsigned>();
								theFilter.stopFlag = valueI ? true : false;
							}
							else
								theFilter.stopFlag = false;

							theFilterList.addElement( theFilter );
						}
					}

					if( theMidiFilterWindow )
					{
						theMidiFilterWindow->refreshFilter();
					}
				}
				xml::Element *xmlMetronomSettings = xmlMidiSettings->getElement( "METRONOM_SETTINGS" );
				if( xmlMetronomSettings )
				{
					firstMetronom = xmlMetronomSettings->getAttribute( "FIRST_METRONOM" );
					otherMetronom = xmlMetronomSettings->getAttribute( "OTHER_METRONOM" );
				}
			}

			delete xmlMidiSettingsDoc;
		}

		settingsChangedFlag = false;
	}
}

// --------------------------------------------------------------------- //
// ----- entry points -------------------------------------------------- //
// --------------------------------------------------------------------- //

