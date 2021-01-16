/*
		Project:		MIDI Tools
		Module:			InstrumentWindow.cpp
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

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include "InstrumentWindow.h"
#include "MidiFile.h"
#include "MidiEditorWindow.h"

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

using namespace winlib;

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

// --------------------------------------------------------------------- //
// ----- class static data --------------------------------------------- //
// --------------------------------------------------------------------- //

unsigned char InstrumentWindow::stringMidiNotes[] = {
	0,
	MIDI_NOTE_E3, MIDI_NOTE_H2, MIDI_NOTE_G2,
	MIDI_NOTE_D2, MIDI_NOTE_A1, MIDI_NOTE_E1
};

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

int InstrumentWindow::drawNote(
	Device &context, unsigned char note,
	bool active, int string,
	int *minBound, int *maxBound )
{
	int dotPos = stringDistance * string;
	int bound = note - stringMidiNotes[string];

	if( bound >= 0 && bound <= 22 )
	{
		bool noteOK;

		if( bound && minBound && maxBound )
		{
			if( *minBound && bound-*minBound > 3 )
				noteOK = false;
			else if( *maxBound && *maxBound - bound > 3 )
			{
				noteOK = false;
				bound = -1;
			}
			else
			{
				noteOK = true;
				if( bound > *maxBound )
					*maxBound = bound;
				if( bound < *minBound )
					*minBound = bound;
			}
		}
		else
			noteOK = true;

		if( noteOK )
		{
			context.getPen().selectPen( Pen::spBlack );

			if( active )
				context.getBrush().create( 255, 0, 0 );
			else
				context.getBrush().create( 0, 255, 0 );

			context.ellipse( nutDistance + boundDistance * bound, dotPos, fingerSize );
			bound = 0;
		}
	}

	return bound;
}

void InstrumentWindow::drawNote( Device &context, unsigned char note, bool markActive )
{
	for( int string=1; string<=6; string++ )
	{
		drawNote(
			context, note,
			markActive && string == currentString, string,
			NULL, NULL
		);
	}
}

// --------------------------------------------------------------------- //
// ----- class protected ----------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class virtuals ------------------------------------------------ //
// --------------------------------------------------------------------- //

ProcessStatus InstrumentWindow::handleCreate( void )
{
	int left = 80;

	STRING	majorLabel = appObject->loadString( winlibGUI::MAJOR_LABEL_id );
	STRING	minorLabel = appObject->loadString( winlibGUI::MINOR_LABEL_id );
	STRING	allLabel = appObject->loadString( winlibGUI::ALL_NOTES_LABEL_id );
	STRING	tmp;
	/*
		dreiklänge
	*/
	int width = int(majorLabel.strlen()) * 10 +20;
	dur.create( this, left, 8, width, 20);
	dur.setText( majorLabel );
	dur.setBackgroundColor( 255, 255, 255 );
	left += width;

	width = int(minorLabel.strlen()) * 10 +20;
	moll.create( this, left, 8, width, 20);
	moll.setText( minorLabel );
	moll.setBackgroundColor( 255, 255, 255 );
	left += width;

	width = 10 +20;
	plus.create( this, left, 8, width, 20);
	plus.setText( "+" );
	plus.setBackgroundColor( 255, 255, 255 );
	left += width;

	width = 30 +20;
	dim.create( this, left, 8, width, 20);
	dim.setText( "dim" );
	dim.setBackgroundColor( 255, 255, 255 );
	left += width +20;

	/*
		vierklänge
	*/
	width = 40 +20;
	dom7.create( this, left, 8, width, 20);
	dom7.setText( "dom7" );
	dom7.setBackgroundColor( 255, 255, 255 );
	left += width;

	width = 30 +20;
	moll7.create( this, left, 8, width, 20);
	moll7.setText( "m 7" );
	moll7.setBackgroundColor( 255, 255, 255 );
	left += width;

	width = 40 +20;
	maj7.create( this, left, 8, width, 20);
	maj7.setText( "maj7" );
	maj7.setBackgroundColor( 255, 255, 255 );
	left += width;

	width = 60 +20;
	mollMaj7.create( this, left, 8, width, 20);
	mollMaj7.setText( "m maj7" );
	mollMaj7.setBackgroundColor( 255, 255, 255 );
	left += width +20;

	/*
		Skalen
	*/
	tmp = majorLabel;
	tmp += " (7)";
	width = int(tmp.strlen())*10 +20;
	durScale.create( this, left, 8, width, 20);
	durScale.setText( tmp );
	durScale.setBackgroundColor( 255, 255, 255 );
	left += width;

	tmp = majorLabel;
	tmp += " (5)";
	width = int(tmp.strlen())*10 +20;
	dur5Scale.create( this, left, 8, width, 20);
	dur5Scale.setText( tmp );
	dur5Scale.setBackgroundColor( 255, 255, 255 );
	left += width;

	tmp = minorLabel;
	tmp += " (7)";
	width = int(tmp.strlen())*10 +20;
	mollScale.create( this, left, 8, width, 20);
	mollScale.setText( tmp );
	mollScale.setBackgroundColor( 255, 255, 255 );
	left += width;

	tmp = minorLabel;
	tmp += " (5)";
	width = int(tmp.strlen())*10 +20;
	moll5Scale.create( this, left, 8, width, 20);
	moll5Scale.setText( tmp );
	moll5Scale.setBackgroundColor( 255, 255, 255 );
	left += width +20;

	width = int(allLabel.strlen()) * 10 +20;
	allNotes.create( this, left, 8, width, 20);
	allNotes.setText( allLabel );
	allNotes.setBackgroundColor( 255, 255, 255 );
	allNotes.setActive();

	return psPROCESSED;
}

ProcessStatus InstrumentWindow::handleMouseMove( WPARAM, const Point &position )
{
	if( !fixNote && stringDistance && boundDistance )
	{
		int newString = (position.y + stringDistance/2) / stringDistance;
		int newBound = position.x > 20 ? ((position.x-20) / boundDistance +1) : 0;
		unsigned char newNote =
			( newString >0 && newString <= 6 )
			? (unsigned char)(stringMidiNotes[newString] + newBound)
			: thePainter.getNote( position.x, position.y );

		if( newString != currentString
		||  newNote != currentNote )
		{
			currentString = newString;
			currentNote = newNote;
			invalidateWindow( false );
		}
	}

	return psDO_DEFAULT;
}

ProcessStatus InstrumentWindow::handleLeftButton( LeftButton leftButton, WPARAM , const Point & )
{
	if( parent && leftButton == lbUP && currentNote <= 127 )
	{
		parent->setNote( currentNote );
		close();
		return psPROCESSED;
	}
	return psDO_DEFAULT;
}

ProcessStatus InstrumentWindow::handleRightButton( RightButton rightButton, WPARAM , const Point &position )
{
	if( rightButton == rbUP && currentNote <= 127 )
	{
		fixNote = !fixNote;
		if( !fixNote )
			handleMouseMove( 0, position );
		return psPROCESSED;
	}
	return psDO_DEFAULT;
}

ProcessStatus InstrumentWindow::handleResize( const Size & )
{
	invalidateWindow( false );

	return psDO_DEFAULT;
}

ProcessStatus InstrumentWindow::handleRepaint( Device &hDC )
{
	Size 			size = getClientSize();

	MemoryDevice	context( hDC, size );

	context.getPen().selectPen( Pen::spBlack );
	context.getBrush().selectBrush( Brush::sbWhite );

	context.rectangle( 0, 0, size.width, size.height );

	stringDistance = size.height/12;
	fingerSize = stringDistance / 5;
	dotSize = fingerSize / 2;
	nutDistance = fingerSize;

	boundDistance = (size.width-nutDistance) / 22;

	/*
		draw the guitar tabs
	*/
	// the nut
	context.getPen().create( Pen::psSolid, nutDistance/2, RGB( 100, 50, 50 ) );
	context.verticalLine( nutDistance, stringDistance, stringDistance*6 );

	// the bounds
	context.getPen().create( Pen::psSolid, 1, RGB( 20, 20, 20 ) );
	for( int i=1; i<22; i++ )
	{
		context.verticalLine( nutDistance+i*boundDistance, stringDistance, stringDistance*6 );
	}

	// the markers
	context.getPen().create( Pen::psSolid, 1, RGB( 200, 75, 75 ) );
	context.getBrush().create( 200, 75, 75 );

	int dotPos = stringDistance * 3 + stringDistance/2;
	context.ellipse( nutDistance + boundDistance * 2 + boundDistance/2, dotPos, dotSize );
	context.ellipse( nutDistance + boundDistance * 4 + boundDistance/2, dotPos, dotSize );
	context.ellipse( nutDistance + boundDistance * 6 + boundDistance/2, dotPos, dotSize );
	context.ellipse( nutDistance + boundDistance * 8 + boundDistance/2, dotPos, dotSize );
	context.ellipse( nutDistance + boundDistance * 11 + boundDistance/2, dotPos-dotSize*2, dotSize );
	context.ellipse( nutDistance + boundDistance * 11 + boundDistance/2, dotPos+dotSize*2, dotSize );
	context.ellipse( nutDistance + boundDistance * 14 + boundDistance/2, dotPos, dotSize );
	context.ellipse( nutDistance + boundDistance * 16 + boundDistance/2, dotPos, dotSize );
	context.ellipse( nutDistance + boundDistance * 18 + boundDistance/2, dotPos, dotSize );

	// the strings
	for( int i=1; i<=6; i++ )
	{
		if( i==currentString )
			context.getPen().create( Pen::psSolid, i, RGB( 0, 200, 0 ) );
		else if( i <= 3 )
			context.getPen().create( Pen::psSolid, i, RGB( 127, 127, 127 ) );
		else
			context.getPen().create( Pen::psSolid, i, RGB( 200, 200, 0  ) );
		context.horizontalLine( 0, size.width, stringDistance*i );
	}

	// the piano
	thePainter.drawPiano( context, size, size.height/2 - 20 );

	// the current note
	if( currentNote <= 127 )
	{
		bool dummy;
		bool printScale = false;
		int		minBound = 0, maxBound = 0;

		const unsigned char *chordNotes = NULL;
		if( dur.isActive() )
			chordNotes = getDurNotes( currentNote );
		else if( durScale.isActive() )
		{
			printScale = true;
			chordNotes = getDurScaleNotes( currentNote );
		}
		else if( dur5Scale.isActive() )
		{
			printScale = true;
			chordNotes = getDur5ScaleNotes( currentNote );
		}
		else if( maj7.isActive() )
			chordNotes = getMaj7Notes( currentNote );
		else if( mollMaj7.isActive() )
			chordNotes = getMollMaj7Notes( currentNote );
		else if( moll.isActive() )
			chordNotes = getMollNotes( currentNote );
		else if( dom7.isActive() )
			chordNotes = getDom7Notes( currentNote );
		else if( moll7.isActive() )
			chordNotes = getMoll7Notes( currentNote );

		else if( mollScale.isActive() )
		{
			printScale = true;
			chordNotes = getMollScaleNotes( currentNote );
		}
		else if( moll5Scale.isActive() )
		{
			printScale = true;
			chordNotes = getMoll5ScaleNotes( currentNote );
		}
		else if( dim.isActive() )
			chordNotes = getDimNotes( currentNote );
		else if( plus.isActive() )
			chordNotes = getPlusNotes( currentNote );

		thePainter.drawNoteOnKey( context, currentNote, true, true, &dummy );
		if( allNotes.isActive() || printScale || currentString < 1 || currentString > 6 )
			drawNote( context, currentNote );
		else
			drawNote( context, currentNote, true, currentString, &minBound, &maxBound );

		context.getPen().selectPen( Pen::spBlack );
		STRING noteName = 	MIDIevent::getNoteText( currentNote );
		context.textOut( 2, 2, noteName );

		if( chordNotes )
		{
			for(int i=1; chordNotes[i] <= 127; i++ )
				thePainter.drawNoteOnKey( context, chordNotes[i], true, true, &dummy );

			if( printScale )
			{
				char offset = 0;
				while( chordNotes[0] + offset > stringMidiNotes[6] )
				{
					offset = char(offset - 12);
				}

				while( chordNotes[0] + offset < stringMidiNotes[1] + 24 )
				{
					for(int i=0; chordNotes[i] <= 127; i++ )
					{
						if( chordNotes[i]+offset != currentNote )
						{
							drawNote(
								context,
								(unsigned char)(chordNotes[i]+offset),
								false
							);
						}
					}

					offset = char(offset + 12);
				}
			}
			else if( currentString >= 1 && currentString <= 6 )
			{
				int	noteStatus;
				unsigned char offset=0;
				int string = currentString-1, i=1;
				while( string > 0 )
				{
					if( chordNotes[i] > 127 )
					{
						offset += (unsigned char)12;
						i=0;
					}
					noteStatus = drawNote(
						context,
						(unsigned char)(chordNotes[i] + offset),
						i==0, string,
						&minBound, &maxBound
					);
					if( noteStatus < 0 )		// string is too high -> ignore note
					{
						if( i != 1 )
						{
							drawNote(
								context,
								(unsigned char)(chordNotes[i] + offset),
								i==0, string+1,
								&minBound, &maxBound
							);
						}
						i++;
					}
					else if( !noteStatus )		// note was printed
					{							// => use next note with next string
						string--;
						i++;
					}
					else
						string--;				// string was too low -> try next
				}
			}
		}
	}

	context.drawToWindow();

	return psPROCESSED;
}

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

