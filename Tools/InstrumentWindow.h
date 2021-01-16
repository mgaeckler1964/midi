/*
		Project:		MIDI Tools
		Module:			InstrumentWindow.h
		Description:	The instruments, chord and scale window
						(opened by recorder or editor)
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

#ifndef INSTRUMENT_WINDOW_H
#define INSTRUMENT_WINDOW_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <winlib/popup.h>
#include <winlib/controlw.h>

#include "PianoPainter.h"
#include "midi.gui.h"

// --------------------------------------------------------------------- //
// ----- imported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module switches ----------------------------------------------- //
// --------------------------------------------------------------------- //

#ifdef __BORLANDC__
#	pragma option -RT-
#	pragma option -b
#	pragma option -w-inl
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

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

class MIDIeditorWindow;
class InstrumentWindow : public winlib::OverlappedWindow
{
	static unsigned char stringMidiNotes[];

	bool				fixNote;
	MIDIeditorWindow	*parent;

	PianoPainter		thePainter;

	winlib::RadioButton	dur, moll, dom7, moll7, maj7, mollMaj7, dim, plus;
	winlib::RadioButton	durScale, dur5Scale, mollScale, moll5Scale;
	winlib::RadioButton	allNotes;

	unsigned char currentNote;
	int currentString;
	int stringDistance, boundDistance, fingerSize, dotSize, nutDistance;

	int drawNote(
		winlib::Device &context,
		unsigned char note, bool active, int string,
		int *minBound, int *maxBound
	);
	void drawNote( winlib::Device &context, unsigned char note, bool markActive = true );

	virtual winlib::ProcessStatus handleCreate( void );
	virtual winlib::ProcessStatus handleRepaint( winlib::Device &hDC );
	virtual winlib::ProcessStatus handleResize( const winlib::Size &newSize);
	virtual winlib::ProcessStatus handleMouseMove( WPARAM modifier, const winlib::Point &position );
	virtual winlib::ProcessStatus handleRightButton( winlib::RightButton rightButton, WPARAM modifier, const winlib::Point &position );
	virtual winlib::ProcessStatus handleLeftButton( winlib::LeftButton leftButton, WPARAM modifier, const winlib::Point &position );

	public:
	InstrumentWindow( winlib::BasicWindow *owner ) : OverlappedWindow( owner )
	{
		fixNote = false;
		currentNote = 255;
		currentString = boundDistance = stringDistance = fingerSize = dotSize = nutDistance = 0;
	}
	winlib::SuccessCode create( MIDIeditorWindow *parent=NULL )
	{
		gak::STRING title = winlib::appObject->loadString( winlibGUI::INSTRUMENT_CAPTION_id );
		setTitle( title );
		this->parent = parent;
		return OverlappedWindow::create( (BasicWindow*)parent );
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
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif

#endif
