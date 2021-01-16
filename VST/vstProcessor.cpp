/*
		Project:		MG Analog Synthesizer
		Module:			vstProcessor.cpp
		Description:	Definition of  VST Audio Processor
		Author:			Martin Gäckler
		Address:		Hopfengasse 15. A-4020 Linz
		Web:			https://www.gaeckler.at/

		Copyright:		(c) 2017-2018 Martin Gäckler

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

#include "pluginterfaces/vst/ivstparameterchanges.h"

#include "vstProcessor.h"
#include "vstController.h"

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

using namespace Steinberg;
using namespace Vst;

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

FUID Processor::cid (0x11223344, 0x55667788, 0x99AABBCC, 0xDDEEFF00);

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

Processor::Processor() : m_voiceProcessor(0)
{
	setControllerClass(Controller::cid);

	memset( &m_paramState, 0, sizeof(m_paramState) );
	m_paramState.masterVolume = 1;
	m_paramState.masterPresence = 0.5;

	m_paramState.voiceParams[0].enable = true;

	for( size_t i=0; i<DEF_NUM_SYNTHS; ++i )
	{
		VoiceParameterState	&voiceParams = m_paramState.voiceParams[i];
		voiceParams.synthVolume = 1;
		voiceParams.synthPan = 0.5;

		voiceParams.amLfoParameter.range.minimum = voiceParams.amLfoParameter.range.maximum = 1;

		voiceParams.envelope.decayVolume = 1;
		voiceParams.envelope.sustainRateMS = 10000;
	}
}

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

tresult PLUGIN_API Processor::initialize (FUnknown* context)
{
	tresult result = AudioEffect::initialize (context);
	if (result == kResultTrue)
	{
		addAudioOutput( STR16("Audio Output"), SpeakerArr::kStereo );
		addEventInput( STR16("Event Input"), 1 );
	}
	return result;
}

tresult PLUGIN_API Processor::setState (IBStream* state)
{
	tresult result = m_paramState.setState(state);
	if( result == kResultTrue && m_voiceProcessor )
	{
		m_voiceProcessor->setParameterState( m_paramState );
	}
	return result;
}

tresult PLUGIN_API Processor::getState (IBStream* state)
{
	return m_paramState.getState( state );
}

tresult PLUGIN_API Processor::setBusArrangements (
	SpeakerArrangement* inputs, int32 numIns, 
	SpeakerArrangement* outputs, int32 numOuts
)
{
	// we only support one stereo output bus
	if( numIns == 0 && numOuts == 1 && outputs[0] == SpeakerArr::kStereo )
	{
		return AudioEffect::setBusArrangements( inputs, numIns, outputs, numOuts );
	}
	return kResultFalse;
}

tresult PLUGIN_API Processor::canProcessSampleSize( int32 symbolicSampleSize )
{
	if( symbolicSampleSize == kSample32 || symbolicSampleSize == kSample64 )
	{
		return kResultTrue;
	}
	return kResultFalse;
}

tresult PLUGIN_API Processor::setActive( TBool state )
{
	if( state )
	{
		if( m_voiceProcessor == 0 )
		{
			if( processSetup.symbolicSampleSize == kSample32 )
			{
				m_voiceProcessor = new MGsynthVoice<Sample32>( 16, processSetup.sampleRate );
			}
			else if( processSetup.symbolicSampleSize == kSample64 )
			{
				m_voiceProcessor = new MGsynthVoice<Sample64>( 24, processSetup.sampleRate );
			}
			else
			{
				return kInvalidArgument;
			}
			m_voiceProcessor->setParameterState( m_paramState );
		}
	}
	else
	{
		if( m_voiceProcessor )
		{
			delete m_voiceProcessor;
			m_voiceProcessor = 0;
		}
	}
	return AudioEffect::setActive(state);
}

tresult PLUGIN_API Processor::process( ProcessData& data )
{
	if( data.inputParameterChanges )
	{
		int32 count = data.inputParameterChanges->getParameterCount();
		for( int32 i = 0; i < count; ++i )
		{
			IParamValueQueue* queue = data.inputParameterChanges->getParameterData (i);
			if( queue )
			{
				int32 sampleOffset;
				ParamValue value;
				ParamID pid = queue->getParameterId ();
				if( queue->getPoint( queue->getPointCount() - 1, sampleOffset, value ) == kResultTrue )
				{
					switch( pid )
					{
						case spiMasterVolume:
						{
							m_paramState.masterVolume = value;
							break;
						}
						case spiMasterPresence:
						{
							m_paramState.masterPresence = value;
							break;
						}
						
						case spiSynthEnabled1:
						{
							m_paramState.voiceParams[0].enable = (value >= 0.5);
							break;
						}
						case spiSynthEnabled2:
						{
							m_paramState.voiceParams[1].enable = (value >= 0.5);
							break;
						}
						case spiSynthEnabled3:
						{
							m_paramState.voiceParams[2].enable = (value >= 0.5);
							break;
						}
						case spiSynthEnabled4:
						{
							m_paramState.voiceParams[3].enable = (value >= 0.5);
							break;
						}

						case spiSynthVolume1:
						{
							m_paramState.voiceParams[0].synthVolume = value;
							break;
						}
						case spiSynthVolume2:
						{
							m_paramState.voiceParams[1].synthVolume = value;
							break;
						}
						case spiSynthVolume3:
						{
							m_paramState.voiceParams[2].synthVolume = value;
							break;
						}
						case spiSynthVolume4:
						{
							m_paramState.voiceParams[3].synthVolume = value;
							break;
						}

						case spiSynthPan1:
						{
							m_paramState.voiceParams[0].synthPan = value;
							break;
						}
						case spiSynthPan2:
						{
							m_paramState.voiceParams[1].synthPan = value;
							break;
						}
						case spiSynthPan3:
						{
							m_paramState.voiceParams[2].synthPan = value;
							break;
						}
						case spiSynthPan4:
						{
							m_paramState.voiceParams[3].synthPan = value;
							break;
						}
						case spiSynthPitch1:
						{
							m_paramState.voiceParams[0].synthPitch = Pitch(value*72-35.5 );
							break;
						}
						case spiSynthPitch2:
						{
							m_paramState.voiceParams[1].synthPitch = Pitch(value*72-35.5 );
							break;
						}
						case spiSynthPitch3:
						{
							m_paramState.voiceParams[2].synthPitch = Pitch(value*72-35.5 );
							break;
						}
						case spiSynthPitch4:
						{
							m_paramState.voiceParams[3].synthPitch = Pitch(value*72-35.5 );
							break;
						}
						case spiSynthPhase1:
						{
							m_paramState.voiceParams[0].synthPhase = value;
							break;
						}
						case spiSynthPhase2:
						{
							m_paramState.voiceParams[1].synthPhase = value;
							break;
						}
						case spiSynthPhase3:
						{
							m_paramState.voiceParams[2].synthPhase = value;
							break;
						}
						case spiSynthPhase4:
						{
							m_paramState.voiceParams[3].synthPhase = value;
							break;
						}

						case spiSub1Phase1:
						{
							m_paramState.voiceParams[0].subOscParameter[0].oscPhase = value * 2*M_PI;
							break;
						}
						case spiSub1Phase2:
						{
							m_paramState.voiceParams[1].subOscParameter[0].oscPhase = value * 2*M_PI;
							break;
						}
						case spiSub1Phase3:
						{
							m_paramState.voiceParams[2].subOscParameter[0].oscPhase = value * 2*M_PI;
							break;
						}
						case spiSub1Phase4:
						{
							m_paramState.voiceParams[3].subOscParameter[0].oscPhase = value * 2*M_PI;
							break;
						}
						case spiSub2Phase1:
						{
							m_paramState.voiceParams[0].subOscParameter[1].oscPhase = value * 2*M_PI;
							break;
						}
						case spiSub2Phase2:
						{
							m_paramState.voiceParams[1].subOscParameter[1].oscPhase = value * 2*M_PI;
							break;
						}
						case spiSub2Phase3:
						{
							m_paramState.voiceParams[2].subOscParameter[1].oscPhase = value * 2*M_PI;
							break;
						}
						case spiSub2Phase4:
						{
							m_paramState.voiceParams[3].subOscParameter[1].oscPhase = value * 2*M_PI;
							break;
						}
						case spiSub1Volume1:
						{
							m_paramState.voiceParams[0].subOscParameter[0].oscVolumeFactor = value;
							break;
						}
						case spiSub1Volume2:
						{
							m_paramState.voiceParams[1].subOscParameter[0].oscVolumeFactor = value;
							break;
						}
						case spiSub1Volume3:
						{
							m_paramState.voiceParams[2].subOscParameter[0].oscVolumeFactor = value;
							break;
						}
						case spiSub1Volume4:
						{
							m_paramState.voiceParams[3].subOscParameter[0].oscVolumeFactor = value;
							break;
						}
						case spiSub2Volume1:
						{
							m_paramState.voiceParams[0].subOscParameter[1].oscVolumeFactor = value;
							break;
						}
						case spiSub2Volume2:
						{
							m_paramState.voiceParams[1].subOscParameter[1].oscVolumeFactor = value;
							break;
						}
						case spiSub2Volume3:
						{
							m_paramState.voiceParams[2].subOscParameter[1].oscVolumeFactor = value;
							break;
						}
						case spiSub2Volume4:
						{
							m_paramState.voiceParams[3].subOscParameter[1].oscVolumeFactor = value; 
							break;
						}
						case spiSub1Pitch1:
						{
							m_paramState.voiceParams[0].subOscParameter[0].oscPitch = FlexPitch( value*2*MAX_PITCH - MAX_PITCH );
							break;
						}
						case spiSub1Pitch2:
						{
							m_paramState.voiceParams[1].subOscParameter[0].oscPitch = FlexPitch( value*2*MAX_PITCH - MAX_PITCH );
							break;
						}
						case spiSub1Pitch3:
						{
							m_paramState.voiceParams[2].subOscParameter[0].oscPitch = FlexPitch( value*2*MAX_PITCH - MAX_PITCH );
							break;
						}
						case spiSub1Pitch4:
						{
							m_paramState.voiceParams[3].subOscParameter[0].oscPitch = FlexPitch( value*2*MAX_PITCH - MAX_PITCH );
							break;
						}
						case spiSub2Pitch1:
						{
							m_paramState.voiceParams[0].subOscParameter[1].oscPitch = FlexPitch( value*2*MAX_PITCH - MAX_PITCH );
							break;
						}
						case spiSub2Pitch2:
						{
							m_paramState.voiceParams[1].subOscParameter[1].oscPitch = FlexPitch( value*2*MAX_PITCH - MAX_PITCH );
							break;
						}
						case spiSub2Pitch3:
						{
							m_paramState.voiceParams[2].subOscParameter[1].oscPitch = FlexPitch( value*2*MAX_PITCH - MAX_PITCH );
							break;
						}
						case spiSub2Pitch4:
						{
							m_paramState.voiceParams[3].subOscParameter[1].oscPitch = FlexPitch( value*2*MAX_PITCH - MAX_PITCH );
							break;
						}

						case spiAMtype1:
						{
							m_paramState.voiceParams[0].amLfoParameter.type = OscilatorType(int(value * double(oscRECTANGLE)));
							break;
						}
						case spiAMtype2:
						{
							m_paramState.voiceParams[1].amLfoParameter.type = OscilatorType(int(value * double(oscRECTANGLE)));
							break;
						}
						case spiAMtype3:
						{
							m_paramState.voiceParams[2].amLfoParameter.type = OscilatorType(int(value * double(oscRECTANGLE)));
							break;
						}
						case spiAMtype4:
						{
							m_paramState.voiceParams[3].amLfoParameter.type = OscilatorType(int(value * double(oscRECTANGLE)));
							break;
						}

						case spiAMfrequency1:
						{
							m_paramState.voiceParams[0].amLfoParameter.frequency = value*MAX_LFO_FREQUENCY;
							break;
						}
						case spiAMfrequency2:
						{
							m_paramState.voiceParams[1].amLfoParameter.frequency = value*MAX_LFO_FREQUENCY;
							break;
						}
						case spiAMfrequency3:
						{
							m_paramState.voiceParams[2].amLfoParameter.frequency = value*MAX_LFO_FREQUENCY;
							break;
						}
						case spiAMfrequency4:
						{
							m_paramState.voiceParams[3].amLfoParameter.frequency = value*MAX_LFO_FREQUENCY;
							break;
						}

						case spiAMrange1:
						{
							m_paramState.voiceParams[0].amLfoParameter.range.minimum = 1- value;
							break;
						}
						case spiAMrange2:
						{
							m_paramState.voiceParams[1].amLfoParameter.range.minimum = 1- value;
							break;
						}
						case spiAMrange3:
						{
							m_paramState.voiceParams[2].amLfoParameter.range.minimum = 1- value;
							break;
						}
						case spiAMrange4:
						{
							m_paramState.voiceParams[3].amLfoParameter.range.minimum = 1- value;
							break;
						}

						case spiFMtype1:
						{
							m_paramState.voiceParams[0].fmLfoParameter.type = OscilatorType(int(value * double(oscRECTANGLE)));
							break;
						}
						case spiFMtype2:
						{
							m_paramState.voiceParams[1].fmLfoParameter.type = OscilatorType(int(value * double(oscRECTANGLE)));
							break;
						}
						case spiFMtype3:
						{
							m_paramState.voiceParams[2].fmLfoParameter.type = OscilatorType(int(value * double(oscRECTANGLE)));
							break;
						}
						case spiFMtype4:
						{
							m_paramState.voiceParams[3].fmLfoParameter.type = OscilatorType(int(value * double(oscRECTANGLE)));
							break;
						}

						case spiFMrelative1:
						{
							m_paramState.voiceParams[0].fmLfoParameter.isFactor = (value >= 0.5);
							break;
						}
						case spiFMrelative2:
						{
							m_paramState.voiceParams[1].fmLfoParameter.isFactor = (value >= 0.5);
							break;
						}
						case spiFMrelative3:
						{
							m_paramState.voiceParams[2].fmLfoParameter.isFactor = (value >= 0.5);
							break;
						}
						case spiFMrelative4:
						{
							m_paramState.voiceParams[3].fmLfoParameter.isFactor = (value >= 0.5);
							break;
						}

						case spiFMfrequency1:
						{
							m_paramState.voiceParams[0].fmLfoParameter.frequency = value*MAX_LFO_FREQUENCY;
							break;
						}
						case spiFMfrequency2:
						{
							m_paramState.voiceParams[1].fmLfoParameter.frequency = value*MAX_LFO_FREQUENCY;
							break;
						}
						case spiFMfrequency3:
						{
							m_paramState.voiceParams[2].fmLfoParameter.frequency = value*MAX_LFO_FREQUENCY;
							break;
						}
						case spiFMfrequency4:
						{
							m_paramState.voiceParams[3].fmLfoParameter.frequency = value*MAX_LFO_FREQUENCY;
							break;
						}

						case spiFMrange1:
						{
							m_paramState.voiceParams[0].fmLfoParameter.range = FlexPitch( value*MAX_PITCH );
							break;
						}
						case spiFMrange2:
						{
							m_paramState.voiceParams[1].fmLfoParameter.range = FlexPitch( value*MAX_PITCH );
							break;
						}
						case spiFMrange3:
						{
							m_paramState.voiceParams[2].fmLfoParameter.range = FlexPitch( value*MAX_PITCH );
							break;
						}
						case spiFMrange4:
						{
							m_paramState.voiceParams[3].fmLfoParameter.range = FlexPitch( value*MAX_PITCH );
							break;
						}

						case spiEnvelope1:
						{
							m_paramState.voiceParams[0].envelope.enabled = (value >= 0.5);
							break;
						}
						case spiEnvelope2:
						{
							m_paramState.voiceParams[1].envelope.enabled = (value >= 0.5);
							break;
						}
						case spiEnvelope3:
						{
							m_paramState.voiceParams[2].envelope.enabled = (value >= 0.5);
							break;
						}
						case spiEnvelope4:
						{
							m_paramState.voiceParams[3].envelope.enabled = (value >= 0.5);
							break;
						}
						case spiDelayMS1:
						{
							m_paramState.voiceParams[0].envelope.delayMS = value *10000.0 + 0.5;
							break;
						}
						case spiDelayMS2:
						{
							m_paramState.voiceParams[1].envelope.delayMS = value *10000.0 + 0.5;
							break;
						}
						case spiDelayMS3:
						{
							m_paramState.voiceParams[2].envelope.delayMS = value *10000.0 + 0.5;
							break;
						}
						case spiDelayMS4:
						{
							m_paramState.voiceParams[3].envelope.delayMS = value *10000.0 + 0.5;
							break;
						}
						case spiInitalVolume1:
						{
							m_paramState.voiceParams[0].envelope.initialVolume = value;
							break;
						}
						case spiInitalVolume2:
						{
							m_paramState.voiceParams[1].envelope.initialVolume = value;
							break;
						}
						case spiInitalVolume3:
						{
							m_paramState.voiceParams[2].envelope.initialVolume = value;
							break;
						}
						case spiInitalVolume4:
						{
							m_paramState.voiceParams[3].envelope.initialVolume = value;
							break;
						}
						case spiAttackMS1:
						{
							m_paramState.voiceParams[0].envelope.attackMS = value *10000.0 + 0.5;
							break;
						}
						case spiAttackMS2:
						{
							m_paramState.voiceParams[1].envelope.attackMS = value *10000.0 + 0.5;
							break;
						}
						case spiAttackMS3:
						{
							m_paramState.voiceParams[2].envelope.attackMS = value *10000.0 + 0.5;
							break;
						}
						case spiAttackMS4:
						{
							m_paramState.voiceParams[3].envelope.attackMS = value *10000.0 + 0.5;
							break;
						}
						case spiDecayVolume1:
						{
							m_paramState.voiceParams[0].envelope.decayVolume = value;
							break;
						}
						case spiDecayVolume2:
						{
							m_paramState.voiceParams[1].envelope.decayVolume = value;
							break;
						}
						case spiDecayVolume3:
						{
							m_paramState.voiceParams[2].envelope.decayVolume = value;
							break;
						}
						case spiDecayVolume4:
						{
							m_paramState.voiceParams[3].envelope.decayVolume = value;
							break;
						}
						case spiDecayMS1:
						{
							m_paramState.voiceParams[0].envelope.decayMS = value *10000.0 + 0.5;
							break;
						}
						case spiDecayMS2:
						{
							m_paramState.voiceParams[1].envelope.decayMS = value *10000.0 + 0.5;
							break;
						}
						case spiDecayMS3:
						{
							m_paramState.voiceParams[2].envelope.decayMS = value *10000.0 + 0.5;
							break;
						}
						case spiDecayMS4:
						{
							m_paramState.voiceParams[3].envelope.decayMS = value *10000.0 + 0.5;
							break;
						}
						case spiSustainRateMS1:
						{
							m_paramState.voiceParams[0].envelope.sustainRateMS = value *10000.0 + 0.5;
							break;
						}
						case spiSustainRateMS2:
						{
							m_paramState.voiceParams[1].envelope.sustainRateMS = value *10000.0 + 0.5;
							break;
						}
						case spiSustainRateMS3:
						{
							m_paramState.voiceParams[2].envelope.sustainRateMS = value *10000.0 + 0.5;
							break;
						}
						case spiSustainRateMS4:
						{
							m_paramState.voiceParams[3].envelope.sustainRateMS = value *10000.0 + 0.5;
							break;
						}
						case spiReleaseRateMS1:
						{
							m_paramState.voiceParams[0].envelope.releaseRateMS = value *10000.0 + 0.5;
							break;
						}
						case spiReleaseRateMS2:
						{
							m_paramState.voiceParams[1].envelope.releaseRateMS = value *10000.0 + 0.5;
							break;
						}
						case spiReleaseRateMS3:
						{
							m_paramState.voiceParams[2].envelope.releaseRateMS = value *10000.0 + 0.5;
							break;
						}
						case spiReleaseRateMS4:
						{
							m_paramState.voiceParams[3].envelope.releaseRateMS = value *10000.0 + 0.5;
							break;
						}
					}
				}
			}
		}

		m_voiceProcessor->setParameterState( m_paramState );
	}

	tresult result;

	// flush mode
	if (data.numOutputs < 1)
	{
		result = kResultTrue;
	}
	else
	{
		result = m_voiceProcessor->process( data );
	}

	if( result == kResultTrue )
	{
		if( data.outputParameterChanges )
		{
			int32 index;
			IParamValueQueue* queue;

			queue = data.outputParameterChanges->addParameterData( spiMaxVoices, index );
			if( queue )
			{
				queue->addPoint( 0, ParamValue(m_voiceProcessor->getMaxVoices() ) / ParamValue(DEF_NUM_PHONES), index);
			}

			queue = data.outputParameterChanges->addParameterData( spiActiveVoices, index );
			if( queue )
			{
				queue->addPoint( 0, ParamValue( m_voiceProcessor->getActiveVoices() ) / ParamValue(DEF_NUM_PHONES ), index);
			}

			queue = data.outputParameterChanges->addParameterData( spiLeftLevel, index );
			if( queue )
			{
				queue->addPoint( 0, ParamValue( m_voiceProcessor->getLeftLevel() ), index);
			}

			queue = data.outputParameterChanges->addParameterData( spiRightLevel, index );
			if( queue )
			{
				queue->addPoint( 0, ParamValue( m_voiceProcessor->getRightLevel() ), index);
			}
		}
		if( m_voiceProcessor->getActiveVoices() == 0 && data.numOutputs > 0 )
		{
			data.outputs[0].silenceFlags = 0x11; // left and right channel are silent
		}
	}

	return result;
}

// --------------------------------------------------------------------- //
// ----- entry points -------------------------------------------------- //
// --------------------------------------------------------------------- //

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif

