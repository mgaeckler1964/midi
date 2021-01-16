/*
		Project:		MIDI Tools
		Module:			ToolBarWindow.cpp
		Description:	The main window
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

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <winlib\winapp.h>

#include "ToolBarWindow.h"
#include "MIDIrecorderWindow.h"
#include "MIDIloopEditor.h"
#include "MIDIdrumWindow.h"
#include "MidiDevices.h"
#include "MIDIsynthesizerWindow.h"

#include "midi_rc.h"

// --------------------------------------------------------------------- //
// ----- imported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module switches ----------------------------------------------- //
// --------------------------------------------------------------------- //

using namespace winlib;
using namespace gak;

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

void ToolBarWindow::openFile( const char *cmdLine )
{
	const char *dotPos = strrchr( cmdLine, '.' );
	if( dotPos )
	{
		if( !strcmpi( dotPos, ".mid" ) )
		{
			MIDIrecorderWindow	*theNewRecorderWindow;
			theNewRecorderWindow = new MIDIrecorderWindow( this );
			theNewRecorderWindow->create();
			theNewRecorderWindow->loadMidiFile( cmdLine );
			theNewRecorderWindow->show();
			theNewRecorderWindow->focus();
			openWindows.addElement( theNewRecorderWindow );
		}
		else if( !strcmpi( dotPos, ".mset" ) )
		{
			MIDIrecorderWindow	*theNewRecorderWindow;
			theNewRecorderWindow = new MIDIrecorderWindow( this );
			theNewRecorderWindow->create();
			theNewRecorderWindow->loadMidiSettings( cmdLine );
			theNewRecorderWindow->show();
			theNewRecorderWindow->focus();
			openWindows.addElement( theNewRecorderWindow );
		}
		else if( !strcmpi( dotPos, ".drum" ) )
		{
			MIDIdrumWindow	*theNewDrumWindow;
			theNewDrumWindow = new MIDIdrumWindow ( this );
			theNewDrumWindow->create();
			theNewDrumWindow->loadDrumPatterns( cmdLine );
			theNewDrumWindow->show();
			theNewDrumWindow->focus();
			openWindows.addElement( theNewDrumWindow );
		}
		else if( !strcmpi( dotPos, ".loop" ) )
		{
			MIDIloopEditor	*theNewLoopWindow;
			theNewLoopWindow = new MIDIloopEditor( this );
			theNewLoopWindow->create();
			theNewLoopWindow->loadLoopFile( cmdLine );
			theNewLoopWindow->show();
			theNewLoopWindow->focus();
			openWindows.addElement( theNewLoopWindow );
		}
	}
}

// --------------------------------------------------------------------- //
// ----- class protected ----------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class virtuals ------------------------------------------------ //
// --------------------------------------------------------------------- //

ProcessStatus ToolBarWindow::handleCreate( void )
{
	doEnterFunction( "ToolBarWindow::handleCreate" );

	STRING	language = appObject->GetProfile( "", "language", "de" );
	if( language == "de" )
	{
		languageCheckBox->setActive();
	}
	else
	{
		languageCheckBox->clrActive();
	}

	return psDO_DEFAULT;
}

bool ToolBarWindow::handleChildClose( BasicWindow *child, bool deleted )
{
	doEnterFunction("ToolBarWindow::handleChildClose");

	for( std::size_t i=openWindows.size()-1; i != openWindows.no_index; i-- )
	{
		if( openWindows[i] == child )
		{
			openWindows.removeElementAt( i );
			break;
		}
	}

	deleted = ToolBarWindow_form::handleChildClose( child, deleted );

	return deleted;
}

ProcessStatus ToolBarWindow::handleCommand( int cmd )
{
	switch( cmd )
	{
		case winlibGUI::recorderButton_id:
		{
			MIDIrecorderWindow	*theNewRecorderWindow;
			theNewRecorderWindow = new MIDIrecorderWindow( this );
			theNewRecorderWindow->create();
			theNewRecorderWindow->show();
			theNewRecorderWindow->focus();
			openWindows.addElement( theNewRecorderWindow );
			break;
		}
		case winlibGUI::drumButton_id:
		{
			MIDIdrumWindow *theNewDrumWindow = new MIDIdrumWindow( this );
			theNewDrumWindow->create();
			theNewDrumWindow->show();
			theNewDrumWindow->focus();
			openWindows.addElement( theNewDrumWindow );
			break;
		}
		case winlibGUI::loopButton_id:
		{
			MIDIloopEditor *theNewLoopWindow = new MIDIloopEditor( this );
			theNewLoopWindow->create();
			theNewLoopWindow->show();
			theNewLoopWindow->focus();
			openWindows.addElement( theNewLoopWindow );
			break;
		}
		case winlibGUI::synthesizerButton_id:
		{
			MIDIsynthesizerWindow *newSynthWindow = new MIDIsynthesizerWindow( this, getSyntesizer() );
			newSynthWindow->create();
			newSynthWindow->show();
			newSynthWindow->focus();
			openWindows.addElement( newSynthWindow );
			break;
		}
		case winlibGUI::midiDevicesButton_id:
		{
			MIDIdeviceDialog	midiDevices;
			midiDevices.create( this );
			break;
		}
		case winlibGUI::playButton_id:
		{
			for( std::size_t i=openWindows.size()-1; i>=0; i-- )
			{
				if( openWindows[i]->hasFocus() )
				{
					openWindows[i]->handleCommand( winlibGUI::playButton_id );
				}
			}
			break;
		}
		case winlibGUI::languageCheckBox_id:
			if( languageCheckBox->isActive() )
			{
				appObject->WriteProfile( false, "", "language", "de" );
			}
			else
			{
				appObject->WriteProfile( false, "", "language", "en" );
			}
			break;
		default:
			return ToolBarWindow_form::handleCommand( cmd );
	}

	return psPROCESSED;
}

ProcessStatus ToolBarWindow::ddeInitiate( const char *application, const char *topic )
{
	if( !strcmpi( application, "MIDI_TOOLS" )
	&&  !strcmpi( topic, "SYSTEM" ) )
	{
		return psPROCESSED;
	}
	return psDO_DEFAULT;
}

ProcessStatus ToolBarWindow::ddeExecute( const char *commandPtr )
{
	if( !strncmpi( commandPtr, "open ", sizeof( "open " ) -1 ) )
	{
		openFile( commandPtr + sizeof( "open" ) );
		return psPROCESSED;
	}
	return psDO_DEFAULT;
}


// --------------------------------------------------------------------- //
// ----- class publics ------------------------------------------------- //
// --------------------------------------------------------------------- //

SuccessCode ToolBarWindow::create( const char *cmdLine )
{
	doEnterFunction( "ToolBarWindow::create" );

	static ACCEL accelerators[] =
	{
		{ FVIRTKEY|FCONTROL,	VK_ADD,	winlibGUI::playButton_id },
		{ FVIRTKEY|FCONTROL,	'R',	winlibGUI::recorderButton_id },
		{ FVIRTKEY|FCONTROL,	'D',	winlibGUI::drumButton_id },
		{ FVIRTKEY|FCONTROL,	'L',	winlibGUI::loopButton_id },
		{ FVIRTKEY|FCONTROL,	'S',	winlibGUI::synthesizerButton_id },
		{ FVIRTKEY|FCONTROL,	'M',	winlibGUI::midiDevicesButton_id },
	};
	setAccelerators( accelerators, arraySize( accelerators ) );

	SuccessCode result = ToolBarWindow_form::create( NULL );
	if( result == scSUCCESS )
	{
		restoreWindowRect();

		if( *cmdLine )
		{
			openFile( cmdLine );
		}
	}

	return result;
}

// --------------------------------------------------------------------- //
// ----- entry points -------------------------------------------------- //
// --------------------------------------------------------------------- //


