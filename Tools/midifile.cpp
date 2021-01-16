/*
		Project:		MIDI Tools
		Module:			midifile.cpp
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

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <stdio.h>
#include <string.h>
#include <math.h>

#include <gak/gaklib.h>
#include <gak/fmtNumber.h>
#include <gak/utils.h>

#include "midifile.h"
#include "midi.gui.h"

// --------------------------------------------------------------------- //
// ----- imported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module switches ----------------------------------------------- //
// --------------------------------------------------------------------- //

#ifdef _MSC_VER
#	pragma warning( disable: 4815 )	// Array mit Größe 0 in Stapelobjekt hat keine Elemente (sofern das Objekt nicht ein Aggregat ist, das mit einem Aggregat initialisiert wurde).
#endif

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

static unsigned char chordNotes[12];

/*
	chords
*/
static char durOffsets[] = { 0, 4, 7, -1 };
static char mollOffsets[] = { 0, 3, 7, -1 };

static char plusOffsets[] = { 0, 4, 8, -1 };
static char dimOffsets[] = { 0, 3, 6, -1 };

static char maj7Offsets[] = { 0, 4, 7, 11, -1 };
static char mollMaj7Offsets[] = { 0, 3, 7, 11, -1 };

static char dom7Offsets[] = { 0, 4, 7, 10, -1 };
static char moll7Offsets[] = { 0, 3, 7, 10, -1 };
/*
	scales
*/
static char durScaleOffsets[] = { 0, 2, 4, 5, 7, 9, 11, -1 };
static char dur5ScaleOffsets[] = { 0, 2, 4, 7, 9, -1 };
static char mollScaleOffsets[] = { 0, 2, 3, 5, 7, 8, 10, -1 };
static char moll5ScaleOffsets[] = { 0, 3, 5, 7, 10, -1 };

// --------------------------------------------------------------------- //
// ----- class static data --------------------------------------------- //
// --------------------------------------------------------------------- //

const char *MIDIevent::midiChannelMessages[] =
{
	"Note Off",
	"Note On",
	"Polyphonic After Touch",
	"Control Change",
	"Program Change",
	"Channel After Touch",
	"Pitch Wheel",
	"SYS_EX"
};

const char *MIDIevent::midiSystemMessages[] =
{
	"System Exclusive",
	"Time Code",
	"Song Position",
	"Song Select",
	"F4",
	"F5",
	"Tune Request",
	"System Exclusive End",
	"Timing",
	"F9",
	"Start",
	"Continue",
	"Stop",
	"FD",
	"Active Sense",
	"Reset"
};

const char *MIDIevent::midiControllerMessages[] =
{
	"Bank Select MSB",
	"Modulation Wheel MSB",
	"Breath Controller MSB",
	"",
	"Foot Controller MSB",
	"Portamento Time MSB",
	"Data Entry MSB",
	"Channel Volume MSB",
	"Balance MSB",
	"",
	"Pan Position MSB",
	"Expression MSB",
	"Effect Control 1 MSB",
	"Effect Control 2 MSB",
	"",
	"",
	"General Purpose Controller 1 MSB",
	"General Purpose Controller 2 MSB",
	"General Purpose Controller 3 MSB",
	"General Purpose Controller 4 MSB",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"Bank Select LSB",
	"Modulation Wheel LSB",
	"Breath Controller LSB",
	"",
	"Foot Controller LSB",
	"Portamento Time LSB",
	"Data Entry LSB",
	"Cannel Volume LSB",
	"Balance LSB",
	"",
	"Pan Position LSB",
	"Expression LSB",
	"Effect Control 1 LSB",
	"Effect Control 2 LSB",
	"",
	"",
	"General Purpose Controller 1 LSB",
	"General Purpose Controller 2 LSB",
	"General Purpose Controller 3 LSB",
	"General Purpose Controller 4 LSB",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"Sustain",
	"Portamento",
	"Sostenuto",
	"Soft Pedal",
	"Legato Footswitch",
	"Hold 2",
	"Sound Controller 1 (Sound Variation)",
	"Sound Controller 2 (Timbre/Harmonic Intens.)",
	"Sound Controller 3 (Release Time)",
	"Sound Controller 4 (Attack Time)",
	"Sound Controller 5 (Brightness)",
	"Sound Controller 6 (Decay Time)",
	"Sound Controller 7 (Vibrato Rate)",
	"Sound Controller 8 (Vibrato Depth)",
	"Sound Controller 9 (Vibrato Delay)",
	"Sound Controller 10",
	"General Purpose Controller 5",
	"General Purpose Controller 6",
	"General Purpose Controller 7",
	"General Purpose Controller 8",
	"Portamento Control",
	"",
	"",
	"",
	"High Resolution Velocity Prefix",
	"",
	"",
	"Effect 1 (Reverb Send)",
	"Effect 2 (Tremolo)",
	"Effect 3 (Chorus)",
	"Effect 4 (Celeste/Detune)",
	"Effect 5 (Phaser)",
	"Data Increment",
	"Data Decrement",
	"Non-Registered Parameter Number LSB",
	"Non-Registered Parameter Number MSB",
	"Registered Parameter Number LSB",
	"Registered Parameter Number MSB",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"Sound OFF",
	"Reset Controllers",
	"Local ON/OFF",
	"All Notes OFF",
	"Omni Mode OFF",
	"Omni Mode ON",
	"Mono Mode ON",
	"Poly Mode ON"
};

const char *MIDIevent::midiNotes[] =
{
	"C",
	"C#",
	"D",
	"D#",
	"E",
	"F",
	"F#",
	"G",
	"G#",
	"A",
	"A#",
	"B"
};

// --------------------------------------------------------------------- //
// ----- prototypes ---------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module functions ---------------------------------------------- //
// --------------------------------------------------------------------- //

static unsigned char *WriteVarLen( unsigned long value, unsigned char *cp )
{
	unsigned long	buffer;

	buffer = (unsigned char)(value & 0x7F);

	while ( (value >>= 7) )
	{
		buffer <<= 8;
		buffer |= ((value & 0x7F) | 0x80);
	}

	while( 1 )
	{
		*cp++ = (unsigned char)(buffer&0xFF);
		if (buffer & 0x80)
			buffer >>= 8;
		else
			break;
	}

	return cp;
}

static unsigned long ReadVarLen( const unsigned char *buffer, size_t *pos )
{
	register unsigned long value;
	register unsigned char c;
	register size_t	i = *pos;

	if ( (value = buffer[i++]) & 0x80 )
	{
	   value &= 0x7F;
	   do
	   {
		 value = (value << 7) + ((c = buffer[i++]) & 0x7F);
	   } while (c & 0x80);
	}

	*pos = i;
	return(value);
}

static void fillChord( unsigned char baseNote, char *offsets )
{
	char offset;

	unsigned char *chordNote = chordNotes;	
	while( (offset = *offsets++) >= 0 )
	{
		*chordNote++ = (char)(baseNote + offset); 
	}
	*chordNote = 255; 
}
// --------------------------------------------------------------------- //
// ----- class inlines ------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class constructors/destructors -------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class static functions ---------------------------------------- //
// --------------------------------------------------------------------- //

int MIDIdata::timeCompare(
	const MIDIevent *event1, const MIDIevent *event2
)
{
	int	compareVal = event1->getTimeCode() - event2->getTimeCode();

	if( !compareVal )
		compareVal = event1->getTrack() - event2->getTrack();

	if( !compareVal )
		compareVal = event1->getIndex() - event2->getIndex();

	if( !compareVal )
	{
		unsigned char msg1 = event1->getMessage();
		unsigned char msg2 = event2->getMessage();

		if( msg1 != msg2 )
		{
			if( (msg1 == MIDI_NOTE_ON || msg1 == MIDI_NOTE_OFF)
			&&  (msg2 != MIDI_NOTE_ON && msg2 != MIDI_NOTE_OFF) )
				compareVal = 1;
			else if( (msg1 != MIDI_NOTE_ON && msg1 != MIDI_NOTE_OFF)
				 &&  (msg2 == MIDI_NOTE_ON || msg2 == MIDI_NOTE_OFF) )
				compareVal = -1;

			if( !compareVal )
			{
				if( msg1 == MIDI_NOTE_ON && msg2 == MIDI_NOTE_OFF )
					compareVal = 1;
				else if( msg1 == MIDI_NOTE_OFF && msg2 == MIDI_NOTE_ON )
					compareVal = -1;
			}
		}
	}

	return compareVal;
}

const char *MIDIevent::getNoteText( unsigned char note )
{
	static char tmpNoteText[32];

	sprintf( tmpNoteText, "%s%d", midiNotes[note%12], (note/12)-2 );

	return tmpNoteText;
}

const char *MIDIevent::getControllerText( unsigned char controller )
{
	static char	tmpControllerText[128];
	const char	*controllerText;

	controllerText = midiControllerMessages[controller];
	if( !*controllerText )
	{
		sprintf( tmpControllerText, "Controller %u", (unsigned int)controller );
		controllerText = tmpControllerText;
	}

	return controllerText;
}


// --------------------------------------------------------------------- //
// ----- class privates ------------------------------------------------ //
// --------------------------------------------------------------------- //

void MIDIdata::convertTimeCodes( int oldBase, int newBase )
{
	doEnterFunction("MIDIdata::convertTimeCodes");

	if( oldBase && newBase && oldBase != newBase )
	{
		double factor = double(oldBase) / double(newBase);
		for( iterator it=begin(), endIT = end(); it != endIT; ++it )
		{
			MIDIevent	&theEvent = *it;
			unsigned long timeCode = theEvent.getTimeCode();
			timeCode = (unsigned long)(double(timeCode) * factor);
			theEvent.setTimeCode( timeCode );
		}
	}
}

void MIDIdata::fixTimeCodes( int fraction )
{
	double	factor, eventTimeCode;
	double	timeFixPoint = getTimePerFulNoteDbl() / double(fraction);

	for( iterator it=begin(), endIT = end(); it != endIT; ++it )
	{
		MIDIevent	&theEvent = *it;

		eventTimeCode = theEvent.getTimeCode();
		factor = eventTimeCode / timeFixPoint;
		factor = floor( factor + 0.5 );
		eventTimeCode = factor * timeFixPoint;

		if( theEvent.getMessage() == MIDI_NOTE_OFF )
			eventTimeCode -= 2;

		theEvent.setTimeCode( (unsigned long)(eventTimeCode+0.5) );
	}
}

// --------------------------------------------------------------------- //
// ----- class protected ----------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class virtuals ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class publics ------------------------------------------------- //
// --------------------------------------------------------------------- //

// align timeCode, so that first note will be in first bar
void MIDIdata::alignTimeCodes( void )
{
	unsigned long timePerBar;
	unsigned long	firstNoteTimeCode = 0, eventTimeCode;

	doEnterFunction("MIDIdata::alignTimeCodes");

	for( const_iterator it=cbegin(), endIT = cend(); it != endIT; ++it )
	{
		if( it->getMessage() == MIDI_NOTE_ON )
		{
			firstNoteTimeCode = it->getTimeCode();
			break;
		}
	}

	if( firstNoteTimeCode )
	{
		timePerBar = getTimePerBar();
		firstNoteTimeCode = firstNoteTimeCode / timePerBar;
		firstNoteTimeCode *= timePerBar;
		for( iterator it=begin(), endIT = end(); it != endIT; ++it )
		{
			eventTimeCode = it->getTimeCode();
			if( eventTimeCode <= firstNoteTimeCode )
				it->setTimeCode( 0 );
			else
				it->setTimeCode( eventTimeCode-firstNoteTimeCode );
		}
	}
}

STRING MIDIdata::getTimeCodeStr( unsigned long unsignedTimeCode ) const
{
	long	timeCode = unsignedTimeCode;
	double	timePerTakt = getTimePerBarDbl();
	double	timePerBeat = timePerTakt/(double)timeSigNumerator;

	double	taktDbl = timeCode/timePerTakt + 0.01;
	int takt = int(taktDbl);
	timeCode -= long(double(takt) * timePerTakt);
	if( timeCode < 0 )
		timeCode = 0;
	int beat = int(timeCode/timePerBeat + 0.01);
	timeCode -= long((double)beat * timePerBeat);
	if( timeCode < 0 )
		timeCode = 0;

	STRING	result = formatNumber( ++takt );
	result += '.';
	result += formatNumber( ++beat );

	result += '.';
	result += formatNumber( timeCode );

	return result;
}

unsigned long MIDIdata::parseTimeCode( const char *timeCodeStr ) const
{
	unsigned long	timeCode;
	int				takt, beat, length;

	double		timePerTakt = getTimePerBarDbl();
	double		timePerBeat = timePerTakt/(double)timeSigNumerator;

	takt = beat = length = 0;
	sscanf( timeCodeStr, "%d.%d.%d", &takt, &beat, &length );

	if( takt )
		takt--;
	if( beat )
		beat--;

	timeCode = (unsigned long)(double(takt) * timePerTakt + double(beat)*timePerBeat + length);

	return timeCode;
}

long MIDIdata::parseRelativeTimeCode( const char *timeCodeStr ) const
{
	return ( *timeCodeStr == '-' ) 
		? -long(parseTimeCode( timeCodeStr+1 ))
		: long(parseTimeCode( timeCodeStr ))
	;
}

/*
	=======================================================================
	track handling functions
	=======================================================================
*/
size_t MIDIdata::getNumTrackData( unsigned short track ) const
{
	size_t numTrackData = 0;
	for( const_iterator it=cbegin(), endIT = cend(); it != endIT; ++it )
	{
		if( it->getTrack() == track )
			numTrackData++;
	}

	return numTrackData;
}

void MIDIdata::removeUnusedTracks( void )
{
	unsigned short curTrack = 0, numTracks = getNumTracks();
	while( curTrack < numTracks )
	{
		if( !getNumTrackData( curTrack ) )
		{
			removeTrack( curTrack );
			numTracks--;
		}
		else
			curTrack++;
	}
}

void MIDIdata::removeTrack( unsigned short track )
{
	size_t			i;
	unsigned short	curTrack;

	i=0;
	while( i<size() )
	{
		curTrack = (*this)[i].getTrack();
		if( curTrack == track )
		{
			removeElementAt( i );
		}
		else if( curTrack > track )
		{
			(*this)[i].setTrack( (short)(curTrack-1) );
			i++;
		}
		else
		{
			i++;
		}
	}

	trackInfo.removeElementAt( track );
}

void MIDIdata::replaceTrack( unsigned short track, const MIDIdata *newData )
{
	unsigned short	curTrack;

	std::size_t		i=0;
	while( i<size() )
	{
		curTrack = (*this)[i].getTrack();
		if( curTrack == track )
		{
			this->removeElementAt( i );
		}
		else
		{
			i++;
		}
	}

	for( 
		const_iterator it = newData->cbegin(), endIT = newData->cend();
		it != endIT;
		++it
	)
	{
		MIDIevent	newEvent = *it;
		newEvent.setTrack( track );
		addElement( newEvent );
	}
}

unsigned short MIDIdata::getNumTracks( void ) const
{
	unsigned short	maxTrack = (unsigned short)trackInfo.size(), 
					curTrack;

	if( maxTrack )
	{
		maxTrack--;
	}

	for( 
		const_iterator it = cbegin(), endIT = cend();
		it != endIT;
		++it
	)
	{
		curTrack = it->getTrack();
		if( curTrack > maxTrack )
			maxTrack = curTrack;
	}

	return (unsigned short)(maxTrack+1);
}

bool MIDIdata::getChannelCodes(
	unsigned long	timeCode,
	unsigned char	channel,
	ChannelCodes	*channelCodes
) const
{
	unsigned char		message;
	unsigned char		data1, data2;
	bool				dataFound = false;

	memset( channelCodes, -1, sizeof( ChannelCodes ) );

	for( 
		const_iterator it = cbegin(), endIT = cend();
		it != endIT;
		++it
	)
	{
		const MIDIevent	&theEvent = *it;
		if( theEvent.getTimeCode() > timeCode )
/*v*/		break;

		if( theEvent.getChannel() == channel )
		{
			dataFound = true;

			message = theEvent.getMessage();
			if( message == MIDI_PROGRAM )
				channelCodes->voiceCodes.program = theEvent.getData1();
			else if( message == MIDI_CONTROLLER )
			{
				data1 = theEvent.getData1();
				data2 = theEvent.getData2();
				if( data1 == MIDI_BANK_MSB )
					channelCodes->voiceCodes.bankMSB = data2;
				else if( data1 == MIDI_BANK_LSB )
					channelCodes->voiceCodes.bankLSB = data2;
				else if( data1 == MIDI_PAN_POSITION || data1 == MIDI_BALANCE_POSITION )
					channelCodes->stereoPos = data2-MIDI_CENTER_PAN;
				else if( data1 == MIDI_VOLUME )
					channelCodes->volume = data2;
			}
		}
	}

	return dataFound;
}

unsigned char MIDIdata::allocateChannel( ChannelCodes *channelCodes )
{
	bool			useNewChannel = true;
	ChannelCodes	channelSettings;

	unsigned char channel;
	unsigned char newChannel = 255;
	for( channel=0; channel<16; channel++ )
	{
		if( !getChannelCodes( 0, channel, &channelSettings ) )
			newChannel = channel;
		else if(
			channelCodes->voiceCodes.program == channelSettings.voiceCodes.program
		&&	channelCodes->voiceCodes.bankMSB == channelSettings.voiceCodes.bankMSB
		&&	channelCodes->voiceCodes.bankLSB == channelSettings.voiceCodes.bankLSB
		&&	channelCodes->stereoPos == channelSettings.stereoPos
		&&	channelCodes->volume == channelSettings.volume
		)
		{
			useNewChannel = false;
			newChannel = channel;
/*v*/		break;
		}
	}

	if( useNewChannel && newChannel < 16 )
	{
		MIDIevent	newEvent;

		if( channelCodes->voiceCodes.program < 128 )
		{
			newEvent.setProgramChange( newChannel, channelCodes->voiceCodes.program );
			addElement( newEvent );
		}
		if( channelCodes->voiceCodes.bankMSB < 128 )
		{
			newEvent.setBankMSB( newChannel, channelCodes->voiceCodes.bankMSB );
			addElement( newEvent );
		}
		if( channelCodes->voiceCodes.bankLSB < 128 )
		{
			newEvent.setBankLSB( newChannel, channelCodes->voiceCodes.bankLSB );
			addElement( newEvent );
		}
		newEvent.setPanPosition( newChannel, channelCodes->stereoPos );
		addElement( newEvent );
		newEvent.setBalancePosition( newChannel, channelCodes->stereoPos );
		addElement( newEvent );
		newEvent.setVolume( newChannel, channelCodes->volume );
		addElement( newEvent );
	}

	return newChannel;
}

void MIDIdata::removeChannel( unsigned char channel )
{
	for( int i=int(size())-1; i >= 0; i-- )
	{
		const MIDIevent &theEvent = (*this)[i];
		if( theEvent.getChannel() == channel )
		{
			removeElementAt( i );
		}
	}
}

int MIDIdata::loadMidiFile( const char *fileName )
{
	doEnterFunction("MIDIdata::loadMidiFile");

	int	errCode = 0;

	FILE	*fp = fopen( fileName, "rb" );
	if( fp )
	{
		MIDIevent			theEvent;

		MTHD_CHUNK			midiHeader;
		MTRK_CHUNK			midiTrack;
		unsigned char		*events;
		size_t				i, length;
		unsigned long		timeCode;
		unsigned char		lastStatusByte, thisStatusByte, type, data1, data2;
		int					bpm, tempo;
		unsigned short		curTrack;
		STRING				name;

		fread( (void *)&midiHeader, sizeof( midiHeader ), 1, fp );

		#if defined( __BORLANDC__ ) || defined( _MSC_VER )
			midiHeader.Length = intlmot(midiHeader.Length);
			midiHeader.Format = (unsigned short)(intimot(midiHeader.Format));
			midiHeader.NumTracks = (unsigned short)(intimot(midiHeader.NumTracks));
			midiHeader.Division = (short)(intimot(midiHeader.Division));
		#endif

		if( !strncmp( midiHeader.ID, "MThd", 4 )
		&&  (midiHeader.Format == 0 || midiHeader.Format == 1)
		&&  midiHeader.Length == 6 )
		{
			clear();
			for( curTrack = 0; curTrack<midiHeader.NumTracks; curTrack++ )
			{
				if( fread( (void *)&midiTrack, sizeof( midiTrack ), 1, fp ) == 1
				&&	!strncmp( midiTrack.ID, "MTrk", 4 ) )
				{
					#if defined( __BORLANDC__ ) || defined( _MSC_VER )
						midiTrack.Length = intlmot(midiTrack.Length);
					#endif

					events = (unsigned char *)malloc( midiTrack.Length );
					if( events )
					{
						if( fread( events, 1, midiTrack.Length, fp ) == midiTrack.Length )
						{
							tempo = (int)(60000000/120);
							i = 0;
							timeCode = 0;
							lastStatusByte = 0;		// to avoid warning
							while( i<midiTrack.Length )
							{
								timeCode += ReadVarLen( events, &i );

								thisStatusByte = events[i++];
								if( thisStatusByte == MIDI_FILE_META )
								{
									type = events[i++];
									length = ReadVarLen( events, &i );

									if( type == MIDI_FILE_TEMPO )
									{
										tempo = 0;
										while( length > 0 )
										{
											tempo = (tempo << 8) | events[i++];
											length--;
										}
									}
									else if( type == MIDI_FILE_TIME_SIG && length == 4 )
									{
										timeSigNumerator = events[i++];
										timeSigDenominator = 1 << events[i++];
										length = 0;
										i += 2;
									}
									else if( type == MIDI_FILE_TRACK_NAME
									|| type == MIDI_FILE_INSTRUMENT
									|| type == MIDI_FILE_DEVICE_NAME )
									{
										name = "";
										while( length > 0 )
										{
											name += (char)events[i++];
											length--;
										}
										if( type == MIDI_FILE_TRACK_NAME )
											setTrackName( curTrack, name );
										else if( type == MIDI_FILE_INSTRUMENT )
											setInstrument( curTrack, name );
										else if( type == MIDI_FILE_DEVICE_NAME )
											setDeviceName( curTrack, name );
									}
									else if( type == MIDI_FILE_PORT
									&& length == 1 )
									{
										setMidiOutDev( curTrack, events[i++] );
									}
									else if( type == MIDI_FILE_SEQUENCER
									&& length == 8
									&& events[i] == 0
									&& !strcmp( (char *)events+i+1, "gak" ) )
									{
										setMidiOutDev( curTrack, events[i+7] );
										i += length;
									}
									else
										i += length;	// ignore data

									// end of track
									if( type == MIDI_FILE_TRACK_END )
/*v*/									break;
								}
								else if( thisStatusByte==MIDI_SYS_EX_START )
								{
									length = ReadVarLen( events, &i );

									theEvent.setTrack( curTrack );
									theEvent.setMessageCode( MIDI_SYS_EX_START );
									theEvent.setTimeCode( timeCode );
									theEvent.setSysExData( events + i, length, false );
									theEvent.setIndex( int(size()) );
									addElement( theEvent );

									i += length;
								}
								else	// midi event
								{
									data2 = 0;
									if( !(thisStatusByte & 0x80) )
									{
										data1 = thisStatusByte;
										thisStatusByte = lastStatusByte;
									}
									else
									{
										data1 = events[i++];
										lastStatusByte = thisStatusByte;
									}
									switch( thisStatusByte >> 4 )
									{
										case MIDI_NOTE_OFF:
										case MIDI_NOTE_ON:
										case MIDI_POLYPHONE:
										case MIDI_CONTROLLER:
										case MIDI_PITCH:
											data2 = events[i++];

										case MIDI_PROGRAM:
										case MIDI_CHANNEL:
											break;

										default:	// ignore system messages
											thisStatusByte = 0;
									}
									if( thisStatusByte )
									{
										theEvent.setIndex( int(size()) );
										theEvent.setTrack( curTrack );
										theEvent.setMessageCode( thisStatusByte, data1, data2 );
										theEvent.setTimeCode( timeCode );
										addElement( theEvent );
									}
								}
							}	// while( i<midiTrack.Length )

							bpm = 60000000 / tempo;
						}	// if( fread( events, 1, midiTrack.Length, fp ) == midiTrack.Length )
						free( events );
					}	// if( events )
				}	// if( !strncmp( midiTrack.ID, "MTrk", 4 ) )
				else
				{
					errCode = winlibGUI::MIDI_BAD_TRACK_HEADER_id;
/*v*/				break;
				}
			} // for( curTrack = 0; curTrack<midiHeader.NumTracks; curTrack++ )

			setBPM( bpm, false, 0 );
			if( midiHeader.Division > 0 )
			{
				if( bpm * midiHeader.Division != 60000 )
					convertTimeCodes(
						 bpm * midiHeader.Division, 60000
					);
			}
			else
			{
				int fraction = -(midiHeader.Division >> 8) * (midiHeader.Division & 0xFF);
				if( fraction != 1000 )
					convertTimeCodes( fraction, 1000 );

			}

			alignTimeCodes();

		}	// if( !strncmp( midiHeader.ID, "MThd", 4 ) ... )
		else
		{
			errCode = winlibGUI::MIDI_BAD_HEADER_id;
		}

		fclose( fp );
	}	// if( fp )
	return errCode;
}

int MIDIdata::saveMidiFile( const char *fileName, bool mergeTracks )
{
	doEnterFunction("MIDIdata::saveMidiFile");

	FILE	*fp = fopen( fileName, "wb" );
	if( fp )
	{
		MIDIevent			theEvent;

		MTHD_CHUNK			midiHeader;
		MTRK_CHUNK			midiTrack;
		unsigned char		*events, *cp, *rollbackPointer;
		size_t				curEvent, numEvents, bufferSize;
		unsigned long		timeCode;
		long				timeCodeOffset;
		unsigned char		msg, statusByte, data1, data2;
		int					bpm = getBPM();
		unsigned short		curTrack, numTracks = getNumTracks();
		STRING				name;

		strncpy( midiHeader.ID, "MThd", 4 );
		midiHeader.Length = 6;
		midiHeader.Format = (short)(mergeTracks || (numTracks <= 1) ? 0 : 1);
		midiHeader.NumTracks = (short)(mergeTracks ? 1 : numTracks);
//		midiHeader.Division = 0xE728;
		midiHeader.Division = (short)(60000/bpm);		// Ticks per beat 60000 Milisec / bpm

		#if defined( __BORLANDC__ ) || defined( _MSC_VER )
			midiHeader.Length = intlmot(midiHeader.Length);
			midiHeader.Format = (unsigned short)(intimot(midiHeader.Format));
			midiHeader.NumTracks = (unsigned short)(intimot(midiHeader.NumTracks));
			midiHeader.Division = (short)(intimot(midiHeader.Division));
		#endif

		fwrite( &midiHeader, sizeof( midiHeader ), 1, fp );

		strncpy( midiTrack.ID, "MTrk", 4 );

		numEvents = size();
		bufferSize = sizeof( MIDIevent ) * numEvents + (7+4+11); // tempo + End Of Track + Sequencer data
		for( curTrack = 0; curTrack < numTracks; curTrack++ )
		{
			name = getTrackName( curTrack );
			bufferSize += name.strlen() + 7;
			name = getInstrument( curTrack );
			bufferSize += name.strlen() + 7;
		}
		/*
			yes I know, this is too much, but sure is sure
		*/
		events = (unsigned char *)malloc( bufferSize );
		if( events )
		{
			for( curTrack = 0; curTrack < numTracks; curTrack++ )
			{
				cp = events;
				timeCode = 0;

				name = getTrackName( curTrack );
				if( !name.isEmpty() )
				{
					*cp++ = 0;
					*cp++ = MIDI_FILE_META;
					*cp++ = MIDI_FILE_TRACK_NAME;
					cp = WriteVarLen( static_cast<unsigned long>(name.strlen()), cp );
					for( size_t i=0; i<name.strlen(); i++ )
					{
						*cp++ = name[i];
					}
				}
				name = getInstrument( curTrack );
				if( !name.isEmpty() )
				{
					*cp++ = 0;
					*cp++ = MIDI_FILE_META;
					*cp++ = MIDI_FILE_INSTRUMENT;
					cp = WriteVarLen( static_cast<unsigned long>(name.strlen()), cp );
					for( size_t i=0; i<name.strlen(); i++ )
					{
						*cp++ = name[i];
					}
				}
				name = getDeviceName( curTrack );
				if( !name.isEmpty() )
				{
					*cp++ = 0;
					*cp++ = MIDI_FILE_META;
					*cp++ = MIDI_FILE_DEVICE_NAME;
					cp = WriteVarLen( static_cast<unsigned long>(name.strlen()), cp );
					for( size_t i=0; i<name.strlen(); i++ )
					{
						*cp++ = name[i];
					}
				}

				/*
					store the midi output device ID
				*/
				*cp++ = 0;
				*cp++ = MIDI_FILE_META;
				*cp++ = MIDI_FILE_PORT;
				*cp++ = '\x01';
				*cp++ = static_cast<unsigned char>(getMidiOutDev( curTrack ));

				*cp++ = 0;
				*cp++ = MIDI_FILE_META;
				*cp++ = MIDI_FILE_SEQUENCER;
				*cp++ = '\x08';
				*cp++ = 0;
				*cp++ = 'C';
				*cp++ = 'R';
				*cp++ = 'E';
				*cp++ = 'S';
				*cp++ = 'D';
				*cp++ = 0;
				*cp++ = (unsigned char)getMidiOutDev( curTrack );

				if( bpm != 120 )
				{
					unsigned long tempo = 60000000/bpm;		// time per beat in microseconds

					*cp++ = 0;
					*cp++ = MIDI_FILE_META;
					*cp++ = MIDI_FILE_TEMPO;
					*cp++ = '\x03';
					*cp++ = (char)(tempo>>16);
					*cp++ = (char)(tempo>>8);
					*cp++ = (char)(tempo>>0);
				}
				if( timeSigNumerator != 4 || timeSigDenominator != 4 )
				{
					*cp++ = 0;
					*cp++ = MIDI_FILE_META;
					*cp++ = MIDI_FILE_TIME_SIG;
					*cp++ = '\x04';
					*cp++ = (char)(timeSigNumerator);
					*cp++ = (char)(positiveBitCount(timeSigDenominator)-1);
					*cp++ = (char)(24 * 4 / timeSigDenominator);
					*cp++ = 8;
				}

				for( curEvent=0; curEvent<numEvents; curEvent++ )
				{
					theEvent = (*this)[curEvent];
					if( mergeTracks || theEvent.getTrack() == curTrack )
					{
						rollbackPointer = cp;

						// time code
						timeCodeOffset = theEvent.getTimeCode() - timeCode;
						if( timeCodeOffset < 0 )
							timeCodeOffset = 0;

						cp = WriteVarLen( timeCodeOffset, cp );

						// status and data bytes
						statusByte = theEvent.getStatusByte();
						msg = theEvent.getMessage();
						data1 = theEvent.getData1();
						data2 = theEvent.getData2();

						if( statusByte == MIDI_SYS_EX_START )
						{
							size_t				len;
							const unsigned char	*sysExData = theEvent.getSysExData( &len, false );

							if( len )
							{
								bufferSize += len;
								unsigned char *newEventBuffer = (unsigned char *)realloc( events, bufferSize );
								cp = newEventBuffer + (cp-events);
								// rollbackPointer = newEventBuffer + (rollbackPointer-events);

								events = newEventBuffer;
							}
							*cp++ = MIDI_SYS_EX_START;
							cp = WriteVarLen( static_cast<unsigned long>(len), cp );
							memcpy( cp, sysExData, len );
							cp += len;
						}
						else if( msg != MIDI_SYSTEM )
						{
							if( msg == MIDI_NOTE_ON && data2 == 0 )
							{
								msg = MIDI_NOTE_OFF;
								statusByte = (unsigned char)((MIDI_NOTE_OFF<<4) | (statusByte&0xF));
							}

							*cp++ = statusByte;
							*cp++ = data1;

							switch( msg )
							{
								case MIDI_NOTE_OFF:
								case MIDI_NOTE_ON:
								case MIDI_POLYPHONE:
								case MIDI_CONTROLLER:
								case MIDI_PITCH:
									*cp++ = data2;

								case MIDI_PROGRAM:
								case MIDI_CHANNEL:
									timeCode = theEvent.getTimeCode();
									break;

								default:		// ignore event
									cp = rollbackPointer;
							}
						}
					} // if( mergeTracks || theEvent.getTrack() == curTrack )
				} // for( curEvent=0; curEvent<numEvents; curEvent++ )

				// the end of track marker
				*cp++ = 0x00;
				*cp++ = MIDI_FILE_META;
				*cp++ = MIDI_FILE_TRACK_END;
				*cp++ = 0x00;

				// now we can finish the header and write the file
				midiTrack.Length = (unsigned long)(cp-events);
				#if defined( __BORLANDC__ ) || defined( _MSC_VER )
					midiTrack.Length = intlmot(midiTrack.Length);
				#endif

				fwrite( &midiTrack, sizeof( midiTrack ), 1, fp );
				fwrite( events, 1, (unsigned long)(cp-events), fp );

				if( mergeTracks )
					break;
			} // for( curTrack = 0; curTrack < numTracks; curTrack++ )

			free( events );
		} // if( events )

		fclose( fp );
	} // if( fp )
	return 0;
}

void MIDIdata::optimizeMIDI( void )
{
	std::size_t		curEvent;
	VoiceCodes		midiVoices[16];
	int				voiceChange[16];
	unsigned char	channel, curChannel, message, data1, data2;
	bool			removed;

	// merge channels if possible
	for( curChannel=0; curChannel<16; curChannel++ )
	{
		midiVoices[curChannel].program = 0;
		midiVoices[curChannel].bankMSB = 0;
		midiVoices[curChannel].bankLSB = 0;
		voiceChange[curChannel] = 0;
	}

	for( curEvent=0; curEvent<size(); curEvent++ )
	{
		const MIDIevent	&theEvent = (*this)[curEvent];

		channel = theEvent.getChannel();
		message = theEvent.getMessage();
		data1 = theEvent.getData1();
		data2 = theEvent.getData2();

		if( message == MIDI_PROGRAM
		|| (message == MIDI_CONTROLLER && (data1 == MIDI_BANK_MSB || data1 == MIDI_BANK_LSB)))
		{
			if( voiceChange[channel] == (1|2|4) )	// do we have a comlete voice change?
			{
				for( curChannel = 0; curChannel<16; curChannel++ )
				{
					if( curChannel != channel
					&& voiceChange[curChannel] == (1|2|4)
					&& midiVoices[curChannel].program == midiVoices[channel].program
					&& midiVoices[curChannel].bankMSB == midiVoices[channel].bankMSB
					&& midiVoices[curChannel].bankLSB == midiVoices[channel].bankLSB )
					{
						for( size_t i=0; i<curEvent; i++ )
						{
							MIDIevent	&theEvent = (*this)[i];
							if( theEvent.getChannel() == channel )
							{
								theEvent.setChannel( curChannel );
							}
						}

						break;
					}

				}
				voiceChange[channel] = 0;
			}

			if( message == MIDI_PROGRAM )
			{
				voiceChange[channel] |= 1;
				midiVoices[channel].program = data1;
			}
			else if( data1 == MIDI_BANK_MSB )
			{
				voiceChange[channel] |= 2;
				midiVoices[channel].bankMSB = data2;
			}
			else
			{
				voiceChange[channel] |= 4;
				midiVoices[channel].bankLSB = data2;
			}
		}
	}

	// remove duplicate voice changes
	for( curChannel=0; curChannel<16; curChannel++ )
	{
		midiVoices[curChannel].program = 0;
		midiVoices[curChannel].bankMSB = 0;
		midiVoices[curChannel].bankLSB = 0;
		voiceChange[curChannel] = 0;
	}
	curEvent=0;
	while( curEvent<size() )
	{
		const MIDIevent &theEvent = (*this)[curEvent];

		removed = false;
		channel = theEvent.getChannel();
		message = theEvent.getMessage();
		data1 = theEvent.getData1();
		data2 = theEvent.getData2();

		if( message == MIDI_PROGRAM )
		{
			if( voiceChange[channel] & 1 )		// we have allready a program change
			{
				if( midiVoices[channel].program == data1 )
				{
					removed = true;
					removeElementAt( curEvent );
				}
				else
				{
					midiVoices[channel].program = data1;
				}
			}
			else
			{
				voiceChange[channel] |= 1;
				midiVoices[channel].program = data1;
			}
		}
		else if( message == MIDI_CONTROLLER )
		{
			if( data1 == MIDI_BANK_MSB )
			{
				if( voiceChange[channel] & 2 )		// we have allready a msb change
				{
					if( midiVoices[channel].bankMSB == data2 )
					{
						removed = true;
						removeElementAt( curEvent );
					}
					else
					{
						midiVoices[channel].bankMSB = data2;
					}
				}
				else
				{
					voiceChange[channel] |= 2;
					midiVoices[channel].bankMSB = data2;
				}
			}
			else if( data1 == MIDI_BANK_LSB )
			{
				if( voiceChange[channel] & 4 )		// we have allready a lsb change
				{
					if( midiVoices[channel].bankLSB == data2 )
					{
						removed = true;
						removeElementAt( curEvent );
					}
					else
					{
						midiVoices[channel].bankLSB = data2;
					}
				}
				else
				{
					voiceChange[channel] |= 4;
					midiVoices[channel].bankLSB = data2;
				}
			}
		}

		if( !removed )
		{
			curEvent++;
		}
	}
}

void MIDIdata::clearAllSelections( void )
{
	for( iterator it=begin(), endIT = end(); it != endIT; ++it )
	{
		MIDIevent	&theEvent = *it;
		theEvent.clearSelected();
	}
}

size_t MIDIdata::findMatchingEvent( size_t i )
{
	size_t				matchIdx = size_t(-1);
	const MIDIevent		&event = (*this)[i];
	unsigned char 		msg = event.getMessage();
	unsigned char		channel = event.getChannel();
	unsigned short		track = event.getTrack();
	unsigned char		note = event.getData1();

	if( msg == MIDI_NOTE_ON )
	{
		for( size_t j=i+1; j<size(); j++ )
		{
			const MIDIevent	&match = (*this)[j];
			if( match.getMessage() == MIDI_NOTE_OFF
			&& channel == match.getChannel()
			&& track == match.getTrack()
			&& note == match.getData1() )
			{
				matchIdx = j;
/*v*/			break;
			}
		}
	}
	else if( msg == MIDI_NOTE_OFF )
	{
		for( size_t j=i-1; j!=-1; j-- )
		{
			const MIDIevent	&match = (*this)[j];
			if( match.getMessage() == MIDI_NOTE_ON
			&& channel == match.getChannel()
			&& track == match.getChannel()
			&& note == match.getData1() )
			{
				matchIdx = j;
/*v*/			break;
			}
		}
	}

	return matchIdx;
}

// --------------------------------------------------------------------- //
// ----- entry points -------------------------------------------------- //
// --------------------------------------------------------------------- //

const unsigned char *getDurNotes( unsigned char base )
{
	fillChord( base, durOffsets );

	return chordNotes;
}

const unsigned char *getDurScaleNotes( unsigned char base )
{
	fillChord( base, durScaleOffsets );

	return chordNotes;
}

const unsigned char *getMollScaleNotes( unsigned char base )
{
	fillChord( base, mollScaleOffsets );

	return chordNotes;
}

const unsigned char *getMoll5ScaleNotes( unsigned char base )
{
	fillChord( base, moll5ScaleOffsets );

	return chordNotes;
}

const unsigned char *getDur5ScaleNotes( unsigned char base )
{
	fillChord( base, dur5ScaleOffsets );

	return chordNotes;
}

const unsigned char *getMaj7Notes( unsigned char base )
{
	fillChord( base, maj7Offsets );

	return chordNotes;
}

const unsigned char *getMollMaj7Notes( unsigned char base )
{
	fillChord( base, mollMaj7Offsets );

	return chordNotes;
}

const unsigned char *getMollNotes( unsigned char base )
{
	fillChord( base, mollOffsets );

	return chordNotes;
}

const unsigned char *getPlusNotes( unsigned char base )
{
	fillChord( base, plusOffsets );

	return chordNotes;
}

const unsigned char *getDimNotes( unsigned char base )
{
	fillChord( base, dimOffsets );

	return chordNotes;
}

const unsigned char *getDom7Notes( unsigned char base )
{
	fillChord( base, dom7Offsets );

	return chordNotes;
}

const unsigned char *getMoll7Notes( unsigned char base )
{
	fillChord( base, moll7Offsets );

	return chordNotes;
}

