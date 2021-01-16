/*
		Project:		MIDI Tools
		Module:			MIDIloopEditor.h
		Description:	The (drum) loop editor
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

#ifndef MIDI_LOOP_EDITOR_H
#define MIDI_LOOP_EDITOR_H

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
#include "midifile.h"
#include "midi_rc.h"
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

// --------------------------------------------------------------------- //
// ----- type definitions ---------------------------------------------- //
// --------------------------------------------------------------------- //

struct LoopEntry
{
	gak::STRING		fileName, listBoxValue;
	int				counter;
	unsigned long	timeCode;
};
typedef gak::Array<LoopEntry>	LoopEntrys;
// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //


class MIDIloopEditor;

class MidiLoopPlayerThread : public gak::Thread
{
	unsigned long		loopPoint;
	MIDIloopEditor		*loopEditor;
	MIDIdata			*midiData;

	virtual void ExecuteThread( void );

	public:
	MidiLoopPlayerThread(
		MIDIloopEditor	*loopEditor,
		MIDIdata		*midiData,
		unsigned long	loopPoint
	)
	{
		this->loopEditor	= loopEditor;
		this->midiData		= midiData;
		this->loopPoint		= loopPoint;
	}
};
typedef gak::SharedObjectPointer<MidiLoopPlayerThread>	MidiLoopPlayerThreadPtr;

class MIDIloopEditor : public winlibGUI::MIDIloopEditor_form, public PlayerWindow
{
	LoopEntrys					theLoopList;
	bool	   					changedFlag;
	int							firstLoopEntry;

	MidiLoopPlayerThreadPtr		midiPlayer;

	private:

	unsigned long compileLoop2Midi( void );
	void saveLoopXmlFile( const gak::STRING &fileName );
	void saveLoopFile( void );

	void playMidi( void );
	void stopPlayMidi( void );

	gak::STRING	makeListBoxEntry( const gak::STRING &fileName );

	public:
	MIDIloopEditor( BasicWindow *owner ) : MIDIloopEditor_form( owner )
	{
		midiPlayer = NULL;
		changedFlag = false;
	}

	void showClock( unsigned long timeCode, const char *time );

	void create( void );
	virtual winlib::ProcessStatus handleCreate( void );
	virtual winlib::ProcessStatus handleDestroy( void );
	virtual bool canClose( void );
	virtual winlib::ProcessStatus handleButtonClick( int btn );
	virtual winlib::ProcessStatus handleCommand( int cmd );
	virtual void handleFile( const char *filename, size_t idx, size_t numFiles );

	void loadLoopFile( const char *cmdLine=NULL );
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
