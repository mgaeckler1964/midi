/*
	this file was created from C:\CRESD\Source\MIDI\Tools\midi.gui with MG Gui Builder
	+++++++++++++++++++++++
	!!!!! Do not Edit !!!!!
	+++++++++++++++++++++++
*/

#include "C:\CRESD\Source\MIDI\Tools\midi.gui.h"

namespace winlibGUI {

	void ToolBarWindow_form::getControls() {
		recorderButton=static_cast<winlib::PushButton*>(findChild(recorderButton_id));
		drumButton=static_cast<winlib::PushButton*>(findChild(drumButton_id));
		loopButton=static_cast<winlib::PushButton*>(findChild(loopButton_id));
		synthesizerButton=static_cast<winlib::PushButton*>(findChild(synthesizerButton_id));
		midiDevicesButton=static_cast<winlib::PushButton*>(findChild(midiDevicesButton_id));
		IDCANCEL_PushButton=static_cast<winlib::PushButton*>(findChild(IDCANCEL));
		languageCheckBox=static_cast<winlib::CheckBox*>(findChild(languageCheckBox_id));
	}	// ToolBarWindow

	void MIDIloopEditor_form::getControls() {
		playButton=static_cast<winlib::PushButton*>(findChild(playButton_id));
		clockText=static_cast<winlib::Label*>(findChild(clockText_id));
		bpmEdit=static_cast<winlib::EditControl*>(findChild(bpmEdit_id));
		playSelect=static_cast<winlib::ComboBox*>(findChild(playSelect_id));
		topFrame=static_cast<winlib::FrameChild*>(findChild(topFrame_id));
		IDCANCEL_PushButton=static_cast<winlib::PushButton*>(findChild(IDCANCEL));
		loadLoopButton=static_cast<winlib::PushButton*>(findChild(loadLoopButton_id));
		saveLoopButton=static_cast<winlib::PushButton*>(findChild(saveLoopButton_id));
		bottomFrame=static_cast<winlib::FrameChild*>(findChild(bottomFrame_id));
		longerButton=static_cast<winlib::PushButton*>(findChild(longerButton_id));
		shorterButton=static_cast<winlib::PushButton*>(findChild(shorterButton_id));
		upButton=static_cast<winlib::PushButton*>(findChild(upButton_id));
		downButton=static_cast<winlib::PushButton*>(findChild(downButton_id));
		rightFrame=static_cast<winlib::FrameChild*>(findChild(rightFrame_id));
		midiListBox=static_cast<winlib::ListBox*>(findChild(midiListBox_id));
	}	// MIDIloopEditor

	void PatternVolume_form::getControls() {
		volumeLabel=static_cast<winlib::Label*>(findChild(volumeLabel_id));
		patternSelect=static_cast<winlib::ComboBox*>(findChild(patternSelect_id));
		volumeBar=static_cast<winlib::TrackBar*>(findChild(volumeBar_id));
		IDOK_PushButton=static_cast<winlib::PushButton*>(findChild(IDOK));
		IDCANCEL_PushButton=static_cast<winlib::PushButton*>(findChild(IDCANCEL));
	}	// PatternVolume

	void ChannelVolume_form::getControls() {
		volumeLabel=static_cast<winlib::Label*>(findChild(volumeLabel_id));
		channelSelect=static_cast<winlib::ComboBox*>(findChild(channelSelect_id));
		volumeBar=static_cast<winlib::TrackBar*>(findChild(volumeBar_id));
		IDOK_PushButton=static_cast<winlib::PushButton*>(findChild(IDOK));
		IDCANCEL_PushButton=static_cast<winlib::PushButton*>(findChild(IDCANCEL));
	}	// ChannelVolume

	void PatternStereo_form::getControls() {
		stereoLabel=static_cast<winlib::Label*>(findChild(stereoLabel_id));
		patternSelect=static_cast<winlib::ComboBox*>(findChild(patternSelect_id));
		stereoBar=static_cast<winlib::TrackBar*>(findChild(stereoBar_id));
		IDOK_PushButton=static_cast<winlib::PushButton*>(findChild(IDOK));
		IDCANCEL_PushButton=static_cast<winlib::PushButton*>(findChild(IDCANCEL));
	}	// PatternStereo

	void SwapChannel_form::getControls() {
		srcChannelSelect=static_cast<winlib::ComboBox*>(findChild(srcChannelSelect_id));
		destChannelSelect=static_cast<winlib::ComboBox*>(findChild(destChannelSelect_id));
		IDOK_PushButton=static_cast<winlib::PushButton*>(findChild(IDOK));
		IDCANCEL_PushButton=static_cast<winlib::PushButton*>(findChild(IDCANCEL));
	}	// SwapChannel

	void MetronomWindow_form::getControls() {
		firstVoiceSelect=static_cast<winlib::ComboBox*>(findChild(firstVoiceSelect_id));
		otherVoiceSelect=static_cast<winlib::ComboBox*>(findChild(otherVoiceSelect_id));
	}	// MetronomWindow

	void MIDIdeviceDialog_form::getControls() {
		IDOK_PushButton=static_cast<winlib::PushButton*>(findChild(IDOK));
		IDCANCEL_PushButton=static_cast<winlib::PushButton*>(findChild(IDCANCEL));
		theDeviceTable=static_cast<winlib::FrameChild*>(findChild(theDeviceTable_id));
	}	// MIDIdeviceDialog

	void MIDIfilterWindow_form::getControls() {
		filterListBox=static_cast<winlib::ListBox*>(findChild(filterListBox_id));
		newButton=static_cast<winlib::PushButton*>(findChild(newButton_id));
		deleteButton=static_cast<winlib::PushButton*>(findChild(deleteButton_id));
		upButton=static_cast<winlib::PushButton*>(findChild(upButton_id));
		IDCANCEL_PushButton=static_cast<winlib::PushButton*>(findChild(IDCANCEL));
		standardsButton=static_cast<winlib::PushButton*>(findChild(standardsButton_id));
		filterNameEdit=static_cast<winlib::EditControl*>(findChild(filterNameEdit_id));
		MIDIrecSelect=static_cast<winlib::ComboBox*>(findChild(MIDIrecSelect_id));
		filterChannelSelect=static_cast<winlib::ComboBox*>(findChild(filterChannelSelect_id));
		conditionSelect=static_cast<winlib::ComboBox*>(findChild(conditionSelect_id));
		filterNote=static_cast<winlib::UpDownButton*>(findChild(filterNote_id));
		MIDIplaySelect=static_cast<winlib::ComboBox*>(findChild(MIDIplaySelect_id));
		actionChannelSelect=static_cast<winlib::ComboBox*>(findChild(actionChannelSelect_id));
		noteOffsetEdit=static_cast<winlib::EditControl*>(findChild(noteOffsetEdit_id));
		stopFlag=static_cast<winlib::CheckBox*>(findChild(stopFlag_id));
		newVolumeEdit=static_cast<winlib::EditControl*>(findChild(newVolumeEdit_id));
	}	// MIDIfilterWindow

	void MIDIchannelsWindow_form::getControls() {
		IDCANCEL_PushButton=static_cast<winlib::PushButton*>(findChild(IDCANCEL));
		defaultButton=static_cast<winlib::PushButton*>(findChild(defaultButton_id));
		activeCheckBox=static_cast<winlib::CheckBox*>(findChild(activeCheckBox_id));
		valueText=static_cast<winlib::Label*>(findChild(valueText_id));
		channelSelect=static_cast<winlib::ComboBox*>(findChild(channelSelect_id));
		deviceLabel=static_cast<winlib::Label*>(findChild(deviceLabel_id));
		groupSelect=static_cast<winlib::ComboBox*>(findChild(groupSelect_id));
		voiceSelect=static_cast<winlib::ComboBox*>(findChild(voiceSelect_id));
		stereoBar=static_cast<winlib::TrackBar*>(findChild(stereoBar_id));
		volumeBar=static_cast<winlib::TrackBar*>(findChild(volumeBar_id));
		effects1Bar=static_cast<winlib::TrackBar*>(findChild(effects1Bar_id));
		effects2Bar=static_cast<winlib::TrackBar*>(findChild(effects2Bar_id));
		effects3Bar=static_cast<winlib::TrackBar*>(findChild(effects3Bar_id));
		effects4Bar=static_cast<winlib::TrackBar*>(findChild(effects4Bar_id));
		effects5Bar=static_cast<winlib::TrackBar*>(findChild(effects5Bar_id));
		sound1Bar=static_cast<winlib::TrackBar*>(findChild(sound1Bar_id));
		sound2Bar=static_cast<winlib::TrackBar*>(findChild(sound2Bar_id));
		sound3Bar=static_cast<winlib::TrackBar*>(findChild(sound3Bar_id));
		sound4Bar=static_cast<winlib::TrackBar*>(findChild(sound4Bar_id));
		sound5Bar=static_cast<winlib::TrackBar*>(findChild(sound5Bar_id));
		expressionBar=static_cast<winlib::TrackBar*>(findChild(expressionBar_id));
	}	// MIDIchannelsWindow

	void MIDIeditorWindow_form::getControls() {
		eventList=static_cast<winlib::ListBox*>(findChild(eventList_id));
		channelSelect=static_cast<winlib::ComboBox*>(findChild(channelSelect_id));
		trackSelect=static_cast<winlib::ComboBox*>(findChild(trackSelect_id));
		timeCodeEdit=static_cast<winlib::EditControl*>(findChild(timeCodeEdit_id));
		voiceRadio=static_cast<winlib::RadioButton*>(findChild(voiceRadio_id));
		groupSelect=static_cast<winlib::ComboBox*>(findChild(groupSelect_id));
		voiceSelect=static_cast<winlib::ComboBox*>(findChild(voiceSelect_id));
		noteRadio=static_cast<winlib::RadioButton*>(findChild(noteRadio_id));
		noteSelect=static_cast<winlib::ComboBox*>(findChild(noteSelect_id));
		noteVolumeEdit=static_cast<winlib::EditControl*>(findChild(noteVolumeEdit_id));
		noteLengthEdit=static_cast<winlib::EditControl*>(findChild(noteLengthEdit_id));
		noteTimeEdit=static_cast<winlib::EditControl*>(findChild(noteTimeEdit_id));
		full=static_cast<winlib::PushButton*>(findChild(full_id));
		half=static_cast<winlib::PushButton*>(findChild(half_id));
		quarter=static_cast<winlib::PushButton*>(findChild(quarter_id));
		eighth=static_cast<winlib::PushButton*>(findChild(eighth_id));
		controllerRadio=static_cast<winlib::RadioButton*>(findChild(controllerRadio_id));
		controllerSelect=static_cast<winlib::ComboBox*>(findChild(controllerSelect_id));
		controllerEdit=static_cast<winlib::EditControl*>(findChild(controllerEdit_id));
		otherRadio=static_cast<winlib::RadioButton*>(findChild(otherRadio_id));
		messageSelect=static_cast<winlib::ComboBox*>(findChild(messageSelect_id));
		data1Edit=static_cast<winlib::EditControl*>(findChild(data1Edit_id));
		data2Edit=static_cast<winlib::EditControl*>(findChild(data2Edit_id));
		sysExRadio=static_cast<winlib::RadioButton*>(findChild(sysExRadio_id));
		saveEntryButton=static_cast<winlib::PushButton*>(findChild(saveEntryButton_id));
		newEntryButton=static_cast<winlib::PushButton*>(findChild(newEntryButton_id));
		deleteEntryButton=static_cast<winlib::PushButton*>(findChild(deleteEntryButton_id));
		instrumentButton=static_cast<winlib::PushButton*>(findChild(instrumentButton_id));
		volumeButton=static_cast<winlib::PushButton*>(findChild(volumeButton_id));
		swapChannelsButton=static_cast<winlib::PushButton*>(findChild(swapChannelsButton_id));
		repeatButton=static_cast<winlib::PushButton*>(findChild(repeatButton_id));
		transposeEdit=static_cast<winlib::EditControl*>(findChild(transposeEdit_id));
		transposeButton=static_cast<winlib::PushButton*>(findChild(transposeButton_id));
		moveEdit=static_cast<winlib::EditControl*>(findChild(moveEdit_id));
		moveButton=static_cast<winlib::PushButton*>(findChild(moveButton_id));
		copyButton=static_cast<winlib::PushButton*>(findChild(copyButton_id));
		IDOK_PushButton=static_cast<winlib::PushButton*>(findChild(IDOK));
		IDCANCEL_PushButton=static_cast<winlib::PushButton*>(findChild(IDCANCEL));
		playButton=static_cast<winlib::PushButton*>(findChild(playButton_id));
	}	// MIDIeditorWindow

	void MIDIrecorderWindow_form::getControls() {
		pianoFrame=static_cast<winlib::FrameChild*>(findChild(pianoFrame_id));
		MIDIthrough=static_cast<winlib::CheckBox*>(findChild(MIDIthrough_id));
		filterEnabledCheckBox=static_cast<winlib::CheckBox*>(findChild(filterEnabledCheckBox_id));
		channelsButton=static_cast<winlib::PushButton*>(findChild(channelsButton_id));
		filterButton=static_cast<winlib::PushButton*>(findChild(filterButton_id));
		loadSettingButton=static_cast<winlib::PushButton*>(findChild(loadSettingButton_id));
		saveSettingButton=static_cast<winlib::PushButton*>(findChild(saveSettingButton_id));
		MIDIholdNote=static_cast<winlib::CheckBox*>(findChild(MIDIholdNote_id));
		MIDIplaySelect=static_cast<winlib::ComboBox*>(findChild(MIDIplaySelect_id));
		playButton=static_cast<winlib::PushButton*>(findChild(playButton_id));
		MIDIrecSelect=static_cast<winlib::ComboBox*>(findChild(MIDIrecSelect_id));
		recordButton=static_cast<winlib::PushButton*>(findChild(recordButton_id));
		authoRhytmCheck=static_cast<winlib::CheckBox*>(findChild(authoRhytmCheck_id));
		bpmEdit=static_cast<winlib::EditControl*>(findChild(bpmEdit_id));
		numeratorEdit=static_cast<winlib::EditControl*>(findChild(numeratorEdit_id));
		denominatorSelect=static_cast<winlib::ComboBox*>(findChild(denominatorSelect_id));
		convertBPMbutton=static_cast<winlib::PushButton*>(findChild(convertBPMbutton_id));
		metronomButton=static_cast<winlib::PushButton*>(findChild(metronomButton_id));
		IDCANCEL_PushButton=static_cast<winlib::PushButton*>(findChild(IDCANCEL));
		loadMidiButton=static_cast<winlib::PushButton*>(findChild(loadMidiButton_id));
		saveMidiButton=static_cast<winlib::PushButton*>(findChild(saveMidiButton_id));
		editButton=static_cast<winlib::PushButton*>(findChild(editButton_id));
		instrumentButton=static_cast<winlib::PushButton*>(findChild(instrumentButton_id));
	}	// MIDIrecorderWindow

	void MIDIdrumWindow_form::getControls() {
		MIDIplaySelect=static_cast<winlib::ComboBox*>(findChild(MIDIplaySelect_id));
		playButton=static_cast<winlib::PushButton*>(findChild(playButton_id));
		bpmEdit=static_cast<winlib::EditControl*>(findChild(bpmEdit_id));
		numeratorEdit=static_cast<winlib::EditControl*>(findChild(numeratorEdit_id));
		denominatorSelect=static_cast<winlib::ComboBox*>(findChild(denominatorSelect_id));
		lengthEdit=static_cast<winlib::EditControl*>(findChild(lengthEdit_id));
		lengthSelector=static_cast<winlib::UpDownButton*>(findChild(lengthSelector_id));
		convertBPMbutton=static_cast<winlib::PushButton*>(findChild(convertBPMbutton_id));
		gridSelect=static_cast<winlib::ComboBox*>(findChild(gridSelect_id));
		lengthSelect=static_cast<winlib::ComboBox*>(findChild(lengthSelect_id));
		clockText=static_cast<winlib::Label*>(findChild(clockText_id));
		voiceSelect=static_cast<winlib::ComboBox*>(findChild(voiceSelect_id));
		addNewPatternButton=static_cast<winlib::PushButton*>(findChild(addNewPatternButton_id));
		playModeSelect=static_cast<winlib::ComboBox*>(findChild(playModeSelect_id));
		volumeButton=static_cast<winlib::PushButton*>(findChild(volumeButton_id));
		panButton=static_cast<winlib::PushButton*>(findChild(panButton_id));
		deleteEventsButton=static_cast<winlib::PushButton*>(findChild(deleteEventsButton_id));
		copyEventsButton=static_cast<winlib::PushButton*>(findChild(copyEventsButton_id));
		moveEventsButton=static_cast<winlib::PushButton*>(findChild(moveEventsButton_id));
		insertBreakButton=static_cast<winlib::PushButton*>(findChild(insertBreakButton_id));
		deleteAllButton=static_cast<winlib::PushButton*>(findChild(deleteAllButton_id));
		patternFrame=static_cast<winlib::FrameChild*>(findChild(patternFrame_id));
		volumeBar=static_cast<winlib::TrackBar*>(findChild(volumeBar_id));
		timeBar=static_cast<winlib::TrackBar*>(findChild(timeBar_id));
		IDCANCEL_PushButton=static_cast<winlib::PushButton*>(findChild(IDCANCEL));
		loadDrumsButton=static_cast<winlib::PushButton*>(findChild(loadDrumsButton_id));
		saveDrumsButton=static_cast<winlib::PushButton*>(findChild(saveDrumsButton_id));
	}	// MIDIdrumWindow

	void MIDIsynthesizerWindow_form::getControls() {
		radioSynthA=static_cast<winlib::RadioButton*>(findChild(radioSynthA_id));
		radioSynthB=static_cast<winlib::RadioButton*>(findChild(radioSynthB_id));
		radioSynthC=static_cast<winlib::RadioButton*>(findChild(radioSynthC_id));
		radioSynthD=static_cast<winlib::RadioButton*>(findChild(radioSynthD_id));
		checkMuteA=static_cast<winlib::CheckBox*>(findChild(checkMuteA_id));
		checkMuteB=static_cast<winlib::CheckBox*>(findChild(checkMuteB_id));
		checkMuteC=static_cast<winlib::CheckBox*>(findChild(checkMuteC_id));
		checkMuteD=static_cast<winlib::CheckBox*>(findChild(checkMuteD_id));
		masterVolume=static_cast<winlib::TrackBar*>(findChild(masterVolume_id));
		presenceValue=static_cast<winlib::TrackBar*>(findChild(presenceValue_id));
		mainVolume=static_cast<winlib::TrackBar*>(findChild(mainVolume_id));
		mainPan=static_cast<winlib::TrackBar*>(findChild(mainPan_id));
		mainPitchValue=static_cast<winlib::Label*>(findChild(mainPitchValue_id));
		mainPitch=static_cast<winlib::TrackBar*>(findChild(mainPitch_id));
		mainPhase=static_cast<winlib::TrackBar*>(findChild(mainPhase_id));
		sub1phase=static_cast<winlib::TrackBar*>(findChild(sub1phase_id));
		sub1volume=static_cast<winlib::TrackBar*>(findChild(sub1volume_id));
		sub1pitch=static_cast<winlib::TrackBar*>(findChild(sub1pitch_id));
		sub2phase=static_cast<winlib::TrackBar*>(findChild(sub2phase_id));
		sub2volume=static_cast<winlib::TrackBar*>(findChild(sub2volume_id));
		sub2pitch=static_cast<winlib::TrackBar*>(findChild(sub2pitch_id));
		amSinus=static_cast<winlib::RadioButton*>(findChild(amSinus_id));
		amAscending=static_cast<winlib::RadioButton*>(findChild(amAscending_id));
		amDescending=static_cast<winlib::RadioButton*>(findChild(amDescending_id));
		amTriangle=static_cast<winlib::RadioButton*>(findChild(amTriangle_id));
		amRectangle=static_cast<winlib::RadioButton*>(findChild(amRectangle_id));
		amFrequency=static_cast<winlib::EditControl*>(findChild(amFrequency_id));
		amRange=static_cast<winlib::TrackBar*>(findChild(amRange_id));
		fmSinus=static_cast<winlib::RadioButton*>(findChild(fmSinus_id));
		fmAscending=static_cast<winlib::RadioButton*>(findChild(fmAscending_id));
		fmDescending=static_cast<winlib::RadioButton*>(findChild(fmDescending_id));
		fmTriangle=static_cast<winlib::RadioButton*>(findChild(fmTriangle_id));
		fmRectangle=static_cast<winlib::RadioButton*>(findChild(fmRectangle_id));
		fmFrequency=static_cast<winlib::EditControl*>(findChild(fmFrequency_id));
		fmRange=static_cast<winlib::TrackBar*>(findChild(fmRange_id));
		envEnabled=static_cast<winlib::CheckBox*>(findChild(envEnabled_id));
		envDelay=static_cast<winlib::EditControl*>(findChild(envDelay_id));
		envVolume1=static_cast<winlib::TrackBar*>(findChild(envVolume1_id));
		envSustainRate=static_cast<winlib::EditControl*>(findChild(envSustainRate_id));
		envAttack=static_cast<winlib::EditControl*>(findChild(envAttack_id));
		envDecay=static_cast<winlib::EditControl*>(findChild(envDecay_id));
		envVolume2=static_cast<winlib::TrackBar*>(findChild(envVolume2_id));
		envReleaseRate=static_cast<winlib::EditControl*>(findChild(envReleaseRate_id));
		saveSynthButton=static_cast<winlib::PushButton*>(findChild(saveSynthButton_id));
		loadSynthButton=static_cast<winlib::PushButton*>(findChild(loadSynthButton_id));
		IDCANCEL_PushButton=static_cast<winlib::PushButton*>(findChild(IDCANCEL));
		check1=static_cast<winlib::CheckBox*>(findChild(check1_id));
		check2=static_cast<winlib::CheckBox*>(findChild(check2_id));
		check3=static_cast<winlib::CheckBox*>(findChild(check3_id));
		check4=static_cast<winlib::CheckBox*>(findChild(check4_id));
		cpuSpeed=static_cast<winlib::Label*>(findChild(cpuSpeed_id));
	}	// MIDIsynthesizerWindow
	const int FileDialogStringList_ids[] = {
		OPEN_MIDI_id,
		SAVE_MIDI_id,
		OPEN_SETTINGS_id,
		SAVE_SETTINGS_id,
		OPEN_LOOP_id,
		SAVE_LOOP_id,
	};
	const int ErrorStringList_ids[] = {
		MIDI_BAD_HEADER_id,
		MIDI_BAD_TRACK_HEADER_id,
		OPEN_IN_FAILED_id,
		OPEN_OUT_FAILED_id,
	};
	const int MessageStringList_ids[] = {
		MIDI_FILE_CHANGED_id,
		SETTINGS_CHANGED_id,
		DRUM_CHANGED_id,
		LOOP_CHANGED_id,
	};
	const int LabelStringList_ids[] = {
		DRUM_EDITOR_TITLE_id,
		STOP_LABEL_id,
		PLAY_LABEL_id,
		RECORD_LABEL_id,
		CHANNEL_LABEL_id,
		ANY_CHANNEL_LABEL_id,
		ALL_MIDI_RECORD_LABEL_id,
		DEFAULT_MIDI_PLAY_LABEL_id,
		ALL_NOTES_LABEL_id,
		MAJOR_LABEL_id,
		MINOR_LABEL_id,
		INSTRUMENT_CAPTION_id,
	};
	const int RecorderFiles_ids[] = {
		MidiFiles_id,
		CsvFiles_id,
	};
	const int DrumFiles_ids[] = {
		DrumFiles_id,
		CsvFiles_id,
		MidiFiles_id,
	};
	const int LoopFiles_ids[] = {
		LoopFiles_id,
		CsvFiles_id,
		MidiFiles_id,
	};
	const int SynthFiles_ids[] = {
		SynthFiles_id,
	};
	const int SettingsFiles_ids[] = {
		SettingsFiles_id,
	};
}	// namespace winlibGUI
