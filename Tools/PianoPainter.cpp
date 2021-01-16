/*
		Project:		MIDI Tools
		Module:			PianoPainter.cpp
		Description:	Draws the keys of a piano 
						(used by recorder and instrument window)
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

#include "PianoPainter.h"

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
using namespace gak;

// --------------------------------------------------------------------- //
// ----- constants ----------------------------------------------------- //
// --------------------------------------------------------------------- //

#define NUM_KEYS		61
#define NUM_WHITES		36

#define BLACK_OFFSET	2

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

int	PianoPainter::durTonleiter[7] =
{
	0, 2, 4, 5, 7, 9, 11
};

KeyboardNotes	PianoPainter::noteInfo[12] =
{
	{ 0, 0, 0, 0, false },
	{ 1, -1, BLACK_OFFSET, 0, true },
	{ 1, 0, 0, 0.5, false },
	{ 2, 0, -BLACK_OFFSET, 0.5, true },
	{ 2, 0, 0, 1, false },
	{ 3, 0, 0, 1.5, false },
	{ 4, -1, BLACK_OFFSET, 1.5, true },
	{ 4, 0, 0, 2, false },
	{ 5, -0.5, 0, 2, true },
	{ 5, 0, 0, 2.5, false },
	{ 6, 0, -BLACK_OFFSET, 2.5, true },
	{ 6, 0, 0, 3, false }
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

double PianoPainter::getKeyLeft( unsigned char midi_note )
{
	double	keyLeft;
	double	whiteWidth = getWhiteWidth();
	double	blackWidth = getBlackWidth();

	unsigned char	note	= (unsigned char)(midi_note-LEFT_OFFSET);
	unsigned char	octave	= (unsigned char)(note / 12);

	note = (unsigned char)(note%12);
	keyLeft = whiteWidth*7*octave;

	keyLeft += whiteWidth*(double)noteInfo[note].whiteFactor;
	keyLeft += blackWidth*noteInfo[note].blackFactor;
	keyLeft += noteInfo[note].keyOffset;

	return keyLeft;
}

// --------------------------------------------------------------------- //
// ----- class protected ----------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class virtuals ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class publics ------------------------------------------------- //
// --------------------------------------------------------------------- //

unsigned char PianoPainter::getNote( int x, int y )
{
	unsigned char note = 255;

	if( y <= windowHeigth && y >= windowHeigth - whiteLength )
	{
		double	whiteWidth = getWhiteWidth();
		double	blackWidth = getBlackWidth();

		int whiteKey = int(x / whiteWidth);
		int octave = whiteKey / 7;
		whiteKey %= 7;

		note = (unsigned char)(octave*12 + durTonleiter[whiteKey] + LEFT_OFFSET);
		if( y < windowHeigth - whiteLength + blackLength )
		{
			// if the next note is a black key check it
			if( whiteKey < 6
			&& durTonleiter[whiteKey+1] - durTonleiter[whiteKey] == 2 )
			{
				if( x >= getKeyLeft( (unsigned char)(note +1) ) )
					note++;
			}

			// if the prev not is a black key check it
			else if( whiteKey
			&& durTonleiter[whiteKey] - durTonleiter[whiteKey-1] == 2 )
			{
				if( x <= getKeyLeft( (unsigned char)(note -1) ) + blackWidth )
					note--;
			}
		}
	}

	return note;
}

int PianoPainter::drawNoteOnKey( Device &context, unsigned char note, bool noteOn, bool isSent, bool *halfTone )
{
	int			xPos, yPos;
	double		width = getBlackWidth();

	xPos = int(getKeyLeft( note ));

	if( isWhiteKey( note ) )
	{
		yPos = int(windowHeigth - width) -1;

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
		xPos += 3;
		*halfTone = false;
	}
	else
	{
		yPos = int(windowHeigth-(whiteLength-blackLength)-width-1);

		if( noteOn )
		{
			if( isSent )
			{
				context.getBrush().selectBrush( Brush::sbWhite );
				context.getPen().selectPen( Pen::spWhite );
			}
			else
			{
				context.getBrush().create( 127, 127, 255 );
				context.getPen().create( Pen::psSolid, 1, 127, 127, 255 );
			}
		}
		else
		{
			context.getBrush().selectBrush( Brush::sbBlack );
			context.getPen().selectPen( Pen::spBlack );
		}

		xPos += 1;
		*halfTone = true;
	}

	context.ellipse( xPos, yPos, xPos+int(width), yPos+int(width)+1 );

	return xPos;
}

void PianoPainter::drawPiano( Device &context, const Size &size, int whiteLength )
{
	this->whiteLength = whiteLength;
	this->blackLength = whiteLength * 3 / 4;
	windowHeigth = size.height;

	whiteWidth = double(size.width) / NUM_WHITES;
	blackWidth	= whiteWidth/2.0;

	double			leftPos, rightPos;

	// white keys
	context.getPen().selectPen( Pen::spBlack );
	context.getBrush().selectBrush( Brush::sbWhite );
	for( unsigned char i=LEFT_OFFSET; i<LEFT_OFFSET+NUM_KEYS; i++ )
	{
		if( isWhiteKey( i ) )
		{
			leftPos = getKeyLeft( i );
			if( isWhiteKey( (unsigned char)(i+1) ) )
				rightPos = getKeyLeft( (unsigned char)(i+1) );
			else
				rightPos = getKeyLeft( (unsigned char)(i+2) );


			context.rectangle( int(leftPos), windowHeigth - whiteLength, int(rightPos)+1, windowHeigth );
		}
	}

	// black keys
	context.getBrush().selectBrush( Brush::sbBlack );
	for( unsigned char i=LEFT_OFFSET; i<LEFT_OFFSET+NUM_KEYS; i++ )
	{
		if( !isWhiteKey( i ) )
		{
			leftPos = getKeyLeft( i );

			context.rectangle( int(leftPos), windowHeigth - whiteLength, int(leftPos+blackWidth)+1, windowHeigth - (whiteLength-blackLength) );
		}
	}
}

// --------------------------------------------------------------------- //
// ----- entry points -------------------------------------------------- //
// --------------------------------------------------------------------- //

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif

