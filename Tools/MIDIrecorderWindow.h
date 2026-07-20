/*
		Project:		MIDI Tools
		Module:			MIDIrecorderWindow.cpp
		Description:	MIDI recording and controlling
		Author:			Martin Gäckler
		Address:		Hofmannsthalweg 14, A-4030 Linz
		Web:			https://www.gaeckler.at/

		Copyright:		(c) 2007-2026 Martin Gäckler

		This program is free software: you can redistribute it and/or modify  
		it under the terms of the GNU General Public License as published by  
		the Free Software Foundation, version 3.

		You should have received a copy of the GNU General Public License 
		along with this program. If not, see <http://www.gnu.org/licenses/>.

		THIS SOFTWARE IS PROVIDED BY Martin Gäckler, Linz, Austria ``AS IS''
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
	unsigned char		m_channel;

	virtual void ExecuteThread();

	public:
	MetronomThread( MIDIrecorderWindow *recWindow, unsigned bpm, unsigned numerator, unsigned denominator )
	{
		assert( bpm );
		assert( numerator );

		m_recWindow = recWindow;
		m_bpm = bpm;
		m_numerator = numerator;
		m_denominator = denominator;
		m_channel = 9;
	}
	unsigned char getChannel() const
	{
		return m_channel;
	}
};
typedef gak::SharedObjectPointer<MetronomThread>	MetronomThreadPtr;

class MidiPlayerThread : public gak::Thread
{
	MIDIrecorderWindow *m_recWindow;
	MIDIdata			*m_midiData;
	size_t				m_midiDev;
	bool				m_autoRhythm;

	virtual void ExecuteThread();

	public:
	MidiPlayerThread( MIDIrecorderWindow *recWindow, MIDIdata *midiData, size_t midiDev, bool autoRhythm )
	{
		m_recWindow = recWindow;
		m_midiData = midiData;
		m_midiDev = midiDev;
		m_autoRhythm = autoRhythm;
	}
};
typedef gak::SharedObjectPointer<MidiPlayerThread>	MidiPlayerThreadPtr;

class MIDIrecorderWindow : public winlibGUI::MIDIrecorderWindow_form, public PlayerWindow
{
	bool						m_midiChangedFlag, m_settingsChangedFlag;
	enum
	{
		MIDI_WAITING, MIDI_PLAYING, MIDI_RECORDING
	} 	m_midiStatus;

	bool						m_filterEnabled;
	bool						m_doNoteHold;

	PianoArea					*m_thePianoArea;

	MidiPlayerThreadPtr			m_midiPlayer;
	MetronomThreadPtr			m_metronomThread;

	MetronomWindow				*m_metronomWindow;
	gak::STRING					m_firstMetronom, m_otherMetronom;
	int							m_metronomWindowXpos, m_metronomWindowYpos;

	FilterList					m_theFilterList;

	gak::STRING					m_lastOptionFile;
	MIDIfilterWindow 			*m_theMidiFilterWindow;
	MIDIchannelsWindow			*m_theMidiChannelsWindow;

	gak::STRING					m_recordLabel,
								m_allRecordLabel, m_defaultPlayerLabel;

	private:
	size_t openInMidi();

	void saveMidi();
	void checkMidiThrough();

	void saveSettings();

	void playMidi();
	void stopPlayMidi( bool dontKill=false );

	void stopRecMidi();
	void recMidi();

	void stopMetronom();
	void startMetronom();

	void playUnfilteredMidiEvent( size_t midiDev, MIDIevent &msg );

	public:
	MIDIrecorderWindow( BasicWindow *owner );

	void create();
	virtual winlib::ProcessStatus handleCreate();
	virtual bool handleChildClose( BasicWindow *child, bool deleted );
	virtual bool canClose();
	virtual winlib::ProcessStatus handleDestroy();
	virtual winlib::ProcessStatus handleButtonClick( int btn );
	virtual winlib::ProcessStatus handleSelectionChange( int control );
	virtual winlib::ProcessStatus handleCommand( int cmd );
	virtual winlib::ProcessStatus handleMessage( UINT message, WPARAM wParam, LPARAM lParam );


	virtual winlib::SuccessCode close();

	void playFinished()
	{
		stopPlayMidi( true );
	}
	void playFilteredMidiEvent( int midiInDev, size_t midiOutDev, MIDIevent &msg );
	void sendChannelSettings( size_t midiDev, unsigned char channel );
	void setSettingsChanged()
	{
		m_settingsChangedFlag = true;
	}

	const DrumVoice &getFirstMetronomVoice()
	{
		const DrumVoice	&firstMetronom = m_metronomWindow->getFirstMetronomVoice();

		if( m_firstMetronom != firstMetronom.voice )
		{
			m_firstMetronom = firstMetronom.voice;
			m_settingsChangedFlag = true;
		}

		return firstMetronom;
	}
	const DrumVoice &getOtherMetronomVoice()
	{
		const DrumVoice	&otherMetronom = m_metronomWindow->getOtherMetronomVoice();

		if( m_otherMetronom != otherMetronom.voice )
		{
			m_otherMetronom = otherMetronom.voice;
			m_settingsChangedFlag = true;
		}

		return otherMetronom;
	}

	bool isMetronomRunning() const
	{
		return m_metronomThread ? true : false;
	}
	void loadMidiSettings( const char *cmdLine=nullptr );
	void loadMidiFile( const char *cmdLine=nullptr );

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
