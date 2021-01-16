/*
		Project:		MIDI Tools
		Module:			MIDIsythesizerWindow.cpp
		Description:	The synthesizer configuration
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

#include <WINLIB/STDDLG.H>

#include "../Synth/synthesizer.h"

#include "midi_rc.h"
#include "MIDIsynthesizerWindow.h"
#include "midifile.h"

// --------------------------------------------------------------------- //
// ----- imported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module switches ----------------------------------------------- //
// --------------------------------------------------------------------- //

#ifdef __BORLANDC__
#	pragma option -RT-
#	pragma option -b
#	pragma option -a4
#	pragma option -pc
#endif

using namespace std;
using namespace winlib;
using namespace gak;

// --------------------------------------------------------------------- //
// ----- constants ----------------------------------------------------- //
// --------------------------------------------------------------------- //

const short		MaxPitch = 36;
const double	FlexPitchPrecision = 100.0;
const short		FlexPitchTrackBarRange = MaxPitch * short(FlexPitchPrecision);

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

void MIDIsynthesizerWindow::writeSettings( ostream &dest )
{
	dest << "MG synthesizer Settings" << endl;
	dest << theSynthesizer.getVolume() << endl;
	dest << theSynthesizer.getPresenceValue() << endl;

	for( unsigned synth=0; synth<DEF_NUM_SYNTHS; synth++ )
	{
		dest << "Synthesis Module " << synth << endl;
		dest << theSynthesizer.isMuted( synth ) << endl;

		dest << theSynthesizer.getVolume( synth ) << endl;
		dest << theSynthesizer.getPan( synth ) << endl;
		dest << theSynthesizer.getPitch( synth ).pitch << endl;
		dest << theSynthesizer.getPhase( synth ) << endl;

		for( unsigned osc=0; osc<2; osc++ )
		{
			dest << "Sub Oscilator Module " << synth << '.' << osc << endl;

			const SubOscParameter &parameter = theSynthesizer.getOSC( synth, osc );
			dest << parameter.oscPhase << endl;
			dest << parameter.oscVolumeFactor << endl;
			dest << parameter.oscPitch.pitch << endl;
		}

		dest << "AM modulator " << synth << endl;
		const AMlfoParameter	&amLfoParameter = theSynthesizer.getAM( synth );
		dest << amLfoParameter.type << endl;

		dest << amLfoParameter.frequency << endl;
		dest << amLfoParameter.range.minimum << endl;
		dest << amLfoParameter.range.maximum << endl;

		dest << "FM modulator " << synth << endl;
		FMlfoParameter fmLfoParameter = theSynthesizer.getFM( synth );
		dest << fmLfoParameter.type << endl;
		dest << fmLfoParameter.isFactor << ' ' << fmLfoParameter.frequency << endl;
		dest << fmLfoParameter.range.pitch << endl;

		const Envelope::Parameter	&envelope = theSynthesizer.getEnvelope( synth );

		dest << "Envelope " << synth << endl;
		dest << envelope.enabled << endl;
		dest << envelope.delayMS << endl;
		dest << envelope.attackMS << endl;
		dest << envelope.decayMS << endl;
		dest << envelope.initialVolume << endl;
		dest << envelope.decayVolume << endl;
		if( envelope.sustainRateMS == (unsigned long)(-1) )
			dest << "-1\n";
		else
			dest << envelope.sustainRateMS << endl;
		if( envelope.releaseRateMS == (unsigned long)(-1) )
			dest << "-1\n";
		else
			dest << envelope.releaseRateMS << endl;
	}
	dest << "MG synthesizer Settings (END)" << endl;
}

void MIDIsynthesizerWindow::writeSettings( void )
{
	SaveFileAsDialog		dlg;

	dlg.setFilename( lastOptionFile );
	if( dlg.getDirectory().isEmpty() )
		dlg.setPersonalMusic();
	if( dlg.create( this, winlibGUI::SAVE_SETTINGS_id, winlibGUI::SynthFiles_ids, winlibGUI::SynthFiles_count ) )
	{
		lastOptionFile = dlg.getFilename();
		std::ofstream	destFile( lastOptionFile );
		if( destFile.rdbuf()->is_open() )
		{
			writeSettings( destFile );
		}
	}
}

void MIDIsynthesizerWindow::readSettings( istream &src )
{
	src.ignore( 1024, '\n' );

	double	volume;
	src >> volume;
	theSynthesizer.setVolume( volume );

	double	presenceValue;
	src >> presenceValue;
	theSynthesizer.setPresenceValue( presenceValue );

	for( unsigned synth=0; synth<DEF_NUM_SYNTHS; synth++ )
	{
		src.ignore( 1024, '\n' );
		src.ignore( 1024, '\n' );

		bool isMuted;
		src >> isMuted;
		theSynthesizer.mute( synth, isMuted );

		src >> volume;
		theSynthesizer.setVolume( synth, volume );

		double pan;
		src >> pan;
		theSynthesizer.setPan( synth, pan );

		short pitch;
		src >> pitch;
		theSynthesizer.setPitch( synth, Pitch(pitch) );

		double phase;
		src >> phase;
		theSynthesizer.setPhase( synth, phase );


		for( unsigned osc=0; osc<2; osc++ )
		{
			double pitch;

			src.ignore( 1024, '\n' );
			src.ignore( 1024, '\n' );
			src >> phase;
			src >> volume;
			src >> pitch;

			SubOscParameter	oscParameter = {
				phase, volume, FlexPitch( pitch )
			};

			theSynthesizer.setupOSC( synth, osc, oscParameter );
		}

		int				oscType;
		AMlfoParameter	amLfoParameter;
		FMlfoParameter	fmLfoParameter;

		src.ignore( 1024, '\n' );
		src.ignore( 1024, '\n' );
		src >> oscType;
		src >> amLfoParameter.frequency;
		src >> amLfoParameter.range.minimum;
		src >> amLfoParameter.range.maximum;

		amLfoParameter.type = OscilatorType(oscType);
		theSynthesizer.setupAM( synth, amLfoParameter );

		src.ignore( 1024, '\n' );
		src.ignore( 1024, '\n' );
		src >> oscType;
		src >> fmLfoParameter.isFactor >> fmLfoParameter.frequency;
		src >> fmLfoParameter.range.pitch;

		fmLfoParameter.type = OscilatorType(oscType);
		theSynthesizer.setupFM( synth, fmLfoParameter );

		bool envEnabled;
		unsigned short delayMS, attackMS, decayMS;
		double initialVolume, decayVolume;
		unsigned short sustainRateMS, releaseRateMS;

		src.ignore( 1024, '\n' );
		src.ignore( 1024, '\n' );
		src >> envEnabled;
		src >> delayMS;
		src >> attackMS;
		src >> decayMS;
		src >> initialVolume;
		src >> decayVolume;
		src >> sustainRateMS;
		src >> releaseRateMS;

		Envelope::Parameter	envelope = {
			envEnabled, delayMS, initialVolume, attackMS, decayMS, decayVolume, sustainRateMS, releaseRateMS
		};
			
		theSynthesizer.setupEnvelope( synth, envelope );
	}
}

void MIDIsynthesizerWindow::readSettings( void )
{
	OpenFileDialog	dlg;

	dlg.setFilename( lastOptionFile );
	if( dlg.getDirectory().isEmpty() )
		dlg.setPersonalMusic();
	if( dlg.create( this, winlibGUI::OPEN_SETTINGS_id, winlibGUI::SynthFiles_id, winlibGUI::SynthFiles_count ) )
	{
		lastOptionFile = dlg.getFilename();

		ifstream	sourceFile( lastOptionFile );
		if( sourceFile.rdbuf()->is_open() )
		{
			readSettings( sourceFile );
			getSettings( currentSynth );
		}
	}
}

void MIDIsynthesizerWindow::getSettings( int newSynth )
{
	reading = true;

	currentSynth = newSynth;

	checkMuteA->setActive( !theSynthesizer.isMuted( 0 ) );
	checkMuteB->setActive( !theSynthesizer.isMuted( 1 ) );
	checkMuteC->setActive( !theSynthesizer.isMuted( 2 ) );
	checkMuteD->setActive( !theSynthesizer.isMuted( 3 ) );

	double newVolume = theSynthesizer.getVolume();
	masterVolume->setPosition( short(newVolume * 1000.0) );

	double newPresenceValue = theSynthesizer.getPresenceValue();
	presenceValue->setPosition( short(newPresenceValue * -1000.0) );

	newVolume = theSynthesizer.getVolume( currentSynth );
	mainVolume->setPosition( short(newVolume * 1000.0) );

	double newPan = theSynthesizer.getPan( currentSynth );
	mainPan->setPosition( short(newPan * 1000.0) );

	short	pitch = theSynthesizer.getPitch( currentSynth ).pitch;
	mainPitch->setPosition( pitch );
	mainPitchValue->setText( formatNumber( pitch ) );

	double newPhase = theSynthesizer.getPhase( currentSynth );
	short phasePosition = short(newPhase * 1000.0 + 0.5);
	mainPhase->setPosition( phasePosition );

	const SubOscParameter &parameter0 = theSynthesizer.getOSC( currentSynth, 0 );
	phasePosition = short(parameter0.oscPhase * 1000.0 + 0.5);
	sub1phase->setPosition( phasePosition );
	sub1volume->setPosition( short(parameter0.oscVolumeFactor * 1000.0) );
	sub1pitch->setPosition( short(parameter0.oscPitch.pitch * FlexPitchPrecision) );

	const SubOscParameter &parameter1 = theSynthesizer.getOSC( currentSynth, 1 );
	phasePosition = short(parameter1.oscPhase * 1000.0 + 0.5);
	sub2phase->setPosition( phasePosition );
	sub2volume->setPosition( short(parameter1.oscVolumeFactor * 1000.0) );
	sub2pitch->setPosition( short(parameter1.oscPitch.pitch * FlexPitchPrecision) );

	const AMlfoParameter &amLfoParameter = theSynthesizer.getAM( currentSynth );
	switch( amLfoParameter.type )
	{
		case oscSINUS:
			amSinus->setActive();
			break;
		case oscASC_SAW:
			amAscending->setActive();
			break;
		case oscDES_SAW:
			amDescending->setActive();
			break;
		case oscRECTANGLE:
			amRectangle->setActive();
			break;
		case oscTRIANGLE:
			amTriangle->setActive();
			break;
	}
	amFrequency->setText( formatFloat( amLfoParameter.frequency ) );
	amRange->setPosition( short((1.0-amLfoParameter.range.minimum) * 1000.0 + 0.5) );
	FMlfoParameter fmLfoParameter = theSynthesizer.getFM( currentSynth );
	switch( fmLfoParameter.type )
	{
		case oscSINUS:
			fmSinus->setActive();
			break;
		case oscASC_SAW:
			fmAscending->setActive();
			break;
		case oscDES_SAW:
			fmDescending->setActive();
			break;
		case oscRECTANGLE:
			fmRectangle->setActive();
			break;
		case oscTRIANGLE:
			fmTriangle->setActive();
			break;
	}
	if( fmLfoParameter.isFactor )
		fmFrequency->setText( STRING('*') + formatFloat( fmLfoParameter.frequency ) );
	else
		fmFrequency->setText( formatFloat( fmLfoParameter.frequency ) );
	fmRange->setPosition( short(fmLfoParameter.range.pitch * FlexPitchPrecision + 0.5) );

	const Envelope::Parameter	&envelope = theSynthesizer.getEnvelope( currentSynth );

	if( envelope.enabled )
	{
		envEnabled->setActive();
	}
	else
	{
		envEnabled->clrActive();
	}

	envDelay->setText( formatNumber(envelope.delayMS) );
	envAttack->setText( formatNumber(envelope.attackMS) );
	envDecay->setText( formatNumber(envelope.decayMS) );

	envVolume1->setPosition( short(envelope.initialVolume * 1000.0) );
	envVolume2->setPosition( short(envelope.decayVolume * 1000.0) );

	envSustainRate->setText( formatNumber(long(envelope.sustainRateMS)) );
	envReleaseRate->setText( formatNumber(long(envelope.releaseRateMS)) );

	reading = false;
}

// --------------------------------------------------------------------- //
// ----- class protected ----------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class virtuals ------------------------------------------------ //
// --------------------------------------------------------------------- //

ProcessStatus MIDIsynthesizerWindow::handleCreate( void )
{
	reading = true;

	masterVolume->setRange( 0, 1000 );

	presenceValue->setRange( -2000, 2000 );
	presenceValue->setTic( 0 );

	mainVolume->setRange( 0, 1000 );

	mainPan->setRange( -1000, 1000 );
	mainPan->setTic( 0 );

	mainPhase->setRange( 0, short(2.0*M_PI*1000.0) );
	static short phaseTicks[] = { 
		short(M_PI/3.0*1000.0), short(M_PI/3.0*2000.0), 
		short(M_PI*1000.0), 
		short(M_PI*4.0/3.0*1000.0), short(M_PI*5.0/3.0*1000.0) 
	};
	mainPhase->setTics( phaseTicks, arraySize(phaseTicks) );

	mainPitch->setRange( -MaxPitch, MaxPitch );
	static short pitchTics[] = { -24, -12, 0, 12, 24 };
	mainPitch->setTics( pitchTics, arraySize(pitchTics) );

	sub1phase->setRange( 0, short(2.0*M_PI*1000.0) );
	sub1phase->setTics( phaseTicks, arraySize(phaseTicks) );
	sub1volume->setRange( 0, 333 );
	sub1pitch->setRange( -FlexPitchTrackBarRange, FlexPitchTrackBarRange );
	sub1pitch->setTic( short(FlexPitchPrecision) );

	sub2phase->setRange( 0, short(2.0*M_PI*1000.0) );
	sub2phase->setTics( phaseTicks, arraySize(phaseTicks) );
	sub2volume->setRange( 0, 111 );
	sub2pitch->setRange( -FlexPitchTrackBarRange, FlexPitchTrackBarRange );
	sub2pitch->setTic( short(FlexPitchPrecision) );

	amSinus->setActive();
	amRange->setRange( 0, 1000 );

	fmSinus->setActive();
	fmRange->setRange( 0, FlexPitchTrackBarRange );
	fmRange->setTic( short(FlexPitchPrecision) ); 

	envVolume1->setRange( 0, 1000 );
	envVolume2->setRange( 0, 1000 );

	radioSynthA->setActive();

	/*
		check CPU speed
	*/
	STRING	cpuSpeedLabel;
	if( !isAudioRunning() )
	{
		clock_t cpuTime = testCPUspeed();
		cpuSpeedLabel = STRING("CPU-speed: ") + formatNumber( cpuTime * 1000 / CLOCKS_PER_SEC ) + " ms ";
	}
	/*
		open devicw
	*/
	if( openSynthesizer() )
	{
		cpuSpeedLabel += formatFloat( getLatency() ) + " ms Latency";
	}
	else
	{
		cpuSpeedLabel += "no device";
	}

	cpuSpeed->setText( cpuSpeedLabel );

	currentSynth = 0;
	getSettings( 0 );

	setTimer( 10000 );

	reading = false;
	return psDO_DEFAULT;
}

ProcessStatus MIDIsynthesizerWindow::handleEditChange( int editControl )
{
	if( !reading )
		return handleCommand( editControl );
	else
		return psDO_DEFAULT;
}

ProcessStatus MIDIsynthesizerWindow::handleButtonClick( int btn )
{
	switch( btn )
	{
		case winlibGUI::radioSynthA_id:
			getSettings( 0 );
			break;
		case winlibGUI::radioSynthB_id:
			getSettings( 1 );
			break;
		case winlibGUI::radioSynthC_id:
			getSettings( 2 );
			break;
		case winlibGUI::radioSynthD_id:
			getSettings( 3 );
			break;

		case winlibGUI::checkMuteA_id:
			theSynthesizer.mute( 0, !checkMuteA->isActive() );
			break;
		case winlibGUI::checkMuteB_id:
			theSynthesizer.mute( 1, !checkMuteB->isActive() );
			break;
		case winlibGUI::checkMuteC_id:
			theSynthesizer.mute( 2, !checkMuteC->isActive() );
			break;
		case winlibGUI::checkMuteD_id:
			theSynthesizer.mute( 3, !checkMuteD->isActive() );
			break;

		case winlibGUI::saveSynthButton_id:
			writeSettings();
			showCPUspeed();
			break;

		case winlibGUI::loadSynthButton_id:
			readSettings();
			showCPUspeed();
			break;

		case winlibGUI::check1_id:
			if( check1->isActive() )
			{
				if( !isAudioRunning() || !theSynthesizer.start( MIDI_NOTE_A1, 127 ) )
				{
					check1->clrActive();
				}
			}
			else
			{
				theSynthesizer.release( MIDI_NOTE_A1 );
			}
			break;
		case winlibGUI::check2_id:
			if( check2->isActive() )
			{
				if( !isAudioRunning() || !theSynthesizer.start( MIDI_NOTE_C2, 127 ) )
				{
					check2->clrActive();
				}
			}
			else
			{
				theSynthesizer.release( MIDI_NOTE_C2 );
			}
			break;
		case winlibGUI::check3_id:
			if( check3->isActive() )
			{
				if( !isAudioRunning() || !theSynthesizer.start( MIDI_NOTE_E2, 127 ) )
				{
					check3->clrActive();
				}
			}
			else
			{
				theSynthesizer.release( MIDI_NOTE_E2 );
			}
			break;
		case winlibGUI::check4_id:
			if( check4->isActive() )
			{
				if( !isAudioRunning() || !theSynthesizer.start( MIDI_NOTE_A2, 127 ) )
				{
					check4->clrActive();
				}
			}
			else
			{
				theSynthesizer.release( MIDI_NOTE_A2 );
			}
			break;

		default:
		{
/*@*/		return MIDIsynthesizerWindow_form::handleButtonClick( btn );
		}
	}
	return psPROCESSED;
}

ProcessStatus MIDIsynthesizerWindow::handleScrollControl( int control )
{
	switch( control )
	{
		case winlibGUI::masterVolume_id:
		{
			double newVolume = double(masterVolume->getPosition())/1000.0;
			showToolHelp( formatFloat( newVolume * 100, 0, 1 ) + '%' );
			theSynthesizer.setVolume( newVolume );
			break;
		}
		case winlibGUI::presenceValue_id:
		{
			double newPresenceValue = double(-presenceValue->getPosition())/1000.0;
			showToolHelp( formatFloat( newPresenceValue ) );
			theSynthesizer.setPresenceValue( newPresenceValue );
			break;
		}
		case winlibGUI::mainVolume_id:
		{
			double newVolume = double(mainVolume->getPosition())/1000.0;
			showToolHelp( formatFloat( newVolume * 100, 0, 1 ) + '%' );
			theSynthesizer.setVolume( currentSynth, newVolume );
			break;
		}
		case winlibGUI::mainPan_id:
		{
			double newPan = double(mainPan->getPosition())/1000.0;
			showToolHelp( STRING( newPan <= -0.01 ?'L' : (newPan < 0.01 ? 'C' : 'R') ) + formatFloat( fabs(newPan) * 100, 0, 0 ) );
			theSynthesizer.setPan( currentSynth, newPan );
			break;
		}
		case winlibGUI::mainPitch_id:
		{
			short pitch = short(mainPitch->getPosition());

			theSynthesizer.setPitch( currentSynth, Pitch(pitch) );
			mainPitchValue->setText( formatNumber( pitch ) );
			break;
		}
		case winlibGUI::mainPhase_id:
		{
			short phasePosition = short(mainPhase->getPosition());
			double newPhase = double(phasePosition)/1000.0;
			theSynthesizer.setPhase( currentSynth, newPhase );
			break;
		}

		case winlibGUI::sub1phase_id:
		case winlibGUI::sub1volume_id:
		case winlibGUI::sub1pitch_id:
		{
			short phasePosition = short(sub1phase->getPosition());
			SubOscParameter	oscParameter;

			oscParameter.oscPhase = double(phasePosition)/1000.0;
			oscParameter.oscVolumeFactor = double(sub1volume->getPosition())/1000.0;
			oscParameter.oscPitch = FlexPitch(double(sub1pitch->getPosition())/FlexPitchPrecision);
			theSynthesizer.setupOSC( currentSynth, 0, oscParameter );
			if( control == winlibGUI::sub1volume_id )
			{
				showToolHelp( formatFloat( oscParameter.oscVolumeFactor * 100, 0, 1 ) + '%' );
			}
			else if( control == winlibGUI::sub1pitch_id )
			{
				showToolHelp( formatFloat( oscParameter.oscPitch.pitch ) );
			}
			break;
		}
		case winlibGUI::sub2phase_id:
		case winlibGUI::sub2volume_id:
		case winlibGUI::sub2pitch_id:
		{
			SubOscParameter	oscParameter;

			short phasePosition = short(sub2phase->getPosition());
			oscParameter.oscPhase = double(phasePosition)/1000.0;
			oscParameter.oscVolumeFactor = double(sub2volume->getPosition())/1000.0;
			oscParameter.oscPitch = FlexPitch( double(sub2pitch->getPosition())/FlexPitchPrecision );
			if( control == winlibGUI::sub2volume_id )
			{
				showToolHelp( formatFloat( oscParameter.oscVolumeFactor * 100, 0, 1 ) + '%' );
			}
			else if( control == winlibGUI::sub2pitch_id )
			{
				showToolHelp( formatFloat( oscParameter.oscPitch.pitch ) );
			}
			theSynthesizer.setupOSC( currentSynth, 1, oscParameter );
			break;
		}

		default:
		{
/*@*/		return MIDIsynthesizerWindow_form::handleScrollControl( control );
		}
	}
	return psPROCESSED;
}

ProcessStatus MIDIsynthesizerWindow::handleCommand( int cmd )
{
	switch( cmd )
	{
		case winlibGUI::amSinus_id:
		case winlibGUI::amAscending_id:
		case winlibGUI::amDescending_id:
		case winlibGUI::amTriangle_id:
		case winlibGUI::amRectangle_id:
		case winlibGUI::amFrequency_id:
		case winlibGUI::amRange_id:
		{
			AMlfoParameter	amLfoParameter;
			amLfoParameter.type = getAmOscilatorType( cmd ); 

			amLfoParameter.frequency = atof( amFrequency->getText() );
			amLfoParameter.range.minimum = 1 - double(amRange->getPosition()) / 1000.0;
			amLfoParameter.range.maximum = 1;
			if( cmd == winlibGUI::amRange_id )
			{
				showToolHelp( formatFloat( (1- amLfoParameter.range.minimum) * 100, 0, 1 ) );
			}
			theSynthesizer.setupAM( currentSynth, amLfoParameter );
			break;
		}

		case winlibGUI::fmSinus_id:
		case winlibGUI::fmAscending_id:
		case winlibGUI::fmDescending_id:
		case winlibGUI::fmTriangle_id:
		case winlibGUI::fmRectangle_id:
		case winlibGUI::fmFrequency_id:
		case winlibGUI::fmRange_id:
		{
			FMlfoParameter	fmLfoParameter;
			STRING			tmpStr =  fmFrequency->getText().stripBlanks();
			const char		*tmpCP = tmpStr;
			
			fmLfoParameter.type = getFmOscilatorType( cmd ); 
			fmLfoParameter.isFactor = *tmpCP == '*';
			if( fmLfoParameter.isFactor )
			{
				tmpCP++;
			}
			fmLfoParameter.frequency = atof( tmpCP );
			fmLfoParameter.range.pitch = double(fmRange->getPosition()) / FlexPitchPrecision;
			if( cmd == winlibGUI::fmRange_id )
			{
				showToolHelp( formatFloat( fmLfoParameter.range.pitch ) );
			}

			theSynthesizer.setupFM( currentSynth, fmLfoParameter );
			break;
		}

		case winlibGUI::envEnabled_id:
		case winlibGUI::envDelay_id:
		case winlibGUI::envAttack_id:
		case winlibGUI::envDecay_id:
		case winlibGUI::envVolume1_id:
		case winlibGUI::envVolume2_id:
		case winlibGUI::envSustainRate_id:
		case winlibGUI::envReleaseRate_id:
		{
			Envelope::Parameter	envelope;

			envelope.enabled = envEnabled->isActive();
			envelope.delayMS = envDelay->getText().getValueN<unsigned short>();
			envelope.attackMS = envAttack->getText().getValueN<unsigned short>();
			envelope.decayMS = envDecay->getText().getValueN<unsigned short>();
			envelope.initialVolume = double(envVolume1->getPosition()) / 1000.0;
			envelope.decayVolume = double(envVolume2->getPosition()) / 1000.0;
			envelope.sustainRateMS = envSustainRate->getText().getValueN<unsigned short>();
			envelope.releaseRateMS = envReleaseRate->getText().getValueN<unsigned short>();

			if( cmd == winlibGUI::envVolume1_id )
			{
				showToolHelp( formatFloat( envelope.initialVolume * 100, 0, 1 ) + '%' );
			}
			else if( cmd == winlibGUI::envVolume2_id )
			{
				showToolHelp( formatFloat( envelope.decayVolume * 100, 0, 1 ) + '%' );
			}
			theSynthesizer.setupEnvelope( currentSynth, envelope );
			break;
		}

		default:
		{
/*@*/		return MIDIsynthesizerWindow_form::handleCommand( cmd );
		}
	}
	return psPROCESSED;
}

void MIDIsynthesizerWindow::handleTimer( void )
{
	setTimer( 1000 );

	double limiter = theSynthesizer.getLimiter();
	STRING	newCpuLabel = formatFloat( 100-limiter*100, 0, 0 ) + "% ";
	newCpuLabel += formatBinary( 
		theSynthesizer.getActivePorts(), 2, 
		unsigned(theSynthesizer.getMaxPhones()) 
	);

	if( cpuSpeedLabel != newCpuLabel )
	{
		cpuSpeedLabel = newCpuLabel;
		cpuSpeed->setText( newCpuLabel );
	}
}

ProcessStatus MIDIsynthesizerWindow::handleDestroy( void )
{
	closeSynthesizer();
	return psDO_DEFAULT;
}

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
