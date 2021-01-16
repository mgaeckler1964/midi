/*
		Project:		MIDI Tools
		Module:			MIDIdrumWindow.cpp
		Description:	The drum patterns editor
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

#include <gak/fmtNumber.h>
#include <gak/numericString.h>
#include <gak/fixedHeapArray.h>

#include <winlib/winlib.h>
#include <winlib/winapp.h>
#include <winlib/device.h>

#include "MIDIdrumWindow.h"
#include "MIDIpatternVolume.h"
#include "MIDIpatternStereo.h"

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

static char *lengthValues[] =
{
	"1",
	"2",
	"4",
	"8",
	"16",
	"32",
	"64"
};


// --------------------------------------------------------------------- //
// ----- class static data --------------------------------------------- //
// --------------------------------------------------------------------- //

int MIDIdrumWindow::lastSelectedLengthIdx	= 4;
int MIDIdrumWindow::lastSelectedGridIdx		= 6;

// --------------------------------------------------------------------- //
// ----- prototypes ---------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module functions ---------------------------------------------- //
// --------------------------------------------------------------------- //

static void removeStereoEvents( MIDIdata *midiData )
{
	for( int i=int(midiData->size())-1; i>=0; i-- )
	{
		const MIDIevent	&theEvent = (*midiData)[i];
		if( theEvent.getMessage() == MIDI_CONTROLLER && theEvent.getTrack() )
		{
			unsigned char controller = theEvent.getData1();
			if( controller == MIDI_PAN_POSITION
			||  controller == MIDI_BALANCE_POSITION )
			{
				midiData->removeElementAt( i );
			}
		}
	}
}

static char findStereoPos( const MIDIdata &midiData, unsigned char channel )
{
	char stereoPos = 0;

	for( 
		MIDIdata::const_iterator it = midiData.cbegin(),
			endIT = midiData.cend();
		it != endIT;
		++it
	)
	{
		const MIDIevent	&theEvent = *it;
		if( theEvent.getChannel() == channel )
		{
			if( theEvent.getMessage() == MIDI_CONTROLLER )
			{
				unsigned char controller = theEvent.getData1();
				if( controller == MIDI_PAN_POSITION
				||  controller == MIDI_BALANCE_POSITION )
				{
					stereoPos = char(theEvent.getData2() - MIDI_CENTER_PAN);
					break;
				}
			}
		}
	}
	return stereoPos;
}

/*
	check for a track which drum pattern and channel it uses
*/
static std::size_t findDrumVoiceNchannel(
	const MIDIdata		&midiData,
	const DrumVoices	&theVoices,
	short				trackNo,
	unsigned char		*theChannel
)
{
	bool			voiceFound;
	ChannelCodes	channelCodes;

	for( 
		MIDIdata::const_iterator it = midiData.cbegin(),
			endIT = midiData.cend();
		it != endIT;
		++it
	)
	{
		const MIDIevent	&theEvent = *it;
		if( theEvent.getTrack() == trackNo )
		{
			if( theEvent.getMessage() == MIDI_NOTE_ON )
			{
				if( theVoices.isGeneralMIDI() )
				{
					for( std::size_t j=0; j<theVoices.size(); j++ )
					{
						const DrumVoice	&theVoice = theVoices[j];
						if( theVoice.note == theEvent.getData1() )
						{
							*theChannel = theEvent.getChannel();
/***/						return j;
						}
					}
				}
				else
				{
					voiceFound = midiData.getChannelCodes(
						theEvent.getTimeCode(),
						theEvent.getChannel(),
						&channelCodes
					);
					if( voiceFound )
					{
						for( std::size_t j=0; j<theVoices.size(); j++ )
						{
							const DrumVoice	&theVoice = theVoices[j];
							if( theVoice.note == theEvent.getData1()
							&& theVoice.voiceCodes.program == channelCodes.voiceCodes.program
							&& theVoice.voiceCodes.bankMSB == channelCodes.voiceCodes.bankMSB
							&& theVoice.voiceCodes.bankLSB == channelCodes.voiceCodes.bankLSB )
							{
								*theChannel = theEvent.getChannel();
/***/							return j;
							}
						}
					}
				}
			}
		}
	}

	return std::size_t(-1);
}

// --------------------------------------------------------------------- //
// ----- class static data --------------------------------------------- //
// --------------------------------------------------------------------- //

const char PatternArea::className[] = "PatternArea";

// --------------------------------------------------------------------- //
// ----- class static functions ---------------------------------------- //
// --------------------------------------------------------------------- //

void PatternArea::registerClass( void )
{
	static bool registered = false;

	if( !registered )
	{
		WNDCLASS	wc;

		fillDefaultClass( &wc );
		wc.lpszClassName	 = className;

		registered = ChildWindow::registerClass( &wc );
	}
}

// --------------------------------------------------------------------- //
// ----- class privates ------------------------------------------------ //
// --------------------------------------------------------------------- //

int PatternArea::findNote(
	unsigned long start, unsigned long end, short trackNo
)
{
	unsigned		i;
	int				foundNote = -1;
	unsigned char	message;
	unsigned long	timeCode;

	// first entry of my track
	for( i=0; i<m_midiData->size() && (*m_midiData)[i].getTrack() != trackNo; i++ )
		;

	while( i<int(m_midiData->size()) )
	{
		const MIDIevent	&theEvent = (*m_midiData)[i];
		timeCode = theEvent.getTimeCode();
		if( timeCode > end )
		{
/*v*/		break;
		}

		message = theEvent.getMessage();
		if( message == MIDI_NOTE_ON )
		{
			foundNote = i;
			if( timeCode >= start )
			{
/*v*/			break;
			}
		}
		else if( message == MIDI_NOTE_OFF )
		{
			if( timeCode > start )
			{
/*v*/			break;
			}
			foundNote = -1;
		}

		// next entry of my track
		for( i++; i<m_midiData->size() && (*m_midiData)[i].getTrack() != trackNo; i++ )
		{
			;
		}
	}

	return foundNote;
}

void MIDIdrumWindow::stopMidiPlay()
{
	doEnterFunction("MIDIdrumWindow::stopPlayMidi");

	if( midiPlayer  )
	{
		midiPlayer->StopThread();
		while( midiPlayer->isRunning )
		{
			idleLoop();
		}
		midiPlayer = NULL;
	}

	stopOutMidi();
	showPlayLabel();
}

void MIDIdrumWindow::startStopMidiPlay()
{
	doEnterFunction("MIDIdrumWindow::startStopMidiPlay");

	/*
		if our current status is playing
		whe change the status to stop
	*/
	if( midiPlayer  )
		stopMidiPlay();
	/*
		else if we got a midiDevice
	*/
	else
	{
		unsigned long	totalTime =
			midiData.getTimePerBar() * lengthSelector->getPosition();

		if( openOutMidi() )
		{
			// now we can start playing
			midiPlayer = new DrumPlayerThread(
				this, &midiData, totalTime
			);
			midiPlayer->setSelectedTrack(
				thePatternArea->getSelectedPattern() + 1
			);
			midiPlayer->setPlayMode( playModeSelect->getSelection() );
			midiPlayer->StartThread();
			showStopLabel();
		}
	}
}

void MIDIdrumWindow::saveMidi( void )
{
	size_t				channelUsageCounter[16];
	unsigned short		srcTrack, numTracks;
	SaveFileAsDialog	dlg;
	STRING				name;
	PatternInfos		&patternList = thePatternArea->getPatternList();

	stopMidiPlay();

	// remove empty tracks and count used channels:
	numTracks = midiData.getNumTracks();

	/*
		check channel usage
	*/

	// init used channel flags:
	memset( channelUsageCounter, 0, 16 );

	for( 
		PatternInfos::const_iterator it = patternList.cbegin(),
			endIT = patternList.cend();
		it != endIT;
		++it
	)
	{
		const PatternInfo	&theInfo = *it;
		channelUsageCounter[theInfo.channel]++;
	}

	for( unsigned char curChannel=0; curChannel<16; curChannel++ )
	{
		if( !channelUsageCounter[curChannel] )
		{
			midiData.removeChannel( curChannel );
		}
	}

	// setup track names
	midiData.setTrackName( 0, "VoiceChange" );
	midiData.setInstrument( 0, "VoiceChange" );
	midiData.setMidiOutDev( 0, getDefaultPlayer() );
	for( srcTrack = 1; srcTrack<numTracks; srcTrack++ )
	{
		name = patternList[srcTrack-1].drumVoice.voice;
		midiData.setTrackName( srcTrack, name );
		midiData.setInstrument( srcTrack, name );
		midiData.setMidiOutDev( srcTrack, getDefaultPlayer() );
	}

	/*
		now we can save the file
	*/

	dlg.setFilename( lastMidiFile );
	if( dlg.getDirectory().isEmpty() )
		dlg.setPersonalMusic();
	dlg.addDefaultExtension( "drum" );
	dlg.addDefaultExtension( "csv" );
	dlg.addDefaultExtension( "mid" );
	if( dlg.create( this, winlibGUI::SAVE_MIDI_id, winlibGUI::DrumFiles_ids, winlibGUI::DrumFiles_count ) )
	{
		int	index = dlg.getFilterIndex();
		lastMidiFile = dlg.getFilename();
		if( index == 1 )
			midiData.saveMidiFile( lastMidiFile, false );
		else if( index == 2 )
			saveMidi2CSV( lastMidiFile );
		else if( index == 3 )
		{
			if( theVoices.isGeneralMIDI() )
			{
				MIDIdata	midiData = this->midiData;
				/*
					add stereo position for each punsh and change channel to 9
					==========================================================
				*/
				char lastStereoPosition = 0;

				for( std::size_t i=0; i<midiData.size(); i++ )
				{
					MIDIevent		&theEvent = midiData[i];
					theEvent.setChannel( 9 );

					if( theEvent.getMessage() == MIDI_NOTE_ON )
					{
						short			track = theEvent.getTrack();
						short			pattern = short(track-1);
						char			stereoPos = patternList[pattern].stereoPos;

						if( lastStereoPosition != stereoPos )
						{
							MIDIevent	stereoEvent;
							stereoEvent.setIndex( theEvent.getIndex() );
							stereoEvent.setTrack( track );
							stereoEvent.setPanPosition( 9, stereoPos );
							stereoEvent.setTimeCode( theEvent.getTimeCode() );
							midiData.addElement( stereoEvent );
							i++;

							lastStereoPosition = stereoPos;
						}
					}
				}
			}
			midiData.saveMidiFile( lastMidiFile, true );
		}
		thePatternArea->clearChangedFlag();
	}
}

void MIDIdrumWindow::changeVolume( void )
{
	std::size_t						numTracks = midiData.getNumTracks();
	PatternVolumeDialog				dialog;
	FixedHeapArray<unsigned char>	maxVolumesBuffer(numTracks);
	unsigned char					*maxVolumes = maxVolumesBuffer.getDataBuffer();

	for( std::size_t curTrack=0; curTrack<numTracks; curTrack++ )
	{
		maxVolumes[curTrack] = 0;
	}
	for(
		MIDIdata::const_iterator it = midiData.cbegin(),
			endIT = midiData.cend();
		it != endIT;
		++it
	)
	{
		const MIDIevent &theEvent = *it;
		if( theEvent.getMessage() == MIDI_NOTE_ON )
		{
			unsigned short	curTrack = theEvent.getTrack();
			unsigned char	curVolume = theEvent.getData2();
			if( maxVolumes[curTrack] < curVolume )
			{
				maxVolumes[curTrack] = curVolume;
			}
			if( maxVolumes[0] < curVolume )
			{
				maxVolumes[0] = curVolume;
			}
		}
	}

	dialog.create( this, &thePatternArea->getPatternList(), maxVolumes, numTracks );
	if( dialog.getModalResult() == IDOK )
	{
		unsigned char newVolume = dialog.getNewVolume();
		unsigned char selectedTrack = dialog.getSelectedPattern();

		if( maxVolumes[selectedTrack] != newVolume )
		{
			double factor;

			if( maxVolumes[selectedTrack] )
			{
				factor = double(newVolume) /
					double(maxVolumes[selectedTrack]);
			}

			for( 
				MIDIdata::iterator it = midiData.begin(),
					endIT = midiData.end();
				it != endIT;
				++it
			)
			{
				MIDIevent &theEvent = *it;
				if( theEvent.getMessage() == MIDI_NOTE_ON
				&&  (
						theEvent.getTrack() == selectedTrack
						|| selectedTrack == 0
					)
				)
				{
					if( maxVolumes[selectedTrack] )
					{
						theEvent.setData2(
							static_cast<unsigned char>(double(theEvent.getData2()) * factor + 0.5)
						);
					}
					else
					{
						theEvent.setData2( newVolume );
					}
				}
			}

			thePatternArea->setChangedFlag();
			invalidateWindow();
		}
	}
}

void MIDIdrumWindow::changeStereoPosition( void )
{
	char 				newStereoPosition;
	PatternStereoDialog	dialog;

	PatternInfos		&patternList = thePatternArea->getPatternList();
	dialog.create( this, patternList );
	if( dialog.getModalResult() == IDOK )
	{
		bool	changed = false;
		bool	found;

		for(
			size_t selectedPattern = 0;
			selectedPattern<patternList.size();
			selectedPattern++
		)
		{
			PatternInfo	&thePattern = patternList[selectedPattern];

			newStereoPosition = dialog.getNewStereoPosition( selectedPattern );

			if( thePattern.stereoPos != newStereoPosition )
			{
				thePattern.stereoPos = newStereoPosition;
				ChannelCodes	channelCodes;
				thePattern.fillChannelCodes( &channelCodes );
				unsigned char newChannel = midiData.allocateChannel( &channelCodes );
				if( newChannel < 16 )
				{
					unsigned char oldChannel = thePattern.channel;
					thePattern.channel = newChannel;

					for( 
						MIDIdata::iterator it = midiData.begin(),
							endIT = midiData.end();
						it != endIT;
						++it
					)
					{
						MIDIevent	&theEvent = *it;
						if( theEvent.getTrack() == (unsigned short)selectedPattern+1 )
						{
							theEvent.setChannel( newChannel );
						}
					}
					found = false;
					for(
						PatternInfos::const_iterator it = patternList.cbegin(),
							endIT = patternList.cend();
						it != endIT;
						++it
					)
					{
						const PatternInfo	&thePattern = *it;
						if( thePattern.channel == oldChannel )
						{
							found = true;
							break;
						}
					}
					if( !found )
					{
						midiData.removeChannel( oldChannel );
					}
					changed = true;
				}
			}
		}
		if( changed )
		{
			thePatternArea->setChangedFlag();
		}
	}
}

void MIDIdrumWindow::loadVoices( void )
{
	Set<STRING>			theUsedVoices;
	MIDIplayerHandle	&theHandle = playerHandles[getDefaultPlayer()];
	STRING				drumsFile = theHandle.getDrumsCSV();

	if( drumsFile != m_lastDrumsFile )
	{
		m_lastDrumsFile = drumsFile;
		theVoices.loadDrumVoices( drumsFile );
		voiceSelect->clearEntries();

		for( 
			DrumVoices::const_iterator it = theVoices.cbegin(),
				endIT = theVoices.cend();
			it != endIT;
			++it
		)
		{
			const STRING &curVoice = it->voice;

			// do not add a voice twice
			if( !theUsedVoices.hasElement(curVoice) )
			{
				theUsedVoices.addElement( curVoice );
				voiceSelect->addEntry( curVoice ) ;
			}
		}

		voiceSelect->selectEntry( 0 );
	}
	theHandle.setGeneralMIDI( theVoices.isGeneralMIDI() );
}

// --------------------------------------------------------------------- //
// ----- class virtuals ------------------------------------------------ //
// --------------------------------------------------------------------- //

STRING PatternArea::getWindowClassName( void ) const
{
	return className;
}

ProcessStatus PatternArea::handleMouseRelease( void )
{
	if( m_dragPattern > 0 )
	{
		m_dragPattern = m_overPattern = 0;
		invalidateWindow();

		return psPROCESSED;
	}

	return psDO_DEFAULT;
}

ProcessStatus PatternArea::handleMouseMove( WPARAM, const Point &position )
{
	if( m_dragPattern >= 0 )
	{
		short	pattern = short(getPatternByMouse( position.y ));
		if( pattern >= m_patternOffset && pattern <= m_numPatterns )
		{
			if( m_overPattern != pattern )
			{
				m_overPattern = pattern;
				invalidateWindow();
			}
		}

		return psPROCESSED;
	}

	return psDO_DEFAULT;
}

ProcessStatus PatternArea::handleLeftButton( LeftButton leftButton, WPARAM, const Point &position )
{
	Size			size = getClientSize();
	int				maxLines = getMaxLines();
	SHORT			kState = GetKeyState( VK_SHIFT );

	kState >>= 8;

	unsigned		numerator = m_midiData->getNumerator();
	unsigned		denominator = m_midiData->getDenominator();
	int				numGrids = m_grid * numerator / denominator;
	int				selGrid = ( position.x > textWidth ) 
					? (position.x-textWidth)*numGrids /(size.width-textWidth)
					: -1
	;

	unsigned short	trackNo = getTrackByMouse( position.y );

	if( leftButton == lbDOWN )
	{
		if( selGrid < 0 && trackNo > m_patternOffset && trackNo <= m_patternOffset + maxLines )
		{	// click on a pattern name -> start drag mode
			m_overPattern = m_dragPattern = trackNo-1;
			captureMouse();
			invalidateWindow();
		}
		return psPROCESSED;
	}
	else if( leftButton == lbUP && m_overPattern == m_dragPattern )
	{
		// click without drag and drop

		unsigned long	startTime, endTime;
		unsigned long	timePerBar = m_midiData->getTimePerBar();
		unsigned long	totalTime = timePerBar * m_numBars;
		int				foundNote;
		unsigned char	channel, note;

		m_overPattern = m_dragPattern = -1;

		if( !trackNo && selGrid >= 0 )	// click on time bar
		{
			m_timePosition = m_midiData->getStartPosition( m_curBar, selGrid, m_grid );
			invalidateWindow();
		}
		else if( selGrid >= 0 && trackNo > m_patternOffset && trackNo <= m_patternOffset + maxLines && trackNo-1 < m_numPatterns )
		{
			// click within grid -> add or remove note
			const PatternInfo &pInfo = m_patternList[trackNo-1];
			note = pInfo.drumVoice.note;
			channel = pInfo.channel;

			startTime = m_midiData->getStartPosition( m_curBar, selGrid, m_grid );
			endTime = startTime + m_midiData->getStartPosition( 0, 1, m_beatLength )-2;

			foundNote = findNote( startTime, endTime, trackNo );

			if( foundNote >= 0 )
			{
				if( kState )	// (de)select a note
				{
					m_selectedPattern = -1;
					MIDIevent	&theEventOn = (*m_midiData)[foundNote];
					for(
						foundNote++;
						size_t(foundNote)<m_midiData->size()
						&& (*m_midiData)[foundNote].getTrack() != trackNo;
						foundNote++
					)
						;
					MIDIevent	&theEventOff = (*m_midiData)[foundNote];
					if( theEventOn.isSelected() )
					{
						theEventOn.clearSelected();
						theEventOff.clearSelected();
					}
					else
					{
						theEventOn.setSelected();
						theEventOff.setSelected();
					}
				}
				else		// remove note
				{
					m_midiData->removeElementAt( foundNote );		// NOTE_ON
					for(
						;
						size_t(foundNote)<m_midiData->size()
						&& (*m_midiData)[foundNote].getTrack() != trackNo;
						foundNote++
					)
						;
					m_midiData->removeElementAt( foundNote );		// NOTE_OFF

					m_changedFlag = true;
				}
				invalidateWindow();
			}
			else if( !kState && endTime < totalTime )
			{
				MIDIevent	newEvent;

				newEvent.setTrack( trackNo );
				newEvent.setMessageCode( MIDI_NOTE_ON, channel, note, m_volume );
				newEvent.setTimeCode( startTime );
				m_midiData->addElement( newEvent );

				newEvent.setTrack( trackNo );
				newEvent.setMessageCode( MIDI_NOTE_OFF, channel, note, m_time );
				newEvent.setTimeCode( endTime );
				m_midiData->addElement( newEvent );

				invalidateWindow();
				m_changedFlag = true;
			}
		}
		else if( trackNo > m_patternOffset && trackNo <= m_patternOffset + maxLines )
		{	// click outside grid but within pattern list
			m_midiData->clearAllSelections();
			m_selectedPattern = short(trackNo -1);
			invalidateWindow();
			getParent()->postMessage( WM_COMMAND, PATTERN_AREA, long(handle()) );
		}

		releaseMouse();
		return psPROCESSED;
	}
	else if( leftButton == lbUP )	// pattern released -> move
	{
		short	curTrack;

		for( 
			MIDIdata::iterator it = m_midiData->begin(),
				endIT = m_midiData->end();
			it != endIT;
			++it
		)
		{
			MIDIevent	&theEvent = *it;
			curTrack = theEvent.getTrack();
			if( curTrack > m_overPattern )
				theEvent.setTrack( (short)(curTrack+1) );
		}

		if( m_overPattern < m_dragPattern )
		{
			m_dragPattern++;
		}

		for( 
			MIDIdata::iterator it = m_midiData->begin(),
				endIT = m_midiData->end();
			it != endIT;
			++it
		)
		{
			MIDIevent	&theEvent = *it;
			curTrack = theEvent.getTrack();
			if( curTrack == m_dragPattern+1 )
				theEvent.setTrack( short(m_overPattern+1) );
		}

		for( 
			MIDIdata::iterator it = m_midiData->begin(),
				endIT = m_midiData->end();
			it != endIT;
			++it
		)
		{
			MIDIevent	&theEvent = *it;
			curTrack = theEvent.getTrack();
			if( curTrack > m_dragPattern )
			{
				theEvent.setTrack( (short)(curTrack-1) );
			}
		}

		m_overPattern = m_dragPattern = -1;
		refreshPatternList();
		invalidateWindow();
		m_changedFlag = true;

		releaseMouse();
		return psPROCESSED;
	}

	return psDO_DEFAULT;
}

ProcessStatus PatternArea::handleRightButton( RightButton rightButton, WPARAM modifier, const Point &position )
{
	if( rightButton == rbUP )
	{
		short			trackNo;
		unsigned long	timeCode;
		int				foundNote, noteOff;
		Size			size = getClientSize();
		int				maxLines = getMaxLines();

		unsigned		numerator = m_midiData->getNumerator();
		unsigned		denominator = m_midiData->getDenominator();
		int				numGrids = m_grid * numerator / denominator;
		int				selGrid = ( position.x > textWidth ) 
						? (position.x-textWidth)*numGrids /(size.width-textWidth)
						: -1
		;

		if( position.y < textHeight )
		{
			trackNo = -1;
		}
		else
		{
			trackNo = short((position.y-textMargin) / textHeight + m_patternOffset);
		}

		if( selGrid >= 0 && trackNo > m_patternOffset && trackNo <= m_patternOffset + maxLines && trackNo-1 < m_numPatterns )
		{
			timeCode = m_midiData->getStartPosition( m_curBar, selGrid, m_grid );

			foundNote = findNote( timeCode, timeCode, trackNo );
			if( foundNote >= 0 )
			{
				noteOff = int(m_midiData->findMatchingEvent( foundNote ));
				if( noteOff >= 0 )
				{
					MIDIevent	&theEventOn = (*m_midiData)[foundNote];
					MIDIevent	&theEventOff = (*m_midiData)[noteOff];
					if( modifier & MK_CONTROL )
					{
						m_changedFlag = true;
						theEventOn.setData2( m_volume );
						theEventOff.setData2( m_time );
						invalidateWindow();
					}
					else
					{
						m_volume = theEventOn.getData2();
						m_time = theEventOff.getData2();
						getParent()->postMessage( WM_USER, m_volume, m_time );
					}
				}
			}
		}

		return psPROCESSED;
	}

	return psDO_DEFAULT;
}

ProcessStatus PatternArea::handleRepaint( Device &hDC )
{
	doEnterFunction("PatternArea::handleRepaint");

	RectBorder		rect;

	Pen				grayPen, bluePen, redPen;
	bool			noteOnFound;

	Size			size = getClientSize();
	unsigned		numerator = m_midiData->getNumerator();
	unsigned		denominator = m_midiData->getDenominator();
	unsigned long	timeStart, timeEnd;
	unsigned long	timePerBar = m_midiData->getTimePerBar();
	unsigned long	minTime = m_curBar * timePerBar;
	unsigned long	maxTime = (m_curBar+1) * timePerBar;
	int				leftPos, rightPos;
	int				maxLines = getMaxLines();
	int 			hexNote, beat;

	grayPen.create( Pen::psDot, 1, RGB( 127, 127, 127 ) );
	bluePen.create( Pen::psDash, 1, RGB( 0, 0, 255 ) );
	redPen.create( Pen::psSolid, 1, RGB( 255, 0, 0 ) );

	hDC.getPen().selectPen( Pen::spBlack );
	hDC.getBrush().selectBrush( Brush::sbWhite );
	hDC.getFont().selectFont( Font::sfAnsiVar );

	hDC.rectangle( 0, 0, size.width, size.height );

	rect.left = textMargin;
	rect.top = textMargin + textHeight;
	rect.right = textWidth;
	rect.bottom = rect.top + textHeight-textMargin/2;

	if( m_timePosition >= minTime && m_timePosition < maxTime )
	{
		int xPos = textWidth + (m_timePosition-minTime)*(size.width-textWidth)/timePerBar;
		hDC.line( xPos, 1, xPos, textHeight );
	}
	for( short curPattern=m_patternOffset; curPattern<m_numPatterns && curPattern<(m_patternOffset+maxLines); curPattern++ )
	{
		short 			curTrack = (short)(curPattern+1);
		const STRING	&voice = m_patternList[curPattern].drumVoice.voice;

		if( curPattern == m_dragPattern )
			hDC.setTextColor( 0, 255, 0 );
		else if( curPattern == m_overPattern )
			hDC.setTextColor( 0, 0, 255 );
		else if( curPattern == m_selectedPattern )
			hDC.setTextColor( 255, 0, 0 );
		else
			hDC.setTextColor( 0, 0, 0 );

		hDC.drawText( voice, rect, DT_LEFT|DT_SINGLELINE );
		hDC.selectPen( bluePen );
		hDC.line( textWidth, rect.bottom, size.width, rect.bottom );


		hDC.selectPen( redPen );
		noteOnFound = false;
		leftPos = textWidth;
		for( 
			MIDIdata::const_iterator it = m_midiData->cbegin(),
				endIT = m_midiData->cend();
			it != endIT;
			++it
		)
		{
			const MIDIevent &theEvent = *it;
			if( theEvent.getTrack() == curTrack
			&&	theEvent.getTimeCode() >= minTime
			&&	theEvent.getTimeCode() < maxTime )
			{
				if( theEvent.getMessage() == MIDI_NOTE_ON )
				{
					unsigned char rgbVal = static_cast<unsigned char>(255-theEvent.getData2()*2);
					timeStart = theEvent.getTimeCode();
					leftPos = textWidth + (timeStart-minTime)*(size.width-textWidth)/timePerBar;
					noteOnFound = true;
					if( theEvent.isSelected() )
						hDC.getBrush().create( 255, 0, 0 );
					else
						hDC.getBrush().create(  rgbVal,rgbVal,rgbVal );

				}
				else if( theEvent.getMessage() == MIDI_NOTE_OFF )
				{
					timeEnd = theEvent.getTimeCode();
					rightPos = textWidth + (timeEnd-minTime)*(size.width-textWidth)/timePerBar;

					hDC.rectangle( leftPos, rect.top, rightPos, rect.bottom-1 );
					noteOnFound = false;
				}
			}
		}
		if( noteOnFound )
		{
			hDC.rectangle( leftPos, rect.top, size.width, rect.bottom-1 );
		}

		rect.top += textHeight;
		rect.bottom += textHeight;
	}

	hexNote = m_grid / 16;
	int numGrids = m_grid * numerator / denominator;
	int nominatorNote = m_grid / denominator;
	rect.top = size.height - textHeight;
	rect.bottom = size.height;
	beat = 1;
	for( int i=0; i<numGrids; i++ )
	{
		int xPos =(size.width-textWidth)*i/numGrids + textWidth;

		if( nominatorNote && i%nominatorNote )
		{
			hDC.selectPen( grayPen );
			if( !hexNote || !(i%hexNote) )
			{
				STRING textBuffer = formatNumber( i+1 );
				rect.left = xPos;
				rect.right = rect.left + (size.width-textWidth)/16;
				hDC.setTextColor( 255, 0, 0 );
				hDC.drawText( textBuffer, rect, DT_LEFT|DT_SINGLELINE );
			}
		}
		else
		{
			hDC.getPen().selectPen( Pen::spBlack );

			STRING textBuffer = formatNumber(m_curBar+1) + '.' + formatNumber(beat);
			rect.left = xPos;
			rect.right = rect.left + (size.width-textWidth)/16;
			hDC.setTextColor( 0, 0, 255 );
			hDC.drawText( textBuffer, rect, DT_LEFT|DT_SINGLELINE );
			if( nominatorNote )
			{
				beat++;
			}
			else
			{
				beat += 2;
			}
		}
		hDC.line( 
			xPos, textHeight+textMargin,
			xPos, size.height-textHeight 
		);
	}

	return psPROCESSED;
}

ProcessStatus PatternArea::handleHorizScroll( HorizScrollCode scrollCode, int , HWND )
{
	switch( scrollCode )
	{
		case hscLEFT:
			if( m_curBar )
			{
				m_curBar = 0;
				setHorizScrollPos( 0 );
				invalidateWindow();
			}
			break;

		case hscLINE_LEFT:
		case hscPAGE_LEFT:
			if( m_curBar )
			{
				m_curBar--;
				setHorizScrollPos( m_curBar );
				invalidateWindow();
			}
			break;
		case hscLINE_RIGHT:
		case hscPAGE_RIGHT:
			if( m_curBar < m_numBars-1 )
			{
				m_curBar++;
				setHorizScrollPos( m_curBar );
				invalidateWindow();
			}
			break;

		case hscRIGHT:
			if( m_curBar < m_numBars-1 )
			{
				m_curBar = m_numBars-1;
				setHorizScrollPos( m_curBar );
				invalidateWindow();
			}
			break;

		case hscTHUMB_TRACK:
		case hscTHUMB_POSITION:
		default:
			return psDO_DEFAULT;
	}

	return psPROCESSED;
}

ProcessStatus PatternArea::handleVertScroll( VertScrollCode scrollCode, int , HWND )
{
	switch( scrollCode )
	{
		case vscTOP:
			if( m_patternOffset )
			{
				m_patternOffset = 0;
				setVertScrollPos( 0 );
				invalidateWindow();
			}
			break;
		case vscLINE_UP:
		case vscPAGE_UP:
			if( m_patternOffset )
			{
				m_patternOffset--;
				setVertScrollPos( m_patternOffset );
				invalidateWindow();
			}
			break;
		case vscLINE_DOWN:
		case vscPAGE_DOWN:
			{
				int maxLines = getMaxLines();
				if( m_patternOffset < m_numPatterns-maxLines)
				{
					m_patternOffset++;
					setVertScrollPos( m_patternOffset );
					invalidateWindow();
				}
			}
			break;

		case vscTHUMB_TRACK:
		case vscTHUMB_POSITION:
		default:
			return psDO_DEFAULT;
	}

	return psPROCESSED;
}

void DrumPlayerThread::ExecuteThread( void )
{
	char		tmpBuffer[128];
	clock_t		actTime, nextTime , clockTime, elapsedTime, startTime = clock();
	int			lastSeconds, seconds, minutes, hours, barTime, lastBars, numBars;

	clockTime = startTime;

	lastBars = lastSeconds = -1;
	barTime = m_midiData->getTimePerBar();
	while( !terminated )
	{
		if( !m_midiData->size() )
		{
			Sleep( totalTime );
		}
		else
		{
			for( 
				MIDIdata::const_iterator it = m_midiData->cbegin(), endIT = m_midiData->cend();
				!terminated && it != endIT;
				++it
			)
			{
				const MIDIevent &midiMsg = *it;

				actTime = clock();

				elapsedTime = actTime - clockTime;
				seconds = elapsedTime/CLK_TCK;
				numBars = elapsedTime / barTime;
				if( seconds != lastSeconds || numBars != lastBars )
				{
					lastSeconds = seconds;
					minutes = seconds / 60;
					seconds = seconds % 60;
					hours = minutes / 60;
					minutes = minutes % 60;

					sprintf(
						tmpBuffer, "%02d:%02d:%02d %03d",
						hours, minutes, seconds, numBars % 1000 
					);
					drumWindow->showClock(tmpBuffer);
				}

				nextTime = startTime + midiMsg.getTimeCode();
				if( nextTime > actTime )
				{
					Sleep((nextTime-actTime)*1000/CLK_TCK);
				}

				if( m_playMode == 0
				|| (m_playMode == 1 && midiMsg.getTrack() != m_selectedTrack)
				|| (m_playMode == 2 && midiMsg.getTrack() == m_selectedTrack) )
				{
					drumWindow->playMidiEvent( midiMsg );
				}
			}
		}
		startTime += totalTime;
	}
}

ProcessStatus MIDIdrumWindow::handleCreate( void )
{
	initMidiPlaySelect( MIDIplaySelect, playButton );

	denominatorSelect->selectEntry( 0 );

	lengthSelector->setRange( 1, 99 );
	lengthSelector->setPosition( 1 );

	thePatternArea->create( patternFrame );
	patternFrame->doLayout();
	thePatternArea->hideVertScrollBar();
	thePatternArea->hideHorizScrollBar();

	volumeButton->disable();
	panButton->disable();

	for( size_t i=0; i<arraySize(lengthValues); i++ )
	{
		gridSelect->addEntry( lengthValues[i] );
		lengthSelect->addEntry( lengthValues[i] );
	}

	playModeSelect->selectEntry( 0 );

	gridSelect->selectEntry( lastSelectedGridIdx );
	lengthSelect->selectEntry( lastSelectedLengthIdx );
	thePatternArea->setGrid(
		atoi( lengthValues[lastSelectedGridIdx] )
	);
	thePatternArea->setBeatLength(
		atoi( lengthValues[lastSelectedLengthIdx] )
	);


	loadVoices();

	volumeBar->setRange( 0, 127 );
	volumeBar->setPosition( 127 );
	thePatternArea->setVolume( 127 );
	timeBar->setRange( 0, 127 );
	timeBar->setPosition( 0 );
	thePatternArea->setTime( 0 );

	return psDO_DEFAULT;
}
ProcessStatus MIDIdrumWindow::handleDestroy( void )
{
	if( midiPlayer )
		stopMidiPlay();

	stopOutMidi();

	return psDO_DEFAULT;
}

bool MIDIdrumWindow::canClose( void )
{
	doEnterFunction("MIDIdrumWindow::canClose");
	saveWindowRect();

	if( midiPlayer )
	{
		stopMidiPlay();
	}

	stopOutMidi();

	if( thePatternArea->getChangedFlag() )
	{
		int button = messageBox(
			winlibGUI::DRUM_CHANGED_id,
			winlibGUI::DRUM_EDITOR_TITLE_id,
			MB_ICONQUESTION|MB_YESNOCANCEL
		);
		if( button == IDYES	)
			saveMidi();
		else if( button == IDCANCEL )
		{
/*@*/		return false;
		}
	}
	return true;
}

ProcessStatus MIDIdrumWindow::handleButtonClick( int btn )
{
	doEnterFunction("MIDIdrumWindow::handleButtonClick");
	switch( btn )
	{
		case winlibGUI::convertBPMbutton_id:
		{
			stopMidiPlay();
			midiData.setTimeSignature(
				numeratorEdit->getText().getValueE<unsigned>(),
				denominatorSelect->getText().getValueE<unsigned>()
			);
			midiData.setBPM( bpmEdit->getText().getValueE<unsigned>(), true, 64 );

			int numBars = midiData.getNumBars();
			if( !numBars )
				numBars++;

			lengthSelector->setPosition( short(numBars) );
			thePatternArea->setNumBars( numBars );

			thePatternArea->invalidateWindow();
			thePatternArea->setChangedFlag();
			break;
		}

		case winlibGUI::addNewPatternButton_id:
			thePatternArea->addNewPattern( voiceSelect->getText() );
			volumeButton->enable();
			panButton->enable();

			break;
		case winlibGUI::deleteEventsButton_id:
			thePatternArea->deleteSelectedEvents();
			checkPattern();

			break;
		case winlibGUI::deleteAllButton_id:
			midiData.clear();
			thePatternArea->refreshPatternList();
			thePatternArea->invalidateWindow();

			thePatternArea->setChangedFlag();

			volumeButton->disable();
			panButton->disable();
			break;

		case winlibGUI::copyEventsButton_id:
			thePatternArea->copyMoveEvents( true );
			break;

		case winlibGUI::moveEventsButton_id:
			thePatternArea->copyMoveEvents( false );
			break;

		case winlibGUI::insertBreakButton_id:
			thePatternArea->insertBreak();
			lengthSelector->setPosition( short(thePatternArea->getNumBars()) );
			break;
		case winlibGUI::saveDrumsButton_id:
			saveMidi();
			break;
		case winlibGUI::loadDrumsButton_id:
			loadDrumPatterns();
			break;
		case winlibGUI::volumeButton_id:
			changeVolume();
			break;
		case winlibGUI::panButton_id:
			changeStereoPosition();
			break;
		default:
/***/		return MIDIdrumWindow_form::handleButtonClick( btn );
	}
	return psPROCESSED;
}

ProcessStatus MIDIdrumWindow::handleSelectionChange( int control )
{
	doEnterFunction("MIDIdrumWindow::handleSelectionChange");
	switch( control )
	{
		case winlibGUI::gridSelect_id:
			lastSelectedGridIdx = gridSelect->getSelection();
			thePatternArea->setGrid(
				atoi( lengthValues[lastSelectedGridIdx] )
			);
			break;
		case winlibGUI::lengthSelect_id:
			lastSelectedLengthIdx = lengthSelect->getSelection();
			thePatternArea->setBeatLength(
				atoi( lengthValues[lastSelectedLengthIdx] )
			);
			break;
		case winlibGUI::MIDIplaySelect_id:
			changeDefaultPlayer();
			loadVoices();
			break;

		case winlibGUI::playModeSelect_id:
			if( midiPlayer )
			{
				midiPlayer->setPlayMode( playModeSelect->getSelection() );
			}
			break;

		case winlibGUI::lengthSelector_id:
			thePatternArea->setNumBars(lengthSelector->getPosition());
			break;

		case winlibGUI::volumeBar_id:
			thePatternArea->setVolume( static_cast<unsigned char>(volumeBar->getPosition()) );
			break;
		case winlibGUI::timeBar_id:
			thePatternArea->setTime( static_cast<unsigned char>(timeBar->getPosition()) );
			break;
		default:
/***/		return MIDIdrumWindow_form::handleSelectionChange( control );
	}
	return psPROCESSED;
}

ProcessStatus MIDIdrumWindow::handleCommand( int cmd )
{
	doEnterFunction("MIDIdrumWindow::handleCommand");
	switch( cmd )
	{
		case PATTERN_AREA:
			if( midiPlayer )
			{
				midiPlayer->setSelectedTrack(
					thePatternArea->getSelectedPattern() + 1
				);
			}
			break;
		case winlibGUI::playButton_id:
			startStopMidiPlay();
			break;

		default:
/***/		return MIDIdrumWindow_form::handleCommand( cmd );
	}
	return psPROCESSED;
}

ProcessStatus MIDIdrumWindow::handleMessage( UINT msg, WPARAM wParam, LPARAM lParam )
{
	if( msg == WM_USER )
	{
		volumeBar->setPosition( short(wParam) );
		timeBar->setPosition( short(lParam) );
		return psPROCESSED;
	}
	else
	{
		return psDO_DEFAULT;
	}
}

// --------------------------------------------------------------------- //
// ----- class publics ------------------------------------------------- //
// --------------------------------------------------------------------- //

void MIDIdrumWindow::create( void )
{
	thePatternArea = new PatternArea(
		this, PATTERN_AREA, &midiData, &theVoices
	);
	SuccessCode error = MIDIdrumWindow_form::create( NULL );
	if( error == scSUCCESS )
	{
		restoreWindowPos();
	}
}

void PatternArea::refreshPatternList( void )
{
	const short 	numTracks = m_midiData->getNumTracks();
	const int		maxLines = getMaxLines();

	m_patternList.clear();

	for( short curTrack=1; curTrack<numTracks; curTrack++ )
	{
		PatternInfo		newPattern;
		unsigned char	theChannel;

		/*
			track#0 contains program and bank changes
		*/
		int voiceIdx = int(findDrumVoiceNchannel(
			*m_midiData,
			*m_theDrumVoices,
			curTrack,
			&theChannel
		));
		if( voiceIdx != -1 )
		{
			newPattern.drumVoice = (*m_theDrumVoices)[voiceIdx];
			newPattern.channel = theChannel;
		}
		else
		{
			newPattern.drumVoice.voice = "Track#";
			newPattern.drumVoice.voice += formatNumber( curTrack );
			theChannel = 9;
			newPattern.channel = theChannel;
		}

		newPattern.stereoPos = findStereoPos( *m_midiData, theChannel );

		m_patternList.addElement( newPattern );
	}

	m_patternOffset = 0;
	m_selectedPattern = -1;
	m_numPatterns = short(m_patternList.size());

	if( m_numPatterns > maxLines )
	{
		showVertScrollBar( 0, m_numPatterns-maxLines );
	}
	else
	{
		hideVertScrollBar();
	}

	removeStereoEvents( m_midiData );
}

void PatternArea::addNewPattern( const STRING &theVoice )
{
	bool	voiceFound = false;
	int		newMaxLines, maxLines = getMaxLines();

	for( size_t i=0; i<size_t(m_numPatterns); i++ )
	{
		if( m_patternList[i].drumVoice.voice == theVoice )
		{
			m_selectedPattern = short(i);
			voiceFound = true;
			break;
		}
	}

	if( !voiceFound )
	{
		PatternInfo	newVoice;
		newVoice.drumVoice = (*m_theDrumVoices)[m_theDrumVoices->findDrumVoice(theVoice)];
		ChannelCodes	channelCodes;
		newVoice.fillChannelCodes( &channelCodes );
		unsigned char newChannel = m_midiData->allocateChannel( &channelCodes );
		if( newChannel )
		{
			newVoice.channel = newChannel;

			m_patternList.addElement( newVoice );
			m_selectedPattern = m_numPatterns;
			m_numPatterns++;
			if( m_numPatterns > maxLines )
			{
				showVertScrollBar( 0, m_numPatterns-maxLines );
				newMaxLines = getMaxLines();
				if( newMaxLines != maxLines )
				{
					maxLines = newMaxLines;
					showVertScrollBar( 0, m_numPatterns-maxLines );
				}
			}

			setChangedFlag();
		}
	}
	if( m_selectedPattern < m_patternOffset )
	{
		m_patternOffset = m_selectedPattern;
		setVertScrollPos( m_patternOffset );
	}
	if( m_selectedPattern >= m_patternOffset+maxLines )
	{
		m_patternOffset = short(m_selectedPattern-maxLines+1);
		setVertScrollPos( m_patternOffset );
	}

	invalidateWindow();
}

void PatternArea::deleteSelectedEvents( void )
{
	bool	changed = false;

	if( m_selectedPattern >= 0 && m_selectedPattern < m_numPatterns )
	{
		int	maxLines = getMaxLines();

		PatternInfo	&theInfo = m_patternList[m_selectedPattern];
		bool found = false;
		for( short i=0; i<m_numPatterns; i++ )
		{
			if( i != m_selectedPattern && m_patternList[i].channel == theInfo.channel )
			{
				found = true;
				break;
			}
		}
		if( !found )
		{
			m_midiData->removeChannel( theInfo.channel );
		}
		m_patternList.removeElementAt( m_selectedPattern );
		m_midiData->removeTrack( short(m_selectedPattern+1) );
		m_numPatterns--;
		if( m_selectedPattern >= m_numPatterns )
		{
			m_selectedPattern = short(m_numPatterns-1);
		}

		if( m_numPatterns <= maxLines )
		{
			hideVertScrollBar();
		}

		changed = true;
	}
	else
	{
		for( size_t i = m_midiData->size()-1; i!=-1; i-- )
		{
			MIDIevent &theEvent = (*m_midiData)[i];
			if( theEvent.isSelected() )
			{
				m_midiData->removeElementAt(i);
				changed = true;
			}
		}
	}

	if( !changed )
	{
		unsigned long breakLength = m_midiData->getStartPosition( 0, 1, m_beatLength );
		unsigned long maxTimePosition = m_timePosition + breakLength;

		for( size_t i=0; i<m_midiData->size(); i++ )
		{
			MIDIevent &theEvent = (*m_midiData)[i];
			unsigned long eventTime = theEvent.getTimeCode();
			if( eventTime >= m_timePosition )
			{
				if( eventTime >= maxTimePosition )
				{
					theEvent.setTimeCode( eventTime-breakLength );
					changed = true;
				}
				else
				{
					if( theEvent.getMessage() != MIDI_NOTE_OFF )
					{
						size_t j= m_midiData->findMatchingEvent( i );
						if( j>i && j != -1 )
						{
							m_midiData->removeElementAt( j );
						}
						m_midiData->removeElementAt( i-- );
					}
				}
			}
		}
	}

	if( changed )
	{
		setChangedFlag();
		invalidateWindow();
	}
}

void PatternArea::copyMoveEvents( bool copyFlag )
{
	MIDIdata		tmpBuffer;
	size_t			i, j, numSelected;
	long			timeCodeOffset;
	unsigned long	newStartTime, newEndTime;
	int				foundNote;
	short			trackNo;
	bool			changed = false;

	// copy the data to an internal buffer
	i=0;
	while( i<m_midiData->size() )
	{
		MIDIevent &theEvent = (*m_midiData)[i];
		if( theEvent.isSelected() )
		{
			theEvent.clearSelected();
			tmpBuffer.addElement( theEvent );
			if( !copyFlag )
			{
				m_midiData->removeElementAt(i);
				changed = true;
			}
			else
				i++;
		}
		else
			i++;
	}
	numSelected = tmpBuffer.size();
	if( numSelected )
	{
		timeCodeOffset = m_timePosition-tmpBuffer[0].getTimeCode();

		for( i=0; i<numSelected; i++ )
		{
			MIDIevent &theOnEvent = tmpBuffer[i];
			if( theOnEvent.getMessage() == MIDI_NOTE_ON )
			{
				trackNo = theOnEvent.getTrack();
				newStartTime = theOnEvent.getTimeCode() + timeCodeOffset;
				for( j=i+1; j<numSelected; j++ )
				{
					MIDIevent &theOffEvent = tmpBuffer[j];
					if( theOffEvent.getTrack() == trackNo )
					{
						newEndTime = theOffEvent.getTimeCode() + timeCodeOffset;
						foundNote = findNote( newStartTime, newEndTime, trackNo );
						if( foundNote < 0 )
						{
							theOnEvent.setTimeCode( newStartTime );
							theOffEvent.setTimeCode( newEndTime );
							theOnEvent.setSelected();
							theOffEvent.setSelected();
							m_midiData->addElement( theOnEvent );
							m_midiData->addElement( theOffEvent );
							changed = true;
						}
					}
				}
			}
		}

		if( changed )
		{
			setChangedFlag();
			invalidateWindow();
		}
	}
}

void PatternArea::insertBreak( void )
{
	unsigned char	msg;
	unsigned long	timeCode;
	unsigned long	breakLength = m_midiData->getStartPosition( 0, 1, m_beatLength );
	bool			changed = false;

	for( std::size_t i=0; i<m_midiData->size(); i++ )
	{
		MIDIevent	&theEvent = (*m_midiData)[i];
		timeCode = theEvent.getTimeCode();
		if( timeCode >= m_timePosition )
		{
			msg = theEvent.getMessage();
			if( msg == MIDI_NOTE_ON )
			{
				std::size_t j = m_midiData->findMatchingEvent( i );
				if( j != -1 )
				{
					timeCode += breakLength;
					changed = true;
					if( timeCode > getMaxTimeCode() )
						setNumBars( getNumBars() + 1 );
					theEvent.setTimeCode( timeCode );

					MIDIevent	&theMatch = (*m_midiData)[j];
					timeCode = theMatch.getTimeCode() + breakLength;
					if( timeCode > getMaxTimeCode() )
					{
						setNumBars( getNumBars() + 1 );
					}
					theMatch.setTimeCode( timeCode );
				}
			}
		}
	}

	if( changed )
	{
		m_selectedPattern = -1;
		setChangedFlag();
		invalidateWindow();
	}
}

void PatternArea::setNumBars( int numBars )
{
	doEnterFunction("PatternArea::setNumBars");
	m_numBars = numBars;
	unsigned long maxTimeCode = getMaxTimeCode();

	if( numBars > 1 )
		showHorizScrollBar( 0, numBars-1 );
	else
		hideHorizScrollBar();

	if( m_curBar >= numBars )
	{
		m_curBar = numBars-1;
		setHorizScrollPos( m_curBar );
	}

	for( size_t i = m_midiData->size()-1; i != -1; i-- )
	{
		MIDIevent	&theEvent = (*m_midiData)[i];
		if( theEvent.getTimeCode() >= maxTimeCode )
		{
			m_midiData->removeElementAt( i );
		}
	}

	invalidateWindow();
	setChangedFlag();
}

void MIDIdrumWindow::loadDrumPatterns( const char *cmdLine )
{
	doEnterFunction("MIDIdrumWindow::loadDrumPatterns");

	if( thePatternArea->getChangedFlag() )
	{
		if(
			messageBox(
				winlibGUI::DRUM_CHANGED_id,
				winlibGUI::DRUM_EDITOR_TITLE_id,
				MB_ICONQUESTION|MB_YESNO
			) == IDYES
		)
		{
			saveMidi();
		}
	}

	stopMidiPlay();
	if( PlayerWindow::loadMidi( this, bpmEdit, winlibGUI::DrumFiles_id, cmdLine ) )
	{
		size_t	midiOutDev = midiData.getMidiOutDev( 0 );
		if( midiOutDev != -1 )
		{
			setDefaultPlayer( midiOutDev );
			loadVoices();
		}

		thePatternArea->refreshPatternList();

		int numBars = midiData.getNumBars();
		numeratorEdit->setText( formatNumber( midiData.getNumerator() ) );
		denominatorSelect->selectEntry( formatNumber( midiData.getDenominator() ) );

		lengthSelector->setPosition( short(numBars) );
		thePatternArea->setNumBars( numBars );
		thePatternArea->invalidateWindow();

		thePatternArea->clearChangedFlag();

		checkPattern();
	}
}

void MIDIdrumWindow::playMidiEvent( const MIDIevent &msg )
{
	if( msg.getMessage() == MIDI_NOTE_ON )
	{
		unsigned short	track = msg.getTrack();
		size_t			pattern = track -1;
		char			stereoPos = thePatternArea->getPatternList()[pattern].stereoPos;

		setStereoPosition( msg.getChannel(), stereoPos, true );
	}
	PlayerWindow::playMidiEvent( msg, true );
}
// --------------------------------------------------------------------- //
// ----- entry points -------------------------------------------------- //
// --------------------------------------------------------------------- //

