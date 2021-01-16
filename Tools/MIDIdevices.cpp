/*
		Project:		MIDI Tools
		Module:			midiDevice.cpp
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
// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include "MidiApplication.h"
#include "MidiDevices.h"

#include "midi_rc.h"

// --------------------------------------------------------------------- //
// ----- imported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

#undef errno
int errno = 0;

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

MIDIplayer		playerHandles;
MIDIrecorder	recorderHandles;

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class static data --------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- prototypes ---------------------------------------------------- //
// --------------------------------------------------------------------- //

void CALLBACK MIDIinProc( HMIDIIN, UINT, DWORD, DWORD, DWORD );

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

#define LABEL_WIDTH		170
#define CONTROL_WIDTH	280
#define BUTTON_WIDTH	50
#define CONTROL_MARGIN	4

ProcessStatus MIDIdeviceDialog::handleCreate( void )
{
	Label			*newLabel;
	PushButton		*newTestButton;

	dlgFont.setFontSize( 8 );
	dlgFont.setFontName( "MS Sans Serif" );

	size_t	numPlayerDev = playerHandles.size();
	size_t	numRecorderDev = recorderHandles.size();
	for( unsigned i=0; i<numPlayerDev; i++ )
	{
		DeviceEditor			&newEditor = editor.createElement();
		const MIDIplayerHandle	&playerHandle = playerHandles[i];

		newLabel = new Label( this );

		newLabel->create( theDeviceTable, 0, 0, LABEL_WIDTH, 20 );
		newLabel->setLayoutData( (new LayoutData())->setX( 0 )->setY( i )->setPadding( CONTROL_MARGIN ) );
		newLabel->setText( playerHandle.getName() );
		newLabel->setFont( dlgFont );

		// instrument
		newEditor.instrument = new EditControl( this );
		newEditor.instrument->hideSelection();
		newEditor.instrument->create( theDeviceTable, 0, 0, CONTROL_WIDTH, 20 );
		newEditor.instrument->setLayoutData( (new LayoutData())->setX( 1 )->setY( i )->setPadding( CONTROL_MARGIN )->setGrowWidth( 1 ) );
		newEditor.instrument->setFont( dlgFont );
		newEditor.instrument->setText( playerHandle.getInstrument() );

		// voices
		newEditor.voices = new EditControl( this );
		newEditor.voices->hideSelection();
		newEditor.voices->create( theDeviceTable, 0, 0, CONTROL_WIDTH, 20 );
		newEditor.voices->setLayoutData( (new LayoutData())->setX( 2 )->setY( i )->setPadding( CONTROL_MARGIN )->setGrowWidth( 1 ) );
		newEditor.voices->setFont( dlgFont );
		newEditor.voices->setText( playerHandle.getVoicesCSV() );

		// drums
		newEditor.drums = new EditControl( this );
		newEditor.drums->hideSelection();
		newEditor.drums->create( theDeviceTable, 0, 0, CONTROL_WIDTH, 20 );
		newEditor.drums->setLayoutData( (new LayoutData())->setX( 3 )->setY( i )->setPadding( CONTROL_MARGIN )->setGrowWidth( 1 ) );
		newEditor.drums->setFont( dlgFont );
		newEditor.drums->setText( playerHandle.getDrumsCSV() );

		// test button
		newTestButton = new PushButton( this );
		newTestButton->create( theDeviceTable, 0, 0, BUTTON_WIDTH, 20 );
		newTestButton->setLayoutData( (new LayoutData())->setX( 4 )->setY( i )->setPadding( CONTROL_MARGIN ) );
		newTestButton->setText( "Test" );
		newTestButton->setFont( dlgFont );
		newTestButton->setId( WM_USER+i );

	}
	for( unsigned i=int(numPlayerDev); i<numPlayerDev+numRecorderDev; i++ )
	{
		DeviceEditor				&newEditor = editor.createElement();
		const MIDIrecorderHandle	&recorderHandle = recorderHandles[i-numPlayerDev];

		recorderHandles.openInMidi( i-numPlayerDev, this );

		newLabel = new Label( this );

		newLabel->create( theDeviceTable, 0, 0, LABEL_WIDTH, 20 );
		newLabel->setLayoutData( (new LayoutData())->setX( 0 )->setY( i )->setPadding( CONTROL_MARGIN ) );
		newLabel->setText( recorderHandle.getName() );
		newLabel->setFont( dlgFont );

		// instrument
		newEditor.instrument = new EditControl( this );
		newEditor.instrument->hideSelection();
		newEditor.instrument->create( theDeviceTable, 0, 0, CONTROL_WIDTH, 20 );
		newEditor.instrument->setLayoutData( (new LayoutData())->setX( 1 )->setY( i )->setPadding( CONTROL_MARGIN )->setGrowWidth( 1 )->setWidth( 3 ) );
		newEditor.instrument->setFont( dlgFont );
		newEditor.instrument->setText( recorderHandle.getInstrument() );
	}

	theDeviceTable->doLayout();
	return psDO_DEFAULT;
}

ProcessStatus MIDIdeviceDialog::handleDestroy( void )
{
	recorderHandles.stopInMidi( this );
	return MIDIdeviceDialog_form::handleDestroy();
}

ProcessStatus MIDIdeviceDialog::handleButtonClick( int control )
{
	if( control >= WM_USER )
	{
		if( playerHandles.openOutMidi( control-WM_USER ) )
		{
			MIDIevent	testEvent;
			testEvent.setNoteOn( 0, MIDI_NOTE_A2, 127 );
			playerHandles.playMidiEvent( control-WM_USER, testEvent );
			Sleep( 1000 );
			testEvent.setNoteOn( 0, MIDI_NOTE_A2, 0 );
			playerHandles.playMidiEvent( control-WM_USER, testEvent );
			testEvent.setNoteOff( 0, MIDI_NOTE_A2, 0 );
			playerHandles.playMidiEvent( control-WM_USER, testEvent );
			playerHandles.stopOutMidi( control-WM_USER );
		}
		else
		{
			messageBox( winlibGUI::OPEN_OUT_FAILED_id, "MIDI devices" );
		}

		return psPROCESSED;
	}
	else
	{
		return MIDIdeviceDialog_form::handleButtonClick( control );
	}
}

ProcessStatus MIDIdeviceDialog::handleOk( void )
{
	size_t	numPlayerDev = playerHandles.size();
	for( size_t i=0; i<numPlayerDev; i++ )
	{
		const DeviceEditor		&newEditor = editor[i];
		MIDIplayerHandle		&playerHandle = playerHandles[i];

		midiApp.WriteProfile( true, playerHandle.getName(), "playInstrument", newEditor.instrument->getText() );
		playerHandle.setInstrument( newEditor.instrument->getText() );
		midiApp.WriteProfile( true, playerHandle.getName(), "voices", newEditor.voices->getText() );
		playerHandle.setVoicesCSV( newEditor.voices->getText() );
		midiApp.WriteProfile( true, playerHandle.getName(), "drums", newEditor.drums->getText() );
		playerHandle.setDrumsCSV( newEditor.drums->getText() );
	}

	size_t	numRecorderDev = recorderHandles.size();
	for( size_t i=0; i<numRecorderDev; i++ )
	{
		const DeviceEditor	&newEditor = editor[numPlayerDev+i];
		MIDIrecorderHandle	&recorderHandle = recorderHandles[i];

		midiApp.WriteProfile( true, recorderHandle.getName(), "recInstrument", newEditor.instrument->getText() );
		recorderHandle.setInstrument( newEditor.instrument->getText() );
	}

	return MIDIdeviceDialog_form::handleOk();
}

ProcessStatus MIDIdeviceDialog::handleMessage( UINT message, WPARAM wParam, LPARAM lParam )
{
	size_t	midiInDev = message-WM_MIDI_INPUT;
	size_t	numPlayerDev = playerHandles.size();
	size_t	numRecorderDev = recorderHandles.size();

	/*
		Check for short messages
	*/
	if( midiInDev < numRecorderDev )
	{
		MIDIevent	newMsg( 
			static_cast<unsigned long>(lParam), 
			static_cast<unsigned long>(wParam) 
		);
		if( newMsg.getMessage() == MIDI_NOTE_ON )
		{
			DeviceEditor	&newEditor = editor[midiInDev+numPlayerDev];
			newEditor.instrument->focus();
			newEditor.instrument->selectAll();
		}

/***/	return psPROCESSED;
	}

	return psDO_DEFAULT;
}

// --------------------------------------------------------------------- //
// ----- class publics ------------------------------------------------- //
// --------------------------------------------------------------------- //

void MIDIplayerHandle::playMidiEvent( const MIDIevent &msg )
{
	size_t			count = 1;
	unsigned char	message = msg.getMessage();
	unsigned char	channel = msg.getChannel();
	unsigned char	note = msg.getData1();
	unsigned char	volume = msg.getData2();
	if( message == MIDI_NOTE_ON && volume )
		playCount[channel][note]++;
	else if( message == MIDI_NOTE_OFF || message == MIDI_NOTE_ON )
	{
		count = playCount[channel][note];
		playCount[channel][note] = 0;
	}
	else if( message == MIDI_CONTROLLER )
	{
		unsigned char	controller = note;
		unsigned char	value = volume;

		ChannelSettings	&cs = channelSettings[channel];
		if( controller == MIDI_PAN_POSITION || controller == MIDI_BALANCE_POSITION)
			cs.stereoPos = value - MIDI_CENTER_PAN;
		else if( controller == MIDI_VOLUME )
			cs.volume = value;
		else if( controller == MIDI_EXPRESSION )
			cs.expression = value;
	}

	for( size_t i=0; i<count; i++ )
		playMidiEvent( msg.getMessageCode() );
}

void MIDIplayerHandle::setStereoPosition( unsigned char channel, char stereoPos )
{
	ChannelSettings &cs = channelSettings[channel];

	if( cs.stereoPos != stereoPos )
	{
		MIDIevent	newEvent;

		newEvent.setPanPosition( channel, stereoPos );
		playMidiEvent( newEvent.getMessageCode() );

		newEvent.setBalancePosition( channel, stereoPos );
		playMidiEvent( newEvent.getMessageCode() );

		cs.stereoPos = stereoPos;
	}
}

// --------------------------------------------------------------------- //
// ----- entry points -------------------------------------------------- //
// --------------------------------------------------------------------- //

void CALLBACK MIDIinProc( HMIDIIN /* midiHandle */, UINT msg, DWORD idx, DWORD midiMsg, DWORD timeCode )
{
	doEnterFunction("MIDIinProc");

	if( msg == MIM_DATA )
	{
		const MIDIrecorderHandle &recorderHandle = recorderHandles[idx];
		recorderHandle.getOwner()->postMessage(
			WM_MIDI_INPUT+idx, midiMsg, timeCode
		);
	}
	else if( msg == MIM_LONGDATA )
	{
		midiMsg = MIDI_SYS_EX_START;

		const MIDIrecorderHandle &recorderHandle = recorderHandles[idx];
		recorderHandle.getOwner()->postMessage(
			WM_MIDI_INPUT+idx+UINT(recorderHandles.size()), midiMsg, timeCode
		);
	}
}


#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif

