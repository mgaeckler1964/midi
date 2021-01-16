/*
		Project:		MIDI Tools
		Module:			statusWindow.h
		Description:	Displays the current status of the synthesizer
						(used by audio)
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

#ifndef MG_SYNTH_STATUS_WINDOW_H
#define MG_SYNTH_STATUS_WINDOW_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <WINLIB/CALLWIN.H>
#include <WINLIB/POPUP.H>
#include <WINLIB/DEVICE.H>

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
	status thread
*/
template <typename SampleT=long>
class StatusWindow : public winlib::ToolWindow
{
	public:
	typedef MidiSynthesizer<SampleT>					MidiSynthesizer;
	typedef typename MidiSynthesizer::StereoSynthesizer	StereoSynthesizer;
	typedef typename StereoSynthesizer::Synthesizer		Synthesizer;
	typedef typename Synthesizer::Oscilator				Oscilator;

	static const int		COL_WIDTH=64;

	private:
	const MidiSynthesizer	&m_synthesizer;

	private:
	virtual winlib::ProcessStatus handleRepaint( winlib::Device &hDC );
	virtual winlib::ProcessStatus handleMessage( UINT msg, WPARAM wParam, LPARAM lParam );

	public:
	StatusWindow(const MidiSynthesizer	&theSynthesizer) 
	: ToolWindow( NULL ), m_synthesizer(theSynthesizer) 
	{
	}

	void paintStatus( winlib::Device &hDC );
};

template <typename SampleT=long>
class StatusThread : public Thread
{
	StatusWindow<SampleT>		*targetWindow;

	virtual void ExecuteThread( void );
	public:
	StatusThread( StatusWindow<SampleT> *theTarget )
	{
		targetWindow = theTarget;
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
void StatusThread<SampleT>::ExecuteThread( void )
{
	const clock_t	rate10ms = CLOCKS_PER_SEC * 10 / 1000;
	DrawDevice		hDC( targetWindow );

	while( !terminated )
	{
		targetWindow->paintStatus( hDC );
		Sleep( rate10ms );
	}
}

template <typename SampleT>
winlib::ProcessStatus StatusWindow<SampleT>::handleRepaint( winlib::Device &hDC )
{
	winlib::Size	size = getClientSize();
	hDC.getPen().selectPen( winlib::Pen::spNull );
	hDC.getBrush().selectBrush( winlib::Brush::sbWhite );
	hDC.rectangle( 0, 0, size.width+1, size.height+1 );
	paintStatus( hDC );

	return winlib::psPROCESSED;
}

template <typename SampleT>
winlib::ProcessStatus StatusWindow<SampleT>::handleMessage( UINT msg, WPARAM /* wParam */, LPARAM /* lParam */ )
{
	if( msg == WM_MOVE )
	{
		invalidateWindow();
	}
	return winlib::psDO_DEFAULT;
}

// --------------------------------------------------------------------- //
// ----- class publics ------------------------------------------------- //
// --------------------------------------------------------------------- //

template <typename SampleT>
void StatusWindow<SampleT>::paintStatus( winlib::Device &hDC )
{
	for( int i=0; i<DEF_NUM_PHONES; i++ )
	{
		int xPos = 32 + i*COL_WIDTH;

		const StereoSynthesizer	&thePhone = m_synthesizer.getPhone( i );

		hDC.getPen().selectPen( winlib::Pen::spBlack );

		if( thePhone.isActive() )
			hDC.getBrush().create( 0, 255, 0 );
		else
			hDC.getBrush().create( 255, 0, 0 );

		hDC.ellipse( xPos+8, 8, 8 );

		int yPos = 16;
		for( unsigned j=0; j<DEF_NUM_SYNTHS; j++ )
		{
			if( !i )
			{
				hDC.textOut( 0, yPos, STRING( char(j+'A') ) );
			}

			const Synthesizer	&theSynth = thePhone.getSynth( j );

			const Oscilator	&osc = theSynth.getMainOSC();
			double			frequency = osc.getFrequency();
			hDC.textOut( xPos, yPos, formatFloat( frequency, 8, 1 ) );
			yPos += 20;

			double volume = osc.getVolume();
			hDC.textOut( xPos, yPos, formatFloat( volume*100, 8, 3 ) );
			yPos += 20;

			for( unsigned k=0; k<DEF_NUM_SUBOSCS; k++ )
			{
				if( !i )
				{
					hDC.textOut( 0, yPos, STRING( char(k+'1') ) );
				}

				const Oscilator	&osc = theSynth.getSubOSC( k );
				double			frequency = osc.getFrequency();
				hDC.textOut( xPos, yPos, formatFloat( frequency, 8, 1 ) );
				yPos += 20;

				double volume = osc.getVolume();
				hDC.textOut( xPos, yPos, formatFloat( volume*100, 8, 3 ) );
				yPos += 20;
			}
		}
	}
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

#endif	// MG_SYNTH_STATUS_WINDOW_H