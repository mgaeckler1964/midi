/*
		Project:		Synthesizer Core
		Module:			synthesizer.h
		Description:	The synthesizer main module with audio stream
		Author:			Martin Gäckler
		Address:		Hopfengasse 15. A-4020 Linz
		Web:			https://www.gaeckler.at/

		Copyright:		(c) 2014-2018 Martin Gäckler

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

#ifndef SYNTHESIZER_H
#define SYNTHESIZER_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <limits>
#include <ctime>
#include <cassert>

#include <math.h>

#include <gak/utils.h>
#include <gak/ringBuffer.h>

#include "midi.h"
#include "manipulator.h"

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

static const unsigned short		bitsPerSample = 16;
static const unsigned long		sampleRate = 44100;
static const double				HALF_TONE_FACTOR = pow( 2, 1.0/12.0 );

static const int DEF_NUM_SUBOSCS = 2;
static const int DEF_NUM_SYNTHS = 4;
static const int DEF_NUM_PHONES = 16;

// --------------------------------------------------------------------- //
// ----- macros -------------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- type definitions ---------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

template <typename T>
struct BasicPitch
{
	T		pitch;

	explicit BasicPitch( T pitch=0 ) : pitch( pitch )
	{
	}
	static double getFrequencyFactor( T pitch )
	{
		return pow( HALF_TONE_FACTOR, pitch );
	}
	double getFrequencyFactor() const
	{
		return getFrequencyFactor( pitch );
	}
};

typedef BasicPitch<short>	Pitch;
typedef BasicPitch<double>	FlexPitch;

struct SubOscParameter
{
	double		oscPhase,
				oscVolumeFactor;
	FlexPitch	oscPitch;
};

struct FMlfoParameter
{
	OscilatorType	type;
	double			frequency;
	bool			isFactor;
	FlexPitch		range;
};

struct AMlfoParameter
{
	OscilatorType	type;
	double			frequency;
	Interval		range;
};

/*
	this is the base for a value producer
	it contains the sample rate.
	NOTE: Alle samplers must use the same sample rate
	The template paramweter T is the type of the sample 
	to produce.
*/
template <class T=long> 
struct StereoSample
{
	T	left, right;
};

/*
	the Synthesizer is a one channel, one note sample producer
*/
template <class T, int NUM_SUBOSC> 
class Synthesizer : public BaseProducer
{
	public:
	typedef Oscilator<T>	Oscilator;

	private:
	Oscilator			m_mainOSC;
	Oscilator			m_osc[NUM_SUBOSC];

	OscManipulator<T>	m_amLFO;
	OscManipulator<T>	m_fmLFO;
	Envelope			m_envelope;

	double				m_frequency;
	bool				m_keyPressed, m_muted;

	SubOscParameter		m_subOscParameter[NUM_SUBOSC];

	FMlfoParameter		m_fmLfoParameter;
	AMlfoParameter		m_amLfoParameter;

	T					m_clipValue;
	unsigned			m_sampleWidth;

	public:
	Synthesizer()
	{
		memset( &m_subOscParameter, 0, sizeof( m_subOscParameter ) );
		memset( &m_fmLfoParameter, 0, sizeof( m_fmLfoParameter ) );
		memset( &m_amLfoParameter, 0, sizeof( m_amLfoParameter ) );
		m_amLfoParameter.range.maximum = m_amLfoParameter.range.minimum = 1;

		m_keyPressed = false;
		m_muted = false;
	}
	/*
		setup
	*/
	void setup( T clipValue, unsigned sampleWidth, unsigned long sampleRate )
	{
		m_mainOSC.setup( clipValue, sampleRate );
		for( unsigned i=0; i<NUM_SUBOSC; i++ )
		{
			m_osc[i].setup( clipValue, sampleRate );
		}
		m_amLFO.setup( clipValue, sampleRate );
		m_fmLFO.setup( clipValue, sampleRate );

		m_clipValue = clipValue;
		m_sampleWidth = sampleWidth;
		setSampleRate( sampleRate );
		setVolume( 1 );
	}
	const Oscilator &getMainOSC( void ) const
	{
		return m_mainOSC;
	}
	const Oscilator &getSubOSC( unsigned i ) const
	{
		return m_osc[i];
	}

	/*
		frequency
	*/
	void setFrequency( double frequency )
	{
		m_frequency = frequency;

		m_mainOSC.setFrequency( frequency );
		for( unsigned i=0; i<NUM_SUBOSC; i++ )
		{
			m_osc[i].setFrequency( frequency * m_subOscParameter[i].oscPitch.getFrequencyFactor() );
		}

		if( m_fmLfoParameter.isFactor )
		{
			m_fmLFO.setFrequency( frequency * m_fmLfoParameter.frequency );
		}
	}
	void setPhase( double phase )
	{
		m_mainOSC.setPhase( phase );
	}
	double getPhase( void ) const
	{
		return m_mainOSC.getPhase();
	}
	/*
		Volume
	*/
	void setVolume( double volume )
	{
		BaseProducer::setVolume( volume );
		m_mainOSC.setVolume( volume );
		for( unsigned i=0; i<NUM_SUBOSC; i++ )
		{
			m_osc[i].setVolume( volume*m_subOscParameter[i].oscVolumeFactor );
		}
	}
	bool isMuted() const
	{
		return m_muted;
	}
	void mute( bool muted )
	{
		m_muted = muted;
	}
	void setPresenceValue( double newPresenceValue )
	{
		m_mainOSC.setPresenceValue( newPresenceValue );
		for( unsigned i=0; i<NUM_SUBOSC; i++ )
		{
			m_osc[i].setPresenceValue( newPresenceValue );
		}
	}
	double getPresenceValue() const
	{
		return m_mainOSC.getPresenceValue();
	}


	/*
		AM configuration
	*/
	void setupAM( const AMlfoParameter &params )
	{
		m_amLfoParameter = params;
		m_amLFO.setOscilatorType( params.type );
		m_amLFO.setFrequency( params.frequency );
		m_amLFO.setRange( params.range );

		// reset original volume after change
		setVolume( getVolume() );
	}
	const AMlfoParameter &getAM() const
	{
		return m_amLfoParameter;
	}

	/*
		FM configuration
	*/
	void setupFM( const FMlfoParameter &params  )
	{
		m_fmLfoParameter = params;

		if( params.isFactor )
		{
			m_fmLFO.setFrequency( m_frequency * params.frequency );
		}
		else
		{
			m_fmLFO.setFrequency( params.frequency );
		}
		double range = params.range.getFrequencyFactor()-1;
		m_fmLFO.setRange( Interval( -range, +range ) );
		m_fmLFO.setOscilatorType( params.type );
	}
	const FMlfoParameter &getFM( void ) const
	{
		return m_fmLfoParameter;
	}
	double getFMfrequency( void ) const
	{
		return fmLFO.getFrequency();
	}

	/*
		envelope configuration
	*/
	public:
	void setupEnvelope( const Envelope::Parameter &envParam )
 	{
		m_envelope.setup( 
			m_sampleWidth, getSampleRate(), 
			envParam
		);

		// reset original volume after change
		setVolume( getVolume() );
	}
	const Envelope::Parameter &getEnvelope( void ) const
 	{
		return m_envelope.get();
	}

	/*
		osc configuration
	*/
	void setupOSC( unsigned subOSC, const SubOscParameter &parameter )
	{
		if( subOSC < NUM_SUBOSC )
		{
			m_subOscParameter[subOSC] = parameter;

			m_osc[subOSC].setPhase( parameter.oscPhase );
			m_osc[subOSC].setVolume( getVolume()*parameter.oscVolumeFactor );
			m_osc[subOSC].setFrequency( m_frequency*parameter.oscPitch.getFrequencyFactor() );
		}
	}
	const SubOscParameter &getOSC( unsigned subOSC ) const
	{
		return m_subOscParameter[subOSC < NUM_SUBOSC ? subOSC : 0];
	}

	/*
		Produce
	*/
	void start()
	{
		m_keyPressed = true;
		m_mainOSC.start();
		for( unsigned i=0; i<NUM_SUBOSC; i++ )
		{
			if( m_subOscParameter[i].oscVolumeFactor )
			{
				m_osc[i].start();
			}
		}

		if( m_envelope.isEnabled() )
		{
			m_envelope.start();
		}

		if( m_amLFO.isEnabled() )
		{
			m_amLFO.start();
		}

		if( m_fmLFO.isEnabled() )
		{
			m_fmLFO.start();
		}
	}
	void release()
	{
		m_keyPressed = false;
		m_envelope.release();
	}
	void stop()
	{
		m_keyPressed = false;
		m_envelope.stop();
	}
	bool isActive() const
	{
		return !m_muted && (m_keyPressed || (m_envelope.isEnabled() && m_envelope.isActive()));
	}
	bool isKeyPressed() const
	{
		return m_keyPressed;
	}
	T getNextSample()
	{
		bool	volumeChanged = false;
		double	newVolume = getVolume();

		if( m_amLFO.isEnabled() )
		{
			volumeChanged = true;
			newVolume *= m_amLFO.getNextValue();
		}
		if( m_envelope.isEnabled() )
		{
			volumeChanged = true;
			newVolume *= m_envelope.getNextValue();
		}

		if( volumeChanged )
		{
			m_mainOSC.setVolume( newVolume );
			for( unsigned osc=0; osc<NUM_SUBOSC; osc++ )
			{
				if( m_subOscParameter[osc].oscVolumeFactor )
				{
					m_osc[osc].setVolume( newVolume*m_subOscParameter[osc].oscVolumeFactor );
				}
			}
		}

		if( m_fmLFO.isEnabled() )
		{
			static double minFm = 1000;
			static double maxFm = -1000;
			double fmValue = m_fmLFO.getNextValue();
			if( fmValue < minFm )
			{
				minFm = fmValue;
			}
			if( fmValue < 0 )
			{
				fmValue = 1.0/(1.0-fmValue);
			}
			else
			{
				fmValue += 1;
			}
			double newFrequency = m_frequency * fmValue;
			m_mainOSC.setFrequency( newFrequency );
			for( unsigned i=0; i<NUM_SUBOSC; i++ )
			{
				if( m_subOscParameter[i].oscVolumeFactor )
				{
					m_osc[i].setFrequency( 
						newFrequency*m_subOscParameter[i].oscPitch.getFrequencyFactor() 
					);
				}
			}
		}

		T sample = m_mainOSC.getNextSample();
		for( unsigned i=0; i<NUM_SUBOSC; i++ )
		{
			if( m_subOscParameter[i].oscVolumeFactor )
			{
				sample += m_osc[i].getNextSample();
			}
		}
		return  sample;
	}
};

template <class T, int NUM_SYNTHS, int NUM_SUBOSC> 
class StereoSynthesizer : public BaseProducer
{
	public:
	typedef Synthesizer<T, NUM_SUBOSC>	Synthesizer;

	private:
	Synthesizer	m_synths[NUM_SYNTHS];
	double		m_pans[NUM_SYNTHS];
	double		m_frequencyFactor[NUM_SYNTHS];

	double		m_frequency, m_pitchFactor;

	public:
	StereoSynthesizer()
	{
		for( unsigned i=0; i<NUM_SYNTHS; i++ )
		{
			m_pans[i] = 0;
			m_frequencyFactor[i] = 1;
			m_pitchFactor = 0;
		}
	}
	const Synthesizer &getSynth( unsigned i ) const
	{
		return m_synths[i];
	}

	/*
		setup
	*/
	void setup( T newClipValue, unsigned sampleWidth, unsigned long sampleRate )
	{
		for( unsigned i=0; i<NUM_SYNTHS; i++ )
		{
			m_synths[i].setup( newClipValue, sampleWidth, sampleRate );
		}
	}

	/*
		frequency
	*/
	double getFrequency()
	{
		return m_frequency;
	}
	void setFrequency( double frequency )
	{
		m_frequency = frequency;
		for( unsigned i=0; i<NUM_SYNTHS; i++ )
		{
			double newFrequency = frequency * m_frequencyFactor[i];
			if( m_pitchFactor )
			{
				newFrequency *= m_pitchFactor;
			}
			m_synths[i].setFrequency( newFrequency );
		}
	}
	void setFrequencyFactor( unsigned synth, double frequencyFactor )
	{
		if( synth < NUM_SYNTHS )
		{
			m_frequencyFactor[synth] = frequencyFactor;

			double newFrequency = m_frequency * frequencyFactor;
			if( m_pitchFactor )
			{
				newFrequency *= m_pitchFactor;
			}
			m_synths[synth].setFrequency( newFrequency );
		}
	}
	void setPitchWheel( double pitchWheel )
	{
		pitchFactor = pitchWheel;
		setFrequency( frequency );
	}
	void setPhase( unsigned synth, double phase )
	{
		if( synth < NUM_SYNTHS )
		{
			m_synths[synth].setPhase( phase );
		}
	}
	double getPhase( unsigned synth ) const
	{
		if( synth < NUM_SYNTHS )
		{
			return m_synths[synth].getPhase();
		}
		return 0;
	}

	/*
		Volume
	*/
	using BaseProducer::getVolume;
	using BaseProducer::setVolume;

	void setVolume( unsigned synth, double newVolume )
	{
		if( synth < NUM_SYNTHS )
		{
			m_synths[synth].setVolume( newVolume );
		}
	}
	double getVolume( unsigned synth ) const
	{
		if( synth < NUM_SYNTHS )
		{
			return m_synths[synth].getVolume();
		}
		return 0;
	}
	void setPresenceValue( double newPresenceValue )
	{
		for( unsigned i=0; i<NUM_SYNTHS; i++ )
		{
			m_synths[i].setPresenceValue( newPresenceValue );
		}
	}
	double getPresenceValue() const
	{
		return m_synths[0].getPresenceValue();
	}
	void setPan( unsigned synth, double pan )
	{
		if( synth < NUM_SYNTHS )
		{
			m_pans[synth] = pan;
		}
	}
	double getPan( unsigned synth ) const
	{
		if( synth < NUM_SYNTHS )
		{
			return m_pans[synth];
		}
		return 0;
	}
	bool isMuted( unsigned synth ) const
	{
		if( synth < NUM_SYNTHS )
		{
			return m_synths[synth].isMuted();
		}
		else
		{
			return true;
		}
	}
	void mute( unsigned synth, bool muteFlag )
	{
		if( synth < NUM_SYNTHS )
			m_synths[synth].mute( muteFlag );
	}


	/*
		AM configuration
	*/
	void setupAM( unsigned synth, const AMlfoParameter &params )
	{
		assert( synth < NUM_SYNTHS );
		{
			m_synths[synth].setupAM( params );
		}
	}
	const AMlfoParameter &getAM( unsigned synth ) const
	{
		assert( synth < NUM_SYNTHS );
		{
			return m_synths[synth].getAM();
		}
	}

	/*
		FM configuration
	*/
	void setupFM( unsigned synth, const FMlfoParameter &params  )
	{
		if( synth < NUM_SYNTHS )
		{
			m_synths[synth].setupFM( params );
		}
	}
	const FMlfoParameter &getFM( unsigned synth ) const
	{
		return m_synths[synth].getFM();
	}
	double getFMfrequency( void ) const
	{
		return m_synths[0].getFMfrequency();
	}

	/*
		envelope configuration
	*/
	void setupEnvelope( unsigned synth, const Envelope::Parameter &envelope )
 	{
		if( synth < NUM_SYNTHS )
		{
			m_synths[synth].setupEnvelope( envelope );
		}
	}
	const Envelope::Parameter &getEnvelope( unsigned synth ) const
 	{
		assert( synth < NUM_SYNTHS );

		return m_synths[synth].getEnvelope();
	}

	/*
		osc configuration
	*/
	void setupOSC( unsigned synth, unsigned subOSC, const SubOscParameter &parameter )
	{
		if( synth < NUM_SYNTHS )
		{
			m_synths[synth].setupOSC( subOSC, parameter );
		}
	}
	const SubOscParameter &getOSC( unsigned synth, unsigned subOSC ) const
	{
		return m_synths[synth < NUM_SYNTHS ? synth : 0].getOSC( subOSC );
	}

	/*
		Produce

	*/
	void start( void )
	{
		for( unsigned i=0; i<NUM_SYNTHS; i++ )
		{
			m_synths[i].start();
		}
	}
	bool isActive( void ) const
	{
		for( unsigned i=0; i<NUM_SYNTHS; i++ )
		{
			if( m_synths[i].isActive() )
			{
				return true;
			}
		}
		return false;
	}
	bool isKeyPressed( void ) const
	{
		for( unsigned i=0; i<NUM_SYNTHS; i++ )
		{
			if( m_synths[i].isKeyPressed() )
			{
				return true;
			}
		}
		return false;
	}
	void release( void )
	{
		for( unsigned i=0; i<NUM_SYNTHS; i++ )
		{
			m_synths[i].release();
		}
	}
	void stop( void )
	{
		for( unsigned i=0; i<NUM_SYNTHS; i++ )
		{
			m_synths[i].stop();
		}
	}

	T getNextSample()
	{
		StereoSample<T> sample;

		getNextSample( sample );
		return (sample.left + sample.right)/2;
	}
	void getNextSample( StereoSample<T> &sample )
	{
		T left, right;
		double	sumVolume = 0;

		left = right = 0;

		for( unsigned i=0; i<NUM_SYNTHS; i++ )
		{
			if( m_synths[i].isActive() )
			{
				T sample = m_synths[i].getNextSample();
				sumVolume += m_synths[i].getVolume();

				if( m_pans[i] > 0 )
				{
					double panFactor = 1-m_pans[i];
					left += T(double(sample) * panFactor);
					right += sample;
				}
				else if( m_pans[i] < 0 )
				{
					left += sample;
					double panFactor = 1+m_pans[i];
					right += T(double(sample) * panFactor);
				}
				else
				{
					left += sample;
					right += sample;
				}

			}
		}

		if( sumVolume )
		{
			double phoneVolume = getVolume() / sumVolume;
			if( phoneVolume )
			{
				left	= T(double(left)*phoneVolume);
				right	= T(double(right)*phoneVolume);

				sample.left = left;
				sample.right = right;
			}
			else
			{
				sample.left = 0;
				sample.right = 0;
			}
		}
		else
		{
			sample.left = 0;
			sample.right = 0;
		}
	}
};

template <class T=long, int NUM_SYNTHS=DEF_NUM_SYNTHS, int NUM_SUBOSC=DEF_NUM_SUBOSCS, int NUM_PHONE=DEF_NUM_PHONES> 
class MidiSynthesizer : public BaseProducer
{
	public:
	typedef StereoSynthesizer<T,NUM_SYNTHS,NUM_SUBOSC>	StereoSynthesizer;

	private:
	StereoSynthesizer	thePhones[NUM_PHONE];
	clock_t				releaseTimes[NUM_PHONE];

	Pitch				pitch[NUM_SYNTHS];

	T					clipValue;
	double				limiter, limiterIncrement;
	unsigned			m_maxPhones;

	double				noteFrequencies[128];

	public:
	MidiSynthesizer()
	{
		limiter = 1;

		noteFrequencies[MIDI_NOTE_A1] = 440;
		for( unsigned i=MIDI_NOTE_A1+1; i<128; i++ )
		{
			noteFrequencies[i] = noteFrequencies[i-1] * HALF_TONE_FACTOR;
		}
		for( unsigned i=MIDI_NOTE_A1-1; i!=unsigned(-1); i-- )
		{
			noteFrequencies[i] = noteFrequencies[i+1] / HALF_TONE_FACTOR;
		}

		m_maxPhones = NUM_PHONE;
	}
	const StereoSynthesizer &getPhone( unsigned i ) const
	{
		return thePhones[i];
	}
	/*
		setup
	*/
	void setup( T newClipValue, unsigned sampleWidth, unsigned long sampleRate )
	{
		if( getSampleRate() )
		{
			return;
		}

		setSampleRate( sampleRate );
		setVolume( 1 );

		clipValue = newClipValue;
		for( unsigned i=0; i<NUM_PHONE; i++ )
		{
			thePhones[i].setup( newClipValue, sampleWidth, sampleRate );
			releaseTimes[i] = 0;
		}

		setVolume( 0, 1 );
		for( unsigned i=1; i<NUM_SYNTHS; i++ )
		{
			setVolume( i, 1 );
			mute( i, true );
		}

		SubOscParameter	oscParameter = {
			0, 0, FlexPitch(0)
		};
		FMlfoParameter	fmLfoParameer = {
			oscSINUS, 0, false, FlexPitch(0)
		};
		AMlfoParameter	amLfoParameer = {
			oscSINUS, 0, Interval( 1, 1 )
		};
		for( unsigned synth=0; synth<NUM_SYNTHS; synth++ )
		{
			for( unsigned subOSC=0; subOSC<NUM_SUBOSC; subOSC++ )
			{
				setupOSC( synth, subOSC, oscParameter );
			}

			setupAM( synth, amLfoParameer );
			setupFM( synth, fmLfoParameer );
			Envelope::Parameter envelope = {
				false, 0, 0, 0, 0, 1, std::numeric_limits<unsigned short>::max(), 0
			};
			setupEnvelope( synth, envelope );
		}
	}
	/*
		frequency
	*/
	private:
	void setFrequencyFactor( unsigned synth, double frequencyFactor )
	{
		for( unsigned i=0; i<NUM_PHONE; i++ )
		{
			thePhones[i].setFrequencyFactor( synth, frequencyFactor );
		}
	}
	public:
	void setPhase( unsigned synth, double phase )
	{
		if( getPhase( synth ) != phase )
		{
			for( unsigned i=0; i<NUM_PHONE; i++ )
			{
				thePhones[i].setPhase( synth, phase );
			}
		}
	}
	double getPhase( unsigned synth ) const
	{
		return thePhones[0].getPhase( synth );
	}
	void setPitch( unsigned synth, const Pitch &newPitch )
	{
		pitch[synth] = newPitch;
		setFrequencyFactor( synth, newPitch.getFrequencyFactor() );
	}
	const Pitch &getPitch( unsigned synth ) const
	{
		return pitch[synth];
	}
	void setPitchWheel( short pitchWheel )
	{
		double	pitchFactor;

		if( !pitchWheel )
			pitchFactor = 0;
		else
		{
			pitchFactor = 2*halfToneFactor*pitchWheel / MIDI_NEUTRAL_PITCH;
			if( pitchFactor > 0 )
				pitchFactor = 1+pitchFactor;
			else
				pitchFactor = 1/(1-pitchFactor);
		}
		for( unsigned i=0; i<NUM_PHONES; i++ )
		{
			thePhones[i].setPitchWheel( pitchFactor );
		}
	}


	/*
		Volume
	*/
	private:
	void calcLimiter( T left, T right )
	{
		left = abs( left );
		right = abs( right );
		if( left > clipValue )
		{
			double newLimiterValue = double(clipValue) / double(left);
			if( limiter > newLimiterValue )
			{
				limiter = newLimiterValue;
				limiterIncrement = (1.0-newLimiterValue)/getSampleRate();
			}
		}
		if( right > clipValue )
		{
			double newLimiterValue = double(clipValue) / double(right);
			if( limiter > newLimiterValue )
			{
				limiter = newLimiterValue;
				limiterIncrement = (1.0-newLimiterValue)/getSampleRate();
			}
		}
		if( left < clipValue && right < clipValue && limiter < 1 )
		{
			limiter += limiterIncrement;
		}
	}
	public:
	using BaseProducer::getVolume;

	void setVolume( double newVolume )
	{
		assert( newVolume >= -0. && newVolume <= 1.0 );

		BaseProducer::setVolume( newVolume );
	}
	void setVolume( unsigned synth, double newVolume )
	{
		for( unsigned i=0; i<NUM_PHONE; i++ )
		{
			thePhones[i].setVolume( synth, newVolume );
		}
	}
	void setPresenceValue( double newPresenceValue )
	{
		assert( newPresenceValue >= -2.0 && newPresenceValue <= 2.0 );

		for( unsigned i=0; i<NUM_PHONE; i++ )
		{
			thePhones[i].setPresenceValue( newPresenceValue );
		}
	}
	double getPresenceValue( void ) const
	{
		return thePhones[0].getPresenceValue();
	}
	double getVolume( unsigned synth ) const
	{
		return thePhones[0].getVolume( synth );
	}
	void setPan( unsigned synth, double pan )
	{
		for( unsigned i=0; i<NUM_PHONE; i++ )
		{
			thePhones[i].setPan( synth, pan );
		}
	}
	double getPan( unsigned synth ) const
	{
		return thePhones[0].getPan( synth );
	}
	bool isMuted( unsigned synth ) const
	{
		return thePhones[0].isMuted( synth );
	}
	void mute( unsigned synth, bool muteFlag )
	{
		for( unsigned i=0; i<NUM_PHONE; i++ )
		{
			thePhones[i].mute( synth, muteFlag );
		}
	}
	double getLimiter(void) const
	{
		return limiter;
	}

	/*
		AM configuration
	*/
	void setupAM( unsigned synth, const AMlfoParameter &params )
	{
		for( unsigned i=0; i<NUM_PHONE; i++ )
		{
			thePhones[i].setupAM( synth, params );
		}
	}
	const AMlfoParameter &getAM( unsigned synth ) const
	{
		return thePhones[0].getAM( synth );
	}

	/*
		FM configuration
	*/
	void setupFM( unsigned synth, const FMlfoParameter &params )
	{
		for( unsigned i=0; i<NUM_PHONE; i++ )
		{
			thePhones[i].setupFM( synth, params );
		}
	}
	const FMlfoParameter &getFM( unsigned synth ) const
	{
		return thePhones[0].getFM( synth );
	}
	double getFMfrequency( void ) const
	{
		return thePhones[0].getFMfrequency();
	}

	/*
		envelope configuration
	*/
	void setupEnvelope( unsigned synth, const Envelope::Parameter &envelope )
 	{
		for( unsigned i=0; i<NUM_PHONE; i++ )
		{
			thePhones[i].setupEnvelope( synth, envelope );
		}
	}
	const Envelope::Parameter &getEnvelope( unsigned synth ) const
 	{
		return thePhones[0].getEnvelope( synth );
	}

	/*
		osc configuration
	*/
	void setupOSC( unsigned synth, unsigned subOSC, const SubOscParameter &parameter )
	{
		for( unsigned i=0; i<NUM_PHONE; i++ )
		{
			thePhones[i].setupOSC( synth, subOSC, parameter );
		}
	}
	const SubOscParameter &getOSC( unsigned synth, unsigned subOSC ) const
	{
		return thePhones[0].getOSC( synth, subOSC );
	}
	/*
		Produce

	*/
	bool start( unsigned char note, unsigned char velocity )
	{
		bool		found = false;
		double		frequency = noteFrequencies[note&0x7F];
		clock_t		oldestRelease = 0;
		unsigned	oldestPhone;

		double		phoneVolume = double(velocity)/127.0;

		// try to find an idle phone
		if( !found )
		{
			for( unsigned i=0; i<m_maxPhones; i++ )
			{
				if( !thePhones[i].isActive() )
				{
					releaseTimes[i] = 0;
					thePhones[i].setFrequency( frequency );
					thePhones[i].setVolume( phoneVolume );
					thePhones[i].start();
					found = true;
					break;
				}
				if( !oldestRelease || (oldestRelease > releaseTimes[i] && releaseTimes[i]) )
				{
					oldestRelease = releaseTimes[i];
					oldestPhone = i;
				}
			}
		}

		// try to find the phone which is playing this note
		if( !found )
		{
			for( unsigned i=0; i<m_maxPhones; i++ )
			{
				if( thePhones[i].getFrequency() == frequency )
				{
					releaseTimes[i] = 0;
					thePhones[i].setVolume( phoneVolume );
					thePhones[i].start();
					found = true;
					break;
				}
			}
		}

		if( !found && oldestRelease )
		{
			releaseTimes[oldestPhone] = 0;
			thePhones[oldestPhone].setFrequency( frequency );
			thePhones[oldestPhone].setVolume( phoneVolume );
			thePhones[oldestPhone].start();
			found = true;
		}

		return found;
	}
	bool release( unsigned short note )
	{
		clock_t	releaseTime = clock();
		bool	found = false;
		double	frequency = noteFrequencies[note&0x7F];

		// try to find the phone which is playing this note
		for( unsigned i=0; i<m_maxPhones; i++ )
		{
			if( thePhones[i].getFrequency() == frequency )
			{
				releaseTimes[i] = releaseTime;
				thePhones[i].release();
				found = true;
			}
		}

		return found;
	}
	void stop()
	{
		for( unsigned i=0; i<NUM_PHONE; i++ )
		{
			thePhones[i].stop();
		}
	}
	T getNextSample()
	{
		StereoSample<T> sample;

		getNextSample( sample );

		return (sample.left + sample.right)/2;
	}
	void getNextSample( StereoSample<T> &sample )
	{
		StereoSample<T>		key;
		T					left, right;

		left = right = 0;

		for( unsigned i=0; i<m_maxPhones; i++ )
		{
			if( thePhones[i].isActive() )
			{
				thePhones[i].getNextSample( key );
				left += key.left;
				right += key.right;
			}
		}

/*
		if( limiter )
		{
			left >>= limiter;
			right >>= limiter;
		}
		if( abs(left) > clipValue )
		{
			size_t newBitCount = positiveBitCount(abs(left)) - positiveBitCount(clipValue);
			left >>= newBitCount;
			right >>= newBitCount;
			limiter += newBitCount;
		}
		if( abs(right) > clipValue )
		{
			size_t newBitCount = positiveBitCount(abs(right)) - positiveBitCount(clipValue);
			left >>= newBitCount;
			right >>= newBitCount;
			limiter += newBitCount;
		}
*/
		calcLimiter( left, right );
		if( limiter < 1 )
		{
			left = T(double(left) * limiter);
			right = T(double(right) * limiter);
		}
		double masterVolume = getVolume();
		if( !masterVolume )
		{
			sample.left = 0;
			sample.right = 0;
		}
		else if( masterVolume < 0.9999 || masterVolume > 1.0001 )
		{
			sample.left = T(double(left) * masterVolume);
			sample.right = T(double(right) * masterVolume);
		}
		else
		{
			sample.left = left;
			sample.right = right;
		}
	}

	/*
		current status
	*/
	unsigned getActivePorts( void ) const
	{
		unsigned result = 0;
		for( unsigned i=0; i<m_maxPhones; i++ )
		{
			if( thePhones[i].isActive() )
			{
				result |= 1<<i;
			}
		}

		return result;
	}
	unsigned getActivePhones( void ) const
	{
		unsigned int result = 0;
		for( unsigned i=0; i<m_maxPhones; i++ )
		{
			if( thePhones[i].isActive() )
			{
				result++;
			}
		}

		return result;
	}
	void setMaxPhones( unsigned maxPhones )
	{
		if( maxPhones < 1 )
		{
			m_maxPhones = 1;
		}
		else if( maxPhones > NUM_PHONE )
		{
			m_maxPhones = NUM_PHONE;
		}
		else
		{
			m_maxPhones = maxPhones;
		}
	}
	unsigned getMaxPhones( void ) const
	{
		return m_maxPhones;
	}

	std::clock_t testCPUspeed( void )
	{
		stop();

		double oldVolume = getVolume();

		SubOscParameter	newOsc = {
			0, 0.5, FlexPitch(1)
		};
		SubOscParameter	oldSubOscs[NUM_SUBOSC];
		for( unsigned i=0; i<NUM_SUBOSC; ++i )
		{
			oldSubOscs[i] = getOSC( 0, i );
			setupOSC( 0, i, newOsc );
		}

		AMlfoParameter	oldAmLfoParameter = getAM( 0 );
		AMlfoParameter	amLfoParameter = {
			oscSINUS, 1, Interval( 0, 1 )
		};
		setupAM( 0, amLfoParameter );

		FMlfoParameter	oldFmLfoParameter = getFM( 0 );
		FMlfoParameter	fmLfoParameter = {
			oscSINUS, 1, false, FlexPitch(1.0)
		};
		setupFM( 0, fmLfoParameter );

		Envelope::Parameter	oldEnvelope = getEnvelope( 0 );
		Envelope::Parameter	newEnvelope = {
			true, 0, 0, 100, 100, 0.5, 300, 500
		};

		setupEnvelope( 0, newEnvelope ); 

		start( MIDI_NOTE_C1, 127 );
		start( MIDI_NOTE_C2, 127 );
		start( MIDI_NOTE_E2, 127 );
		start( MIDI_NOTE_G2, 127 );
		clock_t	startTime = clock();
		for( unsigned long i=0; i<getSampleRate(); i++ )
		{
			getNextSample();
		}
		clock_t cpuTime = clock()-startTime;
		stop();
		unsigned maxPhones = unsigned(double(CLOCKS_PER_SEC)/double(cpuTime)/2.0);
		setMaxPhones( maxPhones );

		setVolume( oldVolume );
		for( unsigned i=0; i<NUM_SUBOSC; ++i )
		{
			setupOSC( 0, i, oldSubOscs[i] );
		}
		setupAM( 0, oldAmLfoParameter );
		setupFM( 0, oldFmLfoParameter );
		setupEnvelope( 0, oldEnvelope ); 

		return cpuTime;
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
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif

#endif