/*
		Project:		Synthesizer Core
		Module:			midi.h
		Description:	Some MIDI constants
		Author:			Martin Gäckler
		Address:		Hopfengasse 15. A-4020 Linz
		Web:			https://www.gaeckler.at/

		Copyright:		(c) 2014-2018 Martin Gäckler

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

#ifndef SYNTH_MIDI_H
#define SYNTH_MIDI_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

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

// status codes
// -----------------------------------
// channel messages
// -----------------------------------
#define MIDI_NOTE_OFF			0x8			// note + velocity
#define MIDI_NOTE_ON			0x9			// note + volume
#define MIDI_POLYPHONE			0xA			// note + pressure
#define MIDI_CONTROLLER			0xB			// controller + value
#define MIDI_PROGRAM			0xC			// patch number
#define MIDI_CHANNEL			0xD			// pressure
#define MIDI_PITCH				0xE			// value LSB + MSB
#define MIDI_SYSTEM				0xF
// -----------------------------------
// system common messages
// -----------------------------------
#define MIDI_SYS_EX_START		0xF0		// + long data
#define MIDI_TIME_CODE			0xF1		// data 1
#define MIDI_SONG_POSITION		0xF2		// data 1 + data 2
#define MIDI_SONG_SELECT		0xF3		// data 1
#define MIDI_SYS_UNDEF4			0xF4
#define MIDI_SYS_UNDEF5			0xF5
#define MIDI_TUNE_REQUEST		0xF6
#define MIDI_SYS_EX_END			0xF7
// -----------------------------------
// real time messages
// -----------------------------------
#define MIDI_TIMING				0xF8
#define MIDI_SYS_UNDEF9			0xF9
#define MIDI_START				0xFA
#define MIDI_CONTINUE			0xFB
#define MIDI_STOP				0xFC
#define MIDI_SYS_UNDEFD			0xFD
#define MIDI_ACTIVE_SENSE		0xFE
#define MIDI_RESET				0xFF

// -----------------------------------

// controller message (data1 && status == MIDI_CONTROLLER)
// data2 is the controller message value
// -------------------------------------------------------
#define MIDI_BANK_MSB			0x00
#define MIDI_VOLUME				0x07
#define MIDI_BALANCE_POSITION	0x08
#define MIDI_PAN_POSITION		0x0A
#define MIDI_EXPRESSION			0x0B
#define MIDI_BANK_LSB			0x20
#define MIDI_SOUND_CONTROLLER	0x46	// - 0x4F
#define MIDI_EFFECT				0x5B	// - 0x5F
#define MIDI_SOUND_OFF			0x78
#define MIDI_LOCAL_ON_OFF		0x7A
#define MIDI_NOTES_OFF			0x7B
// -------------------------------------------------------

// controler message values for MIDI_STEREO_POSITION
// -------------------------------------------------------
#define MIDI_CENTER_PAN			(64)
#define MIDI_LEFT_POSITION		(-MIDI_CENTER_PAN)
#define MIDI_RIGHT_POSITION		(MIDI_LEFT_POSITION + 127)
// -------------------------------------------------------
#define MIDI_NEUTRAL_PITCH_MSB	64
#define MIDI_NEUTRAL_PITCH		(MIDI_NEUTRAL_PITCH_MSB<<7)

// NOTES
#define MIDI_NOTE_C1			36
#define MIDI_NOTE_D1			38
#define MIDI_NOTE_E1			40
#define MIDI_NOTE_F1			41
#define MIDI_NOTE_G1			43
#define MIDI_NOTE_A1			45
#define MIDI_NOTE_H1			47
#define MIDI_NOTE_C2			48
#define MIDI_NOTE_D2			50
#define MIDI_NOTE_E2			52
#define MIDI_NOTE_F2			53
#define MIDI_NOTE_G2			55
#define MIDI_NOTE_A2			57
#define MIDI_NOTE_H2			59
#define MIDI_NOTE_C3			60
#define MIDI_NOTE_D3			62
#define MIDI_NOTE_E3			64
#define MIDI_NOTE_F3			65
#define MIDI_NOTE_G3			67
#define MIDI_NOTE_A3			69
#define MIDI_NOTE_H3			71
#define MIDI_NOTE_C4			72
#define MIDI_NOTE_D4			74
#define MIDI_NOTE_E4			76
#define MIDI_NOTE_F4			77
#define MIDI_NOTE_G4			79
#define MIDI_NOTE_A4			81
#define MIDI_NOTE_H4			83

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

#endif	// SYNTH_MIDI_H