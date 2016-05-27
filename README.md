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

* How to use:
- Compile the VST plugins from the .csd files using Cabbage (https://github.com/rorywalsh/cabbage/releases)
- Insert the analyzer plugin on one track (track 1) of your VST host, route audio to it
- Insert a control signal mixer plugin (mediator, MIDIator ...) on another track
- Analyzed signals are sent as OSC from the analyzer to the mediator, the mediator mixes and shapes the signals, and outputs OSC or MIDI
- Route OSC or MIDI to your preferred modulation destination

