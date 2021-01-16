/*
		Project:		MIDI Tools
		Module:			midiDevice.h
		Description:	The management for the MIDI devices available
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

#ifndef MIDI_DEVICES_H
#define MIDI_DEVICES_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <gak/thread.h>

#include <winlib/dialogwi.h>
#include <winlib/controlw.h>

#include "midifile.h"
#include "MidiApplication.h"
#include "../Synth/synthesizer.h"
#include "audio.h"

#include "midi.gui.h"

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

// --------------------------------------------------------------------- //
// ----- constants ----------------------------------------------------- //
// --------------------------------------------------------------------- //

#define WM_MIDI_INPUT	(WM_USER)

// --------------------------------------------------------------------- //
// ----- macros -------------------------------------------------------- //
// --------------------------------------------------------------------- //

#if 0
#define midiInStart( x ) {}
#define midiInStop( x ) {}
#define midiInReset( x ) {}
#define midiInClose( x ) {}
#define midiInOpen( hndl, idx, d1, d2, d3 ) ((*hndl = (HMIDIIN)((idx)+1)), MMSYSERR_NOERROR)
#define midiInGetNumDevs() (4)
#define midiInGetDevCaps( i, midiRecDev, dummy ) ( sprintf((midiRecDev)->szPname, "Midi In %d", i ) )

#define midiOutOpen( hndl, idx, d1, d2, d3 ) ((*hndl = (HMIDIOUT)((idx)+1)), MMSYSERR_NOERROR)
#define midiOutShortMsg( midiOutHandle, msgCode ) {}
#define midiOutClose( x ) {}
#define midiOutGetNumDevs() (4)
#define midiOutGetDevCaps( i, midiRecDev, dummy ) ( sprintf((midiRecDev)->szPname, "Midi Out %d", i ) )

#define midiInPrepareHeader( handle, buffer, size )
#define midiInUnPrepareHeader( handle, buffer, size )
#define midiInAddBuffer( handle, buffer, size )
#endif

// --------------------------------------------------------------------- //
// ----- type definitions ---------------------------------------------- //
// --------------------------------------------------------------------- //

struct ChannelSettings : public ChannelCodes
{
	bool		active;
	gak::STRING	group, voice;

	ChannelSettings()
	{
		active = false;
	}
};

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

void CALLBACK MIDIinProc( HMIDIIN, UINT, DWORD, DWORD, DWORD );

class MIDIdevice
{
	friend class MIDIrecorder;

	protected:
	gak::STRING		name, instrument;

	public:
	MIDIdevice()
	{
	}
	gak::STRING getName( void ) const
	{
		return name;
	}
	gak::STRING getInstrument( void ) const
	{
		return instrument;
	}
	void setInstrument( const gak::STRING &newInstrument )
	{
		instrument = newInstrument;
	}
};

class MIDIrecorderHandle : public MIDIdevice
{
	friend class MIDIrecorder;

	HMIDIIN						m_midiInHandle;
	MIDIHDR						m_sysExHeader;
//	char						m_sysExBuffer[10241024L];
	const winlib::BasicWindow	*m_owner;

	private:
	void createBuffer( void )
	{
		memset( &m_sysExHeader, 0, sizeof( m_sysExHeader ) );
		m_sysExHeader.lpData = static_cast<LPSTR>( malloc( 10241024L ) );
//		m_sysExHeader.lpData = (LPSTR)sysExBuffer;
		m_sysExHeader.dwBufferLength = 10241024L;
	}
	void freeBuffer( void )
	{
		free( m_sysExHeader.lpData );
		memset( &m_sysExHeader, 0, sizeof( m_sysExHeader ) );
	}

	protected:
	void open( std::size_t idx, const winlib::BasicWindow *newOwner )
	{
		doEnterFunction("MIDIrecorderHandle::open");

		if( !m_owner )
		{
			MMRESULT resultCode = midiInOpen( 
				&m_midiInHandle, UINT(idx), DWORD(MIDIinProc), idx, CALLBACK_FUNCTION 
			);
			if( m_midiInHandle && MMSYSERR_NOERROR == resultCode )
			{
				createBuffer();
				midiInPrepareHeader( m_midiInHandle, &m_sysExHeader, sizeof(m_sysExHeader) );
				midiInAddBuffer( m_midiInHandle, &m_sysExHeader, sizeof(m_sysExHeader) );

				midiInStart( m_midiInHandle );
				m_owner = newOwner;
			}
			else
			{
				m_midiInHandle = 0;
			}
		}
	}
	void prepareSysExHeader( void )
	{
		m_sysExHeader.dwFlags = 0;
		midiInPrepareHeader( m_midiInHandle, &m_sysExHeader, sizeof( m_sysExHeader ) );
	}

	const unsigned char *getSysExBuffer( std::size_t *len )
	{
		midiInUnprepareHeader( m_midiInHandle, &m_sysExHeader, sizeof( m_sysExHeader ) );
		*len = m_sysExHeader.dwBytesRecorded;
		return (const unsigned char *)m_sysExHeader.lpData;
	}

	void close( const winlib::BasicWindow *owner )
	{
		doEnterFunction("MIDIrecorderHandle::close");

		if( m_midiInHandle && m_owner == owner )
		{
			// stop reset unprepare close 8-(
			// stop reset close free 8-(

			midiInStop( m_midiInHandle );
			midiInReset( m_midiInHandle );

			if( m_sysExHeader.lpData )
			{
				midiInUnprepareHeader( m_midiInHandle, &m_sysExHeader, sizeof( m_sysExHeader ) );
				freeBuffer();
			}

			if( midiInClose( m_midiInHandle ) == MMSYSERR_NOERROR )
			{
				m_midiInHandle = 0;
				m_owner = NULL;
			}
		}
	}



	public:
	MIDIrecorderHandle( HMIDIIN handle=0 )
	{
		m_owner = NULL;
		m_midiInHandle = handle;
		memset( &m_sysExHeader, 0, sizeof( m_sysExHeader ) );
	}
	~MIDIrecorderHandle()
	{
		if( m_sysExHeader.lpData )
		{
			free( m_sysExHeader.lpData );
		}
	}
	int operator == ( const MIDIrecorderHandle &other ) const
	{
		return this->m_midiInHandle == other.m_midiInHandle;
	}
	const winlib::BasicWindow *getOwner( void ) const
	{
		return m_owner;
	}
};

class MIDIrecorder : public gak::Array<MIDIrecorderHandle>
{
	public:
	static const std::size_t no_index = gak::Array<MIDIrecorderHandle>::no_index;
	void initDevices( void )
	{
		MIDIINCAPS		midiRecDev;
		std::size_t			midiCount = midiInGetNumDevs();

		for( std::size_t i=0; i<midiCount; i++ )
		{
			MIDIrecorderHandle &newHandle = createElement();

			midiInGetDevCaps( i, &midiRecDev, sizeof( midiRecDev ) );
			newHandle.name = midiRecDev.szPname;
			newHandle.instrument = midiApp.GetProfile( newHandle.name, "recInstrument", newHandle.name );
		}
	}
	std::size_t getNumOpenDevices( const winlib::BasicWindow *owner )
	{
		std::size_t	openDevices = 0;

		for( 
			const_iterator it = cbegin(), endIT = cend(); it != endIT; ++it
		)
		{
			const MIDIrecorderHandle &handle = *it;

			if( handle.m_midiInHandle && handle.m_owner == owner )
			{
				openDevices++;
			}
		}

		return openDevices;
	}
	void openInMidi( std::size_t idx, const winlib::BasicWindow *owner )
	{
		doEnterFunction("MIDIrecorder::open");
		MIDIrecorderHandle &handle = (*this)[idx];
		handle.open( idx, owner );
	}
	void stopInMidi( std::size_t idx, const winlib::BasicWindow *owner )
	{
		doEnterFunction("MIDIrecorder::stopInMidi(std::size_t,const BasicWindow*)");
		MIDIrecorderHandle &handle = (*this)[idx];
		handle.close( owner );
	}
	void stopInMidi( const winlib::BasicWindow *owner )
	{
		doEnterFunction("MIDIrecorder::stopInMidi(const BasicWindow *)");

		if( getNumOpenDevices( owner ) )
		{
			for( iterator it=begin(), endIT = end(); it != endIT; ++it )
			{
				MIDIrecorderHandle &handle = *it;
				handle.close( owner );
			}
		}
	}

	const unsigned char *getSysExBuffer( std::size_t idx, std::size_t *len )
	{
		MIDIrecorderHandle &handle = (*this)[idx];
		return handle.getSysExBuffer( len );
	}
	void prepareSysExHeader( std::size_t idx )
	{
		MIDIrecorderHandle &handle = (*this)[idx];
		handle.prepareSysExHeader();
	}
	std::size_t findElement( HMIDIIN midiHandle, void *owner )
	{
		std::size_t idx = Array<MIDIrecorderHandle>::findElement( midiHandle );
		if( idx != no_index && (*this)[idx].m_owner != owner )
		{
			idx = no_index;
		}

		return idx;
	}
	std::size_t getRecorder( const gak::STRING &recorderName )
	{
		std::size_t i;
		for( i=size() -1; i != no_index; i-- )
		{
			const MIDIrecorderHandle &theHandle = (*this)[i];
			if( recorderName == theHandle.name || recorderName == theHandle.instrument )
				break;
		}

		return i;
	}
};

class MIDIplayerHandle : public MIDIdevice
{
	friend class MIDIplayer;

	ChannelSettings		channelSettings[16];
	HMIDIOUT			midiOutHandle;
	gak::STRING			voicesCSV, drumsCSV;
	bool				generalMIDI;
	std::size_t			openCount;
	std::size_t			playCount[16][128];

#ifndef __BORLANDC__
	bool				internalSynthesizer;
#endif

	void playMidiEvent( unsigned long msgCode )
	{
		if( midiOutHandle )
		{
			if( midiOutHandle != HMIDIOUT( -1 ) )
			{
				midiOutShortMsg( midiOutHandle, msgCode );
			}
			else
			{
				synthesizerMsg( msgCode );
			}
		}
	}

	public:
	MIDIplayerHandle()
	{
		midiOutHandle = 0;
		for( std::size_t i=0; i<16; i++ )
		{
			channelSettings[i].volume = 127;
			channelSettings[i].expression = 127;
			channelSettings[i].stereoPos = 0;
			for( std::size_t j=0; j<5; j++ )
			{
				channelSettings[i].sounds[j] = 64;
				channelSettings[i].effects[j] = 0;
			}
		}
		voicesCSV = "gm_voices.csv";
		drumsCSV = "gm_drums.csv";
		generalMIDI = true;
		openCount = 0;
#ifndef __BORLANDC__
		internalSynthesizer = false;
#endif
		memset( playCount, 0, sizeof( playCount ) );
	}

	gak::STRING getVoicesCSV( void ) const
	{
		return voicesCSV;
	}
	gak::STRING getDrumsCSV( void ) const
	{
		return drumsCSV;
	}
	bool isGeneralMIDI( void ) const
	{
		return generalMIDI;
	}
	void setGeneralMIDI( bool generalMIDI )
	{
		this->generalMIDI = generalMIDI;
	}

	void setVoicesCSV( const gak::STRING &newVoices )
	{
		voicesCSV = newVoices;
	}
	void setDrumsCSV( const gak::STRING &newDrums )
	{
		drumsCSV = newDrums;
	}

	bool isOutOpen() const
	{
		return midiOutHandle ? true : false;
	}

	void sendSysExData( const unsigned char *data, std::size_t len )
	{
		if( midiOutHandle && midiOutHandle != HMIDIOUT( -1 ) )
		{
			MIDIHDR		sysExHeader;
			memset( &sysExHeader, 0, sizeof( sysExHeader ) );
			sysExHeader.dwBufferLength = DWORD(len);
			sysExHeader.lpData = (LPSTR)data;
			midiOutPrepareHeader( midiOutHandle, &sysExHeader, sizeof( sysExHeader ) );
			midiOutLongMsg( midiOutHandle, &sysExHeader, sizeof( sysExHeader ) );
			midiOutUnprepareHeader( midiOutHandle, &sysExHeader, sizeof( sysExHeader ) );
		}
	}

	void playMidiEvent( const MIDIevent &msg );
	void setStereoPosition( unsigned char channel, char stereoPos );
	ChannelSettings *getChannelSettings( void )
	{
		return channelSettings;
	}
};

class MIDIplayer : public gak::Array<MIDIplayerHandle>
{
	std::size_t		midiCount;

	public:
	static const std::size_t no_index = gak::Array<MIDIplayerHandle>::no_index;
	void initDevices( void )
	{
		clear();
		MIDIOUTCAPS		midiPlayDev;
		unsigned int	midiCount = midiOutGetNumDevs()+1;

		for( std::size_t i=0; i<midiCount; i++ )
		{
			MIDIplayerHandle	&newHandle = createElement();

			if( i<midiCount-1 )
			{
				midiOutGetDevCaps( i, &midiPlayDev, sizeof( midiPlayDev ) );
				newHandle.name = midiPlayDev.szPname;
			}
			else
			{
				newHandle.name = "MGsynthesizer";
				newHandle.internalSynthesizer = true;
			}

			newHandle.instrument = midiApp.GetProfile( newHandle.name, "playInstrument", newHandle.name );
			newHandle.voicesCSV = midiApp.GetProfile( newHandle.name, "voices", "gm_voices.csv" );
			newHandle.drumsCSV = midiApp.GetProfile( newHandle.name, "drums", "gm_drums.csv" );
		}

#if !defined( NDEBUG ) && 0
		for( std::size_t i=midiCount; i<midiCount+2; i++ )
		{
			MIDIplayerHandle	&newHandle = (*this)[i];
			newHandle.name = "DebugDev #";
			i >> newHandle.name;

			newHandle.instrument = midiApp.GetProfile( newHandle.name, "playInstrument", newHandle.name );
			newHandle.voices = midiApp.GetProfile( newHandle.name, "voices", "gm_voices.csv" );
			newHandle.drums = midiApp.GetProfile( newHandle.name, "drums", "gm_drums.csv" );
		}
#endif
	}


	void playMidiEvent( std::size_t midiDev, const MIDIevent &msg, bool drummerMode=false )
	{
		if( midiDev < size() )
		{
			MIDIplayerHandle &dev = (*this)[midiDev];

			unsigned char status = msg.getStatusByte();
			if( status == MIDI_SYS_EX_START )
			{
				std::size_t			len;
				const unsigned char	*data;

				data = msg.getSysExData( &len, true );
				if( len && data )
					dev.sendSysExData( data, len );

			}
			else
			{
				if( drummerMode && dev.isGeneralMIDI() && msg.getChannel() != 9 )
				{
					MIDIevent	newEvent = msg;
					newEvent.setChannel( 9 );
					dev.playMidiEvent( newEvent );
				}
				else
					dev.playMidiEvent( msg );
			}
		}
	}
	void setStereoPosition(
		std::size_t midiDev,
		unsigned char channel, char stereoPosition,
		bool drummerMode=false )
	{
		if( midiDev < size() )
		{
			MIDIplayerHandle &dev = (*this)[midiDev];
			if( drummerMode && dev.isGeneralMIDI() )
				channel = 9;
			dev.setStereoPosition( channel, stereoPosition );
		}
	}
	bool isOutOpen( std::size_t midiDev )
	{
		if( midiDev < size() )
			return (*this)[midiDev].isOutOpen();
		else
			return false;
	}
	bool openOutMidi( std::size_t midiDev )
	{
		doEnterFunction("MIDIplayer::openOutMidi");
		bool ok = false;

		if( midiDev < size() )
		{
			MIDIplayerHandle &dev = (*this)[midiDev];

			if( !dev.midiOutHandle )
			{
				if( dev.internalSynthesizer )
				{
					if( openSynthesizer() )
					{
						dev.midiOutHandle = HMIDIOUT(-1);
					}
				}
				else
				{
					midiOutOpen( &dev.midiOutHandle, UINT(midiDev), 0, 0, 0 ); // no callback
				}
				if( dev.midiOutHandle )
				{
					ok = true;
					dev.openCount = 1;
				}
			}
			else
			{
				ok = true;
				dev.openCount++;
			}

			if( ok && dev.openCount == 1 )
			{
				MIDIevent	theEvent;

				stopSoundMidi( midiDev );

				for( unsigned char channel=0; channel<=15; channel++ )
				{
					theEvent.setLocalOn( channel );
					dev.playMidiEvent( theEvent );
					theEvent.setLocalOff( channel );
					dev.playMidiEvent( theEvent );
				}
			}
		}
		return ok;
	}
	void stopSoundMidi( std::size_t midiDev )
	{
		doEnterFunction("MIDIplayer::stopSoundMidi");
		if( midiDev < size() )
		{
			MIDIplayerHandle &dev = (*this)[midiDev];

			if( dev.midiOutHandle )
			{
				MIDIevent	theEvent;

				theEvent.setReset();
				dev.playMidiEvent( theEvent );
				for( unsigned char channel=0; channel<=15; channel++ )
				{
					theEvent.setNotesStop( channel );
					dev.playMidiEvent( theEvent );
					theEvent.setSoundStop( channel );
					dev.playMidiEvent( theEvent );
				}
			}
		}
	}
	void stopOutMidi( std::size_t midiDev )
	{
		doEnterFunction("MIDIplayer::stopOutMidi");
		if( midiDev < size() )
		{
			MIDIplayerHandle &dev = (*this)[midiDev];

			if( dev.midiOutHandle )
			{
				dev.openCount--;
				if( !dev.openCount )
				{
					stopSoundMidi( midiDev );

					if( dev.midiOutHandle != HMIDIOUT( -1 ) )
						midiOutClose( dev.midiOutHandle );
					else
						closeSynthesizer();

					dev.midiOutHandle = 0;
				}
			}
		}
	}
	void stopAllOutMidi()
	{
		for( std::size_t midiDev = 0; midiDev < size(); midiDev++ )
		{
			stopOutMidi( midiDev );
		}
	}
	void stopAllSoundMidi()
	{
		for( std::size_t midiDev =0; midiDev < size(); midiDev++ )
		{
			stopSoundMidi( midiDev );
		}
	}
	std::size_t getPlayer( const gak::STRING &playerName )
	{
		std::size_t i;
		for( i=size() -1; i != no_index; i-- )
		{
			const MIDIplayerHandle &theHandle = (*this)[i];
			if( playerName == theHandle.name || playerName == theHandle.instrument )
			{
/*v*/			break;
			}
		}

		return i;
	}
};

struct DeviceEditor
{
	winlib::EditControl	*instrument, *voices, *drums;

	DeviceEditor()
	{
		instrument = voices = drums = NULL;
	}
};

class MIDIdeviceDialog : public winlibGUI::MIDIdeviceDialog_form
{
//	PushButton					buttonOK, buttonCancel;
	winlib::Font 				dlgFont;
	gak::Array<DeviceEditor>	editor;

	virtual winlib::ProcessStatus handleCreate( void );
	virtual winlib::ProcessStatus handleDestroy( void );
	virtual winlib::ProcessStatus handleOk( void );
	virtual winlib::ProcessStatus handleButtonClick( int control );
	virtual winlib::ProcessStatus handleMessage( UINT message, WPARAM wParam, LPARAM lParam );

	public:
	MIDIdeviceDialog() : MIDIdeviceDialog_form(NULL), dlgFont( this ) 
	{
	}

	winlib::SuccessCode create( winlib::BasicWindow *parent )
	{
		return MIDIdeviceDialog_form::create( parent );
	}
};

// --------------------------------------------------------------------- //
// ----- exported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

extern MIDIplayer 	playerHandles;
extern MIDIrecorder	recorderHandles;

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
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif

#endif
