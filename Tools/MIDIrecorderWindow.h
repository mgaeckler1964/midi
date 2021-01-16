/*
		Project:		MIDI Tools
		Module:			MIDIrecorderWindow.cpp
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

#ifndef MIDI_RECORDER_H
#define MIDI_RECORDER_H

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <time.h>

#include <gak/thread.h>

#include <winlib/area.h>
#include <winlib/dialogwi.h>
#include <winlib/controlw.h>
#include <winlib/stddlg.h>

#include "PlayerWindow.h"
#include "midiChannelsWindow.h"
#include "midiFilterWindow.h"
#include "MetronomWindow.h"
#include "midifile.h"
#include "midi.gui.h"

// --------------------------------------------------------------------- //
// ----- module switches ----------------------------------------------- //
// --------------------------------------------------------------------- //

#ifdef __BORLANDC__
#	pragma option -RT-
#	ifdef __WIN32__
#		pragma option -a4
#		pragma option -pc
#	else
#		pragma option -po
#		pragma option -a2
#	endif
#endif

// --------------------------------------------------------------------- //
// ----- constants ----------------------------------------------------- //
// --------------------------------------------------------------------- //

#define CONTROLLER_LABEL	"Controller"		// output goes to input device/channel
#define CONTROLLER_DEVICE	-1
#define DEFAULT_LABEL		"Standard"			// output goes to default device
#define DEFAULT_DEVICE		-2
#define ALL_LABEL			"Alle"				// Input from any device
#define ALL_DEVICE			-1

// --------------------------------------------------------------------- //
// ----- type definitions ---------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //


class PianoArea;
class MIDIrecorderWindow;

class MetronomThread : public gak::Thread
{
	MIDIrecorderWindow	*m_recWindow;
	unsigned			m_bpm, m_numerator, m_denominator;
	unsigned char		channel;

	virtual void ExecuteThread( void );

	public:
	MetronomThread( MIDIrecorderWindow *recWindow, unsigned bpm, unsigned numerator, unsigned denominator )
	{
		assert( bpm );
		assert( numerator );

		m_recWindow = recWindow;
		m_bpm = bpm;
		m_numerator = numerator;
		m_denominator = denominator;
		channel = 9;
	}
	unsigned char getChannel( void ) const
	{
		return channel;
	}
};
typedef gak::SharedObjectPointer<MetronomThread>	MetronomThreadPtr;

class MidiPlayerThread : public gak::Thread
{
	MIDIrecorderWindow *recWindow;
	MIDIdata			*midiData;
	size_t				midiDev;
	bool				autoRhythm;

	virtual void ExecuteThread( void );

	public:
	MidiPlayerThread( MIDIrecorderWindow *recWindow, MIDIdata *midiData, size_t midiDev, bool autoRhythm )
	{
		this->recWindow = recWindow;
		this->midiData = midiData;
		this->midiDev = midiDev;
		this->autoRhythm = autoRhythm;
	}
};
typedef gak::SharedObjectPointer<MidiPlayerThread>	MidiPlayerThreadPtr;

class MIDIrecorderWindow : public winlibGUI::MIDIrecorderWindow_form, public PlayerWindow
{
	bool						midiChangedFlag, settingsChangedFlag;
	enum
	{
		MIDI_WAITING, MIDI_PLAYING, MIDI_RECORDING
	} 	midiStatus;

	bool						filterEnabled;
	bool						doNoteHold;

	PianoArea					*thePianoArea;

	MidiPlayerThreadPtr			midiPlayer;
	MetronomThreadPtr			metronomThread;

	MetronomWindow				*metronomWindow;
	gak::STRING					firstMetronom, otherMetronom;
	int							metronomWindowXpos, metronomWindowYpos;

	FilterList					theFilterList;

	gak::STRING					lastOptionFile;
	MIDIfilterWindow 			*theMidiFilterWindow;
	MIDIchannelsWindow			*theMidiChannelsWindow;

	gak::STRING					recordLabel,
								allRecordLabel, defaultPlayerLabel;

	private:
	size_t openInMidi( void );

	void saveMidi( void );
	void checkMidiThrough( void );

	void saveSettings( void );

	void playMidi( void );
	void stopPlayMidi( bool dontKill=false );

	void stopRecMidi( void );
	void recMidi( void );

	void stopMetronom( void );
	void startMetronom( void );

	void playUnfilteredMidiEvent( size_t midiDev, MIDIevent &msg );

	public:
	MIDIrecorderWindow( BasicWindow *owner );

	void create( void );
	virtual winlib::ProcessStatus handleCreate( void );
	virtual bool handleChildClose( BasicWindow *child, bool deleted );
	virtual bool canClose( void );
	virtual winlib::ProcessStatus handleDestroy( void );
	virtual winlib::ProcessStatus handleButtonClick( int btn );
	virtual winlib::ProcessStatus handleSelectionChange( int control );
	virtual winlib::ProcessStatus handleCommand( int cmd );
	virtual winlib::ProcessStatus handleMessage( UINT message, WPARAM wParam, LPARAM lParam );


	virtual winlib::SuccessCode close( void );

	void playFinished( void )
	{
		stopPlayMidi( true );
	}
	void playFilteredMidiEvent( int midiInDev, size_t midiOutDev, MIDIevent &msg );
	void sendChannelSettings( size_t midiDev, unsigned char channel );
	void setSettingsChanged( void )
	{
		settingsChangedFlag = true;
	}

	const DrumVoice &getFirstMetronomVoice( void )
	{
		const DrumVoice	&firstMetronom = metronomWindow->getFirstMetronomVoice();

		if( this->firstMetronom != firstMetronom.voice )
		{
			this->firstMetronom = firstMetronom.voice;
			settingsChangedFlag = true;
		}

		return firstMetronom;
	}
	const DrumVoice &getOtherMetronomVoice( void )
	{
		const DrumVoice	&otherMetronom = metronomWindow->getOtherMetronomVoice();

		if( this->otherMetronom != otherMetronom.voice )
		{
			this->otherMetronom = otherMetronom.voice;
			settingsChangedFlag = true;
		}

		return otherMetronom;
	}

	bool isMetronomRunning( void ) const
	{
		return metronomThread ? true : false;
	}
	void loadMidiSettings( const char *cmdLine=NULL );
	void loadMidiFile( const char *cmdLine=NULL );

	size_t findMidiInHandle( HMIDIIN midiHandle )
	{
		return recorderHandles.findElement( midiHandle, this );
	}
};

// --------------------------------------------------------------------- //
// ----- exported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class privates ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class virtuals ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class publics ------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- entry points -------------------------------------------------- //
// --------------------------------------------------------------------- //

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -a.
#	pragma option -p.
#endif

#endif
