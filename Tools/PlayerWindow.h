/*
		Project:		MIDI Tools
		Module:			PlayerWindow.h
		Description:	Base for all windows playing MIDI
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
	size_t				m_defaultPlayer;
	winlib::PushButton	*m_MIDIplayButton;
	gak::STRING			m_playLabel, m_stopLabel;

	protected:
	MIDIdata		m_midiData;
	gak::STRING		m_lastMidiFile;

	protected:
	void saveMidi2CSV( const char *filename );
	bool loadMidi( const winlib::BasicWindow *parent, winlib::EditControl *bpmEdit, int filter, const char *cmdLine=nullptr );

	PlayerWindow()
	{
		m_defaultPlayer = 0;
	}

	public:
	size_t getDefaultPlayer() const
	{
		return m_defaultPlayer;
	}
	bool isOutOpen() const
	{
		return playerHandles.isOutOpen( m_defaultPlayer );
	}
	void changeDefaultPlayer()
	{
		m_defaultPlayer = m_MIDIplaySelect->getSelection();
	}
	void setDefaultPlayer( size_t midiDev )
	{
		if( midiDev != playerHandles.no_index )
		{
			m_MIDIplaySelect->selectEntry( int(midiDev) );
		}
		m_defaultPlayer = midiDev;
	}
	bool openOutMidi()
	{
		m_defaultPlayer = m_MIDIplaySelect->getSelection();
		return playerHandles.openOutMidi( m_defaultPlayer );
	}
	void stopOutMidi()
	{
		playerHandles.stopOutMidi( m_defaultPlayer );
	}

	void playMidiEvent( const MIDIevent &msg, bool drummerMode=false )
	{
		playerHandles.playMidiEvent( m_defaultPlayer, msg, drummerMode );
	}
	void setStereoPosition( unsigned char channel, char stereoPosition, bool drummerMode=false )
	{
		playerHandles.setStereoPosition( m_defaultPlayer, channel, stereoPosition, drummerMode );
	}

	void showPlayLabel()
	{
		m_MIDIplayButton->setText( m_playLabel );
	}
	void showStopLabel()
	{
		m_MIDIplayButton->setText( m_stopLabel );
	}
	const gak::STRING &getStopLabel() const
	{
		return m_stopLabel;
	}
	void enableMidiPlaySelect()
	{
		m_MIDIplaySelect->enable();
		m_MIDIplayButton->enable();
	}
	void disableMidiPlaySelect()
	{
		m_MIDIplaySelect->disable();
		m_MIDIplayButton->disable();
	}
	void initMidiPlaySelect( winlib::ComboBox *playSelect, winlib::PushButton *playButton );
 
	gak::STRING getSelectedMidiPlayer() const
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
