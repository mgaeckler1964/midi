/*
		Project:		MIDI Tools
		Module:			PlayerWindow.cpp
		Description:	Base for all windows playing MIDI
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

#include <gak/fmtNumber.h>

#include <winlib/stddlg.h>

#include "PlayerWindow.h"
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

void PlayerWindow::saveMidi2CSV( const char *filename )
{
	doEnterFunction("PlayerWindow::saveMidi2CSV");
	FILE	*fp = fopen( filename, "w" );

	if( fp )
	{
		fprintf( fp,
			"%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\n",
			"Track",
			"Channel",
			"TimeCode",
			"Index",
			"MessageCode",
			"MessageText",
			"Data1",
			"Note",
			"Controller Text",
			"Data2",
			"SysEx"
		);
		for( 
			MIDIdata::const_iterator it = midiData.cbegin(), 
				endIT = midiData.cend();
			it != endIT;
			++it
		)
		{
			const MIDIevent	&theEvent = *it;
			unsigned char	msg = theEvent.getStatusByte();
			if( msg != MIDI_SYS_EX_START )
			{
				fprintf( fp,
					"%d,%d,%lu,%d,%X,\"%s\",%d,\"%s\",\"%s\",%d\n",
					(int)theEvent.getTrack(),
					(int)theEvent.getChannel(),
					theEvent.getTimeCode(),
					(int)theEvent.getIndex(),
					(int)theEvent.getMessage(),
					theEvent.getMessageText(),
					(int)theEvent.getData1(),
					theEvent.getNoteText(),
					theEvent.getControllerText(),
					(int)theEvent.getData2()
				);
			}
			else
			{
				char	tmp[4];
				STRING	sysEx;
				size_t	len;

				const unsigned char *buffer = theEvent.getSysExData( &len, true );

				for( size_t i=0; i<len; i++ )
				{
					sprintf( tmp, "%02X%c", buffer[i], buffer[i] > 32 ? buffer[i] : '.' );
					sysEx += tmp;
					if( !(i%80) )
						sysEx += '\n';
				}

				fprintf( fp,
					"%d,%lu,%X,\"%s\",,,,,,%s\n",
					(int)theEvent.getTrack(),
					theEvent.getTimeCode(),
					(int)theEvent.getMessage(),
					theEvent.getMessageText(),
					(const char *)sysEx
				);
			}
		}
		fclose( fp );
	}
}

// --------------------------------------------------------------------- //
// ----- class protected ----------------------------------------------- //
// --------------------------------------------------------------------- //


bool PlayerWindow::loadMidi( const BasicWindow *parent, EditControl *bpmEdit, int filter, const char *cmdLine )
{
	bool midiLoaded = false;
	doEnterFunction("PlayerWindow::loadMidi");
	OpenFileDialog	dlg;

	dlg.setFilename( lastMidiFile );
	if( dlg.getDirectory().isEmpty() )
		dlg.setPersonalMusic();

	if( cmdLine || dlg.create( parent, winlibGUI::OPEN_MIDI_id, filter, 1 ) )
	{
		lastMidiFile = cmdLine ? cmdLine : dlg.getFilename();
		int errCode = midiData.loadMidiFile( lastMidiFile );
		if( !errCode )
		{
			if( bpmEdit )
			{
				STRING	bpm = formatNumber( midiData.getBPM() );

				bpmEdit->setText( bpm );
			}
			midiLoaded = true;
		}
		else
		{
			parent->messageBox( (unsigned short)errCode );
		}
	}
	return midiLoaded;
}

// --------------------------------------------------------------------- //
// ----- class virtuals ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class publics ------------------------------------------------- //
// --------------------------------------------------------------------- //

void PlayerWindow::initMidiPlaySelect( ComboBox *playSelect, PushButton *playButton )
{
	m_MIDIplaySelect = playSelect;
	m_MIDIplayButton = playButton;
	for( 
		MIDIplayer::const_iterator it = playerHandles.cbegin(), 
			endIT = playerHandles.cend();
		it != endIT;
		++it
	)
	{
		m_MIDIplaySelect->addEntry( it->getInstrument() );
	}

	if( playerHandles.size() )
	{
		m_MIDIplaySelect->selectEntry( 0 );
	}
	else
	{
		disableMidiPlaySelect();
	}

	stopLabel = appObject->loadString( winlibGUI::STOP_LABEL_id );
	playLabel = appObject->loadString( winlibGUI::PLAY_LABEL_id );
}

// --------------------------------------------------------------------- //
// ----- entry points -------------------------------------------------- //
// --------------------------------------------------------------------- //

