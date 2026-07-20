/*
		Project:		MIDI Tools
		Module:			MidiFilterWindow.cpp
		Description:	The filter settings (used by recorder window)
		Author:			Martin Gäckler
		Address:		Hofmannsthalweg 14, A-4030 Linz
		Web:			https://www.gaeckler.at/

		Copyright:		(c) 2007-2026 Martin Gäckler

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

#include <fstream>

#include <gak/numericString.h>
#include <gak/csv.h>
#include <gak/fmtNumber.h>

#include <winlib/winapp.h>

#include "MIDIfilterWindow.h"
#include "MIDIrecorderWindow.h"

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

void MIDIfilterWindow::moveFilter()
{
	int	selectedId = filterListBox->getSelection();

	if( selectedId > 0 && selectedId < (int)m_theFilterList->size() )
	{
		FilterSettings	tmp = (*m_theFilterList)[selectedId-1];
		(*m_theFilterList)[selectedId-1] = (*m_theFilterList)[selectedId];
		(*m_theFilterList)[selectedId] = tmp;

		refreshFilter(selectedId-1);

		m_theRecorderWindow->setSettingsChanged();
	}
}

void MIDIfilterWindow::showValues( size_t selectedId )
{
	const FilterSettings &theSettings = (*m_theFilterList)[selectedId];
	filterNameEdit->setText( theSettings.name );
	MIDIrecSelect->selectEntry( theSettings.midiInName );
	filterChannelSelect->selectEntry( theSettings.filterChannel );
	conditionSelect->selectEntry( theSettings.condition );
	filterNote->setPosition( theSettings.note );

	MIDIplaySelect->selectEntry( theSettings.midiOutName );
	actionChannelSelect->selectEntry( theSettings.actionChannel );

	noteOffsetEdit->setText( formatNumber( theSettings.noteOffset ) );
	newVolumeEdit->setText( theSettings.newVolume );

	if( theSettings.stopFlag )
	{
		stopFlag->setActive();
	}
	else
	{
		stopFlag->clrActive();
	}
}

void MIDIfilterWindow::deleteFilter()
{
	int	selectedId = filterListBox->getSelection();
	if( selectedId >= 0 && selectedId < int(m_theFilterList->size()) )
	{
		m_theFilterList->removeElementAt( selectedId );
		filterListBox->deleteEntry( selectedId );
		if( selectedId >= int(m_theFilterList->size()) )
		{
			selectedId = int(m_theFilterList->size())-1;
		}
		filterListBox->selectEntry( selectedId );
		if( selectedId >= 0 )
		{
			showValues( selectedId );
		}

		m_theRecorderWindow->setSettingsChanged();
	}
}

void MIDIfilterWindow::createNewFilter()
{
	size_t	i = m_theFilterList->size();
	STRING	newName = newButton->getText();

	if( i )
	{
		newName += formatNumber( i );
	}

	FilterSettings	&theSettings = m_theFilterList->createElement();
	filterListBox->addEntry( newName );
	filterListBox->selectEntry( int(i) );

	theSettings.name = newName;
	filterNameEdit->setText( newName );

	theSettings.filterChannel = static_cast<unsigned char>(filterChannelSelect->getSelection());
	theSettings.midiInName = MIDIrecSelect->getSelectedText();
	theSettings.midiInDev = MIDIrecSelect->getSelection();
	if( theSettings.midiInDev >= int(m_midiInCount-1) )
	{
		theSettings.midiInDev = ALL_DEVICE;
	}

	theSettings.condition = FilterCondition(conditionSelect->getSelection());
	theSettings.note = static_cast<unsigned char>(filterNote->getPosition());

	theSettings.actionChannel = static_cast<unsigned char>(actionChannelSelect->getSelection());
	theSettings.midiOutName = MIDIplaySelect->getSelectedText();
	if( theSettings.midiOutName == CONTROLLER_LABEL )
	{
		theSettings.midiOutDev = CONTROLLER_DEVICE;
	}
	else if( theSettings.midiOutName == DEFAULT_LABEL )
	{
		theSettings.midiOutDev = DEFAULT_DEVICE;
	}
	else
	{
		theSettings.midiOutDev = MIDIplaySelect->getSelection();
	}
	theSettings.noteOffset = noteOffsetEdit->getText().getValueN<signed char>();
	theSettings.newVolume = newVolumeEdit->getText();
	theSettings.autoLevel = 0.0;

	theSettings.stopFlag = stopFlag->isActive();

	m_theRecorderWindow->setSettingsChanged();
}

void MIDIfilterWindow::createStandardFilter( const STRING &newName, unsigned char channel, FilterCondition condition )
{
	bool			filterFound = false;

	static STRING	allRecorderLabel;

	if( allRecorderLabel.isEmpty() )
	{
		allRecorderLabel = appObject->loadString( winlibGUI::ALL_MIDI_RECORD_LABEL_id );
	}

	for(
		FilterList::const_iterator it = m_theFilterList->cbegin(),
			endIT = m_theFilterList->cend();
		it != endIT;
		++it
	)
	{
		if( it->name == newName )
		{
			filterFound = true;
			break;
		}
	}

	if( !filterFound )
	{
		FilterSettings	&theSettings = m_theFilterList->createElement();
		filterListBox->addEntry( newName );

		theSettings.name = newName;

		theSettings.filterChannel = channel;
		theSettings.condition = condition;
		theSettings.note = 0;

		theSettings.actionChannel = channel;
		theSettings.noteOffset = 0;
		theSettings.stopFlag = true;
		theSettings.newVolume = newVolumeEdit->getText();
		theSettings.autoLevel = 0.0;

		theSettings.midiInName = allRecorderLabel;
		theSettings.midiInDev = ALL_DEVICE;
		theSettings.midiOutName = CONTROLLER_LABEL;
		theSettings.midiOutDev = CONTROLLER_DEVICE;

		m_theRecorderWindow->setSettingsChanged();
	}
}

void MIDIfilterWindow::createStandardFilter()
{
	static STRING	channelLabel;

	unsigned char	channel;
	STRING			newName;

	if( channelLabel.isEmpty() )
	{
		channelLabel = appObject->loadString( winlibGUI::CHANNEL_LABEL_id );
	}

	for( channel=0; channel<=15; channel++ )
	{
		newName = channelLabel;
		newName += formatNumber(channel+1);

		createStandardFilter( newName, channel, GREATER_EQUAL );

		newName = "Control";
		newName += formatNumber(channel+1);

		createStandardFilter( newName, channel, CONTROL );
	}
}

// --------------------------------------------------------------------- //
// ----- class protected ----------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class virtuals ------------------------------------------------ //
// --------------------------------------------------------------------- //

ProcessStatus MIDIfilterWindow::handleCreate()
{
	m_lastSelectedFilter = size_t(-1);
	ChannelSelect::fill( filterChannelSelect );
	filterNote->setRange( 0, 127 );

	ChannelSelect::fill( actionChannelSelect );

	filterChannelSelect->addEntry( appObject->loadString( winlibGUI::ANY_CHANNEL_LABEL_id ) );
	actionChannelSelect->addEntry( CONTROLLER_LABEL );

	filterChannelSelect->selectEntry( 16 );
	conditionSelect->selectEntry( 3 );
	actionChannelSelect->selectEntry( 16 );

	m_midiInCount = recorderHandles.size();
	for( 
		MIDIrecorder::const_iterator it = recorderHandles.cbegin(),
			endIT = recorderHandles.cend();
		it != endIT;
		++it
	)
	{
		MIDIrecSelect->addEntry( it->getInstrument() );
	}
	MIDIrecSelect->addEntry( appObject->loadString( winlibGUI::ALL_MIDI_RECORD_LABEL_id ) );
	MIDIrecSelect->selectEntry( int(m_midiInCount) );

	m_midiOutCount = playerHandles.size();
	for( 
		MIDIplayer::const_iterator it = playerHandles.cbegin(),
			endIT = playerHandles.cend();
		it != endIT;
		++it
	)
	{
		MIDIplaySelect->addEntry( it->getInstrument() );
	}
	MIDIplaySelect->addEntry( CONTROLLER_LABEL );
	MIDIplaySelect->addEntry( appObject->loadString( winlibGUI::DEFAULT_MIDI_PLAY_LABEL_id ) );
	MIDIplaySelect->selectEntry( int(m_midiOutCount) );

	refreshFilter( 0 );

	return psDO_DEFAULT;
}

ProcessStatus MIDIfilterWindow::handleScrollControl( int control )
{
	if( control == winlibGUI::filterNote_id )
	{
		int	selectedId = filterListBox->getSelection();
		if( selectedId >= 0 && selectedId < (int)m_theFilterList->size() )
		{
			FilterSettings	&theSettings = (*m_theFilterList)[selectedId];
			theSettings.note = static_cast<unsigned char>(filterNote->getPosition());

			m_theRecorderWindow->setSettingsChanged();
		}
		return psPROCESSED;
	}
	else
/***/	return MIDIfilterWindow_form::handleScrollControl( control );
}

ProcessStatus MIDIfilterWindow::handleSelectionChange( int control )
{
	int	selectedId = filterListBox->getSelection();
	if( selectedId >= 0 && selectedId < int(m_theFilterList->size()) )
	{
		FilterSettings	&theSettings = (*m_theFilterList)[selectedId];
		switch( control )
		{
			case winlibGUI::filterListBox_id:
			{
				showValues( selectedId );
				break;
			}
			case winlibGUI::filterChannelSelect_id:
			{
				theSettings.filterChannel = static_cast<unsigned char>(filterChannelSelect->getSelection());
				m_theRecorderWindow->setSettingsChanged();
				break;
			}
			case winlibGUI::conditionSelect_id:
			{
				theSettings.condition = FilterCondition(conditionSelect->getSelection());
				m_theRecorderWindow->setSettingsChanged();
				break;
			}
			case winlibGUI::actionChannelSelect_id:
			{
				theSettings.actionChannel = static_cast<unsigned char>(actionChannelSelect->getSelection());
				m_theRecorderWindow->setSettingsChanged();
				break;
			}
			case winlibGUI::MIDIrecSelect_id:
			{
				theSettings.midiInName = MIDIrecSelect->getSelectedText();
				theSettings.midiInDev = MIDIrecSelect->getSelection();
				if( theSettings.midiInDev >= int(m_midiInCount-1) )
				{
					theSettings.midiInDev = ALL_DEVICE;
				}

				m_theRecorderWindow->setSettingsChanged();
				break;
			}
			case winlibGUI::MIDIplaySelect_id:
			{
				theSettings.midiOutName = MIDIplaySelect->getSelectedText();
				if( theSettings.midiOutName == CONTROLLER_LABEL )
					theSettings.midiOutDev = CONTROLLER_DEVICE;
				else if( theSettings.midiOutName == DEFAULT_LABEL )
					theSettings.midiOutDev = DEFAULT_DEVICE;
				else
					theSettings.midiOutDev = MIDIplaySelect->getSelection();

				m_theRecorderWindow->setSettingsChanged();
				break;
			}
			default:
/***/			return MIDIfilterWindow_form::handleSelectionChange( control );
		}
	}

	return psPROCESSED;
}

ProcessStatus MIDIfilterWindow::handleButtonClick( int control )
{
	if( control == winlibGUI::stopFlag_id )
	{
		int	selectedId = filterListBox->getSelection();
		if( selectedId >= 0 && selectedId < int(m_theFilterList->size()) )
		{
			FilterSettings	&theSettings = (*m_theFilterList)[selectedId];
			theSettings.stopFlag = stopFlag->isActive();

			m_theRecorderWindow->setSettingsChanged();
		}
		return psPROCESSED;
	}
	else
	{
/***/	return MIDIfilterWindow_form::handleButtonClick( control );
	}

}


ProcessStatus MIDIfilterWindow::handleEditChange( int control )
{
	int	selectedId = filterListBox->getSelection();
	if( selectedId >= 0 && selectedId < (int)m_theFilterList->size() )
	{
		FilterSettings	&theSettings = (*m_theFilterList)[selectedId];
		switch( control )
		{
			case winlibGUI::filterNameEdit_id:
			{
				STRING			newName = filterNameEdit->getText();
				if( newName != theSettings.name )
				{
					theSettings.name = newName;
					filterListBox->replaceEntry( selectedId, newName );
					filterListBox->selectEntry( selectedId );
					m_theRecorderWindow->setSettingsChanged();
				}
				break;
			}
			case winlibGUI::noteOffsetEdit_id:
			{
				signed char noteOffset = noteOffsetEdit->getText().getValueN<signed char>();
				if( theSettings.noteOffset != noteOffset )
				{
					theSettings.noteOffset = noteOffset;
					m_theRecorderWindow->setSettingsChanged();
				}
				break;
			}
			case winlibGUI::newVolumeEdit_id:
			{
				STRING newVolume = newVolumeEdit->getText();
				if( theSettings.newVolume != newVolume )
				{
					theSettings.newVolume = newVolume;
					theSettings.autoLevel = 0.0;
					m_theRecorderWindow->setSettingsChanged();
				}
				break;
			}
			default:
/***/			return MIDIfilterWindow_form::handleEditChange( control );
		}
	}

	return psPROCESSED;
}

ProcessStatus MIDIfilterWindow::handleCommand( int cmd )
{
	switch( cmd )
	{
		case winlibGUI::upButton_id:
			moveFilter();
			break;

		case winlibGUI::newButton_id:
			createNewFilter();
			break;

		case winlibGUI::deleteButton_id:
			deleteFilter();
			break;

		case winlibGUI::standardsButton_id:
			createStandardFilter();
			break;

		default:
			return MIDIfilterWindow_form::handleCommand( cmd );
	}
	return psPROCESSED;
}

SuccessCode MIDIfilterWindow::handleClose()
{
	hide();
	return scERROR;
}

ProcessStatus MIDIfilterWindow::handleCancel()
{
	hide();
	return psPROCESSED;
}

// --------------------------------------------------------------------- //
// ----- class publics ------------------------------------------------- //
// --------------------------------------------------------------------- //

void MIDIfilterWindow::refreshFilter( size_t newSelected )
{
	filterListBox->clearEntries();
	if( m_theFilterList->size() )
	{
		for( 
			FilterList::const_iterator it = m_theFilterList->cbegin(),
				endIT = m_theFilterList->cend();
			it != endIT;
			++it
		)
		{
			filterListBox->addEntry( it->name );
		}

		filterListBox->selectEntry( int(newSelected) );
		showValues( newSelected );
	}
}

// --------------------------------------------------------------------- //
// ----- entry points -------------------------------------------------- //
// --------------------------------------------------------------------- //


