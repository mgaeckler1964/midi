<?xml version="1.0" encoding="iso-8859-1" ?>
<setup>
<application title="MG Analog Synthesizer" version="3.2.0.0" author="Martin Gaeckler" />
<icons Group="MG Analog Synthesizer">
<icon title="Readme" target="PROGRAM\readme_de.txt" main="" dbUpd="" />
<icon title="Readme (EN)" target="PROGRAM\readme_en.txt" main="" dbUpd="" />
<icon title="Readme (En)" target="PROGRAM\readme_en.txt" main="" dbUpd="" />
</icons>
<source>
<group name="PROGRAM" destination="$ProgramFiles64">
<file source="readme_en.txt" />
<file source="readme_de.txt" />
</group>
<group name="VST64" destination="$VstPlugins64Bit">
<file source="..\..\..\Program Files\Common Files\VST3\gak\mgSynth.vst3" />
</group>
<group name="VSTpresets" destination="$VstPresets">
<file source="..\..\User\gak\VST3 Presets\Martin Gaeckler\MG Analog Synthesizer\Multi\bass.vstpreset" />
<file source="..\..\User\gak\VST3 Presets\Martin Gaeckler\MG Analog Synthesizer\Multi\kick.vstpreset" />
<file source="..\..\User\gak\VST3 Presets\Martin Gaeckler\MG Analog Synthesizer\Multi\mgSynth.vstpreset" />
</group>
</source>
</setup>