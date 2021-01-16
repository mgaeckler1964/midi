/*
		Project:		Synthesizer Core
		Module:			oscilator.h
		Description:	Oscilators (SINUS etc.) for the synthesizer
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

#ifndef OSCILATOR_H
#define OSCILATOR_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

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

#undef M_PI

const double M_PI = 3.14159265358979323846;

// --------------------------------------------------------------------- //
// ----- macros -------------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- type definitions ---------------------------------------------- //
// --------------------------------------------------------------------- //

typedef unsigned long timeCode_t;

struct Interval
{
	double	minimum, maximum;

	Interval(double	minimum = -1, double maximum=1) : minimum(minimum), maximum(maximum)
	{
	}
};

enum OscilatorType 
{ 
	oscUNKNOWN = -1, oscSINUS, oscASC_SAW, oscDES_SAW, oscTRIANGLE, oscRECTANGLE 
};


// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

class BaseProducer
{
	private:
	timeCode_t	m_sampleRate;
	timeCode_t	m_flankLength;
	double		m_volume;

	public:
	BaseProducer()
	{
		m_volume = 0;
		m_flankLength = m_sampleRate = 0;
	}
	virtual ~BaseProducer()
	{
	}

	static timeCode_t ms2sampleCount( timeCode_t sampleRate, unsigned long ms )
	{
		return sampleRate * ms / 1000;
	}

	/*
		setup
	*/
	virtual void setSampleRate( timeCode_t sampleRate )
	{
		m_sampleRate = sampleRate;
		m_flankLength = ms2sampleCount( getSampleRate(), 1 )/2;
	}
	timeCode_t getSampleRate( void ) const
	{
		return m_sampleRate;
	}
	virtual bool isEnabled( void ) const
	{
		return m_sampleRate != 0 && m_volume != 0;
	}
	void disable()
	{
		m_volume = 0;
	}
	void enable()
	{
		m_volume = 1;
	}

	/*
		frequency
	*/
	virtual void setFrequency( double /* frequency */ )
	{
	}

	/*
		Volume
	*/
	void setVolume( double volume )
	{
		m_volume = volume;
	}
	double getVolume( void ) const
	{
		return m_volume;
	}

	/*
		Flank
	*/
	timeCode_t getFlank() const
	{
		return m_flankLength;
	}
};

/*
	this is the base clas for an oscilator

	the clip value is the max positive aplitude, the Oscilator
	should produce.
*/
template <class T=long> 
class Oscilator : public BaseProducer
{
	static const timeCode_t	maxFlankFraction = 16;

	private:
	typedef double (Oscilator<T>::*PlainFuncPtr)( void ) const;

	timeCode_t		m_numSamples,		// # of samples per one wave
					m_numSamples2;		// # of samples per half wave

	double			m_numSamplesDbl,
					m_numSamples2Dbl;

	double			m_frequency;

	T				m_clipValue;
	double			m_clipValueDbl;
	double			m_volumeFactor,		// calculated based on presence and frequency 
					m_presenceValue;

	timeCode_t		m_timeCode;			// last sample
	double			m_phase;
	timeCode_t		m_startCode;

	OscilatorType	m_theOscilatorType;
	PlainFuncPtr	m_thePlainFunc;
	Interval		m_nativeRange,
					m_range;
	double			m_rangeFactor,
					m_rangeOffset;

	private:
	timeCode_t calcStartCode( void )
	{
		m_startCode = timeCode_t(m_phase * m_numSamplesDbl / (2*M_PI) +0.5);
		if( m_numSamples && m_startCode >= m_numSamples )
		{
			m_startCode = m_startCode % m_numSamples;
		}
		return m_startCode;
	}
	void calcVolumeFactor( double volume, double frequency, double presenceValue )
	{
		m_volumeFactor = m_clipValueDbl * volume;
		if( frequency && presenceValue )
		{
			double tmpFactor = pow(440.0,presenceValue)/pow(frequency, presenceValue);
			if( tmpFactor < 64 )
			{
				m_volumeFactor *= tmpFactor;
			}
			else
			{
				m_volumeFactor = 0;		// avoid clipping
			}
		}
	}
	public:
	Oscilator()
	{
		m_frequency = 0;
		m_numSamples = 0;
		m_numSamples2 = 0;
		m_numSamplesDbl = 0;
		m_numSamples2Dbl = 0;

		m_clipValue = 0;
		m_clipValueDbl = 0;
		m_volumeFactor = 0;
		m_presenceValue = 0;

		m_startCode = 0;
		m_phase = 0;

		setOscilatorType( oscSINUS );
	}

	/*
		setup
	*/
	void setup( T clipValue, timeCode_t sampleRate )
	{
		m_clipValue = clipValue;
		m_clipValueDbl = double( clipValue );

		setSampleRate( sampleRate );

		m_timeCode = 0;

		setVolume( 0 );

		m_range.minimum = -1;
		m_range.maximum = 1;
		setOscilatorType( oscSINUS );
		m_presenceValue = 0;
	}

	private:
	void calculateRange( const Interval &range )
	{
		m_rangeFactor = (range.maximum-range.minimum) / (m_nativeRange.maximum-m_nativeRange.minimum);
		m_rangeOffset = range.minimum-m_nativeRange.minimum*m_rangeFactor;
	}
	public:
	void setRange( const Interval &range )
	{
		m_range = range;
		calculateRange( range );
	}
	const Interval &getRange( void ) const
	{
		return m_range;
	}

	/*
		frequency
	*/
	protected:
	timeCode_t getNumSamples( void ) const
	{
		return m_numSamples;
	}
	double getNumSamplesDbl( void ) const
	{
		return m_numSamplesDbl;
	}

	public:
	void setFrequency( double frequency )
	{
		m_frequency = frequency;
		if( frequency && m_presenceValue )
		{
			calcVolumeFactor( getVolume(), frequency, m_presenceValue );
		}

		if( frequency )
		{
			double ratio = double(m_timeCode) / m_numSamplesDbl;

			m_numSamplesDbl = double(getSampleRate())/frequency;
			m_numSamples2Dbl = m_numSamplesDbl/2;
			m_numSamples = timeCode_t(m_numSamplesDbl);
			m_numSamples2 = m_numSamples/2;

			if( m_numSamples > 1 )
			{
				m_timeCode = timeCode_t(m_numSamplesDbl * ratio + 0.5);
			}
			else
			{
				m_numSamplesDbl = 0;
				m_numSamples = 0;
				m_frequency = 0;
			}
		}
		else
		{
			m_numSamplesDbl = 0;
			m_numSamples = 0;
		}
		calcStartCode();
	}
	void setPhase( double newPhase )
	{
		m_phase = newPhase;
		m_timeCode += calcStartCode();
		if( m_numSamples && m_timeCode >= m_numSamples )
		{
			m_timeCode = m_timeCode % m_numSamples;
		}
	}
	double getPhase() const
	{
		return m_phase;
	}
	double getFrequency( void ) const
	{
		return m_frequency;
	}
	bool isEnabled( void ) const
	{
		return m_frequency != 0.0 && BaseProducer::isEnabled();
	}

	/*
		Volume
	*/
	T getClipValue( void ) const
	{
		return clipValue;
	}
	void setVolume( double volume )
	{
		BaseProducer::setVolume( volume );
		calcVolumeFactor( volume, m_frequency, m_presenceValue );
	}
	void setPresenceValue( double newPresenceValue )
	{
		m_presenceValue = newPresenceValue;
		calcVolumeFactor( getVolume(), m_frequency, newPresenceValue );
	}
	double getPresenceValue() const
	{
		return m_presenceValue;
	}

	/*
		Produce
	*/

	private:
	timeCode_t getTimeCode( void ) const
	{
		return m_timeCode;
	}
	double getPlain() const
	{
		return (this->*m_thePlainFunc)();
	}
	double getPlainWithRange()
	{
		double nextVal = getPlain();
		if( m_rangeFactor != 1 )
		{
			nextVal *= m_rangeFactor;
		}
		if( m_rangeOffset )
		{
			nextVal += m_rangeOffset;
		}
		return nextVal;
	}
	public:

	void start()
	{
		m_timeCode = m_startCode;
	}

	double getNextPlainWithRange()
	{
		double nextVal = getPlainWithRange();
		if( ++m_timeCode >= m_numSamples )
		{
			m_timeCode -= m_numSamples;
		}

		return nextVal;
	}

	T getNextSample()
	{
		return T(getNextPlainWithRange() * m_volumeFactor);
	}
/*
	here are all possible types of oscilators we are using
*/
	void setOscilatorType( OscilatorType type )
	{
		m_theOscilatorType = type;
		switch( type )
		{
			case oscSINUS:
				m_thePlainFunc = &Oscilator<T>::getSinus;
				break;
			case oscASC_SAW:
				m_thePlainFunc = &Oscilator<T>::getAscSaw;
				break;
			case oscDES_SAW:
				m_thePlainFunc = &Oscilator<T>::getDescSaw;
				break;
			case oscTRIANGLE:
				m_thePlainFunc = &Oscilator<T>::getTriangle;
				break;
			case oscRECTANGLE:
				m_thePlainFunc = &Oscilator<T>::getRectangle;
				break;
		}
		calculateNativeRange();
		calculateRange( m_range );
	}
	OscilatorType getOscilatorType() const
	{
		return theOscilatorType;
	}

	private:
	double getSinus() const
	{
		return sin( 2.0*M_PI* double(getTimeCode()) / getNumSamplesDbl() );
	}
	double getAscSaw() const
	{
		const timeCode_t	flank = getFlank();
		const timeCode_t	timeCode = getTimeCode();
		const timeCode_t	numSamples = getNumSamples();

		if( !flank || flank * maxFlankFraction > numSamples )
			return double(timeCode) / getNumSamplesDbl();
		else if( timeCode > numSamples-flank )
			return double(numSamples-timeCode) / double( flank );
		else
			return double(timeCode) / double(numSamples-flank);
	}
	double getDescSaw() const
	{
		const timeCode_t	flank = getFlank();
		const timeCode_t	timeCode = getTimeCode();
		const timeCode_t	numSamples = getNumSamples();

		if( !flank || flank * maxFlankFraction > numSamples )
			return double(numSamples-timeCode) / getNumSamplesDbl();
		else if( timeCode < flank )
			return double(timeCode)/double(flank);
		else
			return double(numSamples-timeCode) / double(numSamples-flank);
	}
	double getRectangle() const
	{
		const timeCode_t	flank = getFlank();
		const timeCode_t	timeCode = getTimeCode();
		const timeCode_t	numSamples = getNumSamples();

		if( !flank || flank * maxFlankFraction > numSamples )
			return timeCode < m_numSamples2 ? 1.0 : 0.0;
		else if( timeCode < flank )
			return double(timeCode)/double(flank);
		else if( timeCode < m_numSamples2 )
			return 1.0;
		else if( timeCode < m_numSamples2+flank )
			return double(m_numSamples2+flank-timeCode) / double( flank );
		else
			return 0;
	}
	double getTriangle() const
	{
		return getTimeCode() < m_numSamples2 
			? double(getTimeCode()) / m_numSamples2Dbl 
			: double(getNumSamples() - getTimeCode() ) / m_numSamples2Dbl;
	}

	void calculateNativeRange( void )
	{
		if( m_theOscilatorType == oscSINUS )
		{
	 		m_nativeRange.minimum = -1;
			m_nativeRange.maximum = 1;
		}
		else
		{
	 		m_nativeRange.minimum = 0;
			m_nativeRange.maximum = 1;
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
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif

#endif