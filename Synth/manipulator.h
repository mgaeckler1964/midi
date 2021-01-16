/*
		Project:		Synthesizer Core
		Module:			manipulators.h
		Description:	Manipulators (AM, FM, envelope) for oscilators
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

#ifndef OSC_MANIPILATOR_H
#define OSC_MANIPILATOR_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include "oscilator.h"

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

// --------------------------------------------------------------------- //
// ----- macros -------------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- type definitions ---------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

/*
	an OSC manipulator is a manipulator which uses its own OSC to
	generate manipulation values
*/
template <class T>
class OscManipulator : public BaseProducer
{
	Oscilator<T>	m_source;

	public:
	/*
		setup
	*/
	void setup( T clipValue, unsigned long sampleRate )
	{
		setSampleRate( sampleRate );
		setVolume( 1 );
		m_source.setup( clipValue, sampleRate );
		m_source.setVolume( 1 );
		m_source.setRange( Interval( 0, 1 ) );
	}

	void setOscilatorType( OscilatorType newSource )
	{
		m_source.setOscilatorType( newSource );
	}
	OscilatorType getOscilatorType() const
	{
		return m_source.getOscilatorType();
	}

	virtual bool isEnabled() const
	{
		return BaseProducer::isEnabled() && m_source.isEnabled();
	}

	/*
		frequency
	*/
	virtual void setFrequency( double frequency )
	{
		BaseProducer::setFrequency( frequency );
		m_source.setFrequency( frequency );
	}
	double getFrequency( void ) const
	{
		return m_source.getFrequency();
	}

	/*
		Volume
	*/
	void setRange( const Interval &range )
	{
		m_source.setRange( range );
	}
	const Interval &getRange() const
	{
		return m_source.getRange();
	}

	/*
		Produce
	*/
	void start()
	{
		m_source.start();
	}
	double getNextValue( void )
	{
		return m_source.getNextPlainWithRange();
	}
};

/*
	the envelope does not generate samples and does not use an oscilator to produce
	manipuilation values
*/
class Envelope : public BaseProducer
{
	public:
	struct Parameter
	{
		bool			enabled;
		unsigned short	delayMS;
		double			initialVolume;
		unsigned short	attackMS;
		unsigned short	decayMS;
		double			decayVolume;
		unsigned short	sustainRateMS;
		unsigned short	releaseRateMS;
	};

	private:
	Parameter		m_parameter;

	unsigned long	delay;

	unsigned long	attack;
	double			attackIncrement;

	unsigned long	decay;
	double			m_decayDecrement;

	double			sustainRate;

	double			releaseRate;

	unsigned		timeCode;
	double			currentValue;
	bool			m_keyPressed;
	double			m_minValue;

	public:
	/*
		setup
	*/
	Envelope()
	{
		disable();
	}
	void setup( unsigned sampleWidth, unsigned long sampleRate, const Parameter &envelop )
	{
		m_parameter = envelop;

		if( m_parameter.enabled )
		{
			enable();
		}
		else
		{
			disable();
		}

		m_minValue = 1.0 / double(1 << (sampleWidth-1));
		setSampleRate( sampleRate );
		delay = ms2sampleCount( sampleRate, m_parameter.delayMS );
		attack = ms2sampleCount( sampleRate, m_parameter.attackMS );
		attackIncrement = (1.0-m_parameter.initialVolume) / double(attack);
		attack += delay;

		decay = ms2sampleCount( sampleRate, m_parameter.decayMS );
		m_decayDecrement = (1.0-m_parameter.decayVolume) / double(decay+1);
		decay += attack;

		if( m_parameter.sustainRateMS == -1 )
		{
			sustainRate = 1;
		}
		else if( m_parameter.sustainRateMS == 0 )
		{
			sustainRate = 0;
		}
		else
		{
			sustainRate = pow( 0.5, 1.0/double(ms2sampleCount( sampleRate, m_parameter.sustainRateMS )) );
		}

		if( m_parameter.releaseRateMS == -1 )
		{
			releaseRate =  1;
		}
		else if( m_parameter.releaseRateMS == 0 )
		{
			releaseRate =  0;
		}
		else
		{
			releaseRate = pow( 0.5, 1.0/double(ms2sampleCount( sampleRate, m_parameter.releaseRateMS )) );
		}

		m_keyPressed = false;
	}
	const Parameter &get( void ) const
	{
		return m_parameter;
	}

	/*
		Produce
	*/
	void start( void )
	{
		timeCode = 0;
		currentValue = 0;
		m_keyPressed = true;
	}
	void release( void )
	{
		m_keyPressed = false;
	}
	void stop( void )
	{
		m_keyPressed = false;
		currentValue = 0;
	}
	bool isActive() const
	{
		return m_parameter.enabled && (m_keyPressed || currentValue >= m_minValue);
	} 
	bool isEnabled() const
	{
		return m_parameter.enabled;
	} 
	double getNextValue( void )
	{
		double	newValue;

		if( !m_keyPressed )
		{
			newValue = currentValue * releaseRate;
		}
		else if( timeCode < delay )
		{
			newValue = 0;
		}
		else if( timeCode == delay && timeCode < attack )
		{
			newValue = m_parameter.initialVolume;
		}
		else if( timeCode < attack )
		{
			newValue = currentValue + attackIncrement;
		}
		else if( timeCode == attack )
		{
			newValue = 1;
		}
		else if( timeCode < decay )
		{
			newValue = currentValue - m_decayDecrement;
		}
		else
		{
			newValue = currentValue * sustainRate;
			if( newValue < m_minValue )
			{
				m_keyPressed = false;
			}
		}

		timeCode++;
		currentValue = newValue;
		return newValue;
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