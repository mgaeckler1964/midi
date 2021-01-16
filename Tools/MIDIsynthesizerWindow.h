/*
		Project:		MIDI Tools
		Module:			MIDIsythesizerWindow.h
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

#ifndef MIDI_SYNTH_WINDOW
#define MIDI_SYNTH_WINDOW

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <gak/fmtNumber.h>

#include <winlib/dialogwi.h>
#include <winlib/CONTROLW.H>

#include "audio.h"
#include "midi.gui.h"

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
 class MIDIsynthesizerWindow : public winlibGUI::MIDIsynthesizerWindow_form
 {
	MidiSynthesizer<>	&theSynthesizer;

	bool				reading;
	int					currentSynth;

	gak::STRING			cpuSpeedLabel;
	gak::STRING			lastOptionFile;

	void writeSettings( void );
	void writeSettings( std::ostream &dest );
	void readSettings( void );
	void readSettings( std::istream &src );
	void getSettings( int newSynth );

 	virtual winlib::ProcessStatus handleCreate( void );
 	virtual winlib::ProcessStatus handleDestroy( void );
	virtual winlib::ProcessStatus handleEditChange( int editControl );
	virtual winlib::ProcessStatus handleButtonClick( int btn );
	virtual winlib::ProcessStatus handleScrollControl( int control );
	virtual winlib::ProcessStatus handleCommand( int cmd );
	virtual void handleTimer( void );

	void showCPUspeed()
	{
		clock_t cpuTime = testCPUspeed();
		cpuSpeedLabel = gak::STRING("CPU-speed: ") + gak::formatNumber( cpuTime * 1000 / CLOCKS_PER_SEC ) + " ms ";
		double latency = getLatency();
		if( latency >= 0 )
		{
			cpuSpeedLabel += gak::formatFloat( latency ) + " ms Latency";
		}

		cpuSpeed->setText( cpuSpeedLabel );
		setTimer( 10000 );
	}

	static OscilatorType getOscilatorType( int cmd )
	{
		switch( cmd )
		{
			case winlibGUI::amSinus_id:
			case winlibGUI::fmSinus_id:
				return oscSINUS;
			case winlibGUI::amAscending_id:
			case winlibGUI::fmAscending_id:
				return oscASC_SAW;
			case winlibGUI::amDescending_id:
			case winlibGUI::fmDescending_id:
				return oscDES_SAW;
			case winlibGUI::amTriangle_id:
			case winlibGUI::fmTriangle_id:
				return oscTRIANGLE;
			case winlibGUI::amRectangle_id:
			case winlibGUI::fmRectangle_id:
				return oscRECTANGLE;
		}

		return oscUNKNOWN;
	}
	OscilatorType getFmOscilatorType( int cmd ) const
	{
		OscilatorType	type = getOscilatorType( cmd );
		if( type == oscUNKNOWN )
		{
			if( fmAscending->isActive() )
			{
				type = oscASC_SAW;
			}
			else if( fmDescending->isActive() )
			{
				type = oscDES_SAW;
			}
			else if( fmTriangle->isActive() )
			{
				type = oscTRIANGLE;
			}
			else if( fmRectangle->isActive() )
			{
				type = oscRECTANGLE;
			}
			else // if( fmSinus->isActive() )
			{
				type = oscSINUS;
			}
		}
		return type;
	}
	OscilatorType getAmOscilatorType( int cmd ) const
	{
		OscilatorType	type = getOscilatorType( cmd );
		if( type == oscUNKNOWN )
		{
			if( amAscending->isActive() )
			{
				type = oscASC_SAW;
			}
			else if( amDescending->isActive() )
			{
				type = oscDES_SAW;
			}
			else if( amTriangle->isActive() )
			{
				type = oscTRIANGLE;
			}
			else if( amRectangle->isActive() )
			{
				type = oscRECTANGLE;
			}
			else // if( amSinus->isActive() )
			{
				type = oscSINUS;
			}
		}
		return type;
	}
	public:
	MIDIsynthesizerWindow( BasicWindow *owner, MidiSynthesizer<> &theSynthesizer ) 
	: MIDIsynthesizerWindow_form( owner ), theSynthesizer(theSynthesizer)
	{
	}
	void create( void )
	{
		reading = false;
		winlib::SuccessCode success = MIDIsynthesizerWindow_form::create( NULL );
		if( success == winlib::scSUCCESS )
		{
			restoreWindowPos();
		}
	}
	winlib::SuccessCode close( void )
	{
		saveWindowRect();
		return MIDIsynthesizerWindow_form::close();
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