/*
		Project:		MIDI Tools
		Module:			MIDIdrumWindow.h
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

#ifndef MIDI_DRUM_WINDOW_H
#define MIDI_DRUM_WINDOW_H

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <time.h>

#include <gak/thread.h>

#include <winlib/ChildWin.h>
#include <winlib/dialogwi.h>
#include <winlib/controlw.h>
#include <winlib/stddlg.h>

#include "PlayerWindow.h"
#include "DrumVoices.h"
#include "midifile.h"
#include "midi.gui.h"

// --------------------------------------------------------------------- //
// ----- module switches ----------------------------------------------- //
// --------------------------------------------------------------------- //

#ifdef __BORLANDC__
#	pragma option -RT-
#	ifdef __WIN32__
#		pragma option -a4
#		pragma option -pc
#	else
#		pragma option -po
#		pragma option -a2
#	endif
#endif

// --------------------------------------------------------------------- //
// ----- constants ----------------------------------------------------- //
// --------------------------------------------------------------------- //

const int	textWidth = 200;
const int	textHeight = 18;
const int	textMargin = 4;

// --------------------------------------------------------------------- //
// ----- type definitions ---------------------------------------------- //
// --------------------------------------------------------------------- //

struct PatternInfo
{
	DrumVoice		drumVoice;
	char			stereoPos;
	unsigned char	volume;
	unsigned char	channel;

	PatternInfo()
	{
		channel = 0;
		stereoPos = 0;
		volume = 127;
	}
	void fillChannelCodes( ChannelCodes *channelCodes )
	{
		channelCodes->stereoPos = stereoPos;
		channelCodes->volume = volume;
		channelCodes->voiceCodes = drumVoice.voiceCodes;
	}
};
typedef gak::Array<PatternInfo>	PatternInfos;

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

class MIDIdrumWindow;

class DrumPlayerThread : public gak::Thread
{
	MIDIdrumWindow		*drumWindow;
	MIDIdata			*m_midiData;
	unsigned long		totalTime;
	int					m_playMode;
	int					m_selectedTrack;

	virtual void ExecuteThread( void );

	public:
	DrumPlayerThread(
		MIDIdrumWindow *drumWindow,
		MIDIdata *midiData,
		unsigned long totalTime
	)
	{
		m_playMode = 0;
		m_selectedTrack = 0;
		this->drumWindow = drumWindow;
		m_midiData = midiData;
		this->totalTime = totalTime;
	}
	void setPlayMode( int playMode )
	{
		m_playMode = playMode;
	}
	void setSelectedTrack( int selectedTrack )
	{
		m_selectedTrack = selectedTrack;
	}
};
typedef gak::SharedObjectPointer<DrumPlayerThread>	DrumPlayerThreadPtr;

class PatternArea : public winlib::ChildWindow
{
	MIDIdata			*m_midiData;
	PatternInfos		m_patternList;
	short				m_overPattern, m_dragPattern, m_selectedPattern, m_numPatterns,
						m_patternOffset;
	unsigned long		m_timePosition;
	int					m_curBar, m_numBars, m_beatLength;
	int					m_grid;
	unsigned char		m_volume, m_time;
	DrumVoices			*m_theDrumVoices;
	bool				m_changedFlag;

	private:
	short getPatternByMouse( int mouseY )
	{
		mouseY -= textMargin+textHeight;
		if( mouseY >= 0 )
			return (short)(mouseY/textHeight + m_patternOffset);
		else
			return -1;
	}
	unsigned short getTrackByMouse( int mouseY )
	{
		return (unsigned short)(getPatternByMouse( mouseY ) +1);
	}
	int findNote( unsigned long start, unsigned long end, short track );

	virtual winlib::ProcessStatus handleMouseRelease( void );
	virtual winlib::ProcessStatus handleMouseMove( WPARAM modifier, const winlib::Point &position );
	virtual winlib::ProcessStatus handleLeftButton( winlib::LeftButton leftButton, WPARAM modifier, const winlib::Point &position );
	virtual winlib::ProcessStatus handleRightButton( winlib::RightButton rightButton, WPARAM modifier, const winlib::Point &position );
	virtual winlib::ProcessStatus handleRepaint( winlib::Device &hDC );
	virtual winlib::ProcessStatus handleHorizScroll( winlib::HorizScrollCode scrollCode, int nPos, HWND scrollBar );
	virtual winlib::ProcessStatus handleVertScroll( winlib::VertScrollCode scrollCode, int nPos, HWND scrollBar );

	private:
	static const char className[];
	public:
	static void registerClass( void );
	virtual gak::STRING getWindowClassName( void ) const;

	PatternArea(
		winlib::BasicWindow *owner, int controlId,
		MIDIdata *midiData, DrumVoices *theDrumVoices )
		: ChildWindow( owner )
	{
		registerClass();
		setId( controlId );
		setParent( owner );

		m_grid = 1;
		m_beatLength = 4;
		m_volume = 127;
		m_time = 0;
		m_curBar = 0;
		m_timePosition = 0;
		m_changedFlag = false;
		m_numBars = 1;
		m_overPattern = m_dragPattern = -1;
		m_numPatterns = m_patternOffset = 0;

		m_midiData = midiData;
		m_theDrumVoices = theDrumVoices;
	}

	void setGrid( int grid )
	{
		m_grid = grid;
		invalidateWindow();
	}
	void setBeatLength( int beatLength )
	{
		m_beatLength = beatLength;
	}
	void setNumBars( int numBars );
	int getNumBars( void ) const
	{
		return m_numBars;
	}
	unsigned long getMaxTimeCode( void ) const
	{
		return m_numBars * m_midiData->getTimePerBar();
	}
	void setVolume( unsigned char volume )
	{
		m_volume = volume;
	}
	void setTime( unsigned char time )
	{
		m_time = time;
	}
	void refreshPatternList( void );
	void addNewPattern( const gak::STRING &voice );
	void deleteSelectedEvents( void );
	void copyMoveEvents( bool copyFlag );
	void insertBreak( void );


	bool getChangedFlag( void ) const
	{
		return m_changedFlag;
	}
	void setChangedFlag( void )
	{
		m_changedFlag = true;
	}
	void clearChangedFlag( void )
	{
		m_changedFlag = false;
	}
	int getMaxLines( void )
	{
		winlib::Size	size = getClientSize();
		int				maxLines = (size.height-textMargin) / textHeight;

		return maxLines -1;
	}
	short getSelectedPattern( void ) const
	{
		return m_selectedPattern;
	}

	PatternInfos &getPatternList( void )
	{
		return m_patternList;
	}
	size_t getNumPattern( void ) const
	{
		return m_patternList.size();
	}
};

class MIDIdrumWindow : public winlibGUI::MIDIdrumWindow_form, public PlayerWindow
{
	private:
	DrumPlayerThreadPtr		midiPlayer;
	gak::STRING				m_lastDrumsFile;
	DrumVoices				theVoices;
	PatternArea				*thePatternArea;

	static int lastSelectedLengthIdx;
	static int lastSelectedGridIdx;

	private:
	void stopMidiPlay( void );
	void startStopMidiPlay( void );

	void saveMidi( void );
	void changeVolume( void );
	void changeStereoPosition( void );

	void loadVoices( void );

	void checkPattern( void )
	{
		if( thePatternArea->getNumPattern() )
		{
			volumeButton->enable();
			panButton->enable();
		}
		else
		{
			volumeButton->disable();
			panButton->disable();
		}
	}

	public:
	MIDIdrumWindow( winlib::BasicWindow *owner ) : MIDIdrumWindow_form( owner )
	{
	}
	void showClock( const char *time )
	{
		clockText->setText( time );
	}

	void create( void );
	virtual winlib::ProcessStatus handleCreate( void );
	virtual winlib::ProcessStatus handleDestroy( void );
	virtual bool canClose( void );
	virtual winlib::ProcessStatus handleButtonClick( int btn );
	virtual winlib::ProcessStatus handleSelectionChange( int control );
	virtual winlib::ProcessStatus handleCommand( int cmd );
	virtual winlib::ProcessStatus handleMessage( UINT msg, WPARAM wParam, LPARAM lParam );
	void loadDrumPatterns( const char *cmdLine=NULL );
	void playMidiEvent( const MIDIevent &msg );
};

// --------------------------------------------------------------------- //
// ----- exported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class privates ------------------------------------------------ //
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
#	pragma option -a.
#	pragma option -p.
#endif

#endif
