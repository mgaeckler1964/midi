/*
	this file was created from C:\CRESD\Source\MIDI\Tools\midi.gui with MG Gui Builder
	+++++++++++++++++++++++
	!!!!! Do not Edit !!!!!
	+++++++++++++++++++++++
*/

#include "C:\CRESD\Source\MIDI\Tools\midi.gui.h"

namespace winlibGUI {

	gak::xml::Document *GuiApplication::getGuiDoc() {
		gak::xml::Document *doc = winlib::Application::getGuiDoc();
		if(!doc) {
			gak::STRING xmlSrc = "<?xml version=\"1.0\" encoding=\"iso-8859-1\" ?>\n"
"<gui>\n"
"<forms>\n"
"<form name=\"ToolBarWindow\" width=\"761\" height=\"99\" caption=\"MG MIDI Tools Version 3.2\" id=\"128\" style=\"277807104\" bgColor=\"COLOR_BTNFACE\" bg_red=\"0\" bg_green=\"0\" bg_blue=\"0\">\n"
"<layoutManager type=\"TableManager\" marginLeft=\"4\" marginRight=\"4\" marginTop=\"4\" marginBottom=\"4\" />\n"
"<child type=\"Label\" name=\"\" caption=\"MG MIDI Tools Version 3.2\" x=\"80\" y=\"26\" width=\"80\" height=\"0\" style=\"1342177291\">\n"
"<layout x=\"0\" y=\"0\" width=\"7\" height=\"1\" growWidth=\"1\" paddingTop=\"4\" paddingBottom=\"4\" paddingLeft=\"4\" paddingRight=\"4\" />\n"
"</child>\n"
"<child type=\"Label\" name=\"\" caption=\"(c) 2007-2026 by Martin G\xE4""ckler\" x=\"132\" y=\"92\" width=\"80\" height=\"0\" style=\"1342242827\">\n"
"<layout x=\"0\" y=\"1\" width=\"7\" height=\"1\" growWidth=\"1\" growHeight=\"0\" paddingTop=\"4\" paddingBottom=\"4\" paddingLeft=\"4\" paddingRight=\"4\" />\n"
"</child>\n"
"<child type=\"Label\" name=\"\" caption=\"Label\" x=\"210\" y=\"177\" width=\"80\" height=\"2\" style=\"1342177298\">\n"
"<layout x=\"0\" y=\"2\" width=\"7\" height=\"1\" growWidth=\"1\" paddingTop=\"4\" paddingBottom=\"4\" paddingLeft=\"4\" paddingRight=\"4\" />\n"
"</child>\n"
"<child type=\"PushButton\" name=\"recorderButton\" caption=\"Recorder\" x=\"76\" y=\"84\" width=\"80\" height=\"25\" style=\"1342242816\" id=\"142\">\n"
"<layout y=\"3\" growWidth=\"1\" paddingLeft=\"4\" paddingRight=\"4\" paddingTop=\"4\" paddingBottom=\"4\" x=\"0\" />\n"
"</child>\n"
"<child type=\"PushButton\" name=\"drumButton\" caption=\"Drum Editor\" x=\"143\" y=\"124\" width=\"80\" height=\"25\" style=\"1342242816\" id=\"143\">\n"
"<layout y=\"3\" x=\"1\" growWidth=\"1\" paddingLeft=\"4\" paddingRight=\"4\" paddingTop=\"4\" paddingBottom=\"4\" />\n"
"</child>\n"
"<child type=\"PushButton\" name=\"loopButton\" caption=\"Loop Editor\" x=\"241\" y=\"174\" width=\"80\" height=\"25\" style=\"1342242816\" id=\"144\">\n"
"<layout y=\"3\" x=\"2\" growWidth=\"1\" paddingLeft=\"4\" paddingRight=\"4\" paddingTop=\"4\" paddingBottom=\"4\" />\n"
"</child>\n"
"<child type=\"PushButton\" name=\"synthesizerButton\" caption=\"Synthesizer\" x=\"109\" y=\"226\" width=\"80\" height=\"25\" style=\"1342242816\" id=\"145\">\n"
"<layout y=\"3\" x=\"3\" growWidth=\"1\" paddingLeft=\"4\" paddingRight=\"4\" paddingTop=\"4\" paddingBottom=\"4\" />\n"
"</child>\n"
"<child type=\"PushButton\" name=\"midiDevicesButton\" caption=\"MIDI Devices\" x=\"38\" y=\"171\" width=\"80\" height=\"25\" style=\"1342242816\" id=\"146\">\n"
"<layout y=\"3\" x=\"4\" growWidth=\"1\" paddingLeft=\"4\" paddingRight=\"4\" paddingTop=\"4\" paddingBottom=\"4\" />\n"
"</child>\n"
"<child type=\"PushButton\" name=\"IDCANCEL\" caption=\"Close\" x=\"402\" y=\"208\" width=\"80\" height=\"25\" style=\"1342242816\" id=\"2\">\n"
"<layout y=\"3\" x=\"5\" growWidth=\"1\" paddingLeft=\"4\" paddingRight=\"4\" paddingTop=\"4\" paddingBottom=\"4\" />\n"
"</child>\n"
"<child type=\"CheckBox\" name=\"languageCheckBox\" caption=\"German\" x=\"69\" y=\"106\" width=\"0\" height=\"0\" style=\"1342242819\" id=\"147\">\n"
"<layout y=\"3\" width=\"1\" x=\"6\" height=\"1\" paddingLeft=\"4\" paddingRight=\"4\" paddingTop=\"4\" paddingBottom=\"4\" />\n"
"</child>\n"
"</form>\n"
"<form name=\"MIDIloopEditor\" width=\"747\" height=\"714\" caption=\"Loop Editor\" id=\"129\" style=\"277807104\" baseClass=\"OverlappedWindow\" bgColor=\"COLOR_BTNFACE\" bg_red=\"0\" bg_green=\"0\" bg_blue=\"0\">\n"
"<layoutManager type=\"AttachmentManager\" marginLeft=\"4\" marginRight=\"4\" marginTop=\"4\" marginBottom=\"4\" />\n"
"<child type=\"FrameChild\" name=\"topFrame\" caption=\"\" x=\"597\" y=\"472\" width=\"80\" height=\"35\" style=\"1342308352\" id=\"152\">\n"
"<layout x=\"0\" y=\"0\" width=\"6\" height=\"1\" growWidth=\"1\" attachment=\"1\" />\n"
"<child type=\"Label\" name=\"\" caption=\"MIDI-Out:\" x=\"64\" y=\"40\" width=\"0\" height=\"0\" style=\"1342242827\">\n"
"<layout x=\"0\" y=\"0\" width=\"1\" height=\"1\" growWidth=\"0\" growHeight=\"0\" paddingLeft=\"4\" paddingRight=\"0\" paddingTop=\"4\" paddingBottom=\"4\" />\n"
"</child>\n"
"<child type=\"PushButton\" name=\"playButton\" caption=\"Play\" x=\"432\" y=\"103\" width=\"80\" height=\"25\" style=\"1342242816\" id=\"148\">\n"
"<layout x=\"4\" y=\"0\" width=\"1\" height=\"1\" growWidth=\"0\" growHeight=\"0\" attachment=\"2\" paddingLeft=\"4\" paddingRight=\"4\" paddingTop=\"4\" paddingBottom=\"4\" />\n"
"</child>\n"
"<child type=\"Label\" name=\"clockText\" caption=\"0000 - 00:00:00\" x=\"507\" y=\"153\" width=\"0\" height=\"0\" style=\"1342242827\" id=\"149\">\n"
"<layout x=\"5\" y=\"0\" width=\"1\" height=\"1\" growWidth=\"0\" growHeight=\"0\" attachment=\"2\" paddingLeft=\"4\" paddingRight=\"4\" paddingTop=\"4\" paddingBottom=\"4\" />\n"
"</child>\n"
"<child type=\"EditControl\" name=\"bpmEdit\" caption=\"\" x=\"451\" y=\"123\" width=\"80\" height=\"23\" style=\"1350631808\" id=\"150\">\n"
"<layout x=\"3\" y=\"0\" width=\"1\" height=\"1\" growWidth=\"0\" growHeight=\"0\" attachment=\"2\" paddingLeft=\"4\" paddingRight=\"4\" paddingTop=\"4\" paddingBottom=\"4\" />\n"
"</child>\n"
"<child type=\"Label\" name=\"\" caption=\"Tempo:\" x=\"342\" y=\"13\" width=\"0\" height=\"0\" style=\"1342242827\">\n"
"<layout x=\"2\" y=\"0\" width=\"1\" height=\"1\" growWidth=\"0\" growHeight=\"0\" attachment=\"2\" paddingLeft=\"4\" paddingRight=\"0\" paddingTop=\"4\" paddingBottom=\"4\" />\n"
"</child>\n"
"<child type=\"ComboBox\" name=\"playSelect\" caption=\"\" x=\"64\" y=\"8\" width=\"344\" height=\"200\" style=\"1344341059\" id=\"151\">\n"
"<layout x=\"1\" y=\"0\" width=\"1\" height=\"1\" growWidth=\"1\" growHeight=\"0\" attachment=\"4\" paddingLeft=\"4\" paddingRight=\"4\" paddingTop=\"4\" paddingBottom=\"4\" />\n"
"</child>\n"
"<layoutManager type=\"AttachmentManager\" />\n"
"</child>\n"
"<child type=\"FrameChild\" name=\"bottomFrame\" caption=\"\" x=\"763\" y=\"433\" width=\"80\" height=\"35\" style=\"1342308352\" id=\"155\">\n"
"<layout y=\"9\" width=\"6\" height=\"1\" x=\"0\" growWidth=\"1\" attachment=\"3\" />\n"
"<layoutManager type=\"AttachmentManager\" />\n"
"<child type=\"PushButton\" name=\"IDCANCEL\" caption=\"Close\" x=\"914\" y=\"220\" width=\"80\" height=\"25\" style=\"1342242816\" id=\"2\">\n"
"<layout y=\"8\" x=\"0\" width=\"1\" height=\"1\" attachment=\"0\" paddingLeft=\"4\" paddingRight=\"4\" paddingTop=\"4\" paddingBottom=\"4\" />\n"
"</child>\n"
"<child type=\"PushButton\" name=\"loadLoopButton\" caption=\"Load...\" x=\"207\" y=\"210\" width=\"80\" height=\"25\" style=\"1342242816\" id=\"153\">\n"
"<layout y=\"8\" x=\"1\" paddingLeft=\"4\" paddingRight=\"4\" paddingTop=\"4\" paddingBottom=\"4\" />\n"
"</child>\n"
"<child type=\"PushButton\" name=\"saveLoopButton\" caption=\"Save...\" x=\"374\" y=\"218\" width=\"80\" height=\"25\" style=\"1342242816\" id=\"154\">\n"
"<layout y=\"8\" x=\"2\" paddingLeft=\"4\" paddingRight=\"4\" paddingTop=\"4\" paddingBottom=\"4\" />\n"
"</child>\n"
"</child>\n"
"<child type=\"FrameChild\" name=\"rightFrame\" caption=\"\" x=\"490\" y=\"417\" width=\"88\" height=\"25\" style=\"1342308352\" id=\"160\">\n"
"<layout x=\"6\" growHeight=\"1\" y=\"1\" attachment=\"2\" />\n"
"<child type=\"PushButton\" name=\"longerButton\" caption=\"Longer\" x=\"573\" y=\"74\" width=\"80\" height=\"25\" style=\"1342242816\" id=\"156\">\n"
"<layout x=\"5\" width=\"1\" height=\"1\" growWidth=\"1\" growHeight=\"0\" y=\"1\" attachment=\"1\" paddingLeft=\"4\" paddingRight=\"4\" paddingTop=\"4\" paddingBottom=\"4\" />\n"
"</child>\n"
"<child type=\"PushButton\" name=\"shorterButton\" caption=\"Shorter\" x=\"583\" y=\"109\" width=\"80\" height=\"25\" style=\"1342242816\" id=\"157\">\n"
"<layout x=\"5\" width=\"1\" height=\"1\" growWidth=\"1\" growHeight=\"0\" y=\"2\" attachment=\"1\" paddingLeft=\"4\" paddingRight=\"4\" paddingTop=\"4\" paddingBottom=\"4\" />\n"
"</child>\n"
"<child type=\"PushButton\" name=\"upButton\" caption=\"Up\" x=\"590\" y=\"157\" width=\"80\" height=\"25\" style=\"1342242816\" id=\"158\">\n"
"<layout x=\"5\" width=\"1\" height=\"1\" growWidth=\"1\" growHeight=\"0\" y=\"3\" attachment=\"1\" paddingLeft=\"4\" paddingRight=\"4\" paddingTop=\"4\" paddingBottom=\"4\" />\n"
"</child>\n"
"<child type=\"PushButton\" name=\"downButton\" caption=\"Down\" x=\"550\" y=\"194\" width=\"80\" height=\"25\" style=\"1342242816\" id=\"159\">\n"
"<layout x=\"5\" width=\"1\" height=\"1\" growWidth=\"1\" growHeight=\"0\" y=\"4\" attachment=\"1\" paddingLeft=\"4\" paddingRight=\"4\" paddingTop=\"4\" paddingBottom=\"4\" />\n"
"</child>\n"
"<layoutManager type=\"AttachmentManager\" />\n"
"</child>\n"
"<child type=\"ListBox\" name=\"midiListBox\" caption=\"\" x=\"116\" y=\"101\" width=\"80\" height=\"25\" style=\"1353777537\" id=\"161\">\n"
"<layout x=\"0\" y=\"1\" width=\"5\" height=\"7\" growWidth=\"1\" growHeight=\"1\" attachment=\"4\" paddingLeft=\"4\" paddingRight=\"4\" paddingTop=\"4\" paddingBottom=\"4\" />\n"
"</child>\n"
"</form>\n"
"<form name=\"PatternVolume\" style=\"314572800\" width=\"538\" height=\"135\" caption=\"Pattern Volumes\" baseClass=\"ModalPopup\" id=\"130\">\n"
"<child type=\"Label\" name=\"\" caption=\"Pattern:\" x=\"8\" y=\"8\" width=\"0\" height=\"0\" style=\"1342242827\">\n"
"<layout x=\"0\" y=\"0\" width=\"1\" height=\"1\" growWidth=\"0\" growHeight=\"0\" paddingLeft=\"0\" paddingRight=\"0\" paddingTop=\"0\" paddingBottom=\"0\" />\n"
"</child>\n"
"<child type=\"Label\" name=\"\" caption=\"Volume:\" x=\"8\" y=\"41\" width=\"0\" height=\"0\" style=\"1342242827\">\n"
"<layout x=\"0\" y=\"1\" width=\"1\" height=\"1\" growWidth=\"0\" growHeight=\"0\" />\n"
"</child>\n"
"<child type=\"Label\" name=\"volumeLabel\" caption=\"000\" x=\"500\" y=\"41\" width=\"0\" height=\"0\" style=\"1342242818\" id=\"162\">\n"
"<layout x=\"3\" y=\"1\" width=\"1\" height=\"1\" growWidth=\"0\" growHeight=\"0\" />\n"
"</child>\n"
"<child type=\"ComboBox\" name=\"patternSelect\" caption=\"\" x=\"80\" y=\"8\" width=\"304\" height=\"300\" style=\"1344341059\" id=\"163\">\n"
"<layout x=\"1\" y=\"0\" width=\"2\" height=\"1\" growWidth=\"1\" growHeight=\"0\" />\n"
"</child>\n"
"<child type=\"TrackBar\" name=\"volumeBar\" caption=\"\" x=\"8\" y=\"61\" width=\"515\" height=\"24\" style=\"1342242816\" id=\"164\">\n"
"<layout y=\"0\" x=\"0\" width=\"1\" height=\"1\" growWidth=\"0\" growHeight=\"0\" paddingLeft=\"0\" paddingRight=\"0\" paddingTop=\"0\" paddingBottom=\"0\" />\n"
"</child>\n"
"<child type=\"PushButton\" name=\"IDOK\" caption=\"OK\" x=\"8\" y=\"100\" width=\"80\" height=\"25\" style=\"1342242816\" id=\"1\" />\n"
"<child type=\"PushButton\" name=\"IDCANCEL\" caption=\"Cancel\" x=\"96\" y=\"100\" width=\"80\" height=\"25\" style=\"1342242816\" id=\"2\" />\n"
"</form>\n"
"<form name=\"ChannelVolume\" style=\"314572800\" width=\"538\" height=\"128\" caption=\"Channel Volume\" baseClass=\"ModalPopup\" id=\"131\">\n"
"<child type=\"Label\" name=\"\" caption=\"Channel:\" x=\"8\" y=\"8\" width=\"0\" height=\"0\" style=\"1342242827\">\n"
"<layout x=\"0\" y=\"0\" width=\"1\" height=\"1\" growWidth=\"0\" growHeight=\"0\" paddingLeft=\"0\" paddingRight=\"0\" paddingTop=\"0\" paddingBottom=\"0\" />\n"
"</child>\n"
"<child type=\"Label\" name=\"\" caption=\"Volume:\" x=\"8\" y=\"41\" width=\"0\" height=\"0\" style=\"1342242827\">\n"
"<layout x=\"0\" y=\"1\" width=\"1\" height=\"1\" growWidth=\"0\" growHeight=\"0\" />\n"
"</child>\n"
"<child type=\"Label\" name=\"volumeLabel\" caption=\"000\" x=\"500\" y=\"41\" width=\"0\" height=\"0\" style=\"1342242818\" id=\"162\">\n"
"<layout x=\"3\" y=\"1\" width=\"1\" height=\"1\" growWidth=\"0\" growHeight=\"0\" />\n"
"</child>\n"
"<child type=\"ComboBox\" name=\"channelSelect\" caption=\"\" x=\"80\" y=\"8\" width=\"100\" height=\"300\" style=\"1342243843\" id=\"165\">\n"
"<layout x=\"1\" y=\"0\" width=\"2\" height=\"1\" growWidth=\"1\" growHeight=\"0\" />\n"
"</child>\n"
"<child type=\"TrackBar\" name=\"volumeBar\" caption=\"\" x=\"8\" y=\"61\" width=\"515\" height=\"24\" style=\"1342242816\" id=\"164\">\n"
"<layout y=\"0\" x=\"0\" width=\"1\" height=\"1\" growWidth=\"0\" growHeight=\"0\" paddingLeft=\"0\" paddingRight=\"0\" paddingTop=\"0\" paddingBottom=\"0\" />\n"
"</child>\n"
"<child type=\"PushButton\" name=\"IDOK\" caption=\"OK\" x=\"8\" y=\"96\" width=\"80\" height=\"25\" style=\"1342242816\" id=\"1\" />\n"
"<child type=\"PushButton\" name=\"IDCANCEL\" caption=\"Cancel\" x=\"96\" y=\"96\" width=\"80\" height=\"25\" style=\"1342242816\" id=\"2\" />\n"
"</form>\n"
"<form name=\"PatternStereo\" style=\"314572800\" width=\"538\" height=\"135\" caption=\"Pattern Pan\" baseClass=\"ModalPopup\" id=\"132\">\n"
"<child type=\"Label\" name=\"\" caption=\"Pattern:\" x=\"8\" y=\"8\" width=\"0\" height=\"0\" style=\"1342242827\">\n"
"<layout x=\"0\" y=\"0\" width=\"1\" height=\"1\" growWidth=\"0\" growHeight=\"0\" paddingLeft=\"0\" paddingRight=\"0\" paddingTop=\"0\" paddingBottom=\"0\" />\n"
"</child>\n"
"<child type=\"Label\" name=\"\" caption=\"Pan:\" x=\"8\" y=\"40\" width=\"48\" height=\"16\" style=\"1342242827\">\n"
"<layout x=\"0\" y=\"1\" width=\"1\" height=\"1\" growWidth=\"0\" growHeight=\"0\" />\n"
"</child>\n"
"<child type=\"Label\" name=\"stereoLabel\" caption=\"000\" x=\"500\" y=\"41\" width=\"0\" height=\"0\" style=\"1342242818\" id=\"166\">\n"
"<layout x=\"3\" y=\"1\" width=\"1\" height=\"1\" growWidth=\"0\" growHeight=\"0\" />\n"
"</child>\n"
"<child type=\"ComboBox\" name=\"patternSelect\" caption=\"\" x=\"80\" y=\"8\" width=\"300\" height=\"300\" style=\"1344341059\" id=\"163\">\n"
"<layout x=\"1\" y=\"0\" width=\"2\" height=\"1\" growWidth=\"1\" growHeight=\"0\" />\n"
"</child>\n"
"<child type=\"TrackBar\" name=\"stereoBar\" caption=\"\" x=\"8\" y=\"61\" width=\"515\" height=\"24\" style=\"1342242816\" id=\"167\">\n"
"<layout y=\"0\" x=\"0\" width=\"1\" height=\"1\" growWidth=\"0\" growHeight=\"0\" paddingLeft=\"0\" paddingRight=\"0\" paddingTop=\"0\" paddingBottom=\"0\" />\n"
"</child>\n"
"<child type=\"PushButton\" name=\"IDOK\" caption=\"OK\" x=\"8\" y=\"100\" width=\"80\" height=\"25\" style=\"1342242816\" id=\"1\" />\n"
"<child type=\"PushButton\" name=\"IDCANCEL\" caption=\"Cancel\" x=\"96\" y=\"100\" width=\"80\" height=\"25\" style=\"1342242816\" id=\"2\" />\n"
"</form>\n"
"<form name=\"SwapChannel\" style=\"314572800\" width=\"293\" height=\"75\" caption=\"Swap Channels\" baseClass=\"ModalPopup\" id=\"133\">\n"
"<child type=\"Label\" name=\"\" caption=\"Channels:\" x=\"8\" y=\"8\" width=\"0\" height=\"0\" style=\"1342242827\" />\n"
"<child type=\"ComboBox\" name=\"srcChannelSelect\" caption=\"\" x=\"100\" y=\"8\" width=\"80\" height=\"300\" style=\"1342243843\" id=\"168\" />\n"
"<child type=\"ComboBox\" name=\"destChannelSelect\" caption=\"\" x=\"200\" y=\"8\" width=\"80\" height=\"300\" style=\"1342243843\" id=\"169\" />\n"
"<child type=\"PushButton\" name=\"IDOK\" caption=\"OK\" x=\"8\" y=\"41\" width=\"80\" height=\"25\" style=\"1342242816\" id=\"1\" />\n"
"<child type=\"PushButton\" name=\"IDCANCEL\" caption=\"Cancel\" x=\"100\" y=\"41\" width=\"80\" height=\"25\" style=\"1342242816\" id=\"2\" />\n"
"</form>\n"
"<form name=\"MetronomWindow\" style=\"281018368\" width=\"315\" height=\"62\" caption=\"Metronom\" baseClass=\"PopupWindow\" id=\"134\">\n"
"<layoutManager type=\"TableManager\" marginLeft=\"4\" marginRight=\"4\" marginTop=\"4\" marginBottom=\"4\" />\n"
"<child type=\"Label\" name=\"\" caption=\"First Click:\" x=\"56\" y=\"26\" width=\"0\" height=\"0\" style=\"1342242827\">\n"
"<layout x=\"0\" y=\"0\" width=\"1\" height=\"1\" growWidth=\"0\" growHeight=\"0\" paddingLeft=\"4\" paddingRight=\"4\" paddingTop=\"4\" paddingBottom=\"4\" />\n"
"</child>\n"
"<child type=\"Label\" name=\"\" caption=\"Other Clicks;\" x=\"0\" y=\"0\" width=\"0\" height=\"0\" style=\"1342242827\">\n"
"<layout x=\"0\" y=\"1\" width=\"1\" height=\"1\" growWidth=\"0\" growHeight=\"0\" paddingLeft=\"4\" paddingRight=\"4\" paddingTop=\"4\" paddingBottom=\"4\" />\n"
"</child>\n"
"<child type=\"ComboBox\" name=\"firstVoiceSelect\" caption=\"\" x=\"216\" y=\"16\" width=\"80\" height=\"300\" style=\"1344341059\" id=\"170\">\n"
"<layout x=\"1\" y=\"0\" width=\"1\" height=\"1\" growWidth=\"1\" growHeight=\"0\" paddingLeft=\"4\" paddingRight=\"4\" paddingTop=\"4\" paddingBottom=\"4\" />\n"
"</child>\n"
"<child type=\"ComboBox\" name=\"otherVoiceSelect\" caption=\"\" x=\"221\" y=\"49\" width=\"80\" height=\"300\" style=\"1344341059\" id=\"171\">\n"
"<layout x=\"1\" y=\"1\" width=\"1\" height=\"1\" growWidth=\"1\" growHeight=\"0\" paddingLeft=\"4\" paddingRight=\"4\" paddingTop=\"4\" paddingBottom=\"4\" />\n"
"</child>\n"
"</form>\n"
"<form name=\"MIDIdeviceDialog\" style=\"277807104\" width=\"800\" height=\"353\" caption=\"MIDI Devices\" baseClass=\"ModalPopup\" id=\"135\" x=\"0\" y=\"0\">\n"
"<layoutManager type=\"AttachmentManager\" marginLeft=\"0\" marginRight=\"0\" marginTop=\"0\" marginBottom=\"0\" />\n"
"<child type=\"FrameChild\" name=\"\" caption=\"\" x=\"319\" y=\"142\" width=\"96\" height=\"25\" style=\"1342308352\">\n"
"<layout attachment=\"2\" growHeight=\"1\" />\n"
"<child type=\"PushButton\" name=\"IDOK\" caption=\"OK\" x=\"8\" y=\"8\" width=\"80\" height=\"25\" style=\"1342242816\" id=\"1\" />\n"
"<child type=\"PushButton\" name=\"IDCANCEL\" caption=\"Cancel\" x=\"8\" y=\"41\" width=\"80\" height=\"25\" style=\"1342242816\" id=\"2\" />\n"
"</child>\n"
"<child type=\"ScrollFrame\" name=\"\" caption=\"\" x=\"91\" y=\"121\" width=\"80\" height=\"25\" style=\"1345454080\">\n"
"<layout attachment=\"4\" growWidth=\"1\" growHeight=\"1\" />\n"
"<child type=\"FrameChild\" name=\"theDeviceTable\" caption=\"\" x=\"100\" y=\"115\" width=\"80\" height=\"25\" style=\"1342308352\" id=\"172\">\n"
"<layoutManager type=\"TableManager\" marginLeft=\"4\" marginRight=\"4\" marginTop=\"4\" marginBottom=\"4\" />\n"
"</child>\n"
"</child>\n"
"</form>\n"
"<form name=\"MIDIfilterWindow\" style=\"281542656\" width=\"854\" height=\"279\" id=\"136\" caption=\"Filter Settings\">\n"
"<child type=\"ListBox\" name=\"filterListBox\" caption=\"\" x=\"8\" y=\"8\" width=\"155\" height=\"230\" style=\"1353777409\" id=\"173\" />\n"
"<child type=\"PushButton\" name=\"newButton\" caption=\"New\" x=\"176\" y=\"8\" width=\"80\" height=\"25\" style=\"1342242816\" id=\"174\" />\n"
"<child type=\"PushButton\" name=\"deleteButton\" caption=\"Delete\" x=\"264\" y=\"8\" width=\"80\" height=\"25\" style=\"1342242816\" id=\"175\" />\n"
"<child type=\"PushButton\" name=\"upButton\" caption=\"Up\" x=\"352\" y=\"8\" width=\"80\" height=\"25\" style=\"1342242816\" id=\"158\" />\n"
"<child type=\"PushButton\" name=\"IDCANCEL\" caption=\"Close\" x=\"8\" y=\"248\" width=\"80\" height=\"25\" style=\"1342242816\" id=\"2\" />\n"
"<child type=\"PushButton\" name=\"standardsButton\" caption=\"Standards\" x=\"96\" y=\"248\" width=\"80\" height=\"25\" style=\"1342242816\" id=\"176\" />\n"
"<child type=\"Label\" name=\"\" caption=\"Name\" x=\"176\" y=\"48\" width=\"0\" height=\"0\" style=\"1342242827\" />\n"
"<child type=\"EditControl\" name=\"filterNameEdit\" caption=\"\" x=\"232\" y=\"48\" width=\"160\" height=\"23\" style=\"1350631808\" id=\"177\" />\n"
"<child type=\"GroupBox\" name=\"\" caption=\"Filter\" x=\"176\" y=\"80\" width=\"660\" height=\"64\" style=\"1342242823\" />\n"
"<child type=\"Label\" name=\"\" caption=\"MIDI-In:\" x=\"184\" y=\"104\" width=\"0\" height=\"0\" style=\"1342242827\" />\n"
"<child type=\"ComboBox\" name=\"MIDIrecSelect\" caption=\"\" x=\"256\" y=\"104\" width=\"200\" height=\"200\" style=\"1344341059\" id=\"178\" />\n"
"<child type=\"Label\" name=\"\" caption=\"Channel:\" x=\"464\" y=\"104\" width=\"0\" height=\"0\" style=\"1342242827\" />\n"
"<child type=\"ComboBox\" name=\"filterChannelSelect\" caption=\"\" x=\"520\" y=\"104\" width=\"80\" height=\"300\" style=\"1342243843\" id=\"179\" />\n"
"<child type=\"Label\" name=\"\" caption=\"Note:\" x=\"608\" y=\"104\" width=\"0\" height=\"0\" style=\"1342242827\" />\n"
"<child type=\"ComboBox\" name=\"conditionSelect\" caption=\"\" x=\"648\" y=\"104\" width=\"80\" height=\"101\" style=\"1342243843\" id=\"180\">\n"
"<items>\n"
"<item>&lt;</item>\n"
"<item>&lt;=</item>\n"
"<item>=</item>\n"
"<item>&gt;=</item>\n"
"<item>&gt;</item>\n"
"<item>Control</item>\n"
"</items>\n"
"</child>\n"
"<child type=\"EditControl\" name=\"\" caption=\"\" x=\"736\" y=\"104\" width=\"80\" height=\"21\" style=\"1350631808\" />\n"
"<child type=\"UpDownButton\" name=\"filterNote\" caption=\"\" x=\"792\" y=\"104\" width=\"21\" height=\"21\" style=\"1342242870\" id=\"181\" />\n"
"<child type=\"GroupBox\" name=\"\" caption=\"Action\" x=\"176\" y=\"152\" width=\"660\" height=\"83\" style=\"1342242823\" />\n"
"<child type=\"Label\" name=\"\" caption=\"MIDI-Out:\" x=\"184\" y=\"176\" width=\"0\" height=\"0\" style=\"1342242827\" />\n"
"<child type=\"ComboBox\" name=\"MIDIplaySelect\" caption=\"\" x=\"256\" y=\"168\" width=\"200\" height=\"200\" style=\"1344341059\" id=\"182\" />\n"
"<child type=\"Label\" name=\"\" caption=\"Channel:\" x=\"464\" y=\"168\" width=\"0\" height=\"0\" style=\"1342242827\" />\n"
"<child type=\"ComboBox\" name=\"actionChannelSelect\" caption=\"\" x=\"520\" y=\"168\" width=\"80\" height=\"300\" style=\"1342243843\" id=\"183\" />\n"
"<child type=\"Label\" name=\"\" caption=\"Note:\" x=\"608\" y=\"168\" width=\"0\" height=\"0\" style=\"1342242827\" />\n"
"<child type=\"EditControl\" name=\"noteOffsetEdit\" caption=\"\" x=\"648\" y=\"168\" width=\"80\" height=\"21\" style=\"1350631808\" id=\"184\" />\n"
"<child type=\"CheckBox\" name=\"stopFlag\" caption=\"Stop\" x=\"736\" y=\"168\" width=\"0\" height=\"0\" style=\"1342242819\" id=\"185\" />\n"
"<child type=\"Label\" name=\"\" caption=\"Volume:\" x=\"184\" y=\"200\" width=\"0\" height=\"0\" style=\"1342242827\" />\n"
"<child type=\"EditControl\" name=\"newVolumeEdit\" caption=\"\" x=\"256\" y=\"200\" width=\"80\" height=\"23\" style=\"1350631808\" id=\"186\" />\n"
"</form>\n"
"<form name=\"MIDIchannelsWindow\" style=\"277807104\" width=\"600\" height=\"536\" caption=\"Channel Settings\" id=\"137\" baseClass=\"PopupWindow\">\n"
"<child type=\"FrameChild\" name=\"\" caption=\"\" x=\"84\" y=\"378\" width=\"80\" height=\"40\" style=\"1342308352\">\n"
"<layout attachment=\"3\" growWidth=\"1\" />\n"
"<child type=\"PushButton\" name=\"IDCANCEL\" caption=\"Close\" x=\"8\" y=\"8\" width=\"80\" height=\"25\" style=\"1342242816\" id=\"2\" />\n"
"<child type=\"PushButton\" name=\"defaultButton\" caption=\"Standard\" x=\"96\" y=\"8\" width=\"80\" height=\"25\" style=\"1342242816\" id=\"187\" />\n"
"<child type=\"CheckBox\" name=\"activeCheckBox\" caption=\"Active\" x=\"184\" y=\"8\" width=\"80\" height=\"25\" style=\"1342242819\" id=\"188\" />\n"
"<child type=\"Label\" name=\"valueText\" caption=\"000\" x=\"280\" y=\"8\" width=\"0\" height=\"0\" style=\"1342246912\" id=\"189\" />\n"
"</child>\n"
"<layoutManager type=\"AttachmentManager\" />\n"
"<child type=\"FrameChild\" name=\"\" caption=\"\" x=\"117\" y=\"93\" width=\"80\" height=\"25\" style=\"1342308352\">\n"
"<child type=\"Label\" name=\"\" caption=\"Channel:\" x=\"0\" y=\"0\" width=\"0\" height=\"0\" style=\"1342242827\">\n"
"<layout x=\"0\" width=\"1\" height=\"1\" paddingLeft=\"4\" paddingRight=\"4\" paddingTop=\"4\" paddingBottom=\"4\" y=\"0\" />\n"
"</child>\n"
"<layout attachment=\"4\" growWidth=\"1\" growHeight=\"1\" />\n"
"<layoutManager type=\"TableManager\" marginLeft=\"4\" marginRight=\"4\" marginTop=\"4\" marginBottom=\"4\" />\n"
"<child type=\"ComboBox\" name=\"channelSelect\" caption=\"\" x=\"89\" y=\"12\" width=\"80\" height=\"300\" style=\"1342243843\" id=\"165\">\n"
"<layout x=\"1\" y=\"0\" width=\"1\" height=\"1\" growWidth=\"1\" growHeight=\"0\" paddingLeft=\"4\" paddingRight=\"4\" paddingTop=\"4\" paddingBottom=\"4\" />\n"
"</child>\n"
"<child type=\"Label\" name=\"deviceLabel\" caption=\"Device\" x=\"0\" y=\"0\" width=\"0\" height=\"0\" style=\"1342242827\" id=\"190\">\n"
"<layout x=\"2\" y=\"0\" width=\"3\" height=\"1\" growWidth=\"1\" growHeight=\"0\" paddingLeft=\"4\" paddingRight=\"4\" paddingTop=\"4\" paddingBottom=\"4\" />\n"
"</child>\n"
"<child type=\"Label\" name=\"\" caption=\"Voice:\" x=\"0\" y=\"0\" width=\"0\" height=\"0\" style=\"1342242827\">\n"
"<layout x=\"0\" width=\"1\" height=\"1\" y=\"1\" paddingLeft=\"4\" paddingRight=\"4\" paddingTop=\"4\" paddingBottom=\"4\" />\n"
"</child>\n"
"<child type=\"ComboBox\" name=\"groupSelect\" caption=\"\" x=\"106\" y=\"50\" width=\"80\" height=\"300\" style=\"1344341059\" id=\"191\">\n"
"<layout y=\"1\" width=\"2\" height=\"1\" growWidth=\"1\" growHeight=\"0\" x=\"1\" paddingLeft=\"4\" paddingRight=\"4\" paddingTop=\"4\" paddingBottom=\"4\" />\n"
"</child>\n"
"<child type=\"ComboBox\" name=\"voiceSelect\" caption=\"\" x=\"249\" y=\"60\" width=\"80\" height=\"300\" style=\"1344341059\" id=\"192\">\n"
"<layout y=\"1\" width=\"2\" height=\"1\" growWidth=\"1\" growHeight=\"0\" x=\"3\" paddingLeft=\"4\" paddingRight=\"4\" paddingTop=\"4\" paddingBottom=\"4\" />\n"
"</child>\n"
"<child type=\"Label\" name=\"\" caption=\"Pan:\" x=\"0\" y=\"0\" width=\"0\" height=\"0\" style=\"1342242827\">\n"
"<layout x=\"0\" width=\"1\" height=\"1\" y=\"2\" paddingLeft=\"4\" paddingRight=\"4\" paddingTop=\"4\" paddingBottom=\"4\" />\n"
"</child>\n"
"<child type=\"TrackBar\" name=\"stereoBar\" caption=\"\" x=\"218\" y=\"58\" width=\"80\" height=\"25\" style=\"1342242816\" id=\"167\">\n"
"<layout x=\"1\" width=\"4\" height=\"1\" growWidth=\"1\" growHeight=\"0\" y=\"2\" paddingLeft=\"4\" paddingRight=\"4\" paddingTop=\"4\" paddingBottom=\"4\" />\n"
"</child>\n"
"<child type=\"Label\" name=\"\" caption=\"Volume:\" x=\"0\" y=\"0\" width=\"0\" height=\"0\" style=\"1342242827\">\n"
"<layout x=\"0\" width=\"1\" height=\"1\" y=\"3\" paddingLeft=\"4\" paddingRight=\"4\" paddingTop=\"4\" paddingBottom=\"4\" />\n"
"</child>\n"
"<child type=\"TrackBar\" name=\"volumeBar\" caption=\"\" x=\"227\" y=\"94\" width=\"80\" height=\"25\" style=\"1342242816\" id=\"164\">\n"
"<layout x=\"1\" width=\"4\" height=\"1\" growWidth=\"1\" growHeight=\"0\" y=\"3\" paddingLeft=\"4\" paddingRight=\"4\" paddingTop=\"4\" paddingBottom=\"4\" />\n"
"</child>\n"
"<child type=\"Label\" name=\"\" caption=\"Effect 1 (Reverb):\" x=\"0\" y=\"0\" width=\"0\" height=\"0\" style=\"1342242827\">\n"
"<layout x=\"0\" width=\"1\" height=\"1\" y=\"4\" paddingLeft=\"4\" paddingRight=\"4\" paddingTop=\"4\" paddingBottom=\"4\" />\n"
"</child>\n"
"<child type=\"TrackBar\" name=\"effects1Bar\" caption=\"\" x=\"236\" y=\"130\" width=\"80\" height=\"25\" style=\"1342242816\" id=\"193\">\n"
"<layout x=\"1\" width=\"4\" height=\"1\" growWidth=\"1\" growHeight=\"0\" y=\"4\" paddingLeft=\"4\" paddingRight=\"4\" paddingTop=\"4\" paddingBottom=\"4\" />\n"
"</child>\n"
"<child type=\"Label\" name=\"\" caption=\"Effect 2 (Tremolo):\" x=\"0\" y=\"0\" width=\"0\" height=\"0\" style=\"1342242827\">\n"
"<layout x=\"0\" width=\"1\" height=\"1\" y=\"5\" paddingLeft=\"4\" paddingRight=\"4\" paddingTop=\"4\" paddingBottom=\"4\" />\n"
"</child>\n"
"<child type=\"TrackBar\" name=\"effects2Bar\" caption=\"\" x=\"243\" y=\"183\" width=\"80\" height=\"25\" style=\"1342242816\" id=\"194\">\n"
"<layout x=\"1\" width=\"4\" height=\"1\" growWidth=\"1\" growHeight=\"0\" y=\"5\" paddingLeft=\"4\" paddingRight=\"4\" paddingTop=\"4\" paddingBottom=\"4\" />\n"
"</child>\n"
"<child type=\"Label\" name=\"\" caption=\"Effect 3 (Chorus):\" x=\"0\" y=\"0\" width=\"0\" height=\"0\" style=\"1342242827\">\n"
"<layout x=\"0\" width=\"1\" height=\"1\" y=\"6\" paddingLeft=\"4\" paddingRight=\"4\" paddingTop=\"4\" paddingBottom=\"4\" />\n"
"</child>\n"
"<child type=\"TrackBar\" name=\"effects3Bar\" caption=\"\" x=\"248\" y=\"246\" width=\"80\" height=\"25\" style=\"1342242816\" id=\"195\">\n"
"<layout x=\"1\" width=\"4\" height=\"1\" growWidth=\"1\" growHeight=\"0\" y=\"6\" paddingLeft=\"4\" paddingRight=\"4\" paddingTop=\"4\" paddingBottom=\"4\" />\n"
"</child>\n"
"<child type=\"Label\" name=\"\" caption=\"Effect 4 (Detune):\" x=\"0\" y=\"0\" width=\"0\" height=\"0\" style=\"1342242827\">\n"
"<layout x=\"0\" width=\"1\" height=\"1\" y=\"7\" paddingLeft=\"4\" paddingRight=\"4\" paddingTop=\"4\" paddingBottom=\"4\" />\n"
"</child>\n"
"<child type=\"TrackBar\" name=\"effects4Bar\" caption=\"\" x=\"251\" y=\"293\" width=\"80\" height=\"25\" style=\"1342242816\" id=\"196\">\n"
"<layout x=\"1\" width=\"4\" height=\"1\" growWidth=\"1\" growHeight=\"0\" y=\"7\" paddingLeft=\"4\" paddingRight=\"4\" paddingTop=\"4\" paddingBottom=\"4\" />\n"
"</child>\n"
"<child type=\"Label\" name=\"\" caption=\"Effect 5 (Phaser):\" x=\"0\" y=\"0\" width=\"0\" height=\"0\" style=\"1342242827\">\n"
"<layout x=\"0\" width=\"1\" height=\"1\" y=\"8\" paddingLeft=\"4\" paddingRight=\"4\" paddingTop=\"4\" paddingBottom=\"4\" />\n"
"</child>\n"
"<child type=\"TrackBar\" name=\"effects5Bar\" caption=\"\" x=\"253\" y=\"340\" width=\"80\" height=\"25\" style=\"1342242816\" id=\"197\">\n"
"<layout x=\"1\" width=\"4\" height=\"1\" growWidth=\"1\" growHeight=\"0\" y=\"8\" paddingLeft=\"4\" paddingRight=\"4\" paddingTop=\"4\" paddingBottom=\"4\" />\n"
"</child>\n"
"<child type=\"Label\" name=\"\" caption=\"Sound 1 (Variation):\" x=\"0\" y=\"0\" width=\"0\" height=\"0\" style=\"1342242827\">\n"
"<layout x=\"0\" width=\"1\" height=\"1\" y=\"9\" paddingLeft=\"4\" paddingRight=\"4\" paddingTop=\"4\" paddingBottom=\"4\" />\n"
"</child>\n"
"<child type=\"TrackBar\" name=\"sound1Bar\" caption=\"\" x=\"266\" y=\"381\" width=\"80\" height=\"25\" style=\"1342242816\" id=\"198\">\n"
"<layout x=\"1\" width=\"4\" height=\"1\" growWidth=\"1\" growHeight=\"0\" y=\"9\" paddingLeft=\"4\" paddingRight=\"4\" paddingTop=\"4\" paddingBottom=\"4\" />\n"
"</child>\n"
"<child type=\"Label\" name=\"\" caption=\"Sound 2 (Timbre/Harmonic):\" x=\"0\" y=\"0\" width=\"0\" height=\"0\" style=\"1342242827\">\n"
"<layout x=\"0\" width=\"1\" height=\"1\" y=\"10\" paddingLeft=\"4\" paddingRight=\"4\" paddingTop=\"4\" paddingBottom=\"4\" />\n"
"</child>\n"
"<child type=\"TrackBar\" name=\"sound2Bar\" caption=\"\" x=\"278\" y=\"421\" width=\"80\" height=\"25\" style=\"1342242816\" id=\"199\">\n"
"<layout x=\"1\" width=\"4\" height=\"1\" growWidth=\"1\" growHeight=\"0\" y=\"10\" paddingLeft=\"4\" paddingRight=\"4\" paddingTop=\"4\" paddingBottom=\"4\" />\n"
"</child>\n"
"<child type=\"Label\" name=\"\" caption=\"Sound 3 (Release Time):\" x=\"0\" y=\"0\" width=\"0\" height=\"0\" style=\"1342242827\">\n"
"<layout x=\"0\" width=\"1\" height=\"1\" y=\"11\" paddingLeft=\"4\" paddingRight=\"4\" paddingTop=\"4\" paddingBottom=\"4\" />\n"
"</child>\n"
"<child type=\"TrackBar\" name=\"sound3Bar\" caption=\"\" x=\"291\" y=\"449\" width=\"80\" height=\"25\" style=\"1342242816\" id=\"200\">\n"
"<layout x=\"1\" width=\"4\" height=\"1\" growWidth=\"1\" growHeight=\"0\" y=\"11\" paddingLeft=\"4\" paddingRight=\"4\" paddingTop=\"4\" paddingBottom=\"4\" />\n"
"</child>\n"
"<child type=\"Label\" name=\"\" caption=\"Sound 4 (Attack Time):\" x=\"0\" y=\"0\" width=\"0\" height=\"0\" style=\"1342242827\">\n"
"<layout x=\"0\" width=\"1\" height=\"1\" y=\"12\" paddingLeft=\"4\" paddingRight=\"4\" paddingTop=\"4\" paddingBottom=\"4\" />\n"
"</child>\n"
"<child type=\"TrackBar\" name=\"sound4Bar\" caption=\"\" x=\"386\" y=\"485\" width=\"80\" height=\"25\" style=\"1342242816\" id=\"201\">\n"
"<layout x=\"1\" width=\"4\" height=\"1\" growWidth=\"1\" growHeight=\"0\" y=\"12\" paddingLeft=\"4\" paddingRight=\"4\" paddingTop=\"4\" paddingBottom=\"4\" />\n"
"</child>\n"
"<child type=\"Label\" name=\"\" caption=\"Sound 5 (Brightness):\" x=\"0\" y=\"0\" width=\"0\" height=\"0\" style=\"1342242827\">\n"
"<layout x=\"0\" width=\"1\" height=\"1\" y=\"13\" paddingLeft=\"4\" paddingRight=\"4\" paddingTop=\"4\" paddingBottom=\"4\" />\n"
"</child>\n"
"<child type=\"TrackBar\" name=\"sound5Bar\" caption=\"\" x=\"265\" y=\"501\" width=\"80\" height=\"25\" style=\"1342242816\" id=\"202\">\n"
"<layout x=\"1\" width=\"4\" height=\"1\" growWidth=\"1\" growHeight=\"0\" y=\"13\" paddingLeft=\"4\" paddingRight=\"4\" paddingTop=\"4\" paddingBottom=\"4\" />\n"
"</child>\n"
"<child type=\"Label\" name=\"\" caption=\"Expression:\" x=\"0\" y=\"0\" width=\"0\" height=\"0\" style=\"1342242827\">\n"
"<layout x=\"0\" width=\"1\" height=\"1\" y=\"14\" paddingLeft=\"4\" paddingRight=\"4\" paddingTop=\"4\" paddingBottom=\"4\" />\n"
"</child>\n"
"<child type=\"TrackBar\" name=\"expressionBar\" caption=\"\" x=\"177\" y=\"489\" width=\"80\" height=\"25\" style=\"1342242816\" id=\"203\">\n"
"<layout x=\"1\" width=\"4\" height=\"1\" growWidth=\"1\" growHeight=\"0\" y=\"14\" paddingLeft=\"4\" paddingRight=\"4\" paddingTop=\"4\" paddingBottom=\"4\" />\n"
"</child>\n"
"</child>\n"
"</form>\n"
"<form name=\"MIDIeditorWindow\" style=\"281542656\" width=\"760\" height=\"585\" caption=\"Midi File Editor\" baseClass=\"ModalPopup\" id=\"138\">\n"
"<child type=\"ListBox\" name=\"eventList\" caption=\"\" x=\"8\" y=\"8\" width=\"302\" height=\"448\" style=\"1353779585\" id=\"204\" />\n"
"<child type=\"Label\" name=\"\" caption=\"Channel:\" x=\"320\" y=\"8\" width=\"0\" height=\"0\" style=\"1342242827\" />\n"
"<child type=\"ComboBox\" name=\"channelSelect\" caption=\"\" x=\"424\" y=\"8\" width=\"80\" height=\"300\" style=\"1344340995\" id=\"165\" />\n"
"<child type=\"Label\" name=\"\" caption=\"Track:\" x=\"528\" y=\"8\" width=\"0\" height=\"0\" style=\"1342242827\" />\n"
"<child type=\"ComboBox\" name=\"trackSelect\" caption=\"\" x=\"576\" y=\"8\" width=\"168\" height=\"300\" style=\"1344341059\" id=\"205\" />\n"
"<child type=\"Label\" name=\"\" caption=\"Bar.Beat.Time:\" x=\"320\" y=\"40\" width=\"0\" height=\"0\" style=\"1342242827\" />\n"
"<child type=\"EditControl\" name=\"timeCodeEdit\" caption=\"\" x=\"424\" y=\"40\" width=\"80\" height=\"25\" style=\"1350631808\" id=\"206\" />\n"
"<child type=\"RadioButton\" name=\"voiceRadio\" caption=\"Voice\" x=\"320\" y=\"80\" width=\"0\" height=\"0\" style=\"1342242825\" id=\"207\" />\n"
"<child type=\"ComboBox\" name=\"groupSelect\" caption=\"\" x=\"336\" y=\"104\" width=\"203\" height=\"300\" style=\"1344341059\" id=\"191\" />\n"
"<child type=\"ComboBox\" name=\"voiceSelect\" caption=\"\" x=\"544\" y=\"104\" width=\"203\" height=\"300\" style=\"1344341059\" id=\"192\" />\n"
"<child type=\"RadioButton\" name=\"noteRadio\" caption=\"Note / Volume / Length (ms) / Velocity\" x=\"320\" y=\"136\" width=\"0\" height=\"0\" style=\"1342242825\" id=\"208\" />\n"
"<child type=\"ComboBox\" name=\"noteSelect\" caption=\"\" x=\"336\" y=\"160\" width=\"60\" height=\"300\" style=\"1344341059\" id=\"209\" />\n"
"<child type=\"EditControl\" name=\"noteVolumeEdit\" caption=\"000\" x=\"400\" y=\"160\" width=\"35\" height=\"23\" style=\"1350631808\" id=\"210\" />\n"
"<child type=\"EditControl\" name=\"noteLengthEdit\" caption=\"000\" x=\"440\" y=\"160\" width=\"35\" height=\"23\" style=\"1350631808\" id=\"211\" />\n"
"<child type=\"EditControl\" name=\"noteTimeEdit\" caption=\"000\" x=\"480\" y=\"160\" width=\"35\" height=\"23\" style=\"1350631808\" id=\"212\" />\n"
"<child type=\"PushButton\" name=\"full\" caption=\"\" x=\"520\" y=\"160\" width=\"23\" height=\"23\" style=\"1342242944\" id=\"213\" />\n"
"<child type=\"PushButton\" name=\"half\" caption=\"\" x=\"544\" y=\"160\" width=\"23\" height=\"23\" style=\"1342242944\" id=\"214\" />\n"
"<child type=\"PushButton\" name=\"quarter\" caption=\"\" x=\"568\" y=\"160\" width=\"23\" height=\"23\" style=\"1342242944\" id=\"215\" />\n"
"<child type=\"PushButton\" name=\"eighth\" caption=\"\" x=\"592\" y=\"160\" width=\"23\" height=\"23\" style=\"1342242944\" id=\"216\" />\n"
"<child type=\"RadioButton\" name=\"controllerRadio\" caption=\"Controller\" x=\"320\" y=\"192\" width=\"0\" height=\"0\" style=\"1342242825\" id=\"217\" />\n"
"<child type=\"ComboBox\" name=\"controllerSelect\" caption=\"\" x=\"336\" y=\"216\" width=\"203\" height=\"300\" style=\"1344341059\" id=\"218\" />\n"
"<child type=\"EditControl\" name=\"controllerEdit\" caption=\"\" x=\"544\" y=\"216\" width=\"35\" height=\"23\" style=\"1350631808\" id=\"219\" />\n"
"<child type=\"RadioButton\" name=\"otherRadio\" caption=\"Other\" x=\"320\" y=\"248\" width=\"0\" height=\"0\" style=\"1342242825\" id=\"220\" />\n"
"<child type=\"ComboBox\" name=\"messageSelect\" caption=\"\" x=\"336\" y=\"272\" width=\"203\" height=\"300\" style=\"1344341059\" id=\"221\" />\n"
"<child type=\"EditControl\" name=\"data1Edit\" caption=\"000\" x=\"544\" y=\"272\" width=\"35\" height=\"23\" style=\"1350631808\" id=\"222\" />\n"
"<child type=\"EditControl\" name=\"data2Edit\" caption=\"000\" x=\"584\" y=\"272\" width=\"35\" height=\"23\" style=\"1350631808\" id=\"223\" />\n"
"<child type=\"RadioButton\" name=\"sysExRadio\" caption=\"System Exclusive\" x=\"320\" y=\"304\" width=\"0\" height=\"0\" style=\"1342242825\" id=\"224\" />\n"
"<child type=\"PushButton\" name=\"saveEntryButton\" caption=\"Apply\" x=\"320\" y=\"336\" width=\"80\" height=\"25\" style=\"1342242816\" id=\"225\" />\n"
"<child type=\"PushButton\" name=\"newEntryButton\" caption=\"New\" x=\"408\" y=\"336\" width=\"80\" height=\"25\" style=\"1342242816\" id=\"226\" />\n"
"<child type=\"PushButton\" name=\"deleteEntryButton\" caption=\"Delete\" x=\"496\" y=\"336\" width=\"80\" height=\"25\" style=\"1342242816\" id=\"227\" />\n"
"<child type=\"PushButton\" name=\"instrumentButton\" caption=\"Instrument\" x=\"320\" y=\"368\" width=\"80\" height=\"25\" style=\"1342242816\" id=\"228\" />\n"
"<child type=\"Label\" name=\"\" caption=\"Edit Multiple Entries:\" x=\"8\" y=\"472\" width=\"0\" height=\"0\" style=\"1342242827\" />\n"
"<child type=\"PushButton\" name=\"volumeButton\" caption=\"Volume\" x=\"8\" y=\"496\" width=\"80\" height=\"25\" style=\"1342242816\" id=\"229\" />\n"
"<child type=\"PushButton\" name=\"swapChannelsButton\" caption=\"Swap Ch.\" x=\"96\" y=\"496\" width=\"80\" height=\"25\" style=\"1342242816\" id=\"230\" />\n"
"<child type=\"PushButton\" name=\"repeatButton\" caption=\"Repeat\" x=\"184\" y=\"496\" width=\"80\" height=\"25\" style=\"1342242816\" id=\"231\" />\n"
"<child type=\"EditControl\" name=\"transposeEdit\" caption=\"000\" x=\"272\" y=\"496\" width=\"35\" height=\"25\" style=\"1350631808\" id=\"232\" />\n"
"<child type=\"PushButton\" name=\"transposeButton\" caption=\"Transp.\" x=\"312\" y=\"496\" width=\"80\" height=\"25\" style=\"1342242816\" id=\"233\" />\n"
"<child type=\"EditControl\" name=\"moveEdit\" caption=\"000\" x=\"400\" y=\"496\" width=\"35\" height=\"25\" style=\"1350631808\" id=\"234\" />\n"
"<child type=\"PushButton\" name=\"moveButton\" caption=\"Move\" x=\"440\" y=\"496\" width=\"80\" height=\"25\" style=\"1342242816\" id=\"235\" />\n"
"<child type=\"PushButton\" name=\"copyButton\" caption=\"Copy\" x=\"528\" y=\"496\" width=\"80\" height=\"25\" style=\"1342242816\" id=\"236\" />\n"
"<child type=\"Label\" name=\"\" caption=\"\" x=\"8\" y=\"540\" width=\"736\" height=\"2\" style=\"1342242832\" />\n"
"<child type=\"PushButton\" name=\"IDOK\" caption=\"OK\" x=\"8\" y=\"552\" width=\"80\" height=\"25\" style=\"1342242816\" id=\"1\" />\n"
"<child type=\"PushButton\" name=\"IDCANCEL\" caption=\"Cancel\" x=\"96\" y=\"552\" width=\"80\" height=\"25\" style=\"1342242816\" id=\"2\" />\n"
"<child type=\"PushButton\" name=\"playButton\" caption=\"Play\" x=\"664\" y=\"552\" width=\"80\" height=\"25\" style=\"1342242816\" id=\"148\" />\n"
"</form>\n"
"<form name=\"MIDIrecorderWindow\" style=\"281673728\" width=\"640\" height=\"400\" baseClass=\"OverlappedWindow\" id=\"139\" caption=\"MIDI Recorder\" bgColor=\"COLOR_BTNFACE\" bg_red=\"0\" bg_green=\"0\" bg_blue=\"0\">\n"
"<child type=\"Label\" name=\"\" caption=\"Out: Dev/Chan Note Velocity\" x=\"8\" y=\"0\" width=\"0\" height=\"0\" style=\"1342242827\" />\n"
"<child type=\"Label\" name=\"\" caption=\"In: Dev/Chan Note Velocity\" x=\"256\" y=\"0\" width=\"0\" height=\"0\" style=\"1342242827\" />\n"
"<child type=\"FrameChild\" name=\"pianoFrame\" caption=\"\" x=\"8\" y=\"16\" width=\"496\" height=\"240\" style=\"1342308352\" id=\"237\" bgColor=\"WHITE_BRUSH\" bg_red=\"0\" bg_green=\"0\" bg_blue=\"0\">\n"
"<layoutManager type=\"SingleChildManager\" marginLeft=\"0\" marginRight=\"0\" marginTop=\"0\" marginBottom=\"0\" />\n"
"</child>\n"
"<child type=\"CheckBox\" name=\"MIDIthrough\" caption=\"Midi Through\" x=\"520\" y=\"16\" width=\"0\" height=\"0\" style=\"1342242819\" id=\"238\" />\n"
"<child type=\"CheckBox\" name=\"filterEnabledCheckBox\" caption=\"Filter\" x=\"520\" y=\"40\" width=\"0\" height=\"0\" style=\"1342242819\" id=\"239\" />\n"
"<child type=\"PushButton\" name=\"channelsButton\" caption=\"Channels\" x=\"520\" y=\"72\" width=\"80\" height=\"25\" style=\"1342242816\" id=\"240\" />\n"
"<child type=\"PushButton\" name=\"filterButton\" caption=\"Filter\" x=\"520\" y=\"104\" width=\"80\" height=\"25\" style=\"1342242816\" id=\"241\" />\n"
"<child type=\"PushButton\" name=\"loadSettingButton\" caption=\"Load...\" x=\"520\" y=\"136\" width=\"80\" height=\"25\" style=\"1342242816\" id=\"242\" />\n"
"<child type=\"PushButton\" name=\"saveSettingButton\" caption=\"Save...\" x=\"520\" y=\"168\" width=\"80\" height=\"25\" style=\"1342242816\" id=\"243\" />\n"
"<child type=\"CheckBox\" name=\"MIDIholdNote\" caption=\"Hold\" x=\"520\" y=\"208\" width=\"0\" height=\"0\" style=\"1342242819\" id=\"244\" />\n"
"<child type=\"Label\" name=\"\" caption=\"MIDI-In:\" x=\"8\" y=\"296\" width=\"0\" height=\"0\" style=\"1342242827\" />\n"
"<child type=\"ComboBox\" name=\"MIDIplaySelect\" caption=\"\" x=\"144\" y=\"264\" width=\"272\" height=\"200\" style=\"1344341059\" id=\"182\" />\n"
"<child type=\"PushButton\" name=\"playButton\" caption=\"Play\" x=\"424\" y=\"264\" width=\"80\" height=\"21\" style=\"1342242816\" id=\"148\" />\n"
"<child type=\"Label\" name=\"\" caption=\"MIDI-Out:\" x=\"8\" y=\"264\" width=\"0\" height=\"0\" style=\"1342242827\" />\n"
"<child type=\"ComboBox\" name=\"MIDIrecSelect\" caption=\"\" x=\"144\" y=\"296\" width=\"274\" height=\"200\" style=\"1344341059\" id=\"178\" />\n"
"<child type=\"PushButton\" name=\"recordButton\" caption=\"Record\" x=\"424\" y=\"296\" width=\"80\" height=\"21\" style=\"1342242816\" id=\"245\" />\n"
"<child type=\"CheckBox\" name=\"authoRhytmCheck\" caption=\"Auto Rhythm.\" x=\"512\" y=\"296\" width=\"0\" height=\"0\" style=\"1342242819\" id=\"246\" />\n"
"<child type=\"Label\" name=\"\" caption=\"Tempo (BPM/Time):\" x=\"8\" y=\"328\" width=\"0\" height=\"0\" style=\"1342242827\" />\n"
"<child type=\"EditControl\" name=\"bpmEdit\" caption=\"120\" x=\"144\" y=\"328\" width=\"32\" height=\"21\" style=\"1350631808\" id=\"150\" />\n"
"<child type=\"EditControl\" name=\"numeratorEdit\" caption=\"4\" x=\"184\" y=\"328\" width=\"32\" height=\"21\" style=\"1350631808\" id=\"247\" />\n"
"<child type=\"ComboBox\" name=\"denominatorSelect\" caption=\"\" x=\"216\" y=\"328\" width=\"40\" height=\"64\" style=\"1350632451\" id=\"248\">\n"
"<items>\n"
"<item>4</item>\n"
"<item>8</item>\n"
"<item>16</item>\n"
"<item>32</item>\n"
"</items>\n"
"</child>\n"
"<child type=\"PushButton\" name=\"convertBPMbutton\" caption=\"Apply\" x=\"272\" y=\"328\" width=\"80\" height=\"21\" style=\"1342242816\" id=\"249\" />\n"
"<child type=\"PushButton\" name=\"metronomButton\" caption=\"Metronome\" x=\"352\" y=\"328\" width=\"80\" height=\"21\" style=\"1342242816\" id=\"250\" />\n"
"<child type=\"PushButton\" name=\"IDCANCEL\" caption=\"Close\" x=\"8\" y=\"368\" width=\"80\" height=\"25\" style=\"1342242816\" id=\"2\" />\n"
"<child type=\"PushButton\" name=\"loadMidiButton\" caption=\"Load...\" x=\"96\" y=\"368\" width=\"80\" height=\"25\" style=\"1342242816\" id=\"251\" />\n"
"<child type=\"PushButton\" name=\"saveMidiButton\" caption=\"Save...\" x=\"184\" y=\"368\" width=\"80\" height=\"25\" style=\"1342242816\" id=\"252\" />\n"
"<child type=\"PushButton\" name=\"editButton\" caption=\"Edit...\" x=\"272\" y=\"368\" width=\"80\" height=\"25\" style=\"1342242816\" id=\"253\" />\n"
"<child type=\"PushButton\" name=\"instrumentButton\" caption=\"Instrument\" x=\"544\" y=\"368\" width=\"80\" height=\"25\" style=\"1342242816\" id=\"228\" />\n"
"</form>\n"
"<form name=\"MIDIdrumWindow\" style=\"281673728\" width=\"1016\" height=\"650\" baseClass=\"OverlappedWindow\" id=\"140\" caption=\"Drum Editor\" bgColor=\"COLOR_BTNFACE\" bg_red=\"0\" bg_green=\"0\" bg_blue=\"0\">\n"
"<child type=\"Label\" name=\"\" caption=\"MIDI-Out:\" x=\"8\" y=\"8\" width=\"0\" height=\"0\" style=\"1342242827\" />\n"
"<child type=\"ComboBox\" name=\"MIDIplaySelect\" caption=\"\" x=\"128\" y=\"8\" width=\"784\" height=\"200\" style=\"1344341059\" id=\"182\" />\n"
"<child type=\"PushButton\" name=\"playButton\" caption=\"Play\" x=\"920\" y=\"8\" width=\"80\" height=\"25\" style=\"1342242816\" id=\"148\" />\n"
"<child type=\"Label\" name=\"\" caption=\"Tempo/Length:\" x=\"8\" y=\"40\" width=\"0\" height=\"0\" style=\"1342242827\" />\n"
"<child type=\"EditControl\" name=\"bpmEdit\" caption=\"000\" x=\"128\" y=\"40\" width=\"32\" height=\"21\" style=\"1350631808\" id=\"150\" />\n"
"<child type=\"EditControl\" name=\"numeratorEdit\" caption=\"000\" x=\"160\" y=\"40\" width=\"32\" height=\"21\" style=\"1350631808\" id=\"247\" />\n"
"<child type=\"ComboBox\" name=\"denominatorSelect\" caption=\"\" x=\"192\" y=\"40\" width=\"40\" height=\"100\" style=\"1342243843\" id=\"248\">\n"
"<items>\n"
"<item>4</item>\n"
"<item>8</item>\n"
"<item>16</item>\n"
"<item>32</item>\n"
"</items>\n"
"</child>\n"
"<child type=\"EditControl\" name=\"lengthEdit\" caption=\"Edit\" x=\"240\" y=\"40\" width=\"32\" height=\"21\" style=\"1350631808\" id=\"254\" />\n"
"<child type=\"UpDownButton\" name=\"lengthSelector\" caption=\"\" x=\"256\" y=\"40\" width=\"21\" height=\"21\" style=\"1342242870\" id=\"255\" />\n"
"<child type=\"PushButton\" name=\"convertBPMbutton\" caption=\"Apply\" x=\"288\" y=\"40\" width=\"80\" height=\"21\" style=\"1342242816\" id=\"249\" />\n"
"<child type=\"Label\" name=\"\" caption=\"Grid:\" x=\"632\" y=\"40\" width=\"0\" height=\"0\" style=\"1342242827\" />\n"
"<child type=\"ComboBox\" name=\"gridSelect\" caption=\"\" x=\"680\" y=\"40\" width=\"80\" height=\"100\" style=\"1342243843\" id=\"256\">\n"
"<items />\n"
"</child>\n"
"<child type=\"Label\" name=\"\" caption=\"Length:\" x=\"776\" y=\"40\" width=\"0\" height=\"0\" style=\"1342242827\" />\n"
"<child type=\"ComboBox\" name=\"lengthSelect\" caption=\"\" x=\"832\" y=\"40\" width=\"80\" height=\"100\" style=\"1342243843\" id=\"257\">\n"
"<items />\n"
"</child>\n"
"<child type=\"Label\" name=\"clockText\" caption=\"00:00:00 000\" x=\"920\" y=\"40\" width=\"0\" height=\"0\" style=\"1342242827\" id=\"149\" />\n"
"<child type=\"Label\" name=\"\" caption=\"Instrument:\" x=\"8\" y=\"72\" width=\"0\" height=\"0\" style=\"1342242827\" />\n"
"<child type=\"ComboBox\" name=\"voiceSelect\" caption=\"\" x=\"128\" y=\"72\" width=\"783\" height=\"300\" style=\"1344341059\" id=\"192\" />\n"
"<child type=\"PushButton\" name=\"addNewPatternButton\" caption=\"Add\" x=\"920\" y=\"72\" width=\"80\" height=\"25\" style=\"1342242816\" id=\"258\" />\n"
"<child type=\"Label\" name=\"\" caption=\"Instruments:\" x=\"8\" y=\"112\" width=\"0\" height=\"0\" style=\"1342242827\" />\n"
"<child type=\"ComboBox\" name=\"playModeSelect\" caption=\"\" x=\"8\" y=\"136\" width=\"112\" height=\"62\" style=\"1342243843\" id=\"259\">\n"
"<items>\n"
"<item>All</item>\n"
"<item>Mute</item>\n"
"<item>Solo</item>\n"
"</items>\n"
"</child>\n"
"<child type=\"PushButton\" name=\"volumeButton\" caption=\"Volume\" x=\"8\" y=\"168\" width=\"112\" height=\"24\" style=\"1342242816\" id=\"229\" />\n"
"<child type=\"PushButton\" name=\"panButton\" caption=\"Pan\" x=\"8\" y=\"200\" width=\"112\" height=\"24\" style=\"1342242816\" id=\"260\" />\n"
"<child type=\"PushButton\" name=\"deleteEventsButton\" caption=\"Delete\" x=\"8\" y=\"232\" width=\"112\" height=\"24\" style=\"1342242816\" id=\"261\" />\n"
"<child type=\"PushButton\" name=\"copyEventsButton\" caption=\"Copy\" x=\"8\" y=\"264\" width=\"112\" height=\"24\" style=\"1342242816\" id=\"262\" />\n"
"<child type=\"PushButton\" name=\"moveEventsButton\" caption=\"Move\" x=\"8\" y=\"296\" width=\"112\" height=\"24\" style=\"1342242816\" id=\"263\" />\n"
"<child type=\"PushButton\" name=\"insertBreakButton\" caption=\"Break\" x=\"8\" y=\"328\" width=\"112\" height=\"24\" style=\"1342242816\" id=\"264\" />\n"
"<child type=\"PushButton\" name=\"deleteAllButton\" caption=\"Delete All\" x=\"8\" y=\"512\" width=\"112\" height=\"24\" style=\"1342242816\" id=\"265\" />\n"
"<child type=\"FrameChild\" name=\"patternFrame\" caption=\"\" x=\"128\" y=\"104\" width=\"872\" height=\"434\" style=\"1342308352\" id=\"266\">\n"
"<layoutManager type=\"SingleChildManager\" marginLeft=\"0\" marginRight=\"0\" marginTop=\"0\" marginBottom=\"0\" />\n"
"</child>\n"
"<child type=\"Label\" name=\"\" caption=\"Volume:\" x=\"8\" y=\"544\" width=\"0\" height=\"0\" style=\"1342242827\" />\n"
"<child type=\"TrackBar\" name=\"volumeBar\" caption=\"\" x=\"128\" y=\"544\" width=\"872\" height=\"25\" style=\"1342242816\" id=\"164\" />\n"
"<child type=\"Label\" name=\"\" caption=\"Velocity:\" x=\"8\" y=\"576\" width=\"80\" height=\"25\" style=\"1342242827\" />\n"
"<child type=\"TrackBar\" name=\"timeBar\" caption=\"\" x=\"128\" y=\"576\" width=\"872\" height=\"25\" style=\"1342242816\" id=\"267\" />\n"
"<child type=\"PushButton\" name=\"IDCANCEL\" caption=\"Close\" x=\"8\" y=\"616\" width=\"80\" height=\"25\" style=\"1342242816\" id=\"2\" />\n"
"<child type=\"PushButton\" name=\"loadDrumsButton\" caption=\"Load...\" x=\"96\" y=\"616\" width=\"80\" height=\"25\" style=\"1342242816\" id=\"268\" />\n"
"<child type=\"PushButton\" name=\"saveDrumsButton\" caption=\"Save...\" x=\"184\" y=\"616\" width=\"80\" height=\"25\" style=\"1342242816\" id=\"269\" />\n"
"<child type=\"Label\" name=\"\" caption=\"Shift+click: De/select, Right click: Copy Volume, Ctrl+right click: Change Volume\" x=\"272\" y=\"616\" width=\"0\" height=\"0\" style=\"1342242827\" />\n"
"</form>\n"
"<form name=\"MIDIsynthesizerWindow\" style=\"281673728\" width=\"942\" height=\"590\" id=\"141\" caption=\"Synthesizer\" baseClass=\"OverlappedWindow\" bgColor=\"COLOR_BTNFACE\" bg_red=\"0\" bg_green=\"0\" bg_blue=\"0\">\n"
"<child type=\"GroupBox\" name=\"\" caption=\"Synth\" x=\"8\" y=\"8\" width=\"920\" height=\"72\" style=\"1342373895\" />\n"
"<child type=\"RadioButton\" name=\"radioSynthA\" caption=\"A\" x=\"16\" y=\"24\" width=\"0\" height=\"0\" style=\"1342242825\" id=\"270\" />\n"
"<child type=\"RadioButton\" name=\"radioSynthB\" caption=\"B\" x=\"48\" y=\"24\" width=\"0\" height=\"0\" style=\"1342242825\" id=\"271\" />\n"
"<child type=\"RadioButton\" name=\"radioSynthC\" caption=\"C\" x=\"80\" y=\"24\" width=\"0\" height=\"0\" style=\"1342242825\" id=\"272\" />\n"
"<child type=\"RadioButton\" name=\"radioSynthD\" caption=\"D\" x=\"112\" y=\"24\" width=\"0\" height=\"0\" style=\"1342242825\" id=\"273\" />\n"
"<child type=\"CheckBox\" name=\"checkMuteA\" caption=\" \" x=\"16\" y=\"40\" width=\"0\" height=\"0\" style=\"1342242819\" id=\"274\" />\n"
"<child type=\"CheckBox\" name=\"checkMuteB\" caption=\" \" x=\"48\" y=\"40\" width=\"0\" height=\"0\" style=\"1342242819\" id=\"275\" />\n"
"<child type=\"CheckBox\" name=\"checkMuteC\" caption=\" \" x=\"80\" y=\"40\" width=\"0\" height=\"0\" style=\"1342242819\" id=\"276\" />\n"
"<child type=\"CheckBox\" name=\"checkMuteD\" caption=\" \" x=\"112\" y=\"40\" width=\"0\" height=\"0\" style=\"1342242819\" id=\"277\" />\n"
"<child type=\"Label\" name=\"\" caption=\"Master Volume:\" x=\"168\" y=\"24\" width=\"0\" height=\"0\" style=\"1342242827\" />\n"
"<child type=\"Label\" name=\"\" caption=\"Tone:\" x=\"552\" y=\"24\" width=\"0\" height=\"0\" style=\"1342242827\" />\n"
"<child type=\"TrackBar\" name=\"masterVolume\" caption=\"\" x=\"168\" y=\"40\" width=\"368\" height=\"32\" style=\"1342242816\" id=\"278\" />\n"
"<child type=\"TrackBar\" name=\"presenceValue\" caption=\"\" x=\"552\" y=\"40\" width=\"368\" height=\"32\" style=\"1342242816\" id=\"279\" />\n"
"<child type=\"GroupBox\" name=\"\" caption=\"Main OSC\" x=\"8\" y=\"88\" width=\"920\" height=\"88\" style=\"1342373895\" />\n"
"<child type=\"Label\" name=\"\" caption=\"Volume:\" x=\"16\" y=\"104\" width=\"0\" height=\"0\" style=\"1342242827\" />\n"
"<child type=\"TrackBar\" name=\"mainVolume\" caption=\"\" x=\"112\" y=\"104\" width=\"368\" height=\"32\" style=\"1342242816\" id=\"280\" />\n"
"<child type=\"Label\" name=\"\" caption=\"Pan:\" x=\"496\" y=\"104\" width=\"0\" height=\"0\" style=\"1342242827\" />\n"
"<child type=\"TrackBar\" name=\"mainPan\" caption=\"\" x=\"552\" y=\"104\" width=\"368\" height=\"32\" style=\"1342242816\" id=\"281\" />\n"
"<child type=\"Label\" name=\"\" caption=\"Pitch:\" x=\"16\" y=\"136\" width=\"0\" height=\"0\" style=\"1342242827\" />\n"
"<child type=\"Label\" name=\"mainPitchValue\" caption=\"000\" x=\"80\" y=\"136\" width=\"32\" height=\"16\" style=\"1342242827\" id=\"282\" bgColor=\"COLOR_BTNFACE\" bg_red=\"0\" bg_green=\"0\" bg_blue=\"0\" />\n"
"<child type=\"TrackBar\" name=\"mainPitch\" caption=\"\" x=\"112\" y=\"136\" width=\"368\" height=\"32\" style=\"1342242816\" id=\"283\" />\n"
"<child type=\"Label\" name=\"\" caption=\"Phase:\" x=\"496\" y=\"136\" width=\"0\" height=\"0\" style=\"1342242827\" />\n"
"<child type=\"TrackBar\" name=\"mainPhase\" caption=\"\" x=\"552\" y=\"136\" width=\"368\" height=\"32\" style=\"1342242816\" id=\"284\" />\n"
"<child type=\"GroupBox\" name=\"\" caption=\"SUB OSCs\" x=\"8\" y=\"184\" width=\"920\" height=\"104\" style=\"1342373895\" />\n"
"<child type=\"Label\" name=\"\" caption=\"Phase:\" x=\"16\" y=\"200\" width=\"0\" height=\"0\" style=\"1342242827\" />\n"
"<child type=\"Label\" name=\"\" caption=\"Volume:\" x=\"320\" y=\"200\" width=\"0\" height=\"0\" style=\"1342242827\" />\n"
"<child type=\"Label\" name=\"\" caption=\"Pitch:\" x=\"624\" y=\"200\" width=\"0\" height=\"0\" style=\"1342242827\" />\n"
"<child type=\"TrackBar\" name=\"sub1phase\" caption=\"\" x=\"16\" y=\"216\" width=\"296\" height=\"32\" style=\"1342242816\" id=\"285\" />\n"
"<child type=\"TrackBar\" name=\"sub1volume\" caption=\"\" x=\"320\" y=\"216\" width=\"296\" height=\"32\" style=\"1342242816\" id=\"286\" />\n"
"<child type=\"TrackBar\" name=\"sub1pitch\" caption=\"\" x=\"624\" y=\"216\" width=\"296\" height=\"32\" style=\"1342242816\" id=\"287\" />\n"
"<child type=\"TrackBar\" name=\"sub2phase\" caption=\"\" x=\"16\" y=\"248\" width=\"296\" height=\"32\" style=\"1342242816\" id=\"288\" />\n"
"<child type=\"TrackBar\" name=\"sub2volume\" caption=\"\" x=\"320\" y=\"248\" width=\"296\" height=\"32\" style=\"1342242816\" id=\"289\" />\n"
"<child type=\"TrackBar\" name=\"sub2pitch\" caption=\"\" x=\"624\" y=\"248\" width=\"296\" height=\"32\" style=\"1342242816\" id=\"290\" />\n"
"<child type=\"GroupBox\" name=\"\" caption=\"AM LFO\" x=\"8\" y=\"288\" width=\"920\" height=\"72\" style=\"1342373895\" />\n"
"<child type=\"RadioButton\" name=\"amSinus\" caption=\"Sinus\" x=\"16\" y=\"304\" width=\"0\" height=\"0\" style=\"1342242825\" id=\"291\" />\n"
"<child type=\"RadioButton\" name=\"amAscending\" caption=\"Ascending\" x=\"80\" y=\"304\" width=\"0\" height=\"0\" style=\"1342242825\" id=\"292\" />\n"
"<child type=\"RadioButton\" name=\"amDescending\" caption=\"Descending\" x=\"176\" y=\"304\" width=\"0\" height=\"0\" style=\"1342242825\" id=\"293\" />\n"
"<child type=\"RadioButton\" name=\"amTriangle\" caption=\"Triangel\" x=\"280\" y=\"304\" width=\"0\" height=\"0\" style=\"1342242825\" id=\"294\" />\n"
"<child type=\"RadioButton\" name=\"amRectangle\" caption=\"Square\" x=\"368\" y=\"304\" width=\"0\" height=\"0\" style=\"1342242825\" id=\"295\" />\n"
"<child type=\"Label\" name=\"\" caption=\"Frequency:\" x=\"16\" y=\"320\" width=\"0\" height=\"0\" style=\"1342242827\" />\n"
"<child type=\"EditControl\" name=\"amFrequency\" caption=\"\" x=\"88\" y=\"320\" width=\"80\" height=\"25\" style=\"1350631808\" id=\"296\" />\n"
"<child type=\"Label\" name=\"\" caption=\"Range:\" x=\"176\" y=\"320\" width=\"0\" height=\"0\" style=\"1342242827\" />\n"
"<child type=\"TrackBar\" name=\"amRange\" caption=\"\" x=\"232\" y=\"320\" width=\"688\" height=\"32\" style=\"1342242816\" id=\"297\" />\n"
"<child type=\"GroupBox\" name=\"\" caption=\"FM LFO\" x=\"8\" y=\"360\" width=\"920\" height=\"72\" style=\"1342373895\" />\n"
"<child type=\"RadioButton\" name=\"fmSinus\" caption=\"Sinus\" x=\"16\" y=\"376\" width=\"0\" height=\"0\" style=\"1342242825\" id=\"298\" />\n"
"<child type=\"RadioButton\" name=\"fmAscending\" caption=\"Ascending\" x=\"80\" y=\"376\" width=\"0\" height=\"0\" style=\"1342242825\" id=\"299\" />\n"
"<child type=\"RadioButton\" name=\"fmDescending\" caption=\"Descending\" x=\"176\" y=\"376\" width=\"0\" height=\"0\" style=\"1342242825\" id=\"300\" />\n"
"<child type=\"RadioButton\" name=\"fmTriangle\" caption=\"Triangel\" x=\"280\" y=\"376\" width=\"0\" height=\"0\" style=\"1342242825\" id=\"301\" />\n"
"<child type=\"RadioButton\" name=\"fmRectangle\" caption=\"Square\" x=\"368\" y=\"376\" width=\"0\" height=\"0\" style=\"1342242825\" id=\"302\" />\n"
"<child type=\"Label\" name=\"\" caption=\"Frequency:\" x=\"16\" y=\"392\" width=\"0\" height=\"0\" style=\"1342242827\" />\n"
"<child type=\"EditControl\" name=\"fmFrequency\" caption=\"\" x=\"88\" y=\"392\" width=\"80\" height=\"25\" style=\"1350631808\" id=\"303\" />\n"
"<child type=\"Label\" name=\"\" caption=\"Range:\" x=\"176\" y=\"392\" width=\"0\" height=\"0\" style=\"1342242827\" />\n"
"<child type=\"TrackBar\" name=\"fmRange\" caption=\"\" x=\"232\" y=\"392\" width=\"688\" height=\"32\" style=\"1342242816\" id=\"304\" />\n"
"<child type=\"GroupBox\" name=\"\" caption=\"Envelope\" x=\"8\" y=\"432\" width=\"920\" height=\"122\" style=\"1342373895\" />\n"
"<child type=\"CheckBox\" name=\"envEnabled\" caption=\"Enable\" x=\"16\" y=\"448\" width=\"0\" height=\"0\" style=\"1342242819\" id=\"305\" />\n"
"<child type=\"Label\" name=\"\" caption=\"Delay:\" x=\"16\" y=\"464\" width=\"0\" height=\"0\" style=\"1342242827\" />\n"
"<child type=\"EditControl\" name=\"envDelay\" caption=\"\" x=\"80\" y=\"464\" width=\"40\" height=\"24\" style=\"1350631808\" id=\"306\" />\n"
"<child type=\"Label\" name=\"\" caption=\"Initial Volume:\" x=\"128\" y=\"464\" width=\"0\" height=\"0\" style=\"1342242827\" />\n"
"<child type=\"TrackBar\" name=\"envVolume1\" caption=\"\" x=\"232\" y=\"464\" width=\"544\" height=\"32\" style=\"1342242816\" id=\"307\" />\n"
"<child type=\"Label\" name=\"\" caption=\"Sustain Rate:\" x=\"784\" y=\"472\" width=\"0\" height=\"0\" style=\"1342242827\" />\n"
"<child type=\"EditControl\" name=\"envSustainRate\" caption=\"\" x=\"880\" y=\"472\" width=\"40\" height=\"24\" style=\"1350631808\" id=\"308\" />\n"
"<child type=\"Label\" name=\"\" caption=\"Attack:\" x=\"16\" y=\"488\" width=\"0\" height=\"0\" style=\"1342242827\" />\n"
"<child type=\"EditControl\" name=\"envAttack\" caption=\"\" x=\"80\" y=\"488\" width=\"40\" height=\"24\" style=\"1350631808\" id=\"309\" />\n"
"<child type=\"Label\" name=\"\" caption=\"Decay:\" x=\"16\" y=\"512\" width=\"0\" height=\"0\" style=\"1342242827\" />\n"
"<child type=\"EditControl\" name=\"envDecay\" caption=\"\" x=\"80\" y=\"512\" width=\"40\" height=\"24\" style=\"1350631808\" id=\"310\" />\n"
"<child type=\"Label\" name=\"\" caption=\"Decay Volume:\" x=\"128\" y=\"512\" width=\"0\" height=\"0\" style=\"1342242827\" />\n"
"<child type=\"TrackBar\" name=\"envVolume2\" caption=\"\" x=\"232\" y=\"512\" width=\"544\" height=\"32\" style=\"1342242816\" id=\"311\" />\n"
"<child type=\"Label\" name=\"\" caption=\"Release Rate:\" x=\"784\" y=\"520\" width=\"0\" height=\"0\" style=\"1342242827\" />\n"
"<child type=\"EditControl\" name=\"envReleaseRate\" caption=\"\" x=\"880\" y=\"520\" width=\"40\" height=\"24\" style=\"1350631808\" id=\"312\" />\n"
"<child type=\"PushButton\" name=\"saveSynthButton\" caption=\"Save...\" x=\"8\" y=\"560\" width=\"80\" height=\"25\" style=\"1342242816\" id=\"313\" />\n"
"<child type=\"PushButton\" name=\"loadSynthButton\" caption=\"Load...\" x=\"96\" y=\"560\" width=\"80\" height=\"25\" style=\"1342242816\" id=\"314\" />\n"
"<child type=\"PushButton\" name=\"IDCANCEL\" caption=\"Close\" x=\"184\" y=\"560\" width=\"80\" height=\"25\" style=\"1342242816\" id=\"2\" />\n"
"<child type=\"CheckBox\" name=\"check1\" caption=\"A\" x=\"776\" y=\"560\" width=\"32\" height=\"16\" style=\"1342242819\" id=\"315\" />\n"
"<child type=\"CheckBox\" name=\"check2\" caption=\"B\" x=\"816\" y=\"560\" width=\"32\" height=\"16\" style=\"1342242819\" id=\"316\" />\n"
"<child type=\"CheckBox\" name=\"check3\" caption=\"C\" x=\"856\" y=\"560\" width=\"32\" height=\"16\" style=\"1342242819\" id=\"317\" />\n"
"<child type=\"CheckBox\" name=\"check4\" caption=\"D\" x=\"896\" y=\"560\" width=\"32\" height=\"16\" style=\"1342242819\" id=\"318\" />\n"
"<child type=\"Label\" name=\"cpuSpeed\" caption=\"0000\" x=\"272\" y=\"560\" width=\"450\" height=\"0\" style=\"1342242827\" id=\"319\" bgColor=\"COLOR_BTNFACE\" bg_red=\"0\" bg_green=\"0\" bg_blue=\"0\" />\n"
"</form>\n"
"</forms>\n"
"<strings>\n"
"<list name=\"FileDialogStringList\">\n"
"<string name=\"OPEN_MIDI\" caption=\"Open MIDI File\" id=\"320\" />\n"
"<string name=\"SAVE_MIDI\" caption=\"Save MIDI File\" id=\"321\" />\n"
"<string name=\"OPEN_SETTINGS\" caption=\"Open Settings\" id=\"322\" />\n"
"<string name=\"SAVE_SETTINGS\" caption=\"Save Settings\" id=\"323\" />\n"
"<string name=\"OPEN_LOOP\" caption=\"Open Loop File\" id=\"324\" />\n"
"<string name=\"SAVE_LOOP\" caption=\"Save Loop File\" id=\"325\" />\n"
"</list>\n"
"<list name=\"ErrorStringList\">\n"
"<string name=\"MIDI_BAD_HEADER\" caption=\"Invalid MIDI File Header\" id=\"326\" />\n"
"<string name=\"MIDI_BAD_TRACK_HEADER\" caption=\"Invalid MIDI Track Header\" id=\"327\" />\n"
"<string name=\"OPEN_IN_FAILED\" caption=\"Unable to open MIDI-In\" id=\"328\" />\n"
"<string name=\"OPEN_OUT_FAILED\" caption=\"Unable to open MIDI-Out\" id=\"329\" />\n"
"</list>\n"
"<list name=\"MessageStringList\">\n"
"<string name=\"MIDI_FILE_CHANGED\" caption=\"MIDI-Data have been changed. Do you want to save?\" id=\"330\" />\n"
"<string name=\"SETTINGS_CHANGED\" caption=\"MIDI-Settings have been changed. Do you want to save?\" id=\"331\" />\n"
"<string name=\"DRUM_CHANGED\" caption=\"Drum data have been changed. Do you want to save?\" id=\"332\" />\n"
"<string name=\"LOOP_CHANGED\" caption=\"Loop-Data have been changed. Do you want to save?\" id=\"333\" />\n"
"</list>\n"
"<list name=\"LabelStringList\">\n"
"<string name=\"DRUM_EDITOR_TITLE\" caption=\"Drum Editor\" id=\"334\" />\n"
"<string name=\"STOP_LABEL\" caption=\"Stop\" id=\"335\" />\n"
"<string name=\"PLAY_LABEL\" caption=\"Play\" id=\"336\" />\n"
"<string name=\"RECORD_LABEL\" caption=\"Record\" id=\"337\" />\n"
"<string name=\"CHANNEL_LABEL\" caption=\"Channel\" id=\"338\" />\n"
"<string name=\"ANY_CHANNEL_LABEL\" caption=\"Any\" id=\"339\" />\n"
"<string name=\"ALL_MIDI_RECORD_LABEL\" caption=\"All\" id=\"340\" />\n"
"<string name=\"DEFAULT_MIDI_PLAY_LABEL\" caption=\"Default\" id=\"341\" />\n"
"<string name=\"ALL_NOTES_LABEL\" caption=\"Notes\" id=\"342\" />\n"
"<string name=\"MAJOR_LABEL\" caption=\"Major\" id=\"343\" />\n"
"<string name=\"MINOR_LABEL\" caption=\"Minor\" id=\"344\" />\n"
"<string name=\"INSTRUMENT_CAPTION\" caption=\"Guitar &amp; Piano\" id=\"345\" />\n"
"</list>\n"
"<list name=\"RecorderFiles\">\n"
"<string name=\"MidiFiles\" caption=\"MIDI Files (*.mid)|*.mid|\" id=\"346\" />\n"
"<string name=\"CsvFiles\" caption=\"Comma Separated (*.csv)|*.csv|\" id=\"347\" />\n"
"</list>\n"
"<list name=\"DrumFiles\">\n"
"<string name=\"DrumFiles\" caption=\"Drum Files (*.drum)|*.drum|\" id=\"348\" />\n"
"<string name=\"CsvFiles\" caption=\"Comma Separated (*.csv)|*.csv|\" id=\"347\" />\n"
"<string name=\"MidiFiles\" caption=\"MIDI Files (*.mid)|*.mid|\" id=\"346\" />\n"
"</list>\n"
"<list name=\"LoopFiles\">\n"
"<string name=\"LoopFiles\" caption=\"Loop Files (*.loop)|*.loop|\" id=\"349\" />\n"
"<string name=\"CsvFiles\" caption=\"Comma Separated (*.csv)|*.csv|\" id=\"347\" />\n"
"<string name=\"MidiFiles\" caption=\"MIDI Files (*.mid)|*.mid|\" id=\"346\" />\n"
"</list>\n"
"<list name=\"SynthFiles\">\n"
"<string name=\"SynthFiles\" caption=\"Synthesizer (*.sset)|*.sset|\" id=\"350\" />\n"
"</list>\n"
"<list name=\"SettingsFiles\">\n"
"<string name=\"SettingsFiles\" caption=\"Settings (*.mset)|*.mset|\" id=\"351\" />\n"
"</list>\n"
"</strings>\n"
"</gui>";
			return Application::getGuiDoc( xmlSrc );
		}
		return doc;
	}


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
