/*
		Project:		MIDI Tools
		Module:			MidiFilterWindow.h
		Description:	The filter settings (used by recorder window)
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

#ifndef MIDI_FILTER_WINDOW_H
#define MIDI_FILTER_WINDOW_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <gak/ci_string.h>

#include <winlib/controlw.h>
#include <winlib/dialogwi.h>

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

enum FilterCondition
{
	LESS_THAN, LESS_EQUAL, EQUAL, GREATER_EQUAL, GREATER_THAN, CONTROL
};

struct FilterSettings
{
	gak::STRING			name, midiInName, midiOutName;
	int					midiInDev, midiOutDev;
	unsigned char		filterChannel, note;
	FilterCondition		condition;
	unsigned char		actionChannel;
	signed char			noteOffset;
	bool				stopFlag;
	gak::CI_STRING		newVolume;
	double				autoLevel;
};
typedef gak::Array<FilterSettings> FilterList;

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

class MIDIrecorderWindow;

class MIDIfilterWindow : public winlibGUI::MIDIfilterWindow_form
{
	MIDIrecorderWindow	*theRecorderWindow;
	size_t				midiInCount, midiOutCount, lastSelectedFilter;

	FilterList			*theFilterList;

	void moveFilter( void );
	void createNewFilter( void );
	void deleteFilter( void );
	void createStandardFilter( const gak::STRING &newName, unsigned char channel, FilterCondition condition );
	void createStandardFilter( void );
	void showValues( size_t selected );

	virtual winlib::ProcessStatus handleCreate( void );
	virtual winlib::ProcessStatus handleScrollControl( int control );
	virtual winlib::ProcessStatus handleSelectionChange( int control );
	virtual winlib::ProcessStatus handleButtonClick( int control );
	virtual winlib::ProcessStatus handleEditChange( int control );
	virtual winlib::ProcessStatus handleCommand( int cmd );
	virtual winlib::SuccessCode handleClose( void );
	virtual winlib::ProcessStatus handleCancel( void );

	public:
	MIDIfilterWindow( winlib::BasicWindow *owner ) : MIDIfilterWindow_form( owner )
	{
	}
	void create( MIDIrecorderWindow	*theRecorderWindow, FilterList *theFilterList )
	{
		this->theRecorderWindow = theRecorderWindow;
		this->theFilterList = theFilterList;
		MIDIfilterWindow_form::create( reinterpret_cast<BasicWindow*>(theRecorderWindow) );
		restoreWindowPos();
	}
	winlib::SuccessCode close()
	{
		saveWindowRect();
		return MIDIfilterWindow_form::close();
	}

	void refreshFilter( size_t newSelected=0 );
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
