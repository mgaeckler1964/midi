/*
		Project:		MIDI Tools
		Module:			MIDIloopEditor.h
		Description:	The (drum) loop editor
		Author:			Martin Gäckler
		Address:		Hofmannsthalweg 14, A-4030 Linz
		Web:			https://www.gaeckler.at/

		Copyright:		(c) 2005-2026 Martin Gäckler

		This program is free software: you can redistribute it and/or modify  
		it under the terms of the GNU General Public License as published by  
		the Free Software Foundation, version 3.

		You should have received a copy of the GNU General Public License 
		along with this program. If not, see <http://www.gnu.org/licenses/>.

		THIS SOFTWARE IS PROVIDED BY Martin Gäckler, Linz, Austria ``AS IS''
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
#include <gak/fmtNumber.h>

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

	m_firstLoopEntry = midiListBox->getSelection();
	for( std::size_t i=0; i<m_theLoopList.size(); i++ )
	{
		LoopEntry	&theLoopEntry = m_theLoopList[i];
		if( i == (size_t)m_firstLoopEntry )
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
		LoopEntrys::const_iterator it = m_theLoopList.cbegin(),
			endIT = m_theLoopList.cend();
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
	m_changedFlag = false;
}

void MIDIloopEditor::saveLoopFile( void )
{
	doEnterFunction("MIDIloopEditor::saveLoopFile");

	SaveFileAsDialog		dlg;

	if( m_midiPlayer  )
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

		m_changedFlag = false;
	}
}


void MIDIloopEditor::playMidi( void )
{
	doEnterFunction("MIDIloopEditor::playMidi");

	/*
		if our current status is playing
		whe change the status to stop
	*/
	if( m_midiPlayer  )
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
			m_midiPlayer = new MidiLoopPlayerThread( this, &midiData, loopPoint );
			m_midiPlayer->StartThread();
			showStopLabel();
		}
	}
}

void MIDIloopEditor::stopPlayMidi( void )
{
	doEnterFunction("MIDIloopEditor::stopPlayMidi");

	if( m_midiPlayer )
	{
		m_midiPlayer->StopThread();
		while( m_midiPlayer->isRunning )
		{
			idleLoop();
		}
		m_midiPlayer = nullptr;
	}

	stopOutMidi();

	midiListBox->selectEntry( m_firstLoopEntry );

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
	bool				firstRun = true;
	std::size_t			firstLoopEntry = 0;
	NumberBuffer		tmpBuffer;
	unsigned char		message;
	clock_t				actTime, nextTime , clockTime, elapsedTime, startTime;
	int					lastSeconds, seconds, minutes, hours;
	int					lastBar, currentBar, timePerBar = m_midiData->getTimePerBar();

	unsigned long	totalTime = (unsigned long)(m_midiData->getNumBars() * m_midiData->getTimePerBarDbl() + 0.5);

	// send voice changes to the device
	for( 
		MIDIdata::const_iterator it = m_midiData->cbegin(),
			endIT = m_midiData->cend();
		!terminated && it != endIT;
		++it
	)
	{
		const MIDIevent &midiMsg = *it;
		message = midiMsg.getMessage();

		if( message != MIDI_NOTE_ON && message != MIDI_NOTE_OFF )
		{
			m_loopEditor->playMidiEvent( midiMsg, true );
		}
	}

	if( m_loopPoint )
	{
			std::size_t i = 0;
			for( 
			MIDIdata::const_iterator it = m_midiData->cbegin(),
				endIT = m_midiData->cend();
			!terminated && it != endIT;
			++it, ++i
		)
			if( it->getTimeCode() >= m_loopPoint )
			{
				firstLoopEntry = i;
				break;
			}
	}

	for( int i=0; i<4 && !terminated; i++ )
	{
		FlashWindow(m_loopEditor->handle(), true);
		Sleep( timePerBar );
	}
	FlashWindow(m_loopEditor->handle(), false);

	lastBar = -1;
	lastSeconds = -1;
	clockTime = startTime  = clock();
	while( !terminated )
	{
		if( !m_midiData->size() )
		{
			Sleep( totalTime );
		}
		else
		{
			for( 
				std::size_t i=(firstRun ? 0 : firstLoopEntry);
				!terminated && i<m_midiData->size(); 
				++i
			)
			{
				const MIDIevent &midiMsg = (*m_midiData)[i];

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

					formatNumberFast( &tmpBuffer, currentBar, 4, '0' );
					tmpBuffer.addCP(" - ");
					appendNumberFast( &tmpBuffer, hours, 2, '0' );
					tmpBuffer.addDigit(':');
					appendNumberFast( &tmpBuffer, minutes, 2, '0' );
					tmpBuffer.addDigit(':');
					appendNumberFast( &tmpBuffer, seconds, 2, '0' );
					m_loopEditor->showClock(midiMsg.getTimeCode(),tmpBuffer.c_str());
				}

				nextTime = startTime + midiMsg.getTimeCode();
				if( nextTime > actTime )
					Sleep((nextTime-actTime)*1000/CLK_TCK);

				m_loopEditor->playMidiEvent( midiMsg, true );
			}
			firstRun = false;
		}
		startTime += totalTime - m_loopPoint;
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
	if( m_midiPlayer )
		stopPlayMidi();

	stopOutMidi();

	return psDO_DEFAULT;
}

bool MIDIloopEditor::canClose( void )
{
	doEnterFunction("MIDIloopEditor::canClose");

	saveWindowRect();
	if( m_midiPlayer )
	{
		stopPlayMidi();
	}

	stopOutMidi();

	if( m_changedFlag )
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
			if( selectedEntry >= 0  && m_theLoopList[selectedEntry].counter < 99 )
			{
				m_theLoopList[selectedEntry].counter++;
				newEntry = m_theLoopList[selectedEntry].listBoxValue;
				newEntry += '\t';
				newEntry += formatNumber(m_theLoopList[selectedEntry].counter, 2);
				midiListBox->replaceEntry( selectedEntry, newEntry );
				midiListBox->selectEntry( selectedEntry );
				m_changedFlag = true;
			}
			break;

		case winlibGUI::shorterButton_id:
			selectedEntry = midiListBox->getSelection();
			if( selectedEntry >= 0 && m_theLoopList[selectedEntry].counter )
			{
				m_theLoopList[selectedEntry].counter--;
				newEntry = m_theLoopList[selectedEntry].listBoxValue;
				newEntry += '\t';
				newEntry += formatNumber(m_theLoopList[selectedEntry].counter, 2);
				midiListBox->replaceEntry( selectedEntry, newEntry );
				midiListBox->selectEntry( selectedEntry );
				m_changedFlag = true;
			}
			break;

		case winlibGUI::upButton_id:
			selectedEntry = midiListBox->getSelection();
			if( selectedEntry > 0 )
			{
				LoopEntry	tmpVal = m_theLoopList[selectedEntry-1];
				m_theLoopList[selectedEntry-1] = m_theLoopList[selectedEntry];
				m_theLoopList[selectedEntry] = tmpVal;

				newEntry = m_theLoopList[selectedEntry].listBoxValue;
				newEntry += '\t';
				newEntry += formatNumber(m_theLoopList[selectedEntry].counter, 2);
				midiListBox->replaceEntry( selectedEntry, newEntry );
				selectedEntry--;
				newEntry = m_theLoopList[selectedEntry].listBoxValue;
				newEntry += '\t';
				newEntry += formatNumber(m_theLoopList[selectedEntry].counter, 2);
				midiListBox->replaceEntry( selectedEntry, newEntry );
				midiListBox->selectEntry( selectedEntry );
				m_changedFlag = true;
			}
			break;

		case winlibGUI::downButton_id:
			selectedEntry = midiListBox->getSelection();
			if( selectedEntry >= 0 && selectedEntry < int(midiListBox->getNumEntries()) -1 )
			{
				LoopEntry	tmpVal = m_theLoopList[selectedEntry+1];
				m_theLoopList[selectedEntry+1] = m_theLoopList[selectedEntry];
				m_theLoopList[selectedEntry] = tmpVal;

				newEntry = m_theLoopList[selectedEntry].listBoxValue;
				newEntry += '\t';
				newEntry += formatNumber(m_theLoopList[selectedEntry].counter, 2);
				midiListBox->replaceEntry( selectedEntry, newEntry );
				selectedEntry++;
				newEntry = m_theLoopList[selectedEntry].listBoxValue;
				newEntry += '\t';
				newEntry += formatNumber(m_theLoopList[selectedEntry].counter, 2);
				midiListBox->replaceEntry( selectedEntry, newEntry );
				midiListBox->selectEntry( selectedEntry );
				m_changedFlag = true;
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
		LoopEntry	&newData = m_theLoopList.createElement();

		STRING	newEntry = filename;
		newData.fileName = newEntry;
		newData.counter = 1;

		newEntry = makeListBoxEntry( newEntry );
		newData.listBoxValue = newEntry;

		newEntry += "\t01";
		midiListBox->addEntry(newEntry);
		midiListBox->selectEntry(int(m_theLoopList.size())-1);

		m_changedFlag = true;
	}
}

// --------------------------------------------------------------------- //
// ----- class publics ------------------------------------------------- //
// --------------------------------------------------------------------- //

void MIDIloopEditor::create( void )
{
	SuccessCode error = MIDIloopEditor_form::create( nullptr );
	if( error == scSUCCESS )
	{
		acceptFiles();
		restoreWindowRect();
	}
}

void MIDIloopEditor::showClock( unsigned long timeCode, const char *time )
{
	bool		found = false;
	const int	numElements = int(m_theLoopList.size());
	clockText->setText( time );
	for( int i=1; i<numElements; i++ )
	{
		if( m_theLoopList[i].timeCode > timeCode )
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
	if( m_midiPlayer  )
		stopPlayMidi();

	if( m_changedFlag )
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

				m_theLoopList.clear();
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

						m_theLoopList.addElement( theLoopEntry );

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


