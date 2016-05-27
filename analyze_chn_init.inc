;    Copyright 2015 Oeyvind Brandtsegg 
;
;    This file is part of the Signal Interaction Toolkit
;
;    The Signal Interaction Toolkit is free software: you can redistribute it and/or modify
;    it under the terms of the GNU General Public License version 3 
;    as published by the Free Software Foundation.
;
;    The Signal Interaction Toolkit is distributed in the hope that it will be useful,
;    but WITHOUT ANY WARRANTY; without even the implied warranty of
;    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;    GNU General Public License for more details.
;
;    You should have received a copy of the GNU General Public License
;    along with The Signal Interaction Toolkit.  
;    If not, see <http://www.gnu.org/licenses/>.

; init chn values for various purposes

        chnset -30, "inputNoisefloor"

        chnset 4, "pitchmethod"
	chnset 31, "pitchFilterSize"
        chnset 100, "pitch_low"       ; string channels due to gui wodget type
        chnset 1200, "pitch_high"


        ; transient detect parameters
        chnset 2000, "preEqHiShelfFq"     ; pre emphasis for transient detection of sibliants
        chnset 0, "preEqHiShelfGain"     ; in dB
        chnset 0, "preEqLoShelfGain"
        chnset 3, "amp_transientThresh"         ; transient trig threshold 
        chnset 6, "amp_transientDecThresh"      ; retrig threshold, how much must the level decay from its local max before allowing new transient trig
        chnset 0.5, "amp_transientDecTime"        ; decay time for the envelope follower, filtering the signal before transient detaction
        chnset 0.2, "amp_transientDoubleLimit"  ; minimum duration between transients, (double trig limit)

        chnset 0.2, "centro_transientThresh"         ; transient trig threshold 
        chnset 0.0, "centro_transientDecThresh"       ; retrig threshold, how much must the level decay from its local max before allowing new transient trig
        chnset 2.0, "centro_transientDecTime"        ; decay time for the envelope follower, filtering the signal before transient detaction
        chnset 0.1, "centro_transientDoubleLimit"       ; minimum duration between transients, (double trig limit)

        chnset 0.8, "kurt_transientThresh"         ; transient trig threshold 
        chnset 0, "kurt_transientDecThresh"       ; retrig threshold, how much must the level decay from its local max before allowing new transient trig
        chnset 0.5, "kurt_transientDecTime"        ; decay time for the envelope follower, filtering the signal before transient detaction
        chnset 0.1, "kurt_transientDoubleLimit"       ; minimum duration between transients, (double trig limit)
  
        chnset 1.5, "pitch_transientThresh"         ; transient trig threshold 
        chnset 0.0, "pitch_transientDecThresh"       ; retrig threshold, how much must the level decay from its local max before allowing new transient trig
        chnset 0.9, "pitch_transientDecTime"        ; decay time for the envelope follower, filtering the signal before transient detaction
        chnset 0.01, "pitch_transientDoubleLimit"       ; minimum duration between transients, (double trig limit)


/*
; fill the mel frequency map (converts from frequency to mel)
        kfq             init 0
        while kfq < gifqToMelMax do
                        tablew 1125*log(1+(kfq/700)), kfq, gifqToMelTab
        kfq             += 1
        od
        kband           init 0
        while kband < 32 do
        kmelFq          table kband, gi32melFqs                                         ; get mel frequency for band
                        tablew 700*(exp(kmelFq/1125)-1), kband, gi32melBandFqs          ; store the freq for this mel freq
        kband           = kband + 1
        od
*/