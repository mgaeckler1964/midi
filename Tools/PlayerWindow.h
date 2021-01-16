/*
		Project:		MIDI Tools
		Module:			PlayerWindow.h
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

#ifndef PLAYER_WINDOW_H
#define PLAYER_WINDOW_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <winlib/dialogwi.h>
#include <winlib/controlw.h>

#include <gak/thread.h>

#include "midifile.h"
#include "midiDevices.h"

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

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

class PlayerWindow
{
	winlib::ComboBox	*m_MIDIplaySelect;
	size_t				defaultPlayer;
	winlib::PushButton	*m_MIDIplayButton;
	gak::STRING			playLabel, stopLabel;

	protected:
	MIDIdata		midiData;
	gak::STRING		lastMidiFile;

	protected:
	void saveMidi2CSV( const char *filename );
	bool loadMidi( const winlib::BasicWindow *parent, winlib::EditControl *bpmEdit, int filter, const char *cmdLine=NULL );

	PlayerWindow()
	{
		defaultPlayer = 0;
	}

	public:
	size_t getDefaultPlayer( void ) const
	{
		return defaultPlayer;
	}
	bool isOutOpen( void ) const
	{
		return playerHandles.isOutOpen( defaultPlayer );
	}
	void changeDefaultPlayer( void )
	{
		defaultPlayer = m_MIDIplaySelect->getSelection();
	}
	void setDefaultPlayer( size_t midiDev )
	{
		if( midiDev != playerHandles.no_index )
		{
			m_MIDIplaySelect->selectEntry( int(midiDev) );
		}
		defaultPlayer = midiDev;
	}
	bool openOutMidi( void )
	{
		defaultPlayer = m_MIDIplaySelect->getSelection();
		return playerHandles.openOutMidi( defaultPlayer );
	}
	void stopOutMidi( void )
	{
		playerHandles.stopOutMidi( defaultPlayer );
	}

	void playMidiEvent( const MIDIevent &msg, bool drummerMode=false )
	{
		playerHandles.playMidiEvent( defaultPlayer, msg, drummerMode );
	}
	void setStereoPosition( unsigned char channel, char stereoPosition, bool drummerMode=false )
	{
		playerHandles.setStereoPosition( defaultPlayer, channel, stereoPosition, drummerMode );
	}

	void showPlayLabel( void )
	{
		m_MIDIplayButton->setText( playLabel );
	}
	void showStopLabel( void )
	{
		m_MIDIplayButton->setText( stopLabel );
	}
	const gak::STRING &getStopLabel( void ) const
	{
		return stopLabel;
	}
	void enableMidiPlaySelect( void )
	{
		m_MIDIplaySelect->enable();
		m_MIDIplayButton->enable();
	}
	void disableMidiPlaySelect( void )
	{
		m_MIDIplaySelect->disable();
		m_MIDIplayButton->disable();
	}
	void initMidiPlaySelect( winlib::ComboBox *playSelect, winlib::PushButton *playButton );
 
	gak::STRING getSelectedMidiPlayer( void ) const
	{
		return m_MIDIplaySelect->getSelectedText();
	}
	void selectMidiPlayer( const gak::STRING &player )
	{
		size_t	midiDev = playerHandles.getPlayer( player );
		if( midiDev != playerHandles.no_index )
		{
			m_MIDIplaySelect->selectEntry( int(midiDev) );
		}
		else
		{
			m_MIDIplaySelect->selectEntry( 0 );
		}
		changeDefaultPlayer();
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
#	pragma option -a.
#	pragma option -p.
#endif

#endif
