/*
	this file was created from C:\CRESD\Source\MIDI\Tools\midi.gui with MG Gui Builder
	+++++++++++++++++++++++
	!!!!! Do not Edit !!!!!
	+++++++++++++++++++++++
*/
#ifndef C__CRESD_SOURCE_MIDI_TOOLS_MIDI_GUI_H
#define C__CRESD_SOURCE_MIDI_TOOLS_MIDI_GUI_H
#include <winlib/popup.h>
#include <winlib/frame.h>
#include <winlib/scrollFrame.h>
#include <winlib/ControlW.h>
#include <winlib/xmlEditorChild.h>
#include <winlib/gridView.h>

namespace winlibGUI {

	const int actionChannelSelect_id=183;
	const int activeCheckBox_id=188;
	const int addNewPatternButton_id=258;
	const int ALL_MIDI_RECORD_LABEL_id=340;
	const int ALL_NOTES_LABEL_id=342;
	const int amAscending_id=292;
	const int amDescending_id=293;
	const int amFrequency_id=296;
	const int amRange_id=297;
	const int amRectangle_id=295;
	const int amSinus_id=291;
	const int amTriangle_id=294;
	const int ANY_CHANNEL_LABEL_id=339;
	const int authoRhytmCheck_id=246;
	const int bottomFrame_id=155;
	const int bpmEdit_id=150;
	const int channelsButton_id=240;
	const int channelSelect_id=165;
	const int ChannelVolume_id=131;
	const int CHANNEL_LABEL_id=338;
	const int check1_id=315;
	const int check2_id=316;
	const int check3_id=317;
	const int check4_id=318;
	const int checkMuteA_id=274;
	const int checkMuteB_id=275;
	const int checkMuteC_id=276;
	const int checkMuteD_id=277;
	const int clockText_id=149;
	const int conditionSelect_id=180;
	const int controllerEdit_id=219;
	const int controllerRadio_id=217;
	const int controllerSelect_id=218;
	const int convertBPMbutton_id=249;
	const int copyButton_id=236;
	const int copyEventsButton_id=262;
	const int cpuSpeed_id=319;
	const int CsvFiles_id=347;
	const int data1Edit_id=222;
	const int data2Edit_id=223;
	const int defaultButton_id=187;
	const int DEFAULT_MIDI_PLAY_LABEL_id=341;
	const int deleteAllButton_id=265;
	const int deleteButton_id=175;
	const int deleteEntryButton_id=227;
	const int deleteEventsButton_id=261;
	const int denominatorSelect_id=248;
	const int destChannelSelect_id=169;
	const int deviceLabel_id=190;
	const int downButton_id=159;
	const int drumButton_id=143;
	const int DrumFiles_id=348;
	const int DRUM_CHANGED_id=332;
	const int DRUM_EDITOR_TITLE_id=334;
	const int editButton_id=253;
	const int effects1Bar_id=193;
	const int effects2Bar_id=194;
	const int effects3Bar_id=195;
	const int effects4Bar_id=196;
	const int effects5Bar_id=197;
	const int eighth_id=216;
	const int envAttack_id=309;
	const int envDecay_id=310;
	const int envDelay_id=306;
	const int envEnabled_id=305;
	const int envReleaseRate_id=312;
	const int envSustainRate_id=308;
	const int envVolume1_id=307;
	const int envVolume2_id=311;
	const int eventList_id=204;
	const int expressionBar_id=203;
	const int filterButton_id=241;
	const int filterChannelSelect_id=179;
	const int filterEnabledCheckBox_id=239;
	const int filterListBox_id=173;
	const int filterNameEdit_id=177;
	const int filterNote_id=181;
	const int firstVoiceSelect_id=170;
	const int fmAscending_id=299;
	const int fmDescending_id=300;
	const int fmFrequency_id=303;
	const int fmRange_id=304;
	const int fmRectangle_id=302;
	const int fmSinus_id=298;
	const int fmTriangle_id=301;
	const int full_id=213;
	const int gridSelect_id=256;
	const int groupSelect_id=191;
	const int half_id=214;
	const int insertBreakButton_id=264;
	const int instrumentButton_id=228;
	const int INSTRUMENT_CAPTION_id=345;
	const int languageCheckBox_id=147;
	const int lengthEdit_id=254;
	const int lengthSelect_id=257;
	const int lengthSelector_id=255;
	const int loadDrumsButton_id=268;
	const int loadLoopButton_id=153;
	const int loadMidiButton_id=251;
	const int loadSettingButton_id=242;
	const int loadSynthButton_id=314;
	const int longerButton_id=156;
	const int loopButton_id=144;
	const int LoopFiles_id=349;
	const int LOOP_CHANGED_id=333;
	const int mainPan_id=281;
	const int mainPhase_id=284;
	const int mainPitch_id=283;
	const int mainPitchValue_id=282;
	const int mainVolume_id=280;
	const int MAJOR_LABEL_id=343;
	const int masterVolume_id=278;
	const int messageSelect_id=221;
	const int metronomButton_id=250;
	const int MetronomWindow_id=134;
	const int MIDIchannelsWindow_id=137;
	const int MIDIdeviceDialog_id=135;
	const int midiDevicesButton_id=146;
	const int MIDIdrumWindow_id=140;
	const int MIDIeditorWindow_id=138;
	const int MidiFiles_id=346;
	const int MIDIfilterWindow_id=136;
	const int MIDIholdNote_id=244;
	const int midiListBox_id=161;
	const int MIDIloopEditor_id=129;
	const int MIDIplaySelect_id=182;
	const int MIDIrecorderWindow_id=139;
	const int MIDIrecSelect_id=178;
	const int MIDIsynthesizerWindow_id=141;
	const int MIDIthrough_id=238;
	const int MIDI_BAD_HEADER_id=326;
	const int MIDI_BAD_TRACK_HEADER_id=327;
	const int MIDI_FILE_CHANGED_id=330;
	const int MINOR_LABEL_id=344;
	const int moveButton_id=235;
	const int moveEdit_id=234;
	const int moveEventsButton_id=263;
	const int newButton_id=174;
	const int newEntryButton_id=226;
	const int newVolumeEdit_id=186;
	const int noteLengthEdit_id=211;
	const int noteOffsetEdit_id=184;
	const int noteRadio_id=208;
	const int noteSelect_id=209;
	const int noteTimeEdit_id=212;
	const int noteVolumeEdit_id=210;
	const int numeratorEdit_id=247;
	const int OPEN_IN_FAILED_id=328;
	const int OPEN_LOOP_id=324;
	const int OPEN_MIDI_id=320;
	const int OPEN_OUT_FAILED_id=329;
	const int OPEN_SETTINGS_id=322;
	const int otherRadio_id=220;
	const int otherVoiceSelect_id=171;
	const int panButton_id=260;
	const int patternFrame_id=266;
	const int patternSelect_id=163;
	const int PatternStereo_id=132;
	const int PatternVolume_id=130;
	const int pianoFrame_id=237;
	const int playButton_id=148;
	const int playModeSelect_id=259;
	const int playSelect_id=151;
	const int PLAY_LABEL_id=336;
	const int presenceValue_id=279;
	const int quarter_id=215;
	const int radioSynthA_id=270;
	const int radioSynthB_id=271;
	const int radioSynthC_id=272;
	const int radioSynthD_id=273;
	const int recordButton_id=245;
	const int recorderButton_id=142;
	const int RECORD_LABEL_id=337;
	const int repeatButton_id=231;
	const int rightFrame_id=160;
	const int saveDrumsButton_id=269;
	const int saveEntryButton_id=225;
	const int saveLoopButton_id=154;
	const int saveMidiButton_id=252;
	const int saveSettingButton_id=243;
	const int saveSynthButton_id=313;
	const int SAVE_LOOP_id=325;
	const int SAVE_MIDI_id=321;
	const int SAVE_SETTINGS_id=323;
	const int SettingsFiles_id=351;
	const int SETTINGS_CHANGED_id=331;
	const int shorterButton_id=157;
	const int sound1Bar_id=198;
	const int sound2Bar_id=199;
	const int sound3Bar_id=200;
	const int sound4Bar_id=201;
	const int sound5Bar_id=202;
	const int srcChannelSelect_id=168;
	const int standardsButton_id=176;
	const int stereoBar_id=167;
	const int stereoLabel_id=166;
	const int stopFlag_id=185;
	const int STOP_LABEL_id=335;
	const int sub1phase_id=285;
	const int sub1pitch_id=287;
	const int sub1volume_id=286;
	const int sub2phase_id=288;
	const int sub2pitch_id=290;
	const int sub2volume_id=289;
	const int SwapChannel_id=133;
	const int swapChannelsButton_id=230;
	const int synthesizerButton_id=145;
	const int SynthFiles_id=350;
	const int sysExRadio_id=224;
	const int theDeviceTable_id=172;
	const int timeBar_id=267;
	const int timeCodeEdit_id=206;
	const int ToolBarWindow_id=128;
	const int topFrame_id=152;
	const int trackSelect_id=205;
	const int transposeButton_id=233;
	const int transposeEdit_id=232;
	const int upButton_id=158;
	const int valueText_id=189;
	const int voiceRadio_id=207;
	const int voiceSelect_id=192;
	const int volumeBar_id=164;
	const int volumeButton_id=229;
	const int volumeLabel_id=162;

	class ToolBarWindow_form : public winlib::PopupWindow {
		public:
		ToolBarWindow_form(winlib::BasicWindow *owner) : PopupWindow(owner) {}
		winlib::SuccessCode create(winlib::BasicWindow*parent) {
			return PopupWindow::create(parent,"ToolBarWindow");
		}

		winlib::PushButton *recorderButton;
		winlib::PushButton *drumButton;
		winlib::PushButton *loopButton;
		winlib::PushButton *synthesizerButton;
		winlib::PushButton *midiDevicesButton;
		winlib::PushButton *IDCANCEL_PushButton;
		winlib::CheckBox *languageCheckBox;
		private:
		virtual void getControls();
	};	// end of ToolBarWindow

	class MIDIloopEditor_form : public winlib::PopupWindow {
		public:
		MIDIloopEditor_form(winlib::BasicWindow *owner) : PopupWindow(owner) {}
		winlib::SuccessCode create(winlib::BasicWindow*parent) {
			return PopupWindow::create(parent,"MIDIloopEditor");
		}

		winlib::PushButton *playButton;
		winlib::Label *clockText;
		winlib::EditControl *bpmEdit;
		winlib::ComboBox *playSelect;
		winlib::FrameChild *topFrame;
		winlib::PushButton *IDCANCEL_PushButton;
		winlib::PushButton *loadLoopButton;
		winlib::PushButton *saveLoopButton;
		winlib::FrameChild *bottomFrame;
		winlib::PushButton *longerButton;
		winlib::PushButton *shorterButton;
		winlib::PushButton *upButton;
		winlib::PushButton *downButton;
		winlib::FrameChild *rightFrame;
		winlib::ListBox *midiListBox;
		private:
		virtual void getControls();
	};	// end of MIDIloopEditor

	class PatternVolume_form : public winlib::ModalPopup {
		public:
		PatternVolume_form(winlib::BasicWindow *owner) : ModalPopup(owner) {}
		winlib::SuccessCode create(winlib::BasicWindow*parent) {
			return ModalPopup::create(parent,"PatternVolume");
		}

		winlib::Label *volumeLabel;
		winlib::ComboBox *patternSelect;
		winlib::TrackBar *volumeBar;
		winlib::PushButton *IDOK_PushButton;
		winlib::PushButton *IDCANCEL_PushButton;
		private:
		virtual void getControls();
	};	// end of PatternVolume

	class ChannelVolume_form : public winlib::ModalPopup {
		public:
		ChannelVolume_form(winlib::BasicWindow *owner) : ModalPopup(owner) {}
		winlib::SuccessCode create(winlib::BasicWindow*parent) {
			return ModalPopup::create(parent,"ChannelVolume");
		}

		winlib::Label *volumeLabel;
		winlib::ComboBox *channelSelect;
		winlib::TrackBar *volumeBar;
		winlib::PushButton *IDOK_PushButton;
		winlib::PushButton *IDCANCEL_PushButton;
		private:
		virtual void getControls();
	};	// end of ChannelVolume

	class PatternStereo_form : public winlib::ModalPopup {
		public:
		PatternStereo_form(winlib::BasicWindow *owner) : ModalPopup(owner) {}
		winlib::SuccessCode create(winlib::BasicWindow*parent) {
			return ModalPopup::create(parent,"PatternStereo");
		}

		winlib::Label *stereoLabel;
		winlib::ComboBox *patternSelect;
		winlib::TrackBar *stereoBar;
		winlib::PushButton *IDOK_PushButton;
		winlib::PushButton *IDCANCEL_PushButton;
		private:
		virtual void getControls();
	};	// end of PatternStereo

	class SwapChannel_form : public winlib::ModalPopup {
		public:
		SwapChannel_form(winlib::BasicWindow *owner) : ModalPopup(owner) {}
		winlib::SuccessCode create(winlib::BasicWindow*parent) {
			return ModalPopup::create(parent,"SwapChannel");
		}

		winlib::ComboBox *srcChannelSelect;
		winlib::ComboBox *destChannelSelect;
		winlib::PushButton *IDOK_PushButton;
		winlib::PushButton *IDCANCEL_PushButton;
		private:
		virtual void getControls();
	};	// end of SwapChannel

	class MetronomWindow_form : public winlib::PopupWindow {
		public:
		MetronomWindow_form(winlib::BasicWindow *owner) : PopupWindow(owner) {}
		winlib::SuccessCode create(winlib::BasicWindow*parent) {
			return PopupWindow::create(parent,"MetronomWindow");
		}

		winlib::ComboBox *firstVoiceSelect;
		winlib::ComboBox *otherVoiceSelect;
		private:
		virtual void getControls();
	};	// end of MetronomWindow

	class MIDIdeviceDialog_form : public winlib::ModalPopup {
		public:
		MIDIdeviceDialog_form(winlib::BasicWindow *owner) : ModalPopup(owner) {}
		winlib::SuccessCode create(winlib::BasicWindow*parent) {
			return ModalPopup::create(parent,"MIDIdeviceDialog");
		}

		winlib::PushButton *IDOK_PushButton;
		winlib::PushButton *IDCANCEL_PushButton;
		winlib::FrameChild *theDeviceTable;
		private:
		virtual void getControls();
	};	// end of MIDIdeviceDialog

	class MIDIfilterWindow_form : public winlib::PopupWindow {
		public:
		MIDIfilterWindow_form(winlib::BasicWindow *owner) : PopupWindow(owner) {}
		winlib::SuccessCode create(winlib::BasicWindow*parent) {
			return PopupWindow::create(parent,"MIDIfilterWindow");
		}

		winlib::ListBox *filterListBox;
		winlib::PushButton *newButton;
		winlib::PushButton *deleteButton;
		winlib::PushButton *upButton;
		winlib::PushButton *IDCANCEL_PushButton;
		winlib::PushButton *standardsButton;
		winlib::EditControl *filterNameEdit;
		winlib::ComboBox *MIDIrecSelect;
		winlib::ComboBox *filterChannelSelect;
		winlib::ComboBox *conditionSelect;
		winlib::UpDownButton *filterNote;
		winlib::ComboBox *MIDIplaySelect;
		winlib::ComboBox *actionChannelSelect;
		winlib::EditControl *noteOffsetEdit;
		winlib::CheckBox *stopFlag;
		winlib::EditControl *newVolumeEdit;
		private:
		virtual void getControls();
	};	// end of MIDIfilterWindow

	class MIDIchannelsWindow_form : public winlib::PopupWindow {
		public:
		MIDIchannelsWindow_form(winlib::BasicWindow *owner) : PopupWindow(owner) {}
		winlib::SuccessCode create(winlib::BasicWindow*parent) {
			return PopupWindow::create(parent,"MIDIchannelsWindow");
		}

		winlib::PushButton *IDCANCEL_PushButton;
		winlib::PushButton *defaultButton;
		winlib::CheckBox *activeCheckBox;
		winlib::Label *valueText;
		winlib::ComboBox *channelSelect;
		winlib::Label *deviceLabel;
		winlib::ComboBox *groupSelect;
		winlib::ComboBox *voiceSelect;
		winlib::TrackBar *stereoBar;
		winlib::TrackBar *volumeBar;
		winlib::TrackBar *effects1Bar;
		winlib::TrackBar *effects2Bar;
		winlib::TrackBar *effects3Bar;
		winlib::TrackBar *effects4Bar;
		winlib::TrackBar *effects5Bar;
		winlib::TrackBar *sound1Bar;
		winlib::TrackBar *sound2Bar;
		winlib::TrackBar *sound3Bar;
		winlib::TrackBar *sound4Bar;
		winlib::TrackBar *sound5Bar;
		winlib::TrackBar *expressionBar;
		private:
		virtual void getControls();
	};	// end of MIDIchannelsWindow

	class MIDIeditorWindow_form : public winlib::ModalPopup {
		public:
		MIDIeditorWindow_form(winlib::BasicWindow *owner) : ModalPopup(owner) {}
		winlib::SuccessCode create(winlib::BasicWindow*parent) {
			return ModalPopup::create(parent,"MIDIeditorWindow");
		}

		winlib::ListBox *eventList;
		winlib::ComboBox *channelSelect;
		winlib::ComboBox *trackSelect;
		winlib::EditControl *timeCodeEdit;
		winlib::RadioButton *voiceRadio;
		winlib::ComboBox *groupSelect;
		winlib::ComboBox *voiceSelect;
		winlib::RadioButton *noteRadio;
		winlib::ComboBox *noteSelect;
		winlib::EditControl *noteVolumeEdit;
		winlib::EditControl *noteLengthEdit;
		winlib::EditControl *noteTimeEdit;
		winlib::PushButton *full;
		winlib::PushButton *half;
		winlib::PushButton *quarter;
		winlib::PushButton *eighth;
		winlib::RadioButton *controllerRadio;
		winlib::ComboBox *controllerSelect;
		winlib::EditControl *controllerEdit;
		winlib::RadioButton *otherRadio;
		winlib::ComboBox *messageSelect;
		winlib::EditControl *data1Edit;
		winlib::EditControl *data2Edit;
		winlib::RadioButton *sysExRadio;
		winlib::PushButton *saveEntryButton;
		winlib::PushButton *newEntryButton;
		winlib::PushButton *deleteEntryButton;
		winlib::PushButton *instrumentButton;
		winlib::PushButton *volumeButton;
		winlib::PushButton *swapChannelsButton;
		winlib::PushButton *repeatButton;
		winlib::EditControl *transposeEdit;
		winlib::PushButton *transposeButton;
		winlib::EditControl *moveEdit;
		winlib::PushButton *moveButton;
		winlib::PushButton *copyButton;
		winlib::PushButton *IDOK_PushButton;
		winlib::PushButton *IDCANCEL_PushButton;
		winlib::PushButton *playButton;
		private:
		virtual void getControls();
	};	// end of MIDIeditorWindow

	class MIDIrecorderWindow_form : public winlib::PopupWindow {
		public:
		MIDIrecorderWindow_form(winlib::BasicWindow *owner) : PopupWindow(owner) {}
		winlib::SuccessCode create(winlib::BasicWindow*parent) {
			return PopupWindow::create(parent,"MIDIrecorderWindow");
		}

		winlib::FrameChild *pianoFrame;
		winlib::CheckBox *MIDIthrough;
		winlib::CheckBox *filterEnabledCheckBox;
		winlib::PushButton *channelsButton;
		winlib::PushButton *filterButton;
		winlib::PushButton *loadSettingButton;
		winlib::PushButton *saveSettingButton;
		winlib::CheckBox *MIDIholdNote;
		winlib::ComboBox *MIDIplaySelect;
		winlib::PushButton *playButton;
		winlib::ComboBox *MIDIrecSelect;
		winlib::PushButton *recordButton;
		winlib::CheckBox *authoRhytmCheck;
		winlib::EditControl *bpmEdit;
		winlib::EditControl *numeratorEdit;
		winlib::ComboBox *denominatorSelect;
		winlib::PushButton *convertBPMbutton;
		winlib::PushButton *metronomButton;
		winlib::PushButton *IDCANCEL_PushButton;
		winlib::PushButton *loadMidiButton;
		winlib::PushButton *saveMidiButton;
		winlib::PushButton *editButton;
		winlib::PushButton *instrumentButton;
		private:
		virtual void getControls();
	};	// end of MIDIrecorderWindow

	class MIDIdrumWindow_form : public winlib::PopupWindow {
		public:
		MIDIdrumWindow_form(winlib::BasicWindow *owner) : PopupWindow(owner) {}
		winlib::SuccessCode create(winlib::BasicWindow*parent) {
			return PopupWindow::create(parent,"MIDIdrumWindow");
		}

		winlib::ComboBox *MIDIplaySelect;
		winlib::PushButton *playButton;
		winlib::EditControl *bpmEdit;
		winlib::EditControl *numeratorEdit;
		winlib::ComboBox *denominatorSelect;
		winlib::EditControl *lengthEdit;
		winlib::UpDownButton *lengthSelector;
		winlib::PushButton *convertBPMbutton;
		winlib::ComboBox *gridSelect;
		winlib::ComboBox *lengthSelect;
		winlib::Label *clockText;
		winlib::ComboBox *voiceSelect;
		winlib::PushButton *addNewPatternButton;
		winlib::ComboBox *playModeSelect;
		winlib::PushButton *volumeButton;
		winlib::PushButton *panButton;
		winlib::PushButton *deleteEventsButton;
		winlib::PushButton *copyEventsButton;
		winlib::PushButton *moveEventsButton;
		winlib::PushButton *insertBreakButton;
		winlib::PushButton *deleteAllButton;
		winlib::FrameChild *patternFrame;
		winlib::TrackBar *volumeBar;
		winlib::TrackBar *timeBar;
		winlib::PushButton *IDCANCEL_PushButton;
		winlib::PushButton *loadDrumsButton;
		winlib::PushButton *saveDrumsButton;
		private:
		virtual void getControls();
	};	// end of MIDIdrumWindow

	class MIDIsynthesizerWindow_form : public winlib::PopupWindow {
		public:
		MIDIsynthesizerWindow_form(winlib::BasicWindow *owner) : PopupWindow(owner) {}
		winlib::SuccessCode create(winlib::BasicWindow*parent) {
			return PopupWindow::create(parent,"MIDIsynthesizerWindow");
		}

		winlib::RadioButton *radioSynthA;
		winlib::RadioButton *radioSynthB;
		winlib::RadioButton *radioSynthC;
		winlib::RadioButton *radioSynthD;
		winlib::CheckBox *checkMuteA;
		winlib::CheckBox *checkMuteB;
		winlib::CheckBox *checkMuteC;
		winlib::CheckBox *checkMuteD;
		winlib::TrackBar *masterVolume;
		winlib::TrackBar *presenceValue;
		winlib::TrackBar *mainVolume;
		winlib::TrackBar *mainPan;
		winlib::Label *mainPitchValue;
		winlib::TrackBar *mainPitch;
		winlib::TrackBar *mainPhase;
		winlib::TrackBar *sub1phase;
		winlib::TrackBar *sub1volume;
		winlib::TrackBar *sub1pitch;
		winlib::TrackBar *sub2phase;
		winlib::TrackBar *sub2volume;
		winlib::TrackBar *sub2pitch;
		winlib::RadioButton *amSinus;
		winlib::RadioButton *amAscending;
		winlib::RadioButton *amDescending;
		winlib::RadioButton *amTriangle;
		winlib::RadioButton *amRectangle;
		winlib::EditControl *amFrequency;
		winlib::TrackBar *amRange;
		winlib::RadioButton *fmSinus;
		winlib::RadioButton *fmAscending;
		winlib::RadioButton *fmDescending;
		winlib::RadioButton *fmTriangle;
		winlib::RadioButton *fmRectangle;
		winlib::EditControl *fmFrequency;
		winlib::TrackBar *fmRange;
		winlib::CheckBox *envEnabled;
		winlib::EditControl *envDelay;
		winlib::TrackBar *envVolume1;
		winlib::EditControl *envSustainRate;
		winlib::EditControl *envAttack;
		winlib::EditControl *envDecay;
		winlib::TrackBar *envVolume2;
		winlib::EditControl *envReleaseRate;
		winlib::PushButton *saveSynthButton;
		winlib::PushButton *loadSynthButton;
		winlib::PushButton *IDCANCEL_PushButton;
		winlib::CheckBox *check1;
		winlib::CheckBox *check2;
		winlib::CheckBox *check3;
		winlib::CheckBox *check4;
		winlib::Label *cpuSpeed;
		private:
		virtual void getControls();
	};	// end of MIDIsynthesizerWindow
	extern const int FileDialogStringList_ids[];
	static const int FileDialogStringList_count = 6;
	extern const int ErrorStringList_ids[];
	static const int ErrorStringList_count = 4;
	extern const int MessageStringList_ids[];
	static const int MessageStringList_count = 4;
	extern const int LabelStringList_ids[];
	static const int LabelStringList_count = 12;
	extern const int RecorderFiles_ids[];
	static const int RecorderFiles_count = 2;
	extern const int DrumFiles_ids[];
	static const int DrumFiles_count = 3;
	extern const int LoopFiles_ids[];
	static const int LoopFiles_count = 3;
	extern const int SynthFiles_ids[];
	static const int SynthFiles_count = 1;
	extern const int SettingsFiles_ids[];
	static const int SettingsFiles_count = 1;
}	// namespace winlibGUI

#endif // C__CRESD_SOURCE_MIDI_TOOLS_MIDI_GUI_H
