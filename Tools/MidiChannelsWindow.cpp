/*
		Project:		MIDI Tools
		Module:			MidiChannelsWindow.cpp
		Description:	The channel settings (used by recorder window)
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

#include "MIDIchannelsWindow.h"
#include "MIDIrecorderWindow.h"
#include "controls.h"

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

void MIDIchannelsWindow::handleVoiceSelection( void )
{
	MIDIevent			theEvent;

	const VoiceEntry	&theVoice = theVoices.getSelectedVoice();

	unsigned char minChannel, maxChannel, theChannel;
	unsigned char selChannel = static_cast<unsigned char>(channelSelect->getSelection());

	if( selChannel < 16 )
		minChannel = maxChannel = selChannel;
	else
	{
		minChannel = 0;
		maxChannel = 15;
	}

	for( theChannel=minChannel; theChannel <= maxChannel; theChannel++ )
	{
		if( !theRecorderWindow->isMetronomRunning() )
		{
			theEvent.setBankMSB( theChannel, theVoice.voiceCodes.bankMSB );
			theRecorderWindow->playMidiEvent( theEvent );
			theEvent.setBankLSB( theChannel, theVoice.voiceCodes.bankLSB );
			theRecorderWindow->playMidiEvent( theEvent );
			theEvent.setProgramChange( theChannel, theVoice.voiceCodes.program );
			theRecorderWindow->playMidiEvent( theEvent );
		}

		channelSettings[theChannel].group = theVoice.group;
		channelSettings[theChannel].voice = theVoice.voice;
		channelSettings[theChannel].voiceCodes.program = theVoice.voiceCodes.program;
		channelSettings[theChannel].voiceCodes.bankMSB = theVoice.voiceCodes.bankMSB;
		channelSettings[theChannel].voiceCodes.bankLSB = theVoice.voiceCodes.bankLSB;
	}

	theRecorderWindow->setSettingsChanged();
}

void MIDIchannelsWindow::handleNewStereoPos( char stereoPos )
{
	MIDIevent		theEvent;
	unsigned char	minChannel, maxChannel, theChannel;
	unsigned char selChannel = static_cast<unsigned char>(channelSelect->getSelection());

	if( selChannel < 16 )
		minChannel = maxChannel = selChannel;
	else
	{
		minChannel = 0;
		maxChannel = 15;
	}

	for( theChannel=minChannel; theChannel <= maxChannel; theChannel++ )
	{
		channelSettings[theChannel].stereoPos = stereoPos;

		theEvent.setBalancePosition( theChannel, stereoPos );
		theRecorderWindow->playMidiEvent( theEvent );
		theEvent.setPanPosition( theChannel, stereoPos );
		theRecorderWindow->playMidiEvent( theEvent );
	}

	theRecorderWindow->setSettingsChanged();
}

void MIDIchannelsWindow::handleNewEffect( unsigned char effect, unsigned char effectValue )
{
	MIDIevent		theEvent;
	unsigned char	minChannel, maxChannel, theChannel;
	unsigned char	selChannel = static_cast<unsigned char>(channelSelect->getSelection());

	if( selChannel < 16 )
		minChannel = maxChannel = selChannel;
	else
	{
		minChannel = 0;
		maxChannel = 15;
	}

	for( theChannel=minChannel; theChannel <= maxChannel; theChannel++ )
	{
		channelSettings[theChannel].effects[effect] = effectValue;

		theEvent.setEffectValue( theChannel, effect, effectValue );
		theRecorderWindow->playMidiEvent( theEvent );
	}

	theRecorderWindow->setSettingsChanged();
}

void MIDIchannelsWindow::handleNewSound( unsigned char sound, unsigned char soundValue )
{
	MIDIevent		theEvent;
	unsigned char	minChannel, maxChannel, theChannel;
	unsigned char	selChannel = static_cast<unsigned char>(channelSelect->getSelection());

	if( selChannel < 16 )
		minChannel = maxChannel = selChannel;
	else
	{
		minChannel = 0;
		maxChannel = 15;
	}

	for( theChannel=minChannel; theChannel <= maxChannel; theChannel++ )
	{
		channelSettings[theChannel].sounds[sound] = soundValue;

		theEvent.setSoundValue( theChannel, sound, soundValue );
		theRecorderWindow->playMidiEvent( theEvent );
	}

	theRecorderWindow->setSettingsChanged();
}

void MIDIchannelsWindow::handleNewExpression( unsigned char expressionValue )
{
	MIDIevent		theEvent;
	unsigned char	minChannel, maxChannel, theChannel;
	unsigned char	selChannel = static_cast<unsigned char>(channelSelect->getSelection());

	if( selChannel < 16 )
		minChannel = maxChannel = selChannel;
	else
	{
		minChannel = 0;
		maxChannel = 15;
	}

	for( theChannel=minChannel; theChannel <= maxChannel; theChannel++ )
	{
		channelSettings[theChannel].expression = expressionValue;

		theEvent.setExpression( theChannel, expressionValue );
		theRecorderWindow->playMidiEvent( theEvent );
	}

	theRecorderWindow->setSettingsChanged();
}

void MIDIchannelsWindow::handleNewVolume( unsigned char volume )
{
	MIDIevent		theEvent;
	unsigned char	minChannel, maxChannel, theChannel;
	unsigned char	selChannel = static_cast<unsigned char>(channelSelect->getSelection());

	if( selChannel < 16 )
		minChannel = maxChannel = selChannel;
	else
	{
		minChannel = 0;
		maxChannel = 15;
	}

	for( theChannel=minChannel; theChannel <= maxChannel; theChannel++ )
	{
		channelSettings[theChannel].volume = volume;
		theEvent.setVolume( theChannel, volume );
		theRecorderWindow->playMidiEvent( theEvent );
	}

	theRecorderWindow->setSettingsChanged();
}

void MIDIchannelsWindow::handleActive( bool active )
{
	unsigned char	minChannel, maxChannel, theChannel;
	unsigned char	selChannel = static_cast<unsigned char>(channelSelect->getSelection());

	if( selChannel < 16 )
		minChannel = maxChannel = selChannel;
	else
	{
		minChannel = 0;
		maxChannel = 15;
	}

	for( theChannel=minChannel; theChannel <= maxChannel; theChannel++ )
	{
		channelSettings[theChannel].active = active;
	}

	theRecorderWindow->setSettingsChanged();
}

void MIDIchannelsWindow::handleDefault( void )
{
	unsigned char	minChannel, maxChannel, channel;
	unsigned char	selChannel = static_cast<unsigned char>(channelSelect->getSelection());

	if( selChannel < 16 )
		minChannel = maxChannel = selChannel;
	else
	{
		minChannel = 0;
		maxChannel = 15;
	}

	for( channel=minChannel; channel <= maxChannel; channel++ )
	{
		ChannelSettings &theChannel = channelSettings[channel];
	
		theChannel.active = true;

		theChannel.stereoPos = 0;
		theChannel.volume = 127;
		theChannel.effects[0] = 0;
		theChannel.effects[1] = 0;
		theChannel.effects[2] = 0;
		theChannel.effects[3] = 0;
		theChannel.effects[4] = 0;
		theChannel.sounds[0] = 64;
		theChannel.sounds[1] = 64;
		theChannel.sounds[2] = 64;
		theChannel.sounds[3] = 64;
		theChannel.sounds[4] = 64;
		theChannel.expression = 127;

		theRecorderWindow->sendChannelSettings( midiDev, channel );
	}

	handleChannelSelection( selChannel );

	theRecorderWindow->setSettingsChanged();
}


void MIDIchannelsWindow::handleChannelSelection( unsigned char channel )
{
	if( channel > 15 )
		channel = 0;

	ChannelSettings &theChannel = channelSettings[channel];

	if( theChannel.group.isEmpty() || theChannel.voice.isEmpty() )
		handleVoiceSelection();
	else
		theVoices.selectVoice( theChannel.group, theChannel.voice );

	stereoBar->setPosition( theChannel.stereoPos );
	volumeBar->setPosition( theChannel.volume );
	effects1Bar->setPosition( theChannel.effects[0] );
	effects2Bar->setPosition( theChannel.effects[1] );
	effects3Bar->setPosition( theChannel.effects[2] );
	effects4Bar->setPosition( theChannel.effects[3] );
	effects5Bar->setPosition( theChannel.effects[4] );
	sound1Bar->setPosition( theChannel.sounds[0] );
	sound2Bar->setPosition( theChannel.sounds[1] );
	sound3Bar->setPosition( theChannel.sounds[2] );
	sound4Bar->setPosition( theChannel.sounds[3] );
	sound5Bar->setPosition( theChannel.sounds[4] );
	expressionBar->setPosition( theChannel.expression );
	activeCheckBox->setActive( theChannel.active );

}


// --------------------------------------------------------------------- //
// ----- class protected ----------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class virtuals ------------------------------------------------ //
// --------------------------------------------------------------------- //

ProcessStatus MIDIchannelsWindow::handleCreate( void )
{
	ChannelSelect::fill( channelSelect );

	stereoBar->setRange( MIDI_LEFT_POSITION, MIDI_RIGHT_POSITION );
	volumeBar->setRange( 0, 127 );

	effects1Bar->setRange( 0, 127 );
	effects2Bar->setRange( 0, 127 );
	effects3Bar->setRange( 0, 127 );
	effects4Bar->setRange( 0, 127 );
	effects5Bar->setRange( 0, 127 );

	sound1Bar->setRange( 0, 127 );
	sound2Bar->setRange( 0, 127 );
	sound3Bar->setRange( 0, 127 );
	sound4Bar->setRange( 0, 127 );
	sound5Bar->setRange( 0, 127 );

	expressionBar->setRange( 0, 127 );

	channelSelect->addEntry( appObject->loadString( winlibGUI::ANY_CHANNEL_LABEL_id ) );
	channelSelect->selectEntry( 0 );

	theVoices.loadVoices( voiceFile, groupSelect, voiceSelect );
	deviceLabel->setText( instrument );

	handleChannelSelection( 0 );

	return psDO_DEFAULT;
}

ProcessStatus MIDIchannelsWindow::handleCommand( int cmd )
{
	int toolValue = 1024;

	switch( cmd )
	{
		case winlibGUI::channelSelect_id:
			handleChannelSelection( 
				static_cast<unsigned char>(channelSelect->getSelection()) 
			);
			break;
		case winlibGUI::groupSelect_id:
			theVoices.handleGroupSelection();
		case winlibGUI::voiceSelect_id:
			handleVoiceSelection();
			break;
		case winlibGUI::stereoBar_id:
			handleNewStereoPos( char(toolValue = stereoBar->getPosition()) );
			break;
		case winlibGUI::volumeBar_id:
			handleNewVolume( static_cast<unsigned char>(toolValue = volumeBar->getPosition()) );
			break;

		case winlibGUI::effects1Bar_id:
			handleNewEffect( 0, static_cast<unsigned char>(toolValue = effects1Bar->getPosition()) );
			break;
		case winlibGUI::effects2Bar_id:
			handleNewEffect( 1, static_cast<unsigned char>(toolValue = effects2Bar->getPosition()) );
			break;
		case winlibGUI::effects3Bar_id:
			handleNewEffect( 2, static_cast<unsigned char>(toolValue = effects3Bar->getPosition()) );
			break;
		case winlibGUI::effects4Bar_id:
			handleNewEffect( 3, static_cast<unsigned char>(toolValue = effects4Bar->getPosition()) );
			break;
		case winlibGUI::effects5Bar_id:
			handleNewEffect( 4, static_cast<unsigned char>(toolValue = effects5Bar->getPosition()) );
			break;

		case winlibGUI::sound1Bar_id:
			handleNewSound( 0, static_cast<unsigned char>(toolValue = sound1Bar->getPosition()) );
			break;
		case winlibGUI::sound2Bar_id:
			handleNewSound( 1, static_cast<unsigned char>(toolValue = sound2Bar->getPosition()) );
			break;
		case winlibGUI::sound3Bar_id:
			handleNewSound( 2, static_cast<unsigned char>(toolValue = sound3Bar->getPosition()) );
			break;
		case winlibGUI::sound4Bar_id:
			handleNewSound( 3, static_cast<unsigned char>(toolValue = sound4Bar->getPosition()) );
			break;
		case winlibGUI::sound5Bar_id:
			handleNewSound( 4, static_cast<unsigned char>(toolValue = sound5Bar->getPosition()) );
			break;

		case winlibGUI::expressionBar_id:
			handleNewExpression( static_cast<unsigned char>(toolValue = expressionBar->getPosition()) );
			break;

		case winlibGUI::activeCheckBox_id:
			handleActive( activeCheckBox->isActive() );
			break;

		case winlibGUI::defaultButton_id:
			handleDefault();
			break;

		default:
			valueText->setText( "" );
			return MIDIchannelsWindow_form::handleCommand( cmd );
	}

	if( toolValue >= -127 && toolValue <= 127 )
	{
		valueText->setText( formatNumber( toolValue ) );
	}
	else
	{
		valueText->setText( "" );
	}

	return psPROCESSED;
}

SuccessCode MIDIchannelsWindow::handleClose( void )
{
	hide();
	return scERROR;
}

ProcessStatus MIDIchannelsWindow::handleCancel( void )
{
	hide();
	return psPROCESSED;
}

// --------------------------------------------------------------------- //
// ----- class publics ------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- entry points -------------------------------------------------- //
// --------------------------------------------------------------------- //


