/*
		Project:		MIDI Tools
		Module:			MIDIpatternStereo.cpp
		Description:	Change the stereo position of one drum pattern 
						(used by drum window)
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

#include "MidiPatternStereo.h"
#include "MIDIfile.h"

#include "midi_rc.h"

// --------------------------------------------------------------------- //
// ----- imported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module switches ----------------------------------------------- //
// --------------------------------------------------------------------- //

using namespace winlib;
using namespace gak;

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

void PatternStereoDialog::handlePatternSelection( unsigned char pattern )
{
	selectedPattern = pattern;
	stereoBar->setPosition( patternList[pattern].stereoPos );
	handleNewStereoPosition( patternList[pattern].stereoPos );
}

void PatternStereoDialog::handleNewStereoPosition( signed char stereoPosition )
{
	stereoLabel->setText( formatNumber( stereoPosition ) );
	patternList[selectedPattern].stereoPos = stereoPosition;
}

// --------------------------------------------------------------------- //
// ----- class protected ----------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class virtuals ------------------------------------------------ //
// --------------------------------------------------------------------- //

ProcessStatus PatternStereoDialog::handleCreate( void )
{
	stereoBar->setRange( MIDI_LEFT_POSITION, MIDI_RIGHT_POSITION );

	for( 
		PatternInfos::const_iterator it = patternList.cbegin(),
			endIT = patternList.cend();
		it != endIT;
		++it
	)
	{
		patternSelect->addEntry( it->drumVoice.voice );
	}
	patternSelect->selectEntry( 0 );
	handlePatternSelection( 0 );

	return psDO_DEFAULT;
}

ProcessStatus PatternStereoDialog::handleCommand( int cmd )
{
	switch( cmd )
	{
		case winlibGUI::patternSelect_id:
			handlePatternSelection( static_cast<unsigned char>(patternSelect->getSelection()) );
			break;
		case winlibGUI::stereoBar_id:
			handleNewStereoPosition( static_cast<unsigned char>(stereoBar->getPosition()) );
			break;

		default:
			return PatternStereo_form::handleCommand( cmd );
	}
	return psPROCESSED;
}


// --------------------------------------------------------------------- //
// ----- class publics ------------------------------------------------- //
// --------------------------------------------------------------------- //

SuccessCode PatternStereoDialog::create(
	BasicWindow			*parent,
	const PatternInfos	&patternList
)
{
	this->patternList = patternList;

	return PatternStereo_form::create( parent );
}

// --------------------------------------------------------------------- //
// ----- entry points -------------------------------------------------- //
// --------------------------------------------------------------------- //

