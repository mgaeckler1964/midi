/*
		Project:		MG Analog Synthesizer
		Module:			vstVoice.cpp
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

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include "vstVoice.h"

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

static const uint64 currentParamStateVersion = 0;

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

// --------------------------------------------------------------------- //
// ----- class protected ----------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class virtuals ------------------------------------------------ //
// --------------------------------------------------------------------- //
   
// --------------------------------------------------------------------- //
// ----- class publics ------------------------------------------------- //
// --------------------------------------------------------------------- //

//-----------------------------------------------------------------------------

tresult VoiceParameterState::setState( IBStreamer &s )
{
	int8 enable;
	if( !s.readInt8(enable) ) 
	{
		return kResultFalse;
	}
	this->enable = bool(enable);

	if( !s.readDouble(synthVolume) ) 
	{
		return kResultFalse;
	}
	if( !s.readDouble(synthPan) ) 
	{
		return kResultFalse;
	}
	if( !s.readInt16(synthPitch.pitch) )
	{
		return kResultFalse;
	}
	if( !s.readDouble(synthPhase) ) 
	{
		return kResultFalse;
	}
	for( int i=0; i<DEF_NUM_SUBOSCS; ++i )
	{
		if( !s.readDouble(subOscParameter[i].oscPhase) ) 
		{
			return kResultFalse;
		}
		if( !s.readDouble(subOscParameter[i].oscVolumeFactor) ) 
		{
			return kResultFalse;
		}
		if( !s.readDouble(subOscParameter[i].oscPitch.pitch) ) 
		{
			return kResultFalse;
		}
	}

	int8 amType;
	if( !s.readInt8(amType) ) 
	{
		return kResultFalse;
	}
	amLfoParameter.type = OscilatorType(amType);

	if( !s.readDouble(amLfoParameter.frequency) ) 
	{
		return kResultFalse;
	}
	if( !s.readDouble(amLfoParameter.range.minimum) ) 
	{
		return kResultFalse;
	}

	int8 fmType;
	if( !s.readInt8(fmType) ) 
	{
		return kResultFalse;
	}
	fmLfoParameter.type = OscilatorType(fmType);

	int8 fmRelative;
	if( !s.readInt8(fmRelative) ) 
	{
		return kResultFalse;
	}
	fmLfoParameter.isFactor = bool(fmRelative);

	if( !s.readDouble(fmLfoParameter.frequency) ) 
	{
		return kResultFalse;
	}
	if( !s.readDouble(fmLfoParameter.range.pitch) ) 
	{
		return kResultFalse;
	}
	int8 enableEnvelope;
	if( !s.readInt8(enableEnvelope) ) 
	{
		return kResultFalse;
	}
	envelope.enabled = bool(enableEnvelope);

	if( !s.readInt16u(envelope.delayMS) ) 
	{
		return kResultFalse;
	}
	if( !s.readDouble(envelope.initialVolume) ) 
	{
		return kResultFalse;
	}
	if( !s.readInt16u(envelope.attackMS) ) 
	{
		return kResultFalse;
	}
	if( !s.readInt16u(envelope.decayMS) ) 
	{
		return kResultFalse;
	}
	if( !s.readDouble(envelope.decayVolume) ) 
	{
		return kResultFalse;
	}
	if( !s.readInt16u(envelope.sustainRateMS) ) 
	{
		return kResultFalse;
	}
	if( !s.readInt16u(envelope.releaseRateMS) ) 
	{
		return kResultFalse;
	}

	return kResultTrue;
}

tresult GlobalParameterState::setState( IBStream *stream )
{
	IBStreamer s( stream, kLittleEndian );
	uint64 version = 0;

	if( !s.readInt64u(version) ) 
	{
		return kResultFalse;
	}
	if( !s.readDouble(masterVolume) ) 
	{
		return kResultFalse;
	}
	if( !s.readDouble(masterPresence) ) 
	{
		return kResultFalse;
	}

	for( size_t i=0; i<DEF_NUM_SYNTHS; ++i )
	{
		if( voiceParams[i].setState( s ) == kResultFalse )
		{
			return kResultFalse;
		}
	}

	return kResultTrue;
}

//-----------------------------------------------------------------------------
tresult VoiceParameterState::getState( IBStreamer &s )
{
	if( !s.writeInt8(int8(enable)) ) 
	{
		return kResultFalse;
	}
	if( !s.writeDouble(synthVolume) ) 
	{
		return kResultFalse;
	}
	if( !s.writeDouble(synthPan) ) 
	{
		return kResultFalse;
	}
	if( !s.writeInt16(synthPitch.pitch) )
	{
		return kResultFalse;
	}
	if( !s.writeDouble(synthPhase) ) 
	{
		return kResultFalse;
	}
	for( int i=0; i<DEF_NUM_SUBOSCS; ++i )
	{
		if( !s.writeDouble(subOscParameter[i].oscPhase) ) 
		{
			return kResultFalse;
		}
		if( !s.writeDouble(subOscParameter[i].oscVolumeFactor) ) 
		{
			return kResultFalse;
		}
		if( !s.writeDouble(subOscParameter[i].oscPitch.pitch) ) 
		{
			return kResultFalse;
		}
	}
	if( !s.writeInt8(int8(amLfoParameter.type)) ) 
	{
		return kResultFalse;
	}
	if( !s.writeDouble(amLfoParameter.frequency) ) 
	{
		return kResultFalse;
	}
	if( !s.writeDouble(amLfoParameter.range.minimum) ) 
	{
		return kResultFalse;
	}
	if( !s.writeInt8(int8(fmLfoParameter.type)) ) 
	{
		return kResultFalse;
	}
	if( !s.writeInt8(int8(fmLfoParameter.isFactor)) ) 
	{
		return kResultFalse;
	}
	if( !s.writeDouble(fmLfoParameter.frequency) ) 
	{
		return kResultFalse;
	}
	if( !s.writeDouble(fmLfoParameter.range.pitch) ) 
	{
		return kResultFalse;
	}
	if( !s.writeInt8(int8(envelope.enabled)) ) 
	{
		return kResultFalse;
	}
	if( !s.writeInt16u(envelope.delayMS) ) 
	{
		return kResultFalse;
	}
	if( !s.writeDouble(envelope.initialVolume) ) 
	{
		return kResultFalse;
	}
	if( !s.writeInt16u(envelope.attackMS) ) 
	{
		return kResultFalse;
	}
	if( !s.writeInt16u(envelope.decayMS) ) 
	{
		return kResultFalse;
	}
	if( !s.writeDouble(envelope.decayVolume) ) 
	{
		return kResultFalse;
	}
	if( !s.writeInt16u(envelope.sustainRateMS) ) 
	{
		return kResultFalse;
	}
	if( !s.writeInt16u(envelope.releaseRateMS) ) 
	{
		return kResultFalse;
	}

	return kResultTrue;
}

tresult GlobalParameterState::getState (IBStream* stream)
{
	IBStreamer s (stream, kLittleEndian);

	if( !s.writeInt64u( currentParamStateVersion ) )
	{
		return kResultFalse;
	}
	
	if( !s.writeDouble(masterVolume) ) 
	{
		return kResultFalse;
	}
	if( !s.writeDouble(masterPresence) ) 
	{
		return kResultFalse;
	}

	for( size_t i=0; i<DEF_NUM_SYNTHS; ++i )
	{
		if( voiceParams[i].getState( s ) == kResultFalse )
		{
			return kResultFalse;
		}
	}

	return kResultTrue;
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

