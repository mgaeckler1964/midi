<?xml version="1.0" encoding="iso-8859-1" ?>
<setup>
<application title="MIDI Tools" version="3.2.0.0" author="Martin Gäckler" />
<icons Group="MG MIDI Tools">
<icon title="MIDI Tools (32-Bit)" target="Program32\midi.exe" main="true" dbUpd="" />
<icon title="MIDI Tools (64-Bit)" target="Program64\midi.exe" main="" dbUpd="" />
<icon title="BDE Admin" target="BDE\BDEADMIN.EXE" main="" dbUpd="" />
<icon title="Datapump" target="BDE\datapump.exe" main="" dbUpd="" />
</icons>
<source>
<group name="PROGRAM" destination="$ProgramFiles64">
<file source="Measuring DR ENv3.pdf" />
<file source="readme.txt" />
<file source="readme_en.txt" />
</group>
<group name="Program32" destination="$ProgramFiles32">
<file source="..\..\Object\Win32\MIDI.exe" />
<file source="gm_drums.csv" />
<file source="gm_voices.csv" />
<file source="psr275_drums.csv" />
<file source="psr275_voices.csv" />
<file source="sy35_voices.csv" />
<file source="Tools\midi.de.gui" />
<file source="Tools\midi.gui" />
</group>
<group name="Program64" destination="$ProgramFiles64">
<file source="..\..\Object\x64\MIDI.exe" />
<file source="gm_drums.csv" />
<file source="gm_voices.csv" />
<file source="psr275_drums.csv" />
<file source="psr275_voices.csv" />
<file source="sy35_voices.csv" />
<file source="Tools\midi.de.gui" />
<file source="Tools\midi.gui" />
</group>
<group name="VST32" destination="$VstPlugins32Bit">
<file source="..\..\..\Program Files (x86)\Common Files\VST3\gak\mgSynth.vst3" />
</group>
<group name="VST64" destination="$VstPlugins64Bit">
<file source="..\..\..\Program Files\Common Files\VST3\gak\mgSynth.vst3" />
</group>
<group name="VSTpresets" destination="$VstPresets">
<file source="..\..\User\gak\VST3 Presets\Martin Gaeckler\MG Analog Synthesizer\Multi" />
<file source="..\..\User\gak\VST3 Presets\Martin Gaeckler\MG Analog Synthesizer\Multi\bass.vstpreset" />
<file source="..\..\User\gak\VST3 Presets\Martin Gaeckler\MG Analog Synthesizer\Multi\kick.vstpreset" />
<file source="..\..\User\gak\VST3 Presets\Martin Gaeckler\MG Analog Synthesizer\Multi\mgSynth.vstpreset" />
</group>
</source>
<bde AliasName="GASTHAUS" RESOURCE="0007" DataPath="" dbVersion="2" />
</setup>