/*
		Project:		MG Analog Synthesizer
		Module:			vstEditor.cpp
		Description:	Wrapper between the GUI and the host
						registers all paramters within the host
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

#include <gak/fmtNumber.h>
#include <gak/numericString.h>
#include <gak/wideString.h>

#include "pluginterfaces/base/ustring.h"
#include "pluginterfaces/vst/ivstmidicontrollers.h"

#include "vstEditor.h"
#include "vstController.h"
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

FUID	Controller::cid (0x00FFEEDD, 0xCCBBAA99, 0x88776655, 0x44332211);

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

tresult PLUGIN_API Controller::initialize (FUnknown* context)
{
	tresult result = EditController::initialize( context );
	if( result == kResultTrue )
	{
		addUnit( new Unit( USTRING( "MG Synth" ), suiRoot, kNoProgramListId ) );
		addUnit( new Unit( USTRING( "Synth A" ), suiSynthA ) );
			addUnit( new Unit( USTRING( "Synth A OSCs" ),		suiSynthA_OSCs,		suiSynthA ) );
			addUnit( new Unit( USTRING( "Synth A AM LFO" ),		suiSynthA_AM_LFO,	suiSynthA ) );
			addUnit( new Unit( USTRING( "Synth A AM LFO" ),		suiSynthA_FM_LFO,	suiSynthA ) );
			addUnit( new Unit( USTRING( "Synth A Envelope" ),	suiSynthA_ENVELOPE,	suiSynthA ) );
		addUnit( new Unit( USTRING( "Synth B" ), suiSynthB ) );
			addUnit( new Unit( USTRING( "Synth B OSCs" ),		suiSynthB_OSCs,		suiSynthB ) );
			addUnit( new Unit( USTRING( "Synth B AM LFO" ),		suiSynthB_AM_LFO,	suiSynthB ) );
			addUnit( new Unit( USTRING( "Synth B AM LFO" ),		suiSynthB_FM_LFO,	suiSynthB ) );
			addUnit( new Unit( USTRING( "Synth B Envelope" ),	suiSynthB_ENVELOPE,	suiSynthB ) );
		addUnit( new Unit( USTRING( "Synth C" ), suiSynthC ) );
			addUnit( new Unit( USTRING( "Synth C OSCs" ),		suiSynthC_OSCs,		suiSynthC ) );
			addUnit( new Unit( USTRING( "Synth C AM LFO" ),		suiSynthC_AM_LFO,	suiSynthC ) );
			addUnit( new Unit( USTRING( "Synth C AM LFO" ),		suiSynthC_FM_LFO,	suiSynthC ) );
			addUnit( new Unit( USTRING( "Synth C Envelope" ),	suiSynthC_ENVELOPE,	suiSynthC ) );
		addUnit( new Unit( USTRING( "Synth D" ), suiSynthD ) );
			addUnit( new Unit( USTRING( "Synth D OSCs" ),		suiSynthD_OSCs,		suiSynthD ) );
			addUnit( new Unit( USTRING( "Synth D AM LFO" ),		suiSynthD_AM_LFO,	suiSynthD ) );
			addUnit( new Unit( USTRING( "Synth D AM LFO" ),		suiSynthD_FM_LFO,	suiSynthD ) );
			addUnit( new Unit( USTRING( "Synth D Envelope" ),	suiSynthD_ENVELOPE,	suiSynthD ) );

		// Init parameters
		Parameter *param;
		
		param = new RangeParameter( USTRING("Master Volume"), spiMasterVolume, USTRING("%"), 0, 100, 100 );
		param->setPrecision( 1 );
		param->setUnitID( suiRoot );
		parameters.addParameter( param );

		param = new RangeParameter( USTRING("Presence"), spiMasterPresence, USTRING(""), -2, +2, 0 );
		param->setPrecision( 1 );
		param->setUnitID( suiRoot );
		parameters.addParameter( param );

		parameters.addParameter( USTRING("Synth A"), 0, 1, 0, ParameterInfo::kCanAutomate, spiSynthEnabled1, suiSynthA );
		parameters.addParameter( USTRING("Synth B"), 0, 1, 0, ParameterInfo::kCanAutomate, spiSynthEnabled2, suiSynthB );
		parameters.addParameter( USTRING("Synth C"), 0, 1, 0, ParameterInfo::kCanAutomate, spiSynthEnabled3, suiSynthC );
		parameters.addParameter( USTRING("Synth D"), 0, 1, 0, ParameterInfo::kCanAutomate, spiSynthEnabled4, suiSynthD );

		param = new RangeParameter( USTRING("A: Volume"), spiSynthVolume1, USTRING("%"), 0, 100, 100 );
		param->setPrecision( 1 );
		param->setUnitID( suiSynthA_OSCs );
		parameters.addParameter( param );
		param = new RangeParameter( USTRING("A: Pan"), spiSynthPan1, USTRING(""), -100, 100, 0 );
		param->setPrecision( 0 );
		param->setUnitID( suiSynthA_OSCs );
		parameters.addParameter( param );
		param = new RangeParameter( USTRING("A: Pitch"), spiSynthPitch1, USTRING(""), -MAX_PITCH, MAX_PITCH, 0 );
		param->setPrecision( 0 );
		param->setUnitID( suiSynthA_OSCs );
		parameters.addParameter( param );
		param = new RangeParameter( USTRING("A: Phase"), spiSynthPhase1, USTRING(L"°"), 0, 360, 0 );
		param->setPrecision( 1 );
		param->setUnitID( suiSynthA_OSCs );
		parameters.addParameter( param );

		param = new RangeParameter( USTRING("A: SubOSC 1 Phase"), spiSub1Phase1, USTRING(L"°"), 0, 360, 0 );
		param->setPrecision( 1 );
		param->setUnitID( suiSynthA_OSCs );
		parameters.addParameter( param );
		param = new RangeParameter( USTRING("A: SubOSC 1 Vol."), spiSub1Volume1, USTRING("%"), 0, 100, 0 );
		param->setPrecision( 0 );
		param->setUnitID( suiSynthA_OSCs );
		parameters.addParameter( param );
		param = new RangeParameter( USTRING("A: SubOSC 1 Pitch"), spiSub1Pitch1, USTRING(""), -MAX_PITCH, MAX_PITCH, 0 );
		param->setPrecision( 2 );
		param->setUnitID( suiSynthA_OSCs );
		parameters.addParameter( param );
		param = new RangeParameter( USTRING("A: SubOSC 2 Phase"), spiSub2Phase1, USTRING(L"°"), 0, 360, 0 );
		param->setPrecision( 1 );
		param->setUnitID( suiSynthA_OSCs );
		parameters.addParameter( param );
		param = new RangeParameter( USTRING("A: SubOSC 2 Vol."), spiSub2Volume1, USTRING("%"), 0, 100, 0 );
		param->setPrecision( 0 );
		param->setUnitID( suiSynthA_OSCs );
		parameters.addParameter( param );
		param = new RangeParameter( USTRING("A: SubOSC 2 Pitch"), spiSub2Pitch1, USTRING(""), -MAX_PITCH, MAX_PITCH, 0 );
		param->setPrecision( 2 );
		param->setUnitID( suiSynthA_OSCs );
		parameters.addParameter( param );
		StringListParameter* filterTypeParam = new StringListParameter (USTRING("A: AM LFO Type"), spiAMtype1);
		filterTypeParam->appendString(USTRING("Sine"));
		filterTypeParam->appendString(USTRING("Ascending"));
		filterTypeParam->appendString(USTRING("Descending"));
		filterTypeParam->appendString(USTRING("Triangle"));
		filterTypeParam->appendString(USTRING("Square"));
		filterTypeParam->setUnitID( suiSynthA_AM_LFO );
		parameters.addParameter( filterTypeParam );
		param = new RangeParameter( USTRING("A: AM Frequency"), spiAMfrequency1, USTRING("Hz"), 0, MAX_LFO_FREQUENCY, 0 );
		param->setPrecision( 1 );
		param->setUnitID( suiSynthA_AM_LFO );
		parameters.addParameter( param );
		param = new RangeParameter( USTRING("A: AM Range"), spiAMrange1, USTRING("%"), 0, 100, 0 );
		param->setPrecision( 1 );
		param->setUnitID( suiSynthA_AM_LFO );
		parameters.addParameter( param );
		filterTypeParam = new StringListParameter (USTRING("A: FM LFO Type"), spiFMtype1);
		filterTypeParam->appendString(USTRING("Sine"));
		filterTypeParam->appendString(USTRING("Ascending"));
		filterTypeParam->appendString(USTRING("Descending"));
		filterTypeParam->appendString(USTRING("Triangle"));
		filterTypeParam->appendString(USTRING("Square"));
		filterTypeParam->setUnitID( suiSynthA_FM_LFO );
		parameters.addParameter( filterTypeParam );
		parameters.addParameter(USTRING("A: FM relative"), 0, 1, 0, ParameterInfo::kCanAutomate, spiFMrelative1, suiSynthA_FM_LFO );
		param = new RangeParameter( USTRING("A: FM Frequency"), spiFMfrequency1, USTRING("Hz"), 0, MAX_LFO_FREQUENCY, 0 );
		param->setPrecision( 1 );
		param->setUnitID( suiSynthA_FM_LFO );
		parameters.addParameter( param );
		param = new RangeParameter( USTRING("A: FM Range"), spiFMrange1, USTRING(""), 0, MAX_PITCH, 0 );
		param->setPrecision( 1 );
		param->setUnitID( suiSynthA_FM_LFO );
		parameters.addParameter( param );
		parameters.addParameter( USTRING("A: Envelope"), 0, 1, 0, ParameterInfo::kCanAutomate, spiEnvelope1, suiSynthA_ENVELOPE );
		param = new RangeParameter( USTRING("A: Delay"), spiDelayMS1, USTRING("ms"), 0, 10000, 0 );
		param->setPrecision( 0 );
		param->setUnitID( suiSynthA_ENVELOPE );
		parameters.addParameter( param );
		param = new RangeParameter( USTRING("A: Init. Vol."), spiInitalVolume1, USTRING("%"), 0, 100, 0 );
		param->setPrecision( 1 );
		param->setUnitID( suiSynthA_ENVELOPE );
		parameters.addParameter( param );
		param = new RangeParameter( USTRING("A: Attack"), spiAttackMS1, USTRING("ms"), 0, 10000, 0 );
		param->setPrecision( 0 );
		param->setUnitID( suiSynthA_ENVELOPE );
		parameters.addParameter( param );
		param = new RangeParameter( USTRING("A: Decay"), spiDecayMS1, USTRING("ms"), 0, 10000, 0 );
		param->setPrecision( 0 );
		param->setUnitID( suiSynthA_ENVELOPE );
		parameters.addParameter( param );
		param = new RangeParameter( USTRING("A: Dec. Vol."), spiDecayVolume1, USTRING("%"), 0, 100, 0 );
		param->setPrecision( 1 );
		param->setUnitID( suiSynthA_ENVELOPE );
		parameters.addParameter( param );
		param = new RangeParameter( USTRING("A: Sustain"), spiSustainRateMS1, USTRING("ms"), 0, 10000, 0 );
		param->setPrecision( 0 );
		param->setUnitID( suiSynthA_ENVELOPE );
		parameters.addParameter( param );
		param = new RangeParameter( USTRING("A: Release"), spiReleaseRateMS1, USTRING("ms"), 0, 10000, 0 );
		param->setPrecision( 0 );
		param->setUnitID( suiSynthA_ENVELOPE );
		parameters.addParameter( param );

		param = new RangeParameter( USTRING("B: Volume"), spiSynthVolume2, USTRING("%"), 0, 100, 100 );
		param->setPrecision( 1 );
		param->setUnitID( suiSynthB_OSCs );
		parameters.addParameter( param );
		param = new RangeParameter( USTRING("B: Pan"), spiSynthPan2, USTRING(""), -100, 100, 0 );
		param->setPrecision( 0 );
		param->setUnitID( suiSynthB_OSCs );
		parameters.addParameter( param );
		param = new RangeParameter( USTRING("B: Pitch"), spiSynthPitch2, USTRING(""), -MAX_PITCH, MAX_PITCH, 0 );
		param->setPrecision( 0 );
		param->setUnitID( suiSynthB_OSCs );
		parameters.addParameter( param );
		param = new RangeParameter( USTRING("B: Phase"), spiSynthPhase2, USTRING(L"°"), 0, 360, 0 );
		param->setPrecision( 1 );
		param->setUnitID( suiSynthB_OSCs );
		parameters.addParameter( param );
		param = new RangeParameter( USTRING("B: SubOSC 1 Phase"), spiSub1Phase2, USTRING(L"°"), 0, 360, 0 );
		param->setPrecision( 1 );
		param->setUnitID( suiSynthB_OSCs );
		parameters.addParameter( param );
		param = new RangeParameter( USTRING("B: SubOSC 1 Vol."), spiSub1Volume2, USTRING("%"), 0, 100, 0 );
		param->setPrecision( 0 );
		param->setUnitID( suiSynthB_OSCs );
		parameters.addParameter( param );
		param = new RangeParameter( USTRING("B: SubOSC 1 Pitch"), spiSub1Pitch2, USTRING(""), -MAX_PITCH, MAX_PITCH, 0 );
		param->setPrecision( 2 );
		param->setUnitID( suiSynthB_OSCs );
		parameters.addParameter( param );
		param = new RangeParameter( USTRING("B: SubOSC 2 Phase"), spiSub2Phase2, USTRING(L"°"), 0, 360, 0 );
		param->setPrecision( 1 );
		param->setUnitID( suiSynthB_OSCs );
		parameters.addParameter( param );
		param = new RangeParameter( USTRING("B: SubOSC 2 Vol."), spiSub2Volume2, USTRING("%"), 0, 100, 0 );
		param->setPrecision( 0 );
		param->setUnitID( suiSynthB_OSCs );
		parameters.addParameter( param );
		param = new RangeParameter( USTRING("B: SubOSC 2 Pitch"), spiSub2Pitch2, USTRING(""), -MAX_PITCH, MAX_PITCH, 0 );
		param->setPrecision( 2 );
		param->setUnitID( suiSynthB_OSCs );
		parameters.addParameter( param );
		filterTypeParam = new StringListParameter (USTRING("B: AM LFO Type"), spiAMtype2);
		filterTypeParam->appendString(USTRING("Sine"));
		filterTypeParam->appendString(USTRING("Ascending"));
		filterTypeParam->appendString(USTRING("Descending"));
		filterTypeParam->appendString(USTRING("Triangle"));
		filterTypeParam->appendString(USTRING("Square"));
		filterTypeParam->setUnitID( suiSynthB_AM_LFO );
		parameters.addParameter( filterTypeParam );
		param = new RangeParameter( USTRING("B: AM Frequency"), spiAMfrequency2, USTRING("Hz"), 0, MAX_LFO_FREQUENCY, 0 );
		param->setPrecision( 1 );
		param->setUnitID( suiSynthB_AM_LFO );
		parameters.addParameter( param );
		param = new RangeParameter( USTRING("B: AM Range"), spiAMrange2, USTRING("%"), 0, 100, 0 );
		param->setPrecision( 1 );
		param->setUnitID( suiSynthB_AM_LFO );
		parameters.addParameter( param );
		filterTypeParam = new StringListParameter (USTRING("B: FM LFO Type"), spiFMtype2);
		filterTypeParam->appendString(USTRING("Sine"));
		filterTypeParam->appendString(USTRING("Ascending"));
		filterTypeParam->appendString(USTRING("Descending"));
		filterTypeParam->appendString(USTRING("Triangle"));
		filterTypeParam->appendString(USTRING("Square"));
		filterTypeParam->setUnitID( suiSynthB_FM_LFO );
		parameters.addParameter( filterTypeParam );
		parameters.addParameter( USTRING("B: FM relative"), 0, 1, 0, ParameterInfo::kCanAutomate, spiFMrelative2, suiSynthB_FM_LFO );
		param = new RangeParameter( USTRING("B: FM Frequency"), spiFMfrequency2, USTRING("Hz"), 0, MAX_LFO_FREQUENCY, 0 );
		param->setPrecision( 1 );
		param->setUnitID( suiSynthB_FM_LFO );
		parameters.addParameter( param );
		param = new RangeParameter( USTRING("B: FM Range"), spiFMrange2, USTRING(""), 0, MAX_PITCH, 0 );
		param->setPrecision( 1 );
		param->setUnitID( suiSynthB_FM_LFO );
		parameters.addParameter( param );
		parameters.addParameter( USTRING("B: Envelope"), 0, 1, 0, ParameterInfo::kCanAutomate, spiEnvelope2, suiSynthB_ENVELOPE );
		param = new RangeParameter( USTRING("B: Delay"), spiDelayMS2, USTRING("ms"), 0, 10000, 0 );
		param->setPrecision( 0 );
		param->setUnitID( suiSynthB_ENVELOPE );
		parameters.addParameter( param );
		param = new RangeParameter( USTRING("B: Init. Vol."), spiInitalVolume2, USTRING("%"), 0, 100, 0 );
		param->setPrecision( 1 );
		param->setUnitID( suiSynthB_ENVELOPE );
		parameters.addParameter( param );
		param = new RangeParameter( USTRING("B: Attack"), spiAttackMS2, USTRING("ms"), 0, 10000, 0 );
		param->setPrecision( 0 );
		param->setUnitID( suiSynthB_ENVELOPE );
		parameters.addParameter( param );
		param = new RangeParameter( USTRING("B: Decay"), spiDecayMS2, USTRING("ms"), 0, 10000, 0 );
		param->setPrecision( 0 );
		param->setUnitID( suiSynthB_ENVELOPE );
		parameters.addParameter( param );
		param = new RangeParameter( USTRING("B: Dec. Vol."), spiDecayVolume2, USTRING("%"), 0, 100, 0 );
		param->setPrecision( 1 );
		param->setUnitID( suiSynthB_ENVELOPE );
		parameters.addParameter( param );
		param = new RangeParameter( USTRING("B: Sustain"), spiSustainRateMS2, USTRING("ms"), 0, 10000, 0 );
		param->setPrecision( 0 );
		param->setUnitID( suiSynthB_ENVELOPE );
		parameters.addParameter( param );
		param = new RangeParameter( USTRING("B: Release"), spiReleaseRateMS2, USTRING("ms"), 0, 10000, 0 );
		param->setPrecision( 0 );
		param->setUnitID( suiSynthB_ENVELOPE );
		parameters.addParameter( param );

		param = new RangeParameter( USTRING("C: Volume"), spiSynthVolume3, USTRING("%"), 0, 100, 100 );
		param->setPrecision( 1 );
		param->setUnitID( suiSynthC_OSCs );
		parameters.addParameter( param );
		param = new RangeParameter( USTRING("C: Pan"), spiSynthPan3, USTRING(""), -100, 100, 0 );
		param->setPrecision( 0 );
		param->setUnitID( suiSynthC_OSCs );
		parameters.addParameter( param );
		param = new RangeParameter( USTRING("C: Pitch"), spiSynthPitch3, USTRING(""), -MAX_PITCH, MAX_PITCH, 0 );
		param->setPrecision( 0 );
		param->setUnitID( suiSynthC_OSCs );
		parameters.addParameter( param );
		param = new RangeParameter( USTRING("C: Phase"), spiSynthPhase3, USTRING(L"°"), 0, 360, 0 );
		param->setPrecision( 1 );
		param->setUnitID( suiSynthC_OSCs );
		parameters.addParameter( param );
		param = new RangeParameter( USTRING("C: SubOSC 1 Phase"), spiSub1Phase3, USTRING(L"°"), 0, 360, 0 );
		param->setPrecision( 1 );
		param->setUnitID( suiSynthC_OSCs );
		parameters.addParameter( param );
		param = new RangeParameter( USTRING("C: SubOSC 1 Vol."), spiSub1Volume3, USTRING("%"), 0, 100, 0 );
		param->setPrecision( 0 );
		param->setUnitID( suiSynthC_OSCs );
		parameters.addParameter( param );
		param = new RangeParameter( USTRING("C: SubOSC 1 Pitch"), spiSub1Pitch3, USTRING(""), -MAX_PITCH, MAX_PITCH, 0 );
		param->setPrecision( 2 );
		param->setUnitID( suiSynthC_OSCs );
		parameters.addParameter( param );
		param = new RangeParameter( USTRING("C: SubOSC 2 Phase"), spiSub2Phase3, USTRING(L"°"), 0, 360, 0 );
		param->setPrecision( 1 );
		param->setUnitID( suiSynthC_OSCs );
		parameters.addParameter( param );
		param = new RangeParameter( USTRING("C: SubOSC 2 Vol."), spiSub2Volume3, USTRING("%"), 0, 100, 0 );
		param->setPrecision( 0 );
		param->setUnitID( suiSynthC_OSCs );
		parameters.addParameter( param );
		param = new RangeParameter( USTRING("C: SubOSC 2 Pitch"), spiSub2Pitch3, USTRING(""), -MAX_PITCH, MAX_PITCH, 0 );
		param->setPrecision( 2 );
		param->setUnitID( suiSynthC_OSCs );
		parameters.addParameter( param );
		filterTypeParam = new StringListParameter (USTRING("C: AM LFO Type"), spiAMtype3);
		filterTypeParam->appendString(USTRING("Sine"));
		filterTypeParam->appendString(USTRING("Ascending"));
		filterTypeParam->appendString(USTRING("Descending"));
		filterTypeParam->appendString(USTRING("Triangle"));
		filterTypeParam->appendString(USTRING("Square"));
		filterTypeParam->setUnitID( suiSynthC_AM_LFO );
		parameters.addParameter( filterTypeParam );
		param = new RangeParameter( USTRING("C: AM Frequency"), spiAMfrequency3, USTRING("Hz"), 0, MAX_LFO_FREQUENCY, 0 );
		param->setPrecision( 1 );
		param->setUnitID( suiSynthC_AM_LFO );
		parameters.addParameter( param );
		param = new RangeParameter( USTRING("C: AM Range"), spiAMrange3, USTRING("%"), 0, 100, 0 );
		param->setPrecision( 1 );
		param->setUnitID( suiSynthC_AM_LFO );
		parameters.addParameter( param );
		filterTypeParam = new StringListParameter (USTRING("C: FM LFO Type"), spiFMtype3);
		filterTypeParam->appendString(USTRING("Sine"));
		filterTypeParam->appendString(USTRING("Ascending"));
		filterTypeParam->appendString(USTRING("Descending"));
		filterTypeParam->appendString(USTRING("Triangle"));
		filterTypeParam->appendString(USTRING("Square"));
		filterTypeParam->setUnitID( suiSynthC_FM_LFO );
		parameters.addParameter( filterTypeParam );
		parameters.addParameter (USTRING("C: FM relative"), 0, 1, 0, ParameterInfo::kCanAutomate, spiFMrelative3, suiSynthC_FM_LFO );
		param = new RangeParameter( USTRING("C: FM Frequency"), spiFMfrequency3, USTRING("Hz"), 0, MAX_LFO_FREQUENCY, 0 );
		param->setPrecision( 1 );
		param->setUnitID( suiSynthC_FM_LFO );
		parameters.addParameter( param );
		param = new RangeParameter( USTRING("C: FM Range"), spiFMrange3, USTRING(""), 0, MAX_PITCH, 0 );
		param->setPrecision( 1 );
		param->setUnitID( suiSynthC_FM_LFO );
		parameters.addParameter( param );
		parameters.addParameter (USTRING("C: Envelope"), 0, 1, 0, ParameterInfo::kCanAutomate, spiEnvelope3, suiSynthC_ENVELOPE );
		param = new RangeParameter( USTRING("C: Delay"), spiDelayMS3, USTRING("ms"), 0, 10000, 0 );
		param->setPrecision( 0 );
		param->setUnitID( suiSynthC_ENVELOPE );
		parameters.addParameter( param );
		param = new RangeParameter( USTRING("C: Init. Vol."), spiInitalVolume3, USTRING("%"), 0, 100, 0 );
		param->setPrecision( 1 );
		param->setUnitID( suiSynthC_ENVELOPE );
		parameters.addParameter( param );
		param = new RangeParameter( USTRING("C: Attack"), spiAttackMS3, USTRING("ms"), 0, 10000, 0 );
		param->setPrecision( 0 );
		param->setUnitID( suiSynthC_ENVELOPE );
		parameters.addParameter( param );
		param = new RangeParameter( USTRING("C: Decay"), spiDecayMS3, USTRING("ms"), 0, 10000, 0 );
		param->setPrecision( 0 );
		param->setUnitID( suiSynthC_ENVELOPE );
		parameters.addParameter( param );
		param = new RangeParameter( USTRING("C: Dec. Vol."), spiDecayVolume3, USTRING("%"), 0, 100, 0 );
		param->setPrecision( 1 );
		param->setUnitID( suiSynthC_ENVELOPE );
		parameters.addParameter( param );
		param = new RangeParameter( USTRING("C: Sustain"), spiSustainRateMS3, USTRING("ms"), 0, 10000, 0 );
		param->setPrecision( 0 );
		param->setUnitID( suiSynthC_ENVELOPE );
		parameters.addParameter( param );
		param = new RangeParameter( USTRING("C: Release"), spiReleaseRateMS3, USTRING("ms"), 0, 10000, 0 );
		param->setPrecision( 0 );
		param->setUnitID( suiSynthC_ENVELOPE );
		parameters.addParameter( param );

		param = new RangeParameter( USTRING("D: Volume"), spiSynthVolume4, USTRING("%"), 0, 100, 100 );
		param->setPrecision( 1 );
		param->setUnitID( suiSynthD_OSCs );
		parameters.addParameter( param );
		param = new RangeParameter( USTRING("D: Pan"), spiSynthPan4, USTRING(""), -100, 100, 0 );
		param->setPrecision( 0 );
		param->setUnitID( suiSynthD_OSCs );
		parameters.addParameter( param );
		param = new RangeParameter( USTRING("D: Pitch"), spiSynthPitch4, USTRING(""), -MAX_PITCH, MAX_PITCH, 0 );
		param->setPrecision( 0 );
		param->setUnitID( suiSynthD_OSCs );
		parameters.addParameter( param );
		param = new RangeParameter( USTRING("D: Phase"), spiSynthPhase4, USTRING(L"°"), 0, 360, 0 );
		param->setPrecision( 1 );
		param->setUnitID( suiSynthD_OSCs );
		parameters.addParameter( param );
		param = new RangeParameter( USTRING("D: SubOSC 1 Phase"), spiSub1Phase4, USTRING(L"°"), 0, 360, 0 );
		param->setPrecision( 1 );
		param->setUnitID( suiSynthD_OSCs );
		parameters.addParameter( param );
		param = new RangeParameter( USTRING("D: SubOSC 1 Vol."), spiSub1Volume4, USTRING("%"), 0, 100, 0 );
		param->setPrecision( 0 );
		param->setUnitID( suiSynthD_OSCs );
		parameters.addParameter( param );
		param = new RangeParameter( USTRING("D: SubOSC 1 Pitch"), spiSub1Pitch4, USTRING(""), -MAX_PITCH, MAX_PITCH, 0 );
		param->setPrecision( 2 );
		param->setUnitID( suiSynthD_OSCs );
		parameters.addParameter( param );
		param = new RangeParameter( USTRING("D: SubOSC 2 Phase"), spiSub2Phase4, USTRING(L"°"), 0, 360, 0 );
		param->setPrecision( 1 );
		param->setUnitID( suiSynthD_OSCs );
		parameters.addParameter( param );
		param = new RangeParameter( USTRING("D: SubOSC 2 Vol."), spiSub2Volume4, USTRING("%"), 0, 100, 0 );
		param->setPrecision( 0 );
		param->setUnitID( suiSynthD_OSCs );
		parameters.addParameter( param );
		param = new RangeParameter( USTRING("D: SubOSC 2 Pitch"), spiSub2Pitch4, USTRING(""), -MAX_PITCH, MAX_PITCH, 0 );
		param->setPrecision( 2 );
		param->setUnitID( suiSynthD_OSCs );
		parameters.addParameter( param );
		filterTypeParam = new StringListParameter (USTRING("D: AM LFO Type"), spiAMtype4);
		filterTypeParam->appendString(USTRING("Sine"));
		filterTypeParam->appendString(USTRING("Ascending"));
		filterTypeParam->appendString(USTRING("Descending"));
		filterTypeParam->appendString(USTRING("Triangle"));
		filterTypeParam->appendString(USTRING("Square"));
		filterTypeParam->setUnitID( suiSynthD_AM_LFO );
		parameters.addParameter( filterTypeParam );
		param = new RangeParameter( USTRING("D: AM Frequency"), spiAMfrequency4, USTRING("Hz"), 0, MAX_LFO_FREQUENCY, 0 );
		param->setPrecision( 1 );
		param->setUnitID( suiSynthD_AM_LFO );
		parameters.addParameter( param );
		param = new RangeParameter( USTRING("D: AM Range"), spiAMrange4, USTRING("%"), 0, 100, 0 );
		param->setPrecision( 1 );
		param->setUnitID( suiSynthD_AM_LFO );
		parameters.addParameter( param );
		filterTypeParam = new StringListParameter (USTRING("D: FM LFO Type"), spiFMtype4);
		filterTypeParam->appendString(USTRING("Sine"));
		filterTypeParam->appendString(USTRING("Ascending"));
		filterTypeParam->appendString(USTRING("Descending"));
		filterTypeParam->appendString(USTRING("Triangle"));
		filterTypeParam->appendString(USTRING("Square"));
		filterTypeParam->setUnitID( suiSynthD_FM_LFO );
		parameters.addParameter( filterTypeParam );
		parameters.addParameter (USTRING("D: FM relative"), 0, 1, 0, ParameterInfo::kCanAutomate, spiFMrelative4, suiSynthD_FM_LFO );
		param = new RangeParameter( USTRING("D: FM Frequency"), spiFMfrequency4, USTRING("Hz"), 0, MAX_LFO_FREQUENCY, 0 );
		param->setPrecision( 1 );
		param->setUnitID( suiSynthD_FM_LFO );
		parameters.addParameter( param );
		param = new RangeParameter( USTRING("D: FM Range"), spiFMrange4, USTRING(""), 0, MAX_PITCH, 0 );
		param->setPrecision( 1 );
		param->setUnitID( suiSynthD_FM_LFO );
		parameters.addParameter( param );
		parameters.addParameter (USTRING("D: Envelope"), 0, 1, 0, ParameterInfo::kCanAutomate, spiEnvelope4, suiSynthD_ENVELOPE );
		param = new RangeParameter( USTRING("D: Delay"), spiDelayMS4, USTRING("ms"), 0, 10000, 0 );
		param->setPrecision( 0 );
		param->setUnitID( suiSynthD_ENVELOPE );
		parameters.addParameter( param );
		param = new RangeParameter( USTRING("D: Init. Vol."), spiInitalVolume4, USTRING("%"), 0, 100, 0 );
		param->setPrecision( 1 );
		param->setUnitID( suiSynthD_ENVELOPE );
		parameters.addParameter( param );
		param = new RangeParameter( USTRING("D: Attack"), spiAttackMS4, USTRING("ms"), 0, 10000, 0 );
		param->setPrecision( 0 );
		param->setUnitID( suiSynthD_ENVELOPE );
		parameters.addParameter( param );
		param = new RangeParameter( USTRING("D: Decay"), spiDecayMS4, USTRING("ms"), 0, 10000, 0 );
		param->setPrecision( 0 );
		param->setUnitID( suiSynthD_ENVELOPE );
		parameters.addParameter( param );
		param = new RangeParameter( USTRING("D: Dec. Vol."), spiDecayVolume4, USTRING("%"), 0, 100, 0 );
		param->setPrecision( 1 );
		param->setUnitID( suiSynthD_ENVELOPE );
		parameters.addParameter( param );
		param = new RangeParameter( USTRING("D: Sustain"), spiSustainRateMS4, USTRING("ms"), 0, 10000, 0 );
		param->setPrecision( 0 );
		param->setUnitID( suiSynthD_ENVELOPE );
		parameters.addParameter( param );
		param = new RangeParameter( USTRING("D: Release"), spiReleaseRateMS4, USTRING("ms"), 0, 10000, 0 );
		param->setPrecision( 0 );
		param->setUnitID( suiSynthD_ENVELOPE );
		parameters.addParameter( param );

		parameters.addParameter( new RangeParameter( USTRING("Active Voices"), spiActiveVoices, 0, 0, DEF_NUM_PHONES, 0, DEF_NUM_PHONES, ParameterInfo::kIsReadOnly ));
		parameters.addParameter( new RangeParameter( USTRING("Max Voices"), spiMaxVoices, 0, 0, DEF_NUM_PHONES, 0, DEF_NUM_PHONES, ParameterInfo::kIsReadOnly ));
		parameters.addParameter (STR16 ("VuPPML"), 0, 0, 0, ParameterInfo::kIsReadOnly, spiLeftLevel);
		parameters.addParameter (STR16 ("VuPPMR"), 0, 0, 0, ParameterInfo::kIsReadOnly, spiRightLevel);
	}
	return kResultTrue;
}

IPlugView * PLUGIN_API Controller::createView( FIDString _name )
{
	ConstString name( _name );
	if( name == ViewType::kEditor )
	{
		return new MGsynthEditor( this );
	}
	return 0;
}

tresult PLUGIN_API Controller::terminate()
{
	return EditController::terminate();
}

tresult PLUGIN_API Controller::setComponentState( IBStream* state )
{
	GlobalParameterState gps;
	tresult result = gps.setState (state);
	if (result == kResultTrue)
	{
		setParamNormalized( spiMasterVolume, gps.masterVolume );
		setParamNormalized( spiMasterPresence, gps.masterPresence );

		setParamNormalized( spiSynthEnabled1, gps.voiceParams[0].enable );
		setParamNormalized( spiSynthEnabled2, gps.voiceParams[1].enable );
		setParamNormalized( spiSynthEnabled3, gps.voiceParams[2].enable );
		setParamNormalized( spiSynthEnabled4, gps.voiceParams[3].enable );
		setParamNormalized( spiSynthVolume1, gps.voiceParams[0].synthVolume );
		setParamNormalized( spiSynthVolume2, gps.voiceParams[1].synthVolume );
		setParamNormalized( spiSynthVolume3, gps.voiceParams[2].synthVolume );
		setParamNormalized( spiSynthVolume4, gps.voiceParams[3].synthVolume );
		setParamNormalized( spiSynthPan1, gps.voiceParams[0].synthPan );
		setParamNormalized( spiSynthPan2, gps.voiceParams[1].synthPan );
		setParamNormalized( spiSynthPan3, gps.voiceParams[2].synthPan );
		setParamNormalized( spiSynthPan4, gps.voiceParams[3].synthPan );
		setParamNormalized( spiSynthPitch1, plainParamToNormalized( spiSynthPitch1, gps.voiceParams[0].synthPitch.pitch ) );
		setParamNormalized( spiSynthPitch2, plainParamToNormalized( spiSynthPitch2, gps.voiceParams[1].synthPitch.pitch ) );
		setParamNormalized( spiSynthPitch3, plainParamToNormalized( spiSynthPitch3, gps.voiceParams[2].synthPitch.pitch ) );
		setParamNormalized( spiSynthPitch4, plainParamToNormalized( spiSynthPitch4, gps.voiceParams[3].synthPitch.pitch ) );
		setParamNormalized( spiSynthPhase1, gps.voiceParams[0].synthPhase );
		setParamNormalized( spiSynthPhase2, gps.voiceParams[1].synthPhase );
		setParamNormalized( spiSynthPhase3, gps.voiceParams[2].synthPhase );
		setParamNormalized( spiSynthPhase4, gps.voiceParams[3].synthPhase );

		setParamNormalized( spiSub1Phase1, plainParamToNormalized( spiSub1Phase1, gps.voiceParams[0].subOscParameter[0].oscPhase ) );
		setParamNormalized( spiSub1Phase2, plainParamToNormalized( spiSub1Phase2, gps.voiceParams[1].subOscParameter[0].oscPhase ) );
		setParamNormalized( spiSub1Phase3, plainParamToNormalized( spiSub1Phase3, gps.voiceParams[2].subOscParameter[0].oscPhase ) );
		setParamNormalized( spiSub1Phase4, plainParamToNormalized( spiSub1Phase4, gps.voiceParams[3].subOscParameter[0].oscPhase ) );
		setParamNormalized( spiSub2Phase1, plainParamToNormalized( spiSub2Phase1, gps.voiceParams[0].subOscParameter[1].oscPhase ) );
		setParamNormalized( spiSub2Phase2, plainParamToNormalized( spiSub2Phase2, gps.voiceParams[1].subOscParameter[1].oscPhase ) );
		setParamNormalized( spiSub2Phase3, plainParamToNormalized( spiSub2Phase3, gps.voiceParams[2].subOscParameter[1].oscPhase ) );
		setParamNormalized( spiSub2Phase4, plainParamToNormalized( spiSub2Phase4, gps.voiceParams[3].subOscParameter[1].oscPhase ) );
		setParamNormalized( spiSub1Volume1, gps.voiceParams[0].subOscParameter[0].oscVolumeFactor );
		setParamNormalized( spiSub1Volume2, gps.voiceParams[1].subOscParameter[0].oscVolumeFactor );
		setParamNormalized( spiSub1Volume3, gps.voiceParams[2].subOscParameter[0].oscVolumeFactor );
		setParamNormalized( spiSub1Volume4, gps.voiceParams[3].subOscParameter[0].oscVolumeFactor );
		setParamNormalized( spiSub2Volume1, gps.voiceParams[0].subOscParameter[1].oscVolumeFactor );
		setParamNormalized( spiSub2Volume2, gps.voiceParams[1].subOscParameter[1].oscVolumeFactor );
		setParamNormalized( spiSub2Volume3, gps.voiceParams[2].subOscParameter[1].oscVolumeFactor );
		setParamNormalized( spiSub2Volume4, gps.voiceParams[3].subOscParameter[1].oscVolumeFactor );
		setParamNormalized( spiSub1Pitch1, plainParamToNormalized( spiSub1Pitch1, gps.voiceParams[0].subOscParameter[0].oscPitch.pitch ) );
		setParamNormalized( spiSub1Pitch2, plainParamToNormalized( spiSub1Pitch2, gps.voiceParams[1].subOscParameter[0].oscPitch.pitch ) );
		setParamNormalized( spiSub1Pitch3, plainParamToNormalized( spiSub1Pitch3, gps.voiceParams[2].subOscParameter[0].oscPitch.pitch ) );
		setParamNormalized( spiSub1Pitch4, plainParamToNormalized( spiSub1Pitch4, gps.voiceParams[3].subOscParameter[0].oscPitch.pitch ) );
		setParamNormalized( spiSub2Pitch1, plainParamToNormalized( spiSub2Pitch1, gps.voiceParams[0].subOscParameter[1].oscPitch.pitch ) );
		setParamNormalized( spiSub2Pitch2, plainParamToNormalized( spiSub2Pitch2, gps.voiceParams[1].subOscParameter[1].oscPitch.pitch ) );
		setParamNormalized( spiSub2Pitch3, plainParamToNormalized( spiSub2Pitch3, gps.voiceParams[2].subOscParameter[1].oscPitch.pitch ) );
		setParamNormalized( spiSub2Pitch4, plainParamToNormalized( spiSub2Pitch4, gps.voiceParams[3].subOscParameter[1].oscPitch.pitch ) );

		setParamNormalized( spiAMtype1, plainParamToNormalized( spiAMtype1, gps.voiceParams[0].amLfoParameter.type ) );
		setParamNormalized( spiAMtype2, plainParamToNormalized( spiAMtype2, gps.voiceParams[1].amLfoParameter.type ) );
		setParamNormalized( spiAMtype3, plainParamToNormalized( spiAMtype3, gps.voiceParams[2].amLfoParameter.type ) );
		setParamNormalized( spiAMtype4, plainParamToNormalized( spiAMtype4, gps.voiceParams[3].amLfoParameter.type ) );
		setParamNormalized( spiAMfrequency1, plainParamToNormalized( spiAMfrequency1, gps.voiceParams[0].amLfoParameter.frequency ) );
		setParamNormalized( spiAMfrequency2, plainParamToNormalized( spiAMfrequency2, gps.voiceParams[1].amLfoParameter.frequency ) );
		setParamNormalized( spiAMfrequency3, plainParamToNormalized( spiAMfrequency3, gps.voiceParams[2].amLfoParameter.frequency ) );
		setParamNormalized( spiAMfrequency4, plainParamToNormalized( spiAMfrequency4, gps.voiceParams[3].amLfoParameter.frequency ) );
		setParamNormalized( spiAMrange1, 1- gps.voiceParams[0].amLfoParameter.range.minimum );
		setParamNormalized( spiAMrange2, 1- gps.voiceParams[1].amLfoParameter.range.minimum );
		setParamNormalized( spiAMrange3, 1- gps.voiceParams[2].amLfoParameter.range.minimum );
		setParamNormalized( spiAMrange4, 1- gps.voiceParams[3].amLfoParameter.range.minimum );
		setParamNormalized( spiFMtype1, plainParamToNormalized( spiFMtype1, gps.voiceParams[0].fmLfoParameter.type ) );
		setParamNormalized( spiFMtype2, plainParamToNormalized( spiFMtype2, gps.voiceParams[1].fmLfoParameter.type ) );
		setParamNormalized( spiFMtype3, plainParamToNormalized( spiFMtype3, gps.voiceParams[2].fmLfoParameter.type ) );
		setParamNormalized( spiFMtype4, plainParamToNormalized( spiFMtype4, gps.voiceParams[3].fmLfoParameter.type ) );
		setParamNormalized( spiFMrelative1, plainParamToNormalized( spiFMrelative1, gps.voiceParams[0].fmLfoParameter.isFactor ) );
		setParamNormalized( spiFMrelative2, plainParamToNormalized( spiFMrelative2, gps.voiceParams[1].fmLfoParameter.isFactor ) );
		setParamNormalized( spiFMrelative3, plainParamToNormalized( spiFMrelative3, gps.voiceParams[2].fmLfoParameter.isFactor ) );
		setParamNormalized( spiFMrelative4, plainParamToNormalized( spiFMrelative4, gps.voiceParams[3].fmLfoParameter.isFactor ) );
		setParamNormalized( spiFMfrequency1, plainParamToNormalized( spiFMfrequency1, gps.voiceParams[0].fmLfoParameter.frequency ) );
		setParamNormalized( spiFMfrequency2, plainParamToNormalized( spiFMfrequency2, gps.voiceParams[1].fmLfoParameter.frequency ) );
		setParamNormalized( spiFMfrequency3, plainParamToNormalized( spiFMfrequency3, gps.voiceParams[2].fmLfoParameter.frequency ) );
		setParamNormalized( spiFMfrequency4, plainParamToNormalized( spiFMfrequency4, gps.voiceParams[3].fmLfoParameter.frequency ) );
		setParamNormalized( spiFMrange1, plainParamToNormalized( spiFMrange1, gps.voiceParams[0].fmLfoParameter.range.pitch ) );
		setParamNormalized( spiFMrange2, plainParamToNormalized( spiFMrange2, gps.voiceParams[1].fmLfoParameter.range.pitch ) );
		setParamNormalized( spiFMrange3, plainParamToNormalized( spiFMrange3, gps.voiceParams[2].fmLfoParameter.range.pitch ) );
		setParamNormalized( spiFMrange4, plainParamToNormalized( spiFMrange4, gps.voiceParams[3].fmLfoParameter.range.pitch ) );
		setParamNormalized( spiEnvelope1, plainParamToNormalized( spiEnvelope1, gps.voiceParams[0].envelope.enabled ) );
		setParamNormalized( spiEnvelope2, plainParamToNormalized( spiEnvelope2, gps.voiceParams[1].envelope.enabled ) );
		setParamNormalized( spiEnvelope3, plainParamToNormalized( spiEnvelope3, gps.voiceParams[2].envelope.enabled ) );
		setParamNormalized( spiEnvelope4, plainParamToNormalized( spiEnvelope4, gps.voiceParams[3].envelope.enabled ) );
		setParamNormalized( spiDelayMS1, plainParamToNormalized( spiDelayMS1, gps.voiceParams[0].envelope.delayMS ) );
		setParamNormalized( spiDelayMS2, plainParamToNormalized( spiDelayMS2, gps.voiceParams[1].envelope.delayMS ) );
		setParamNormalized( spiDelayMS3, plainParamToNormalized( spiDelayMS3, gps.voiceParams[2].envelope.delayMS ) );
		setParamNormalized( spiDelayMS4, plainParamToNormalized( spiDelayMS4, gps.voiceParams[3].envelope.delayMS ) );
		setParamNormalized( spiInitalVolume1, gps.voiceParams[0].envelope.initialVolume );
		setParamNormalized( spiInitalVolume2, gps.voiceParams[1].envelope.initialVolume );
		setParamNormalized( spiInitalVolume3, gps.voiceParams[2].envelope.initialVolume );
		setParamNormalized( spiInitalVolume4, gps.voiceParams[3].envelope.initialVolume );
		setParamNormalized( spiAttackMS1, plainParamToNormalized( spiAttackMS1, gps.voiceParams[0].envelope.attackMS ) );
		setParamNormalized( spiAttackMS2, plainParamToNormalized( spiAttackMS2, gps.voiceParams[1].envelope.attackMS ) );
		setParamNormalized( spiAttackMS3, plainParamToNormalized( spiAttackMS3, gps.voiceParams[2].envelope.attackMS ) );
		setParamNormalized( spiAttackMS4, plainParamToNormalized( spiAttackMS4, gps.voiceParams[3].envelope.attackMS ) );
		setParamNormalized( spiDecayMS1, plainParamToNormalized( spiDecayMS1, gps.voiceParams[0].envelope.decayMS ) );
		setParamNormalized( spiDecayMS2, plainParamToNormalized( spiDecayMS2, gps.voiceParams[1].envelope.decayMS ) );
		setParamNormalized( spiDecayMS3, plainParamToNormalized( spiDecayMS3, gps.voiceParams[2].envelope.decayMS ) );
		setParamNormalized( spiDecayMS4, plainParamToNormalized( spiDecayMS4, gps.voiceParams[3].envelope.decayMS ) );
		setParamNormalized( spiDecayVolume1, gps.voiceParams[0].envelope.decayVolume );
		setParamNormalized( spiDecayVolume2, gps.voiceParams[1].envelope.decayVolume );
		setParamNormalized( spiDecayVolume3, gps.voiceParams[2].envelope.decayVolume );
		setParamNormalized( spiDecayVolume4, gps.voiceParams[3].envelope.decayVolume );
		setParamNormalized( spiSustainRateMS1, plainParamToNormalized( spiSustainRateMS1, gps.voiceParams[0].envelope.sustainRateMS ) );
		setParamNormalized( spiSustainRateMS2, plainParamToNormalized( spiSustainRateMS2, gps.voiceParams[1].envelope.sustainRateMS ) );
		setParamNormalized( spiSustainRateMS3, plainParamToNormalized( spiSustainRateMS3, gps.voiceParams[2].envelope.sustainRateMS ) );
		setParamNormalized( spiSustainRateMS4, plainParamToNormalized( spiSustainRateMS4, gps.voiceParams[3].envelope.sustainRateMS ) );
		setParamNormalized( spiReleaseRateMS1, plainParamToNormalized( spiReleaseRateMS1, gps.voiceParams[0].envelope.releaseRateMS ) );
		setParamNormalized( spiReleaseRateMS2, plainParamToNormalized( spiReleaseRateMS2, gps.voiceParams[1].envelope.releaseRateMS ) );
		setParamNormalized( spiReleaseRateMS3, plainParamToNormalized( spiReleaseRateMS3, gps.voiceParams[2].envelope.releaseRateMS ) );
		setParamNormalized( spiReleaseRateMS4, plainParamToNormalized( spiReleaseRateMS4, gps.voiceParams[3].envelope.releaseRateMS ) );
	}
	return result;
}

tresult PLUGIN_API Controller::getParamStringByValue( ParamID tag, ParamValue valueNormalized, String128 string )
{
	tresult result = kResultFalse;

	if( tag == spiSynthPan1 || tag == spiSynthPan2 
	|| tag == spiSynthPan3 || tag == spiSynthPan4 )
	{
		Parameter *parameter = getParameterObject( tag );
		if( parameter )
		{
			gak::STRING	str;

			double plain = parameter->toPlain( valueNormalized );
			if( plain < 0 )
			{
				str = "L";
				plain = -plain;
			}
			else if( plain > 0 )
			{
				str = "R";
			}
			else
			{
				str = "C";
			}
			if( plain )
			{
				str += gak::formatFloat( plain, 2, 0 );
			}

			UStringBuffer<64>	uStr;
			uStr.assign( str );
			uStr.copyTo( string, 128 );

			result = kResultTrue;
		}
	}
	else
	{
		result = EditControllerEx1::getParamStringByValue( tag, valueNormalized, string );
	}
	return result;
}

tresult PLUGIN_API Controller::getParamValueByString( ParamID tag, TChar* string, ParamValue& valueNormalized )
{
	tresult result = kResultFalse;

	if( tag == spiSynthPan1 || tag == spiSynthPan2 
	|| tag == spiSynthPan3 || tag == spiSynthPan4 )
	{
		Parameter *parameter = getParameterObject( tag );
		if( parameter )
		{
			gak::uSTRING	wideString( string );
			gak::STRING		asciiString = wideString.toString();
			if( asciiString == "C" )
			{
				valueNormalized = 0.5;
				result = kResultTrue;
			}
			else
			{
				int plain = gak::getValue<int>( asciiString + size_t(1) );
				if( asciiString[0U] == 'L' )
				{
					plain = -plain;
				}
				valueNormalized = parameter->toNormalized( plain );
				result = kResultTrue;
			}
		}
	}
	else
	{
		result = EditControllerEx1::getParamValueByString( tag, string, valueNormalized );
	}
	return result;
}

tresult PLUGIN_API Controller::getMidiControllerAssignment( int32 busIndex, int16 channel, CtrlNumber midiControllerNumber, ParamID& id/*out*/)
{
	if( busIndex == 0 && channel == 0 )
	{
		id = -1;
		switch( midiControllerNumber )
		{
			case kCtrlVolume:			// 7
				id = spiMasterVolume;
				break;
			case kCtrlExpression:		// 11
				id = spiMasterPresence;
				break;
			case kCtrlGPC1:				// 16
				id = spiSynthVolume1;
				break;
			case kCtrlGPC2:				// 17
				id = spiSynthVolume2;
				break;
			case kCtrlGPC3:				// 18
				id = spiSynthVolume3;
				break;
			case kCtrlGPC4:				// 19
				id = spiSynthVolume4;
				break;
			case 70:
				id = spiAMrange1;
				break;
			case 71:
				id = spiAMrange2;
				break;
			case 72:
				id = spiAMrange3;
				break;
			case 73:
				id = spiAMrange4;
				break;
			case 74:
				id = spiFMrange1;
				break;
			case 75:
				id = spiFMrange2;
				break;
			case 76:
				id = spiFMrange3;
				break;
			case 77:
				id = spiFMrange4;
				break;
			case 78:
				id = spiReleaseRateMS1;
				break;
			case 79:
				id = spiReleaseRateMS2;
				break;
			case 80:
				id = spiReleaseRateMS3;
				break;
			case 81:
				id = spiReleaseRateMS4;
				break;
		}
		return id != -1 ? kResultTrue : kResultFalse;
	}
	return kResultFalse;
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

