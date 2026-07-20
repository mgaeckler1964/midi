/*
		Project:		MIDI Tools
		Module:			MIDIEditorWindow.h
		Description:	A MIDI Editor (used by recorder)
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

#ifndef MIDI_EDITOR_WINDOW_H
#define MIDI_EDITOR_WINDOW_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include "PlayerWindow.h"
#include "voices.h"
#include "midi.gui.h"
#include "controls.h"

// --------------------------------------------------------------------- //
// ----- imported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

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

// --------------------------------------------------------------------- //
// ----- macros -------------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- type definitions ---------------------------------------------- //
// --------------------------------------------------------------------- //

struct MidiEditorEvent
{
	gak::STRING		title;
	int				index;
	unsigned short	track;
	unsigned char	channel;
	unsigned long	timeCode;
	unsigned char	message;
	union
	{
		VoiceCodes		voiceCodes;
		struct
		{
			unsigned char	note;
			unsigned char	volume;
			unsigned long	length;
			unsigned char	time;
		} sound;
		struct
		{
			unsigned char	data1;
			unsigned char	data2;
		} midiData;
	} midiEvent;
	MIDIevent	sysExEvent;

};

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

class MIDIeditorWindow;

class MidiEditPlayerThread : public gak::Thread
{
	MIDIdata	 			*m_midiData;
	size_t		 			m_midiDev;
	const MIDIeditorWindow	*m_owner;

	virtual void ExecuteThread();

	public:
	MidiEditPlayerThread( MIDIdata *midiData, size_t midiDev, const MIDIeditorWindow *owner )
	{
		m_midiData = midiData;
		m_midiDev = midiDev;
		m_owner = owner;
	}
};

typedef gak::SharedObjectPointer<MidiEditPlayerThread>	MidiEditPlayerThreadPtr;

class MIDIeditorWindow : public winlibGUI::MIDIeditorWindow_form
{
	private:
	MIDIdata						*m_midiData;
	gak::STRING						m_voiceFile;
	gak::Array<MidiEditorEvent>		m_editorEvents;
	gak::Array<TrackInfo>			m_trackInfo;

	VoiceArray						m_theVoices;

	gak::STRING						m_playLabel, m_stopLabel;

	size_t							m_midiDev;
	MIDIdata						m_playerData;
	MidiEditPlayerThreadPtr			m_midiPlayer;

	static int eventCompare( const MidiEditorEvent &e1, const MidiEditorEvent &e2 );

	void loadMidi();
	void copyValues2Editor();
	void deleteEntry(  bool selectNext );
	size_t newEntry( const MidiEditorEvent &newEvent );
	void newEntry();
	void handleMessageType( int messageRadio );
	void changeVolume();
	void swapChannels();
	void repeatEntries();
	void transposeEntries();
	void copyMoveEntries( bool copy );
	void convertEditorEvents( MIDIdata *midiData, bool selected );

	virtual winlib::ProcessStatus handleCreate();
	virtual winlib::ProcessStatus handleOk();
	virtual winlib::ProcessStatus handleButtonClick( int btn );
	virtual winlib::ProcessStatus handleCommand( int cmd );
	virtual winlib::ProcessStatus handleSelectionChange( int control );

	public:
	MIDIeditorWindow() : MIDIeditorWindow_form( nullptr ) {}
	~MIDIeditorWindow();
	winlib::SuccessCode create( winlib::BasicWindow *parent, MIDIdata *midiData, const gak::STRING &voiceFile, size_t midiDev )
	{
		m_midiData = midiData;
		m_voiceFile = voiceFile;
		m_midiDev = midiDev;
		return MIDIeditorWindow_form::create( parent );
	}
	void setNote( unsigned char note )
	{
		voiceRadio->clrActive();
		noteRadio->setActive();
		controllerRadio->clrActive();
		otherRadio->clrActive();
		sysExRadio->clrActive();
		handleMessageType( winlibGUI::noteRadio_id );
		noteSelect->selectEntry( note );
	}
	void showPlayLabel()
	{
		playButton->setText( m_playLabel );
	}
	void showStopLabel()
	{
		playButton->setText( m_stopLabel );
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

// --------------------------------------------------------------------- //
// ----- class static functions ---------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class privates ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class protected ----------------------------------------------- //
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
