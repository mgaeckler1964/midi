/*
		Project:		MG Analog Synthesizer
		Module:			vstVoice.h
		Description:	Wrapper between vstProcessor and the synthesizer core
						Defines all paramter IDs
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

#ifndef MG_VST_VOICE_H
#define MG_VST_VOICE_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <cmath>

#include <gak/math.h>

#include "base/source/fstreamer.h"

#include "pluginterfaces/vst/vsttypes.h"
#include "pluginterfaces/base/ibstream.h"
#include "pluginterfaces/vst/ivstaudioprocessor.h"
#include "pluginterfaces/vst/ivstevents.h"
#include "pluginterfaces/vst/ivstunits.h"

#include "../Synth/synthesizer.h"

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

// --------------------------------------------------------------------- //
// ----- constants ----------------------------------------------------- //
// --------------------------------------------------------------------- //

static const double		MAX_LFO_FREQUENCY = 800;
static const double		MAX_PITCH = 36;

static const Steinberg::Vst::ParamValue	LEVEL_POWER = 1.0/5.0;
static const Steinberg::Vst::ParamValue	LEVEL_DIVISOR = std::pow( 2.0, LEVEL_POWER );

// --------------------------------------------------------------------- //
// ----- macros -------------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- type definitions ---------------------------------------------- //
// --------------------------------------------------------------------- //

enum SynthUnitIDs
{
	suiRoot = Steinberg::Vst::kRootUnitId,
	suiSynthA,
		suiSynthA_OSCs,
		suiSynthA_AM_LFO,
		suiSynthA_FM_LFO,
		suiSynthA_ENVELOPE,
	suiSynthB,
		suiSynthB_OSCs,
		suiSynthB_AM_LFO,
		suiSynthB_FM_LFO,
		suiSynthB_ENVELOPE,
	suiSynthC,
		suiSynthC_OSCs,
		suiSynthC_AM_LFO,
		suiSynthC_FM_LFO,
		suiSynthC_ENVELOPE,
	suiSynthD,
		suiSynthD_OSCs,
		suiSynthD_AM_LFO,
		suiSynthD_FM_LFO,
		suiSynthD_ENVELOPE,
};

enum SynthParameterIDs
{
	spiMasterVolume,
	spiMasterPresence,

	spiSynthEnabled1,
	spiSynthEnabled2,
	spiSynthEnabled3,
	spiSynthEnabled4,

	spiActiveVoices,
	spiMaxVoices,
	spiLeftLevel,
	spiRightLevel,

	spiSynthVolume1 = 100,
	spiSynthPan1,
	spiSynthPitch1,
	spiSynthPhase1,
	spiSub1Volume1,
	spiSub1Phase1,
	spiSub1Pitch1,
	spiSub2Volume1,
	spiSub2Phase1,
	spiSub2Pitch1,

	spiAMtype1,
	spiAMfrequency1,
	spiAMrange1,

	spiFMtype1,
	spiFMrelative1,
	spiFMfrequency1,
	spiFMrange1,

	spiEnvelope1,
	spiDelayMS1,
	spiInitalVolume1,
	spiAttackMS1,
	spiDecayMS1,
	spiDecayVolume1,
	spiSustainRateMS1,
	spiReleaseRateMS1,

	spiSynthVolume2 = 200,
	spiSynthPan2,
	spiSynthPitch2,
	spiSynthPhase2,
	spiSub1Volume2,
	spiSub1Phase2,
	spiSub1Pitch2,
	spiSub2Volume2,
	spiSub2Phase2,
	spiSub2Pitch2,

	spiAMtype2,
	spiAMfrequency2,
	spiAMrange2,

	spiFMtype2,
	spiFMrelative2,
	spiFMfrequency2,
	spiFMrange2,

	spiEnvelope2,
	spiDelayMS2,
	spiInitalVolume2,
	spiAttackMS2,
	spiDecayMS2,
	spiDecayVolume2,
	spiSustainRateMS2,
	spiReleaseRateMS2,

	spiSynthVolume3 = 300,
	spiSynthPan3,
	spiSynthPitch3,
	spiSynthPhase3,
	spiSub1Volume3,
	spiSub1Phase3,
	spiSub1Pitch3,
	spiSub2Volume3,
	spiSub2Phase3,
	spiSub2Pitch3,

	spiAMtype3,
	spiAMfrequency3,
	spiAMrange3,

	spiFMtype3,
	spiFMrelative3,
	spiFMfrequency3,
	spiFMrange3,

	spiEnvelope3,
	spiDelayMS3,
	spiInitalVolume3,
	spiAttackMS3,
	spiDecayMS3,
	spiDecayVolume3,
	spiSustainRateMS3,
	spiReleaseRateMS3,

	spiSynthVolume4 = 400,
	spiSynthPan4,
	spiSynthPitch4,
	spiSynthPhase4,
	spiSub1Volume4,
	spiSub1Phase4,
	spiSub1Pitch4,
	spiSub2Volume4,
	spiSub2Phase4,
	spiSub2Pitch4,

	spiAMtype4,
	spiAMfrequency4,
	spiAMrange4,

	spiFMtype4,
	spiFMrelative4,
	spiFMfrequency4,
	spiFMrange4,

	spiEnvelope4,
	spiDelayMS4,
	spiInitalVolume4,
	spiAttackMS4,
	spiDecayMS4,
	spiDecayVolume4,
	spiSustainRateMS4,
	spiReleaseRateMS4,

	kNumGlobalParameters
};

struct VoiceParameterState
{
	typedef Steinberg::tresult			tresult;
	typedef Steinberg::IBStreamer		IBStreamer;

	typedef Steinberg::Vst::ParamValue	ParamValue;

	bool				enable;

	ParamValue			synthVolume;	// [0, +1]
	ParamValue			synthPan;		// [-1, +1]
	Pitch				synthPitch;
	double				synthPhase;		// [0, 2*pi]

	SubOscParameter		subOscParameter[DEF_NUM_SUBOSCS];
	AMlfoParameter		amLfoParameter;
	FMlfoParameter		fmLfoParameter;
	Envelope::Parameter	envelope;

	tresult setState( IBStreamer &s );
	tresult getState( IBStreamer &s );
};

struct GlobalParameterState
{
	typedef Steinberg::tresult			tresult;
	typedef Steinberg::IBStream			IBStream;

	typedef Steinberg::Vst::ParamValue	ParamValue;

	ParamValue masterVolume;	// [0, +1]
	ParamValue masterPresence;	// [-2, +2]

	VoiceParameterState	voiceParams[DEF_NUM_SYNTHS];

	tresult setState( IBStream *stream );
	tresult getState( IBStream *stream );
};

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

class SynthVoiceBase
{
	public:
	typedef Steinberg::Vst::ParamValue	ParamValue;

	protected:
	ParamValue	m_leftLevel, m_rightLevel;

	private:
	static ParamValue meterValue( ParamValue level )
	{
		return std::pow( level, LEVEL_POWER ) / LEVEL_DIVISOR;
	}
	public:
	SynthVoiceBase()
	{
		m_leftLevel = m_rightLevel = 0;
	}
	virtual ~SynthVoiceBase()
	{
	}

	virtual Steinberg::tresult process( Steinberg::Vst::ProcessData& data ) = 0;
	virtual void setParameterState( const GlobalParameterState &newState ) = 0;
	virtual int getMaxVoices() const = 0;
	virtual int getActiveVoices() const = 0;
	ParamValue getLeftLevel() const
	{
		return meterValue( m_leftLevel );
	}
	ParamValue getRightLevel() const
	{
		return meterValue( m_rightLevel );
	}
};

template <typename SampleT>
class MGsynthVoice : public SynthVoiceBase
{
	MidiSynthesizer<SampleT>	m_theSynthesizer;

	public:
	MGsynthVoice( unsigned sampleWidth, unsigned long sampleRate ) : m_theSynthesizer()
	{
		m_theSynthesizer.setup( 1.0, sampleWidth, sampleRate );
		m_theSynthesizer.testCPUspeed();
	}
	~MGsynthVoice()
	{
	}
	virtual Steinberg::tresult process( Steinberg::Vst::ProcessData& data );
	virtual void setParameterState( const GlobalParameterState &newState );
	virtual int getMaxVoices() const
	{
		return m_theSynthesizer.getMaxPhones();
	}
	virtual int getActiveVoices() const
	{
		return m_theSynthesizer.getActivePhones();
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

template <typename SampleT>
Steinberg::tresult MGsynthVoice<SampleT>::process( Steinberg::Vst::ProcessData& data )
{
	Steinberg::int32 numSamples = data.numSamples;

	Steinberg::Vst::IEventList	*inputEvents = data.inputEvents;
	Steinberg::Vst::Event		e = {0};
	bool						hasEvent = false;
	Steinberg::int32			eventIndex = 0;
	Steinberg::int32			numEvents = inputEvents ? inputEvents->getEventCount () : 0;

	// get the first event
	if( numEvents )
	{
		inputEvents->getEvent (0, e);
		hasEvent = true;
	}

	SampleT	*leftBuffer = reinterpret_cast<SampleT*>(data.outputs[0].channelBuffers32[0]);
	SampleT	*rightBuffer = reinterpret_cast<SampleT*>(data.outputs[0].channelBuffers32[1]);

	m_leftLevel = m_rightLevel = 0;
	StereoSample<SampleT> sample;
	for( Steinberg::int32 i=0; i<numSamples; ++i )
	{
		while( hasEvent && e.sampleOffset <= i )
		{
			switch( e.type )
			{
				//-----------------------
				case Event::kNoteOnEvent:
				{
				/*
					if (e.noteOn.noteId == -1)
					{
						e.noteOn.noteId = e.noteOn.pitch;
					}
				*/
					m_theSynthesizer.start( e.noteOn.pitch, e.noteOn.velocity*127 + 0.5 );
					//data.outputEvents->addEvent (e);

					break;
				}
				//-----------------------
				case Event::kNoteOffEvent:
				{
				/*
					if (e.noteOff.noteId == -1)
					{
						e.noteOff.noteId = e.noteOff.pitch;
					}
				*/
					m_theSynthesizer.release( e.noteOff.pitch );
					break;
				}
				//-----------------------
				case Event::kNoteExpressionValueEvent:
				{
					break;
				}
			}
			
			// get next event
			eventIndex++;
			if( eventIndex < numEvents )
			{
				if( inputEvents->getEvent( eventIndex, e ) != kResultTrue )
				{
					hasEvent = false;
				}
			}
			else
			{
				hasEvent = false;
			}
		}	// while( hasEvent && e.sampleOffset <= i )

		m_theSynthesizer.getNextSample( sample );
		*leftBuffer++ = sample.left;
		*rightBuffer++ = sample.right;

		ParamValue channelSample = gak::math::abs(sample.left);
		if( m_leftLevel < channelSample )
		{
			m_leftLevel = channelSample;
		}
		channelSample = gak::math::abs(sample.right);
		if( m_rightLevel < channelSample )
		{
			m_rightLevel = channelSample;
		}
	}

	return Steinberg::kResultTrue;
}

template <typename SampleT>
void MGsynthVoice<SampleT>::setParameterState( const GlobalParameterState &newState )
{
	m_theSynthesizer.setVolume( newState.masterVolume );
	m_theSynthesizer.setPresenceValue( newState.masterPresence*-4.+2. );

	for( unsigned synth = 0; synth<DEF_NUM_SYNTHS; ++synth )
	{
		const VoiceParameterState	&voiceParam = newState.voiceParams[synth];

		m_theSynthesizer.mute( synth, !voiceParam.enable );

		m_theSynthesizer.setVolume( synth, voiceParam.synthVolume );
		m_theSynthesizer.setPan( synth, voiceParam.synthPan*2.-1. );
		m_theSynthesizer.setPitch( synth, voiceParam.synthPitch );
		m_theSynthesizer.setPhase( synth, voiceParam.synthPhase * 2 * M_PI );

		for( unsigned osc=0; osc<DEF_NUM_SUBOSCS; ++osc )
		{
			const SubOscParameter	&parameter = m_theSynthesizer.getOSC( synth, osc );
			if( memcmp( &parameter, &voiceParam.subOscParameter[osc], sizeof(SubOscParameter) ) )
			{
				m_theSynthesizer.setupOSC( synth, osc, voiceParam.subOscParameter[osc] );
			}
		}

		const AMlfoParameter	&amLFO = m_theSynthesizer.getAM( synth );
		if( memcmp( &amLFO, &voiceParam.amLfoParameter, sizeof(AMlfoParameter) ) )
		{
			m_theSynthesizer.setupAM( synth, voiceParam.amLfoParameter );
		}

		const FMlfoParameter	&fmLFO = m_theSynthesizer.getFM( synth );
		if( memcmp( &fmLFO, &voiceParam.fmLfoParameter, sizeof(FMlfoParameter) ) )
		{
			m_theSynthesizer.setupFM( synth, voiceParam.fmLfoParameter );
		}

		const Envelope::Parameter	&envelope = m_theSynthesizer.getEnvelope( synth );
		if( memcmp( &envelope, &voiceParam.envelope, sizeof(Envelope::Parameter) ) )
		{
			m_theSynthesizer.setupEnvelope( synth, voiceParam.envelope );
		}
	}
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

#endif	// MG_VST_VOICE_H