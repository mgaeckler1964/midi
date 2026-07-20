/*
		Project:		MIDI Tools
		Module:			MIDIloopEditor.h
		Description:	The (drum) loop editor
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
typedef gak::Array<LoopEntry>	LoopEntries;
// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //


class MIDIloopEditor;

class MidiLoopPlayerThread : public gak::Thread
{
	unsigned long		m_loopPoint;
	MIDIloopEditor		*m_loopEditor;
	MIDIdata			*m_midiData;

	virtual void ExecuteThread();

	public:
	MidiLoopPlayerThread(
		MIDIloopEditor	*loopEditor,
		MIDIdata		*midiData,
		unsigned long	loopPoint
	)
	{
		m_loopEditor	= loopEditor;
		m_midiData		= midiData;
		m_loopPoint		= loopPoint;
	}
};
typedef gak::SharedObjectPointer<MidiLoopPlayerThread>	MidiLoopPlayerThreadPtr;

class MIDIloopEditor : public winlibGUI::MIDIloopEditor_form, public PlayerWindow
{
	LoopEntries					m_theLoopList;
	bool	   					m_changedFlag;
	int							m_firstLoopEntry;

	MidiLoopPlayerThreadPtr		m_midiPlayer;

	private:

	unsigned long compileLoop2Midi();
	void saveLoopXmlFile( const gak::STRING &fileName );
	void saveLoopFile();

	void playMidi();
	void stopPlayMidi();

	gak::STRING	makeListBoxEntry( const gak::STRING &fileName );

	public:
	MIDIloopEditor( BasicWindow *owner ) : MIDIloopEditor_form( owner )
	{
		m_midiPlayer = nullptr;
		m_changedFlag = false;
	}

	void showClock( unsigned long timeCode, const char *time );

	void create();
	virtual winlib::ProcessStatus handleCreate();
	virtual winlib::ProcessStatus handleDestroy();
	virtual bool canClose();
	virtual winlib::ProcessStatus handleButtonClick( int btn );
	virtual winlib::ProcessStatus handleCommand( int cmd );
	virtual void handleFile( const char *filename, size_t idx, size_t numFiles );

	void loadLoopFile( const char *cmdLine=nullptr );
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
