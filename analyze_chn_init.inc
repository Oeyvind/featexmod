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

; init chn values for various purposes

        chnset -61, "inputNoisefloor"

        chnset 1, "pitchmethod"
	chnset 25, "pitchFilterSize"
        chnset 150, "pitch_low"       ; string channels due to gui wodget type
        chnset 800, "pitch_high"

        ; transient detect parameters
        chnset 2000, "preEqHiShelfFq"     ; pre emphasis for transient detection of sibliants
        chnset 5, "preEqHiShelfGain"     ; in dB
        chnset -5, "preEqLoShelfGain"
        chnset 2, "amp_transientThresh"         ; transient trig threshold 
        chnset 4, "amp_transientDecThresh"      ; retrig threshold, how much must the level decay from its local max before allowing new transient trig
        chnset 0.5, "amp_transientDecTime"        ; decay time for the envelope follower, filtering the signal before transient detaction
        chnset 0.05, "amp_transientDoubleLimit"  ; minimum duration between transients, (double trig limit)
        
        chnset 0.01, "rhythmAutocorrPeakDelta"
        