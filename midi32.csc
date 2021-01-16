<?xml version="1.0" encoding="iso-8859-1" ?>
<setup>
<application title="MG MIDI Tools" version="3.2.0.0" author="Martin Gaeckler" />
<icons Group="MG MIDI Tools (x86)">
<icon title="MG MIDI Tools" target="PROGRAM\midi.exe" main="true" dbUpd="" />
<icon title="Readme" target="PROGRAM\readme.txt" main="" dbUpd="" />
<icon title="Readme (En)" target="PROGRAM\readme_en.txt" main="" dbUpd="" />
</icons>
<source>
<group name="PROGRAM" destination="$ProgramFiles32">
<file source="readme.txt" />
<file source="readme_en.txt" />
<file source="..\..\Object\Win32\midi.de.gui" />
<file source="..\..\Object\Win32\MIDI.exe" />
<file source="..\..\Object\Win32\midi.gui" />
<file source="gm_drums.csv" />
<file source="gm_voices.csv" />
<file source="psr275_drums.csv" />
<file source="psr275_voices.csv" />
<file source="sy35_voices.csv" />
</group>
</source>
</setup>