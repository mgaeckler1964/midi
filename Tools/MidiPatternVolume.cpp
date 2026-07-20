/*
		Project:		MIDI Tools
		Module:			MIDIpatternVolume.cpp
		Description:	Change the volume of one drum pattern 
						(used by drum window)
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

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <gak/fmtNumber.h>

#include "MidiPatternVolume.h"

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

void PatternVolumeDialog::handlePatternSelection( unsigned char pattern )
{
	m_selectedPattern = pattern;
	volumeBar->setPosition( m_patternVolumes[pattern] );
	handleNewVolume( m_patternVolumes[pattern] );
}

void PatternVolumeDialog::handleNewVolume( unsigned char volume )
{
	volumeLabel->setText( formatNumber( volume ) );
	m_patternVolumes[m_selectedPattern] = volume;
}

// --------------------------------------------------------------------- //
// ----- class protected ----------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class virtuals ------------------------------------------------ //
// --------------------------------------------------------------------- //

ProcessStatus PatternVolumeDialog::handleCreate()
{
	size_t	i;

	volumeBar->setRange( 0, 127 );


	patternSelect->addEntry( "Alle" );
	for( i=0; i<m_numPatterns-1; i++ )
	{
		patternSelect->addEntry( (*m_patternList)[i].drumVoice.voice );
	}
	patternSelect->selectEntry( 0 );
	handlePatternSelection( 0 );

	return psDO_DEFAULT;
}

ProcessStatus PatternVolumeDialog::handleCommand( int cmd )
{
	switch( cmd )
	{
		case winlibGUI::patternSelect_id:
			handlePatternSelection( static_cast<unsigned char>(patternSelect->getSelection()) );
			break;
		case winlibGUI::volumeBar_id:
			handleNewVolume( static_cast<unsigned char>(volumeBar->getPosition()) );
			break;

		default:
			return PatternVolume_form::handleCommand( cmd );
	}
	return psPROCESSED;
}


// --------------------------------------------------------------------- //
// ----- class publics ------------------------------------------------- //
// --------------------------------------------------------------------- //

SuccessCode PatternVolumeDialog::create(
	BasicWindow			*parent,
	PatternInfos		*patternList,
	const unsigned char *patternVolumes, size_t numPatterns
)
{

	m_patternVolumes = (unsigned char*)malloc( numPatterns * sizeof(unsigned char) );

	if( m_patternVolumes )
	{
		m_patternList = patternList;
		m_numPatterns = numPatterns;

		for( size_t i=0; i<numPatterns; i++ )
			m_patternVolumes[i] = patternVolumes[i];
	}


	return PatternVolume_form::create( parent );
}

// --------------------------------------------------------------------- //
// ----- entry points -------------------------------------------------- //
// --------------------------------------------------------------------- //

