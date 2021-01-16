/*
		Project:		MIDI Tools
		Module:			MIDIloopEditor.h
		Description:	The (drum) loop editor
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

#include <gak/xmlParser.h>
#include <gak/directory.h>
#include <gak/numericString.h>

#include <winlib/winlib.h>
#include <winlib/winapp.h>

#include "MIDIloopEditor.h"

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
// ----- class constructors/destructors -------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class privates ------------------------------------------------ //
// --------------------------------------------------------------------- //

unsigned long MIDIloopEditor::compileLoop2Midi( void )
{
	unsigned long	loopPoint = 0;
	MIDIdata		entryData;
	double			entryLength, timeOffset = 0;

	midiData.clear();
	midiData.setBPM( atoi( bpmEdit->getText() ), false, 0 );

	firstLoopEntry = midiListBox->getSelection();
	for( std::size_t i=0; i<theLoopList.size(); i++ )
	{
		LoopEntry	&theLoopEntry = theLoopList[i];
		if( i == (size_t)firstLoopEntry )
		{
			loopPoint = (unsigned long)(timeOffset);
		}

		entryData.clear();
		if( !entryData.loadMidiFile(theLoopEntry.fileName ) )
		{
			if( entryData.getBPM() != midiData.getBPM() )
				entryData.setBPM( midiData.getBPM(), true, 64 );

			theLoopEntry.timeCode = (unsigned long)(timeOffset);
			entryLength = entryData.getNumBars() * entryData.getTimePerBarDbl();

			for( int j=0; j<theLoopEntry.counter; j++ )
			{
				for( 
					MIDIdata::const_iterator it = entryData.cbegin(),
						endIT = entryData.cend();
					it != endIT;
					++it
				)
				{
					MIDIevent	theEvent = *it;
					if( timeOffset )
					{
						theEvent.setTimeCode( (unsigned long)(theEvent.getTimeCode() + timeOffset) );
					}
					midiData.addElement( theEvent );
				}

				timeOffset += entryLength;
			}
		}
	}

	midiData.optimizeMIDI();

	return loopPoint;
}

void MIDIloopEditor::saveLoopXmlFile( const STRING &fileName )
{
	int 	startEntry = midiListBox->getSelection();
	STRING	relativePath;

	xml::Any *xmlMidiLoop = new xml::Any( "MIDI_LOOP" );
	xmlMidiLoop->setStringAttribute( "BPM", bpmEdit->getText() );
	xmlMidiLoop->setIntegerAttribute( "START", startEntry );

	for( 
		LoopEntrys::const_iterator it = theLoopList.cbegin(),
			endIT = theLoopList.cend();
		it != endIT;
		++it
	)
	{
		if( it->counter )
		{
			xml::Any *xmlLoopEntry = static_cast<xml::Any*>(xmlMidiLoop->addObject( new xml::Any( "LOOP_ENTRY" ) ));

			relativePath = makeRelPath( fileName, it->fileName );
			xmlLoopEntry->setStringAttribute( "FILE_NAME", relativePath );

			xmlLoopEntry->setIntegerAttribute( "COUNTER", it->counter );
		}
	}

	STRING	xmlCode = xmlMidiLoop->generateDoc();
	std::ofstream	fStream( fileName );
	if( fStream.good() )
	{
		fStream << xmlCode;
	}
	changedFlag = false;
}

void MIDIloopEditor::saveLoopFile( void )
{
	doEnterFunction("MIDIloopEditor::saveLoopFile");

	SaveFileAsDialog		dlg;

	if( midiPlayer  )
		stopPlayMidi();

	dlg.setFilename( lastMidiFile );
	if( dlg.getDirectory().isEmpty() )
		dlg.setPersonalMusic();
	dlg.addDefaultExtension( "loop" );
	dlg.addDefaultExtension( "csv" );
	dlg.addDefaultExtension( "mid" );
	if( dlg.create( this, winlibGUI::SAVE_LOOP_id, winlibGUI::LoopFiles_ids, winlibGUI::LoopFiles_count ) )
	{
		lastMidiFile = dlg.getFilename();
		if( dlg.getFilterIndex() == 1 )
			saveLoopXmlFile( lastMidiFile );
		else if( dlg.getFilterIndex() == 2 )
		{
			compileLoop2Midi();
			saveMidi2CSV( lastMidiFile );
		}
		else
		{
			compileLoop2Midi();
			midiData.saveMidiFile( lastMidiFile, true );
		}

		changedFlag = false;
	}
}


void MIDIloopEditor::playMidi( void )
{
	doEnterFunction("MIDIloopEditor::playMidi");

	/*
		if our current status is playing
		whe change the status to stop
	*/
	if( midiPlayer  )
		stopPlayMidi();
	/*
		else if we got a midiDevice
	*/
	else
	{
		unsigned long loopPoint = compileLoop2Midi();
		if( midiData.size() && openOutMidi() )
		{
			// now we can start playing
			midiPlayer = new MidiLoopPlayerThread( this, &midiData, loopPoint );
			midiPlayer->StartThread();
			showStopLabel();
		}
	}
}

void MIDIloopEditor::stopPlayMidi( void )
{
	doEnterFunction("MIDIloopEditor::stopPlayMidi");

	if( midiPlayer )
	{
		midiPlayer->StopThread();
		while( midiPlayer->isRunning )
		{
			idleLoop();
		}
		midiPlayer = NULL;
	}

	stopOutMidi();

	midiListBox->selectEntry( firstLoopEntry );

	showPlayLabel();
}

STRING MIDIloopEditor::makeListBoxEntry( const STRING &fileName )
{
	STRING	newEntry = fileName;

	if( strlen( newEntry ) > 80 )
	{
		newEntry += strlen( newEntry ) - 80;
		newEntry.insStr( 0, "..." );
	}
	return newEntry;
}


// --------------------------------------------------------------------- //
// ----- class virtuals ------------------------------------------------ //
// --------------------------------------------------------------------- //

void MidiLoopPlayerThread::ExecuteThread( void )
{
	bool			firstRun = true;
	std::size_t		firstLoopEntry = 0;
	char			tmpBuffer[32];
	unsigned char	message;
	clock_t			actTime, nextTime , clockTime, elapsedTime, startTime;
	int				lastSeconds, seconds, minutes, hours;
	int				lastBar, currentBar, timePerBar = midiData->getTimePerBar();

	unsigned long	totalTime = (unsigned long)(midiData->getNumBars() * midiData->getTimePerBarDbl() + 0.5);

	// send voice changes to the device
	for( 
		MIDIdata::const_iterator it = midiData->cbegin(),
			endIT = midiData->cend();
		!terminated && it != endIT;
		++it
	)
	{
		const MIDIevent &midiMsg = *it;
		message = midiMsg.getMessage();

		if( message != MIDI_NOTE_ON && message != MIDI_NOTE_OFF )
		{
			loopEditor->playMidiEvent( midiMsg, true );
		}
	}

	if( loopPoint )
	{
			std::size_t i = 0;
			for( 
			MIDIdata::const_iterator it = midiData->cbegin(),
				endIT = midiData->cend();
			!terminated && it != endIT;
			++it, ++i
		)
			if( it->getTimeCode() >= loopPoint )
			{
				firstLoopEntry = i;
				break;
			}
	}

	for( int i=0; i<4 && !terminated; i++ )
	{
		FlashWindow(loopEditor->handle(), true);
		Sleep( timePerBar );
	}
	FlashWindow(loopEditor->handle(), false);

	lastBar = -1;
	lastSeconds = -1;
	clockTime = startTime  = clock();
	while( !terminated )
	{
		if( !midiData->size() )
		{
			Sleep( totalTime );
		}
		else
		{
			for( 
				std::size_t i=(firstRun ? 0 : firstLoopEntry);
				!terminated && i<midiData->size(); 
				++i
			)
			{
				const MIDIevent &midiMsg = (*midiData)[i];

				actTime = clock();

				elapsedTime = actTime - clockTime;
				seconds = elapsedTime/CLK_TCK;
				currentBar = (actTime - startTime) / timePerBar + 1;
				if( seconds != lastSeconds || currentBar != lastBar )
				{
					lastSeconds = seconds;
					lastBar = currentBar;
					minutes = seconds / 60;
					seconds = seconds % 60;
					hours = minutes / 60;
					minutes = minutes % 60;

					sprintf(
						tmpBuffer, "%04d - %02d:%02d:%02d",
						currentBar, hours, minutes, seconds
					);
					loopEditor->showClock(midiMsg.getTimeCode(),tmpBuffer);
				}

				nextTime = startTime + midiMsg.getTimeCode();
				if( nextTime > actTime )
					Sleep((nextTime-actTime)*1000/CLK_TCK);

				loopEditor->playMidiEvent( midiMsg, true );
			}
			firstRun = false;
		}
		startTime += totalTime - loopPoint;
	}
}

ProcessStatus MIDIloopEditor::handleCreate( void )
{
	initMidiPlaySelect( playSelect, playButton );

	int	tabStop = 325;
	midiListBox->setTabStops( 1, &tabStop );

	return psDO_DEFAULT;
}

ProcessStatus MIDIloopEditor::handleDestroy( void )
{
	if( midiPlayer )
		stopPlayMidi();

	stopOutMidi();

	return psDO_DEFAULT;
}

bool MIDIloopEditor::canClose( void )
{
	doEnterFunction("MIDIloopEditor::canClose");

	saveWindowRect();
	if( midiPlayer )
	{
		stopPlayMidi();
	}

	stopOutMidi();

	if( changedFlag )
	{
		int button = messageBox(
			winlibGUI::LOOP_CHANGED_id,
			"Loop Editor",
			MB_ICONQUESTION|MB_YESNOCANCEL
		);
		if( button == IDYES	)
			saveLoopFile();
		else if( button == IDCANCEL )
		{
/*@*/		return false;
		}
	}
	return true;
}

ProcessStatus MIDIloopEditor::handleCommand( int cmd )
{
	doEnterFunction("MIDIloopEditor::handleCommand");

	switch( cmd )
	{
		case winlibGUI::playButton_id:
			playMidi();
			break;
		default:
/***/		return MIDIloopEditor_form::handleCommand( cmd );
	}
	return psPROCESSED;
}


ProcessStatus MIDIloopEditor::handleButtonClick( int btn )
{
	doEnterFunction("MIDIloopEditor::handleButtonClick");

	int		selectedEntry;
	STRING	newEntry;

	switch( btn )
	{
		case winlibGUI::saveLoopButton_id:
			saveLoopFile();
			break;

		case winlibGUI::loadLoopButton_id:
			loadLoopFile();
			break;

		case winlibGUI::longerButton_id:
			selectedEntry = midiListBox->getSelection();
			if( selectedEntry >= 0  && theLoopList[selectedEntry].counter < 99 )
			{
				theLoopList[selectedEntry].counter++;
				newEntry = theLoopList[selectedEntry].listBoxValue;
				newEntry += '\t';
				newEntry += formatNumber(theLoopList[selectedEntry].counter, 2);
				midiListBox->replaceEntry( selectedEntry, newEntry );
				midiListBox->selectEntry( selectedEntry );
				changedFlag = true;
			}
			break;

		case winlibGUI::shorterButton_id:
			selectedEntry = midiListBox->getSelection();
			if( selectedEntry >= 0 && theLoopList[selectedEntry].counter )
			{
				theLoopList[selectedEntry].counter--;
				newEntry = theLoopList[selectedEntry].listBoxValue;
				newEntry += '\t';
				newEntry += formatNumber(theLoopList[selectedEntry].counter, 2);
				midiListBox->replaceEntry( selectedEntry, newEntry );
				midiListBox->selectEntry( selectedEntry );
				changedFlag = true;
			}
			break;

		case winlibGUI::upButton_id:
			selectedEntry = midiListBox->getSelection();
			if( selectedEntry > 0 )
			{
				LoopEntry	tmpVal = theLoopList[selectedEntry-1];
				theLoopList[selectedEntry-1] = theLoopList[selectedEntry];
				theLoopList[selectedEntry] = tmpVal;

				newEntry = theLoopList[selectedEntry].listBoxValue;
				newEntry += '\t';
				newEntry += formatNumber(theLoopList[selectedEntry].counter, 2);
				midiListBox->replaceEntry( selectedEntry, newEntry );
				selectedEntry--;
				newEntry = theLoopList[selectedEntry].listBoxValue;
				newEntry += '\t';
				newEntry += formatNumber(theLoopList[selectedEntry].counter, 2);
				midiListBox->replaceEntry( selectedEntry, newEntry );
				midiListBox->selectEntry( selectedEntry );
				changedFlag = true;
			}
			break;

		case winlibGUI::downButton_id:
			selectedEntry = midiListBox->getSelection();
			if( selectedEntry >= 0 && selectedEntry < int(midiListBox->getNumEntries()) -1 )
			{
				LoopEntry	tmpVal = theLoopList[selectedEntry+1];
				theLoopList[selectedEntry+1] = theLoopList[selectedEntry];
				theLoopList[selectedEntry] = tmpVal;

				newEntry = theLoopList[selectedEntry].listBoxValue;
				newEntry += '\t';
				newEntry += formatNumber(theLoopList[selectedEntry].counter, 2);
				midiListBox->replaceEntry( selectedEntry, newEntry );
				selectedEntry++;
				newEntry = theLoopList[selectedEntry].listBoxValue;
				newEntry += '\t';
				newEntry += formatNumber(theLoopList[selectedEntry].counter, 2);
				midiListBox->replaceEntry( selectedEntry, newEntry );
				midiListBox->selectEntry( selectedEntry );
				changedFlag = true;
			}
			break;
		default:
/***/		return MIDIloopEditor_form::handleButtonClick( btn );
	}
	return psPROCESSED;
}

void MIDIloopEditor::handleFile( const char *filename, size_t, size_t )
{
	const char	*dotPos = strrchr( filename, '.' );

	if( dotPos
	&& (!strcmpi( dotPos, ".mid" ) || !strcmpi( dotPos, ".drum" ) ) )
	{
		LoopEntry	newData;

		STRING	newEntry = filename;
		newData.fileName = newEntry;
		newData.counter = 1;

		newData.listBoxValue = makeListBoxEntry( newEntry );

		theLoopList.addElement( newData );


		newEntry += "\t01";

		midiListBox->addEntry( newData.listBoxValue );
		midiListBox->selectEntry(int(theLoopList.size())-1);

		changedFlag = true;
	}
}

// --------------------------------------------------------------------- //
// ----- class publics ------------------------------------------------- //
// --------------------------------------------------------------------- //

void MIDIloopEditor::create( void )
{
	SuccessCode error = MIDIloopEditor_form::create( NULL );
	if( error == scSUCCESS )
	{
		restoreWindowRect();
	}
}

void MIDIloopEditor::showClock( unsigned long timeCode, const char *time )
{
	bool		found = false;
	const int	numElements = int(theLoopList.size());
	clockText->setText( time );
	for( int i=1; i<numElements; i++ )
	{
		if( theLoopList[i].timeCode > timeCode )
		{
			midiListBox->selectEntry( i-1 );
			found = true;
			break;
		}
	}
	if( !found )
	{
		midiListBox->selectEntry( numElements-1 );
	}
}

void MIDIloopEditor::loadLoopFile( const char *cmdLine )
{
	if( midiPlayer  )
		stopPlayMidi();

	if( changedFlag )
	{
		if(
			messageBox(
				winlibGUI::LOOP_CHANGED_id,
				"Loop Editor",
				MB_ICONQUESTION|MB_YESNO
			) == IDYES
		)
		{
			saveLoopFile();
		}
	}

	OpenFileDialog	dlg;

	dlg.setFilename( lastMidiFile );
	if( dlg.getDirectory().isEmpty() )
	{
		dlg.setPersonalMusic();
	}
	if( cmdLine || dlg.create( this, winlibGUI::OPEN_LOOP_id, winlibGUI::LoopFiles_id ) )
	{
		lastMidiFile = cmdLine ? cmdLine : dlg.getFilename();

		xml::Parser	theParser( lastMidiFile );

		xml::Document *xmlMidiLoopDoc = theParser.readFile( false );
		if( xmlMidiLoopDoc )
		{
			xml::Element *xmlMidiLoop = xmlMidiLoopDoc->getElement( "MIDI_LOOP" );
			if( xmlMidiLoop )
			{
				STRING		value;

				value = xmlMidiLoop->getAttribute( "BPM" );
				if( value[0U] )
					bpmEdit->setText( value );

				theLoopList.clear();
				midiListBox->clearEntries();

				for( size_t i=0; i<xmlMidiLoop->getNumObjects(); i++ )
				{
					xml::Element *xmlLoopEntry = xmlMidiLoop->getElement( i );

					if( xmlLoopEntry && xmlLoopEntry->getTag() == "LOOP_ENTRY" )
					{
						LoopEntry	theLoopEntry;
						STRING		fileName;
						value = xmlLoopEntry->getAttribute( "FILE_NAME" );
						fileName = makeFullPath( lastMidiFile, value );
						theLoopEntry.fileName = fileName;
						theLoopEntry.listBoxValue = makeListBoxEntry( theLoopEntry.fileName );
						value = xmlLoopEntry->getAttribute( "COUNTER" );
						theLoopEntry.counter = value.getValueE<unsigned>();

						theLoopList.addElement( theLoopEntry );

						value = theLoopEntry.listBoxValue;
						value += '\t';
						value += formatNumber(theLoopEntry.counter, 2);

						midiListBox->addEntry( value );
					}
				}

				value = xmlMidiLoop->getAttribute( "START" );
				if( value[0U] )
				{
					int startEntry;
					startEntry = value.getValueE<unsigned>();
					midiListBox->selectEntry( startEntry );
				}
			}
		}
	}

}

// --------------------------------------------------------------------- //
// ----- entry points -------------------------------------------------- //
// --------------------------------------------------------------------- //


