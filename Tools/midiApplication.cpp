/*
		Project:		MIDI Tools
		Module:			midiApplication.cpp
		Description:	The application controller
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
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <winlib/f_type.h>

#include "midiApplication.h"
#include "ToolBarWindow.h"

// --------------------------------------------------------------------- //
// ----- constants ----------------------------------------------------- //
// --------------------------------------------------------------------- //

using namespace winlib;
using namespace gak;

// --------------------------------------------------------------------- //
// ----- type definitions ---------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module data --------------------------------------------------- //
// --------------------------------------------------------------------- //

MIDIapplication	midiApp;

// --------------------------------------------------------------------- //
// ----- class privates ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class virtuals ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class publics ------------------------------------------------- //
// --------------------------------------------------------------------- //

bool MIDIapplication::startApplication( HINSTANCE , const char * )
{
	doEnableLogEx(gakLogging::llInfo);
	doIgnoreThreads();

	playerHandles.initDevices();
	recorderHandles.initDevices();

	FileTypeRegistry	fType;

	STRING	language = GetProfile( "", "language", "" );
	if( !language.isEmpty() )
	{
		STRING	ressourceLib = "midi_";
		ressourceLib += language;
		ressourceLib += ".dll";

		HINSTANCE resInstance = LoadLibrary( ressourceLib );
		if( int(resInstance) > 32 )
		{
			setResInstance( resInstance );
		}
		setCountry( language );
	}

	getFileType( "MID", "open", &fType );
	fType.extension = ".MID";
	if( !fType.type[0U] )
		fType.type = "midiFile";
	if( !fType.type_description[0U] )
		fType.type_description = "MIDI Datei";
	fType.cmd = "midiEdit";
	fType.cmd_description = "&Bearbeiten mit MIDI Tools";
	if( !fType.icon[0U] )
	{
		fType.icon = getFileName();
		fType.icon += ",0";
	}
	fType.commandLine = '"';
	fType.commandLine += getFileName();
	fType.commandLine += '\"';
	fType.ddeCommand = "open %1",
	fType.ddeIfExec = "",
	fType.application = "MIDI_TOOLS";
	fType.topic = "";

	addFileType( &fType );


	fType.extension = ".MSET";
	fType.type = "midiSettings";
	fType.type_description = "MIDI Tools Einstellungen";
	fType.cmd = "open";
	fType.cmd_description = "Ö&ffnen";
	fType.icon = getFileName();
	fType.icon += ",1";
	fType.commandLine = '"';
	fType.commandLine += getFileName();
	fType.commandLine += '\"';
	fType.ddeCommand = "open %1",
	fType.ddeIfExec = "",
	fType.application = "MIDI_TOOLS";
	fType.topic = "";

	addFileType( &fType );

	fType.extension = ".DRUM";
	fType.type = "midiDrumPatterns";
	fType.type_description = "MIDI Tools Drum Patterns";
	fType.cmd = "open";
	fType.cmd_description = "Ö&ffnen";
	fType.icon = getFileName();
	fType.icon += ",2";
	fType.commandLine = '"';
	fType.commandLine += getFileName();
	fType.commandLine += '\"';
	fType.ddeCommand = "open %1",
	fType.ddeIfExec = "",
	fType.application = "MIDI_TOOLS";
	fType.topic = "";

	addFileType( &fType );

	fType.extension = ".LOOP";
	fType.type = "midiLoop";
	fType.type_description = "MIDI Tools Loops";
	fType.cmd = "open";
	fType.cmd_description = "Ö&ffnen";
	fType.icon = getFileName();
	fType.icon += ",3";
	fType.commandLine = '"';
	fType.commandLine += getFileName();
	fType.commandLine += '\"';
	fType.ddeCommand = "open %1",
	fType.ddeIfExec = "",
	fType.application = "MIDI_TOOLS";
	fType.topic = "";

	addFileType( &fType );

	return false;
}

CallbackWindow *MIDIapplication::createMainWindow( const char *cmdLine, int  )
{
	ToolBarWindow *theToolBar;

	theToolBar = new ToolBarWindow;
	SuccessCode result = theToolBar->create( cmdLine );
	if( result == scSUCCESS )
	{
		return theToolBar;
	}

	theToolBar->messageBox( "Unable to create main window" );

	delete theToolBar;
	return NULL;
}

void MIDIapplication::deleteMainWindow( BasicWindow *mainWindow )
{
	delete mainWindow;
}


// --------------------------------------------------------------------- //
// ----- entry points -------------------------------------------------- //
// --------------------------------------------------------------------- //


