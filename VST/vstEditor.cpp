/*
		Project:		MG Analog Synthesizer
		Module:			vstEditor.cpp
		Description:	The GUI definition
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

#include <gak/map.h>
#include <gak/array.h>

#undef WIN32

#include "vstEditor.h"

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

void MGsynthEditor::switchState( EditorState state )
{
	m_state = state;

	int32_t	startParamID = getFirstParamID( state );

	if( startParamID >= 0 )
	{
		int32_t	endParamID = startParamID + (spiReleaseRateMS1-spiSynthVolume1);
		for( int32_t paramID = startParamID, tagID=spiSynthVolume1; paramID <=  endParamID; ++paramID, ++tagID )
		{

			Controls	&controls = m_controls[tagID];

			for(
				Controls::iterator it = controls.begin(), endIT = controls.end();
				it != endIT;
				++it
			)
			{
				CControl	*control = *it;
				control->setTag( paramID );
			}
		}
	}
}

// --------------------------------------------------------------------- //
// ----- class protected ----------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class virtuals ------------------------------------------------ //
// --------------------------------------------------------------------- //
   
CView* MGsynthEditor::verifyView( CView* view, const UIAttributes& attributes, IUIDescription* description )
{
	CControl *control = dynamic_cast<CControl*>( view );
	if( control && control->getTag() != -1 && control->getListener() == this )
	{
		int32_t	tag = control->getTag();

		switch( tag )
		{
			case esSynthA:
				m_synthA = control;
				m_synthA->setValue( 1 );
				break;
			case esSynthB:
				m_synthB = control;
				break;
			case esSynthC:
				m_synthC = control;
				break;
			case esSynthD:
				m_synthD = control;
				break;

			case spiSynthEnabled1:
				m_synthAEnable = control;
				break;
			case spiSynthEnabled2:
				m_synthBEnable = control;
				break;
			case spiSynthEnabled3:
				m_synthCEnable = control;
				break;
			case spiSynthEnabled4:
				m_synthDEnable = control;
				break;
			default:
				if( tag >= spiSynthVolume1 && tag < spiSynthVolume2 )
				{
					Controls	&controls = m_controls[tag];
					controls.addElement( control );
				}
		}
	}

	return VST3Editor::verifyView( view, attributes, description );
}

void MGsynthEditor::valueChanged( CControl* pControl )
{
	int32_t tag = pControl->getTag();
	if( tag >= esSynthA && tag <= esSynthD )
	{
		CControl	*other = NULL;
		double		newValue = 0;

		switch( m_state )
		{
			case esSynthA:
				other = m_synthA;
				break;
			case esSynthB:
				other = m_synthB;
				break;
			case esSynthC:
				other = m_synthC;
				break;
			case esSynthD:
				other = m_synthD;
				break;
		}

		if( tag != m_state )
		{
			switchState( static_cast<EditorState>(tag) );
		}
		else
		{
			newValue = 1;
		}
		if( other )
		{
			other->setValueNormalized( newValue );
			other->invalid();
		}

		CControl	*enableControl = NULL;
		switch( tag )
		{
			case esSynthA:
				enableControl = m_synthAEnable;
				break;
			case esSynthB:
				enableControl = m_synthBEnable;
				break;
			case esSynthC:
				enableControl = m_synthCEnable;
				break;
			case esSynthD:
				enableControl = m_synthDEnable;
				break;
		}

		if( enableControl && enableControl->getValueNormalized() <= 0.5 )
		{
			enableControl->setValueNormalized( 1 );
			enableControl->invalid();
			VST3Editor::valueChanged( enableControl );
		}
	}
	else
	{
		VST3Editor::valueChanged( pControl );
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

