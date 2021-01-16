/*
		Project:		MIDI Tools
		Module:			Voices.cpp
		Description:	Keyboard voices, Program change bank select
						(used by channel settings and editor)
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

#include <fstream>

#include <gak/csv.h>

#include <winlib/winapp.h>

#include "voices.h"
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

void VoiceArray::addVoice( const STRING &theVoice )
{
	for( 
		ArrayOfStrings::const_iterator	it = m_currentVoices.cbegin(), endIT = m_currentVoices.cend();
		it != endIT;
		++it
	)
	{
		if( *it == theVoice )
		{
			return;
		}
	}

	m_currentVoices.addElement( theVoice );
	m_voiceSelect->addEntry( theVoice );
}

void VoiceArray::addVoiceGroup( const STRING &theVoiceGroup )
{
	for( 
		ArrayOfStrings::const_iterator	it = m_voiceGroups.cbegin(), endIT = m_voiceGroups.cend();
		it != endIT;
		++it
	)
	{
		if( *it == theVoiceGroup )
		{
			return;
		}
	}

	m_voiceGroups.addElement( theVoiceGroup );
	m_groupSelect->addEntry( theVoiceGroup );
}

void VoiceArray::loadVoices( const STRING &voiceFile )
{
	const char	  	*cp;
	ArrayOfStrings	fieldDef;
	FieldSet		record;
	VoiceEntry	  	theVoice;
	STRING		  	path = appObject->getPathName( voiceFile );

	clear();
	m_voiceGroups.clear();
	m_currentVoices.clear();
	m_groupSelect->clearEntries();
	m_voiceSelect->clearEntries();

	std::ifstream fp( path );
	if( fp.good() )
	{
		readCSVLine( fp, &fieldDef );

		while( !fp.eof() )
		{
			readFIELD_SET( fp, fieldDef, &record );
			if( fp.eof() )
/*v*/			break;

			theVoice.group = (STRING)record["VOICE_GROUP"];
			theVoice.voice = (STRING)record["VOICE"];
			if( !theVoice.group.isEmpty() && !theVoice.voice.isEmpty() )
			{
				STRING program = record["PROGRAM"];
				cp = static_cast<const char *>(program);
				theVoice.voiceCodes.program = cp && *cp
					? (unsigned char)atoi( cp )
					: (unsigned char)255
				;
				STRING bank_msb = record["BANK_MSB"];
				cp = static_cast<const char *>(bank_msb);
				theVoice.voiceCodes.bankMSB = cp && *cp
					? (unsigned char)atoi( cp )
					: (unsigned char)255
				;
				STRING bank_lsb = record["BANK_LSB"];
				cp = static_cast<const char *>(bank_lsb);
				theVoice.voiceCodes.bankLSB = cp && *cp
					? (unsigned char)atoi( cp )
					: (unsigned char)255
				;

				addElement( theVoice );

				addVoiceGroup( theVoice.group );
			}
		}
	}

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

const VoiceEntry &VoiceArray::getSelectedVoice( void ) const
{
	size_t		voiceIdx;
	size_t		selectedGroup = m_groupSelect->getSelection();
	size_t		selectedVoice = m_voiceSelect->getSelection();
	STRING		currentGroup = m_voiceGroups[selectedGroup];
	STRING		currentVoice = m_currentVoices[selectedVoice];

	for( voiceIdx=0; voiceIdx<size(); voiceIdx++ )
	{
		if( (*this)[voiceIdx].group == currentGroup && (*this)[voiceIdx].voice == currentVoice )
			break;
	}

	return (*this)[voiceIdx];
}

void VoiceArray::handleGroupSelection( void )
{
	size_t	selectedGroup = m_groupSelect->getSelection();
	STRING	currentGroup = m_voiceGroups[selectedGroup];

	m_voiceSelect->clearEntries();
	m_currentVoices.clear();

	for(
		const_iterator it = cbegin(), endIT = cend();
		it != endIT;
		++it 
	)
	{
		if( it->group == currentGroup )
		{
			addVoice( it->voice );
		}
	}

	m_voiceSelect->selectEntry( 0 );
}


void VoiceArray::selectVoice( const STRING &currentGroup, const STRING &currentVoice )
{
	if( currentGroup[0U] && currentVoice[0U] )
	{
		for( 
			std::size_t voiceGroupIdx=0; 
			voiceGroupIdx<m_voiceGroups.size(); 
			++voiceGroupIdx
		)
		{
			if( m_voiceGroups[voiceGroupIdx] == currentGroup )
			{
				m_groupSelect->selectEntry( int(voiceGroupIdx) );
				handleGroupSelection();
				break;
			}
		}

		for( 
			std::size_t voiceIdx=0; 
			voiceIdx<m_currentVoices.size(); 
			voiceIdx++ 
		)
		{
			if( m_currentVoices[voiceIdx] == currentVoice )
			{
				m_voiceSelect->selectEntry( int(voiceIdx) );
				break;
			}
		}
	}
	else
	{
		m_groupSelect->selectEntry( 0 );
		handleGroupSelection();
	}
}

void VoiceArray::selectVoice( const VoiceCodes &voiceCodes )
{
	for( 
		const_iterator it = cbegin(), endIT = cend();
		it != endIT;
		++it
	)
	{
		const VoiceEntry &voiceEntry = *it;
		if( voiceEntry.voiceCodes.program == voiceCodes.program
		&&	voiceEntry.voiceCodes.bankMSB == voiceCodes.bankMSB
		&&	voiceEntry.voiceCodes.bankLSB == voiceCodes.bankLSB )
		{
			selectVoice( voiceEntry.group, voiceEntry.voice );
			break;
		}
	}
}

std::size_t VoiceArray::findVoice( const VoiceCodes &voiceCodes )
{
	for( std::size_t voiceIdx=0; voiceIdx<size(); voiceIdx++ )
	{
		const VoiceEntry &voiceEntry = (*this)[voiceIdx];
		if( voiceEntry.voiceCodes.program == voiceCodes.program
		&&	voiceEntry.voiceCodes.bankMSB == voiceCodes.bankMSB
		&&	voiceEntry.voiceCodes.bankLSB == voiceCodes.bankLSB )
		{
/***/		return voiceIdx;
		}
	}

	return std::size_t(-1);
}

// --------------------------------------------------------------------- //
// ----- entry points -------------------------------------------------- //
// --------------------------------------------------------------------- //


