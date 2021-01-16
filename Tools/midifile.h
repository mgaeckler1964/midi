/*
		Project:		MIDI Tools
		Module:			midifile.h
		Description:	MIDI messages and midi files
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

#ifndef MIDI_FILE_H
#define MIDI_FILE_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <gak/sortedArray.h>
#include <gak/array.h>
#include <gak/types.h>

#include "../Synth/midi.h"

// --------------------------------------------------------------------- //
// ----- imported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module switches ----------------------------------------------- //
// --------------------------------------------------------------------- //

#ifdef __BORLANDC__
#	pragma option -RT-
#	pragma option -a2
#	ifdef __WIN32__
#		pragma option -pc
#	else
#		pragma option -po
#	endif
#endif
#ifdef _MSC_VER
#	pragma warning( disable: 4200 )	// Nicht dem Standard entsprechende Erweiterung: Null-Array in Struktur/Union
#endif

// --------------------------------------------------------------------- //
// ----- constants ----------------------------------------------------- //
// --------------------------------------------------------------------- //

// MIDI File Events
#define MIDI_FILE_META			0xFF

#define MIDI_FILE_SEQ_NUM		0x00
#define MIDI_FILE_COMMENT		0x01
#define MIDI_FILE_COPYRIGHT		0x02
#define MIDI_FILE_TRACK_NAME	0x03
#define MIDI_FILE_INSTRUMENT	0x04
#define MIDI_FILE_LYRIC			0x05
#define MIDI_FILE_MARKER		0x06
#define MIDI_FILE_CUE_POINT		0x07
#define MIDI_FILE_DEVICE_NAME	0x09
#define MIDI_FILE_CHANNEL		0x20
#define MIDI_FILE_PORT			0x21
#define MIDI_FILE_TRACK_END		0x2F
#define MIDI_FILE_TEMPO			0x51
#define MIDI_FILE_SMPTE			0x54
#define MIDI_FILE_TIME_SIG		0x58
#define MIDI_FILE_KEY_SIG		0x59
#define MIDI_FILE_SEQUENCER		0x7F

// --------------------------------------------------------------------- //
// ----- macros -------------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- type definitions ---------------------------------------------- //
// --------------------------------------------------------------------- //

#ifdef _MSC_VER
#	pragma pack(push, 2)
#	pragma warning( push )
#endif

struct MTHD_CHUNK
{
   /* Here's the 8 byte header that all chunks must have */
   char           ID[4];  /* This will be 'M','T','h','d' */
   unsigned long  Length; /* This will be 6 */

   /* Here are the 6 bytes */
   unsigned short	Format;
   unsigned short	NumTracks;
   short 			Division;
};

struct MTRK_CHUNK
{
   /* Here's the 8 byte header that all chunks must have */
   char           ID[4];   /* This will be 'M','T','r','k' */
   unsigned long  Length;  /* This will be the actual size of Data[] */

   /* Here are the data bytes */
   unsigned char  Data[];  /* Its actual size is Data[Length] */
};

#ifdef _MSC_VER
#	pragma pack( pop )
#endif

struct VoiceCodes
{
	unsigned char	program, bankMSB, bankLSB;
};

struct ChannelCodes
{
	VoiceCodes		voiceCodes;
	unsigned char	volume, expression;
	char			stereoPos;
	unsigned char	effects[5];
	unsigned char	sounds[5];
};

struct SysExData
{
	size_t	usageCount, len;
	unsigned char data[1];
};

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

struct TrackInfo
{
	size_t		midiOutDev;
	gak::STRING	trackName;
	gak::STRING	instrument;
	gak::STRING	deviceName;

	TrackInfo()
	{
		midiOutDev = (size_t)-1;
	};
};

class MIDIevent
{
	private:
	int				m_index;
	unsigned short	m_track;
	gak::uint32		m_msg;		// three bytes data2|data1|channel (4bit)|statusCode(4bit)
	gak::uint32		m_timeCode;
	bool			m_selected;
	SysExData		*m_sysExData;

	static const char *midiChannelMessages[];
	static const char *midiSystemMessages[];
	static const char *midiControllerMessages[];
	static const char *midiNotes[];

	void setSysExData( SysExData *sysExData )
	{
		if( m_sysExData )
		{
			m_sysExData->usageCount--;
			if( !m_sysExData->usageCount )
			{
				free( m_sysExData );
			}
		}
		m_sysExData = sysExData;
		if( m_sysExData )
		{
			m_sysExData->usageCount++;
		}
	}
	void copyMidiEvent( const MIDIevent &source )
	{
		this->m_track = source.m_track;
		this->m_index = source.m_index;
		this->m_msg = source.m_msg;
		this->m_timeCode = source.m_timeCode;
		this->m_selected = source.m_selected;

		setSysExData( source.m_sysExData );
	}
	static unsigned short makeShort( unsigned char lsb, unsigned char msb )
	{
		return (msb<<7)|lsb;
	}
	public:
	MIDIevent( )
	{
		m_track = 0;
		m_index = -1;
		m_msg = 0;
		m_timeCode = 0;
		m_selected = false;
		m_sysExData = NULL;
	}
	MIDIevent( gak::uint32 timeCode, gak::uint32 msg )
	{
		m_track = 0;
		m_index = -1;
		m_timeCode = timeCode;
		m_msg = msg;
		m_selected = false;
		m_sysExData = NULL;
	}
	MIDIevent( unsigned short track, gak::uint32 timeCode, gak::uint32 msg )
	{
		m_index = -1;
		m_track = track;
		m_timeCode = timeCode;
		m_msg = msg;
		m_selected = false;
		m_sysExData = NULL;
	}
	MIDIevent( const MIDIevent &source )
	{
		m_sysExData = NULL;
		copyMidiEvent( source );
	}
	const MIDIevent &operator = ( const MIDIevent &source )
	{
		copyMidiEvent( source );
		return *this;
	}

	void setSysExData( const unsigned char *sysExData, size_t len, bool withStart )
	{
		if( !withStart )
		{
			len += 1;
		}

		SysExData *newSysExData = static_cast<SysExData *>(malloc( sizeof( SysExData ) + len ));
		if( newSysExData )
		{
			newSysExData->usageCount = 0;
			newSysExData->len = len;
			if( withStart )
			{
				memcpy( newSysExData->data, sysExData, len );
			}
			else
			{
				memcpy( newSysExData->data+1, sysExData, len-1 );
				newSysExData->data[0] = MIDI_SYS_EX_START;
			}
			setSysExData( newSysExData );
			setMessageCode( MIDI_SYS_EX_START );
		}
	}
	const unsigned char *getSysExData( size_t *len, bool withStart ) const
	{
		if( getStatusByte() == MIDI_SYS_EX_START )
		{
			if( withStart )
			{
				*len = m_sysExData->len;
				return m_sysExData->data;
			}
			else
			{
				*len = m_sysExData->len-1;
				return m_sysExData->data+1;
			}
		}
		else
		{
			*len = 0;
			return NULL;
		}
	}

	static const char *MIDIevent::getNoteText( unsigned char note );
	const char *MIDIevent::getNoteText( void ) const
	{
		unsigned char msg = getMessage();
		if( msg == MIDI_NOTE_ON || msg == MIDI_NOTE_OFF )
			return getNoteText( getData1() );
		else
			return "";
	}

	static const char *getControllerText( unsigned char controller );
	const char *getControllerText( void ) const
	{
		if( getMessage() == MIDI_CONTROLLER )
			return getControllerText( getData1() );
		else
			return "";
	}

	unsigned char getData1( void ) const
	{
		return static_cast<unsigned char>((m_msg >> 8) & 0xFF);
	}
	void setData1( unsigned char data1 )
	{
		m_msg &= 0xFFFF00FF;
		m_msg |= static_cast<unsigned long>(data1)<<8;
	}

	unsigned char getData2( void ) const
	{
		return static_cast<unsigned char>((m_msg >> 16) & 0xFF);
	}
	void setData2( unsigned char data2 )
	{
		m_msg &= 0xFF00FFFF;
		m_msg |= static_cast<unsigned long>(data2)<<16;
	}
	unsigned short getShortData( void ) const
	{
		return makeShort( getData1(), getData2() );
	}

	unsigned char getStatusByte( void ) const
	{
		return static_cast<unsigned char>(m_msg & 0xFF);
	}
	void setStatusByte( unsigned char statusByte )
	{
		m_msg &= 0xFFFFFF00;
		m_msg |= static_cast<unsigned long>(statusByte);
	}

	unsigned long getMessageCode( void ) const
	{
		return m_msg;
	}
	void setMessageCode( unsigned char statusByte, unsigned char data1, unsigned char data2 )
	{
		m_msg = static_cast<unsigned long>(statusByte) 
			| (static_cast<unsigned long>(data1)<<8) 
			| (static_cast<unsigned long>(data2)<<16);
	}
	void setMessageCode( unsigned char status, unsigned char channel, unsigned char data1, unsigned char data2 )
	{
		setMessageCode( static_cast<unsigned char>((status<<4)|channel), data1, data2 );
	}
	void setMessageCode( unsigned long msg )
	{
		m_msg = msg;
	}
	void setReset( void )
	{
		setMessageCode( MIDI_RESET, 0, 0 );
	}

	void setNoteOn( unsigned char channel, unsigned char note, unsigned char volume )
	{
		setMessageCode( MIDI_NOTE_ON, channel, note, volume );
	}
	void setNoteOff( unsigned char channel, unsigned char note, unsigned char velocity )
	{
		setMessageCode( MIDI_NOTE_OFF, channel, note, velocity );
	}
	void setProgramChange( unsigned char channel, unsigned char program )
	{
		setMessageCode( MIDI_PROGRAM, channel, program, 0 );
	}
	void setBankMSB( unsigned char channel, unsigned char bankMsb )
	{
		setMessageCode( MIDI_CONTROLLER, channel, MIDI_BANK_MSB, bankMsb );
	}
	void setBankLSB( unsigned char channel, unsigned char bankLsb )
	{
		setMessageCode( MIDI_CONTROLLER, channel, MIDI_BANK_LSB, bankLsb );
	}
	void setSoundStop( unsigned char channel )
	{
		setMessageCode( MIDI_CONTROLLER, channel, MIDI_SOUND_OFF, 0 );
	}
	void setNotesStop( unsigned char channel )
	{
		setMessageCode( MIDI_CONTROLLER, channel, MIDI_NOTES_OFF, 0 );
	}
	void setLocalOff( unsigned char channel )
	{
		setMessageCode( MIDI_CONTROLLER, channel, MIDI_LOCAL_ON_OFF, 0 );
	}
	void setLocalOn( unsigned char channel )
	{
		setMessageCode( MIDI_CONTROLLER, channel, MIDI_LOCAL_ON_OFF, 0x7F );
	}
	void setVolume( unsigned char channel, unsigned char volume )
	{
		setMessageCode( MIDI_CONTROLLER, channel, MIDI_VOLUME, volume );
	}
	void setBalancePosition( unsigned char channel, char position )
	{
		setMessageCode(
			MIDI_CONTROLLER, channel,
			MIDI_BALANCE_POSITION,
			(unsigned char)(position+MIDI_CENTER_PAN)
		);
	}
	void setPanPosition( unsigned char channel, char position )
	{
		setMessageCode(
			MIDI_CONTROLLER, channel,
			MIDI_PAN_POSITION,
			(unsigned char)(position+MIDI_CENTER_PAN)
		);
	}
	void setExpression(
		unsigned char channel,
		unsigned char expression
	)
	{
		setMessageCode(
			MIDI_CONTROLLER, channel,
			MIDI_EXPRESSION, expression
		);
	}
	void setSoundValue(
		unsigned char channel,
		unsigned char soundController,
		unsigned char soundValue
	)
	{
		setMessageCode(
			MIDI_CONTROLLER, channel,
			(unsigned char)(MIDI_SOUND_CONTROLLER+soundController), soundValue
		);
	}
	void setEffectValue(
		unsigned char channel,
		unsigned char effect,
		unsigned char effectsValue
	)
	{
		setMessageCode(
			MIDI_CONTROLLER, channel,
			(unsigned char)(MIDI_EFFECT+effect), effectsValue
		);
	}
	unsigned char getMessage( void ) const
	{
		return (unsigned char)(getStatusByte()>>4);
	}
	void setMessage( unsigned char statusCode )
	{
		m_msg &= ~0xF0;
		m_msg |= statusCode<<4;
	}
	static const char *getChannelMessageText( unsigned char message )
	{
		if( message >= MIDI_NOTE_OFF && message < MIDI_SYSTEM )
			return midiChannelMessages[message-MIDI_NOTE_OFF];
		else
			return NULL;
	}
	static const char *getSystemMessageText( unsigned char message )
	{
		return midiSystemMessages[message];
	}
	const char *getMessageText( void ) const
	{
		unsigned char message = getMessage();
		if( message != MIDI_SYSTEM )
			return getChannelMessageText(message);
		else
			return getSystemMessageText( getChannel() );
	}
	unsigned char getChannel( void ) const
	{
		return static_cast<unsigned char>(m_msg & 0x0F);
	}
	void setChannel( unsigned char channel )
	{
		channel &= 0xF;
		m_msg &= ~0xF;
		m_msg |= channel;
	}

	unsigned long getTimeCode( void ) const
	{
		return m_timeCode;
	}
	void setTimeCode( unsigned long timeCode )
	{
		m_timeCode = timeCode;
	}
	unsigned short getTrack( void ) const
	{
		return m_track;
	}
	void setTrack( unsigned short track )
	{
		m_track = track;
	}
	void setSelected( void )
	{
		m_selected = true;
	}
	void clearSelected( void )
	{
		m_selected = false;
	}
	bool isSelected( void ) const
	{
		return m_selected;
	}

	int getIndex( void ) const
	{
		return m_index;
	}
	void setIndex( int index )
	{
		m_index = index;
	}
};

class MIDIdata : public gak::SortedArray<MIDIevent>
{
	gak::Array<TrackInfo>	trackInfo;
	int						BeatsPerMinute;
	unsigned				timeSigNumerator, timeSigDenominator;

	void fixTimeCodes( int fraction );
	void convertTimeCodes( int oldBase, int newBase );

	void setDefaults( void )
	{
		BeatsPerMinute = 120;
		timeSigNumerator = 4;
		timeSigDenominator = 4;
	}
	public:
	static int timeCompare(
		const MIDIevent *event1, const MIDIevent *event2
	);

	MIDIdata() : SortedArray<MIDIevent>()
	{
		setDefaults();
	}

	void setBPM( int newBPM, bool convert, int fraction )
	{
		if( convert )
			convertTimeCodes( BeatsPerMinute, newBPM );

		BeatsPerMinute = newBPM;

		if( fraction > 0 )
			fixTimeCodes( fraction );
	}
	int getBPM( void ) const
	{
		return BeatsPerMinute;
	}
	void setTimeSignature( unsigned timeSigNumerator, unsigned timeSigDenominator )
	{
		this->timeSigNumerator = timeSigNumerator;
		this->timeSigDenominator = timeSigDenominator;
	}
	unsigned getNumerator( void ) const
	{
		return timeSigNumerator;
	}
	unsigned getDenominator( void ) const
	{
		return timeSigDenominator;
	}

	int getTimePerFulNote( void ) const
	{
		return 60000 * 4 / getBPM();
	}
	double getTimePerFulNoteDbl( void ) const
	{
		return 60000.0 * 4.0 / double(getBPM());
	}

	int getTimePerBar( void ) const
	{
		return 60000 * 4 * timeSigNumerator / timeSigDenominator / getBPM();
	}
	double getTimePerBarDbl( void ) const
	{
		return 60000.0 * 4.0 * double(timeSigNumerator) / double(timeSigDenominator) / double(getBPM());
	}
/*
	int getNumBars( void ) const
	{
		return int(double(getLength()) / getTimePerBarDbl() + 0.5);
	}
*/
	int getNumBars( void ) const
	{
		doEnterFunction("MIDIdata::getNumBars");
		unsigned long length = getLength();
		int barTime = getTimePerBar();
		int numBars = length / barTime;
		if( length % barTime )
		{
			numBars += 1;
		}

		return numBars;
	}

	void alignTimeCodes( void );
	gak::STRING getTimeCodeStr( unsigned long timeCode ) const;
	unsigned long parseTimeCode( const char *timeCode ) const;
	long parseRelativeTimeCode( const char *timeCode ) const;

	int getStartPosition( int bar, int note, int length )
	{
		int timePerBar = getTimePerBar();
		int timePerFullNote = getTimePerFulNote();
		return bar * timePerBar + note * timePerFullNote/length;
	}
/*
	=======================================================================
	track handling functions
	=======================================================================
*/
	void removeTrack( unsigned short track );
	void replaceTrack( unsigned short track, const MIDIdata *newData );
	unsigned short getNumTracks( void ) const;
	gak::STRING getTrackName( unsigned short track )
	{
		return trackInfo[track].trackName;
	}
	void setTrackName( unsigned short track, const gak::STRING &trackName )
	{
		trackInfo.getOrCreateElementAt(track).trackName = trackName;
	}
	size_t getNumTrackData( unsigned short track ) const;
	void removeUnusedTracks( void );

	const gak::Array<TrackInfo> &getTrackInfo( void ) const
	{
		return trackInfo;
	}
	void setTrackInfo( const gak::Array<TrackInfo> &trackInfo )
	{
		this->trackInfo = trackInfo;
	}

	gak::STRING getInstrument( unsigned short track )
	{
		return trackInfo[track].instrument;
	}
	void setInstrument( unsigned short track, const gak::STRING &instrument )
	{
		trackInfo[track].instrument = instrument;
	}
	gak::STRING getDeviceName( unsigned short track )
	{
		return trackInfo[track].deviceName;
	}
	void setDeviceName( unsigned short track, const gak::STRING &deviceName )
	{
		trackInfo[track].deviceName = deviceName;
	}

	size_t getMidiOutDev( unsigned short track )
	{
		return track < trackInfo.size() ? trackInfo[track].midiOutDev : trackInfo.no_index;
	}
	void setMidiOutDev( unsigned short track, size_t midiOutDev )
	{
		trackInfo.getOrCreateElementAt(track).midiOutDev = midiOutDev;
	}

	unsigned long getLength( void ) const
	{
		doEnterFunction("MIDIdata::getLength");

		if( size() > 0 )
		{
			return this->crbegin()->getTimeCode();
		}
		else
		{
			return 0;
		}
	}

	bool getChannelCodes(
		unsigned long	timeCode,
		unsigned char	channel,
		ChannelCodes	*channelCodes
	) const;
	unsigned char allocateChannel( ChannelCodes *channelCodes );
	void removeChannel( unsigned char channel );

	int loadMidiFile( const char *fileName );
	int saveMidiFile( const char *fileName, bool mergeTracks );
	void optimizeMIDI( void );

	void clearAllSelections( void );

	void clear( void )
	{
		SortedArray<MIDIevent>::clear();
		trackInfo.clear();
		setDefaults();
	}

	size_t	findMatchingEvent( size_t i );

	void addElement( MIDIevent &newEvent )
	{
		if( newEvent.getIndex() < 0 )
		{
			newEvent.setIndex( int(size()) );
		}
		SortedArray<MIDIevent>::addElement( newEvent );
	}
};

namespace gak
{
	template<>
	inline int compare<MIDIevent> ( const MIDIevent &e1, const MIDIevent &e2 )
	{
		return MIDIdata::timeCompare( &e1, &e2 );
	}
}


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

const unsigned char *getDurNotes( unsigned char base );
const unsigned char *getDurScaleNotes( unsigned char base );
const unsigned char *getDur5ScaleNotes( unsigned char base );
const unsigned char *getMaj7Notes( unsigned char base );

const unsigned char *getMollNotes( unsigned char base );
const unsigned char *getMollScaleNotes( unsigned char base );
const unsigned char *getMoll5ScaleNotes( unsigned char base );
const unsigned char *getMollMaj7Notes( unsigned char base );

const unsigned char *getPlusNotes( unsigned char base );
const unsigned char *getDimNotes( unsigned char base );

const unsigned char *getDom7Notes( unsigned char base );
const unsigned char *getMoll7Notes( unsigned char base );

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
#	pragma option -a.
#	pragma option -p.
#endif
#ifdef _MSC_VER
#	pragma warning( pop )
#endif

#endif	// MIDI_FILE_H
