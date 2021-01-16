/*
		Project:		MIDI Tools
		Module:			MIDIEditorWindow.h
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
	MIDIdata	 			*midiData;
	size_t		 			midiDev;
	const MIDIeditorWindow	*owner;

	virtual void ExecuteThread( void );

	public:
	MidiEditPlayerThread( MIDIdata *midiData, size_t midiDev, const MIDIeditorWindow *owner )
	{
		this->midiData = midiData;
		this->midiDev = midiDev;
		this->owner = owner;
	}
};

typedef gak::SharedObjectPointer<MidiEditPlayerThread>	MidiEditPlayerThreadPtr;

class MIDIeditorWindow : public winlibGUI::MIDIeditorWindow_form
{
	private:
	MIDIdata						*midiData;
	gak::STRING						voiceFile;
	gak::Array<MidiEditorEvent>		editorEvents;
	gak::Array<TrackInfo>			trackInfo;

	VoiceArray						theVoices;

	gak::STRING						playLabel, stopLabel;

	size_t							midiDev;
	MIDIdata						playerData;
	MidiEditPlayerThreadPtr			midiPlayer;

	static int eventCompare( const MidiEditorEvent &e1, const MidiEditorEvent &e2 );

	void loadMidi( void );
	void copyValues2Editor( void );
	void deleteEntry(  bool selectNext );
	size_t newEntry( const MidiEditorEvent &newEvent );
	void newEntry( void );
	void handleMessageType( int messageRadio );
	void changeVolume( void );
	void swapChannels( void );
	void repeatEntries( void );
	void transposeEntries( void );
	void copyMoveEntries( bool copy );
	void convertEditorEvents( MIDIdata *midiData, bool selected );

	virtual winlib::ProcessStatus handleCreate( void );
	virtual winlib::ProcessStatus handleOk( void );
	virtual winlib::ProcessStatus handleButtonClick( int btn );
	virtual winlib::ProcessStatus handleCommand( int cmd );
	virtual winlib::ProcessStatus handleSelectionChange( int control );

	public:
	MIDIeditorWindow() : MIDIeditorWindow_form( NULL ) {}
	~MIDIeditorWindow();
	winlib::SuccessCode create( winlib::BasicWindow *parent, MIDIdata *midiData, const gak::STRING &voiceFile, size_t midiDev )
	{
		this->midiData = midiData;
		this->voiceFile = voiceFile;
		this->midiDev = midiDev;
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
	void showPlayLabel( void )
	{
		playButton->setText( playLabel );
	}
	void showStopLabel( void )
	{
		playButton->setText( stopLabel );
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
