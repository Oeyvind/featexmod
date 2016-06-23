;    Copyright 2016 Oeyvind Brandtsegg 
;
;    This file is part of the Feature-Extract-Modulator package
;
;    The Feature-Extract-Modulator is free software: you can redistribute it and/or modify
;    it under the terms of the GNU General Public License version 3 
;    as published by the Free Software Foundation.
;
;    The Feature-Extract-Modulator is distributed in the hope that it will be useful,
;    but WITHOUT ANY WARRANTY; without even the implied warranty of
;    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;    GNU General Public License for more details.
;
;    You should have received a copy of the GNU General Public License
;    along with The Feature-Extract-Modulator package.  
;    If not, see <http://www.gnu.org/licenses/>.

# Feature-Extract-Modulator
A toolkit to analyze features of an audio signal, and using these features as control signals to modulate parameters of other processes.
Typically used to modulate effect processing parameters. Used on the same sound it provides adaptive effects, used on another source sound it provides cross-adaptive effects.

The Feature-Extract-Modulator is a further development of the "interprocessing" Signal Interaction Toolkit of 2015. 
Due to possible conflicts of VST id's it is advised not to have both the "interprocessing" and the "featexmod" toolkit installed in your VST path.
Move your VST/interprocessing folder (if you have one) to another location on your computer and you should be fine.


* How to use:
- Compile the VST plugins from the .csd files using Cabbage (https://github.com/rorywalsh/cabbage/releases)
- Insert the analyzer plugin on one track (track 1) of your VST host, route audio to it
- Insert a control signal mixer plugin (mediator, MIDIator ...) on another track
- Analyzed signals are sent as OSC from the analyzer to the mediator, the mediator mixes and shapes the signals, and outputs OSC or MIDI
- Route OSC or MIDI to your preferred modulation destination

* Changes to the GUI
- The gui code is in part autogenerated via Python.
- to generate gui code e.g. for the vst_MIDIator.csd, you should run: python codeUtility.py vst_MIDIator
- Python writes the GUI code to effectName_gui_scratchpad.inc, from where it can be copied into the csd. 
