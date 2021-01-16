/*
		Project:		MIDI Tools
		Module:			DrumVoices.cpp
		Description:	MIDI voices and notes for drums
						(used by metronom and drum window)
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
#ifdef __BORLANDC__
#include <fstream.h>
#else
#include <fstream>
#endif

#include <winlib/winapp.h>
#include <gak/csv.h>

#include "DrumVoices.h"

// --------------------------------------------------------------------- //
// ----- imported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module switches ----------------------------------------------- //
// --------------------------------------------------------------------- //

using namespace gak;
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

void DrumVoices::loadDrumVoices( const STRING &drumsFile )
{
	STRING	path = appObject->getPathName( drumsFile );

	clear();
	m_generalMIDI = true;
	std::ifstream fp( path );
	if( fp.good() )
	{
		ArrayOfStrings	fieldDef;
		FieldSet		record;
		DrumVoice		theVoice;

		readCSVLine( fp, &fieldDef );

		while( !fp.eof() )
		{
			readFIELD_SET( fp, fieldDef, &record );
			if( fp.eof() )
			{
/*v*/			break;
			}

			theVoice.voice = record["VOICE"];
			if( !theVoice.voice.isEmpty() )
			{
				STRING		program = record["PROGRAM"];
				if( !program.isEmpty() )
				{
					theVoice.voiceCodes.program = program.getValueN<unsigned char>();
					m_generalMIDI = false;
				}
				else
				{
					theVoice.voiceCodes.program = 255;
				}

				STRING bank_msb = record["BANK_MSB"];
				if( !program.isEmpty() )
				{
					theVoice.voiceCodes.bankMSB = bank_msb.getValueN<unsigned char>();
					m_generalMIDI = false;
				}
				else
				{
					theVoice.voiceCodes.bankMSB = 255;
				}

				STRING bank_lsb = record["BANK_LSB"];
				if( !program.isEmpty() )
				{
					theVoice.voiceCodes.bankLSB = bank_lsb.getValueN<unsigned char>();
					m_generalMIDI = false;
				}
				else
				{
					theVoice.voiceCodes.bankLSB = 255;
				}

				theVoice.note = static_cast<unsigned char>( record["NOTE"] );

				addElement( theVoice );
			}
		}
	}
}

size_t DrumVoices::findDrumVoice( const STRING &voice )
{
	size_t	idx;
	for( idx=size()-1; idx != no_index; idx-- )
	{
		const DrumVoice	&drumVoice = (*this)[idx];
		if( drumVoice.voice == voice )
		{
			break;
		}
	}
	return idx;
}

// --------------------------------------------------------------------- //
// ----- entry points -------------------------------------------------- //
// --------------------------------------------------------------------- //

