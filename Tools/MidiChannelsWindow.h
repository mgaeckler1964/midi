/*
		Project:		MIDI Tools
		Module:			MidiChannelsWindow.h
		Description:	The channel settings (used by recorder window)
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

#ifndef MIDI_CHANNELS_WINDOW_H
#define MIDI_CHANNELS_WINDOW_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <winlib/dialogwi.h>

#include "voices.h"
#include "MidiDevices.h"

#include "midi.gui.h"

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

class MIDIrecorderWindow;

class MIDIchannelsWindow : public winlibGUI::MIDIchannelsWindow_form
{
	MIDIrecorderWindow	*m_theRecorderWindow;
	VoiceArray			m_theVoices;

	size_t				m_midiDev;
	ChannelSettings		*m_channelSettings;
	gak::STRING			m_voiceFile, m_instrument;

	void handleChannelSelection( unsigned char channel );
	void handleVoiceSelection();
	void handleNewStereoPos( char stereoPosition );
	void handleNewVolume( unsigned char volume );
	void handleActive( bool active );
	void handleNewEffect( unsigned char effect, unsigned char effectValue );
	void handleNewSound( unsigned char sound, unsigned char soundValue );
	void handleNewExpression( unsigned char expression );
	void handleDefault();

	virtual winlib::ProcessStatus handleCreate();
	virtual winlib::ProcessStatus handleCommand( int cmd );
	virtual winlib::SuccessCode handleClose();
	virtual winlib::ProcessStatus handleCancel();

	public:
	MIDIchannelsWindow( BasicWindow *owner ) : MIDIchannelsWindow_form( owner )
	{
		watch();
	}
	
	void create( MIDIrecorderWindow	*theRecorderWindow, size_t midiDev )
	{
		MIDIplayerHandle	&playerHandle = playerHandles[midiDev];

		m_theRecorderWindow = theRecorderWindow;
		m_midiDev = midiDev;

		m_channelSettings = playerHandle.getChannelSettings();
		m_voiceFile = playerHandle.getVoicesCSV();
		m_instrument = playerHandle.getInstrument();

		MIDIchannelsWindow_form::create( reinterpret_cast<BasicWindow*>(theRecorderWindow) );
		restoreWindowRect();
	}
	winlib::SuccessCode close()
	{
		saveWindowRect();
		return MIDIchannelsWindow_form::close();
	}


	void refreshWindow()
	{
		handleChannelSelection( 
			static_cast<unsigned char>(channelSelect->getSelection())
		);
	}
	void setChannelSettings( size_t midiDev )
	{
		if( m_midiDev != midiDev )
		{
			MIDIplayerHandle	&playerHandle = playerHandles[midiDev];

			m_midiDev = midiDev;

			m_channelSettings = playerHandle.getChannelSettings();
			m_voiceFile = playerHandle.getVoicesCSV();
			m_instrument = playerHandle.getInstrument();

			m_theVoices.loadVoices( m_voiceFile, groupSelect, voiceSelect );

			deviceLabel->setText( m_instrument );

			refreshWindow();
		}
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
