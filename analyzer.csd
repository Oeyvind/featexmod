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

<Cabbage>
form size(740, 750), caption("Analyzer"), pluginID("anlz")
image bounds(0, 0, 740, 500), file("background.jpg"), shape("round")

label text("channel"), bounds(15, 16, 75, 12)
combobox channel("chan"), bounds(80, 12, 60, 25), items("1", "2", "3", "4"), value(1)

rslider bounds(15, 40, 65, 65), text("Noisefloor"), channel("inputNoisefloor"), range(-96, 0, -40) 
label text("pitch track method"), bounds(90, 50, 120, 12)
combobox channel("pitchmethod"), bounds(90, 68, 120, 15), items("ptrack", "plltrack", "pitchamdf", "epoch"), value(1)
button channel("pitchMonitor"),bounds(90, 88, 120, 15), text("pitch monitor"), colour:0("black"), colour:1("green"), latching(1)

checkbox channel("pause"),bounds(220, 5, 15, 15), value(0)
label bounds(170, 5, 50, 12), text("pause"),  colour(50, 50, 50, 255)
checkbox channel("enableDisplay"),bounds(345, 5, 15, 15), value(0)
label bounds(255, 5, 90, 12), text("enableDisplay"),  colour(50, 50, 50, 255)

checkbox channel("autonormalize"),bounds(220, 20, 15, 15), value(0)
label bounds(170, 20, 50, 12), text("normalize"),  colour(50, 50, 50, 255)
checkbox channel("autocalibrate"),bounds(345, 20, 15, 15), value(0)
label bounds(255, 20, 90, 12), text("calibrate"),  colour(50, 50, 50, 255)

rslider bounds(220, 40, 65, 65), text("pFiltSize"), channel("pitchFilterSize"), range(3, 255, 25, 0.3, 1) 
numberbox bounds(290, 43, 60, 15), channel("pitch_high"), range(50, 2000, 800)
numberbox bounds(290, 64, 60, 15), channel("pitch_low"), range(50, 2000, 150)
label bounds(290, 89, 50, 12), text("p range"),  colour(50, 50, 50, 255)

label bounds(15, 120, 202, 12), text("pre emphasis for amp trans)"),  colour(50, 50, 50, 255)
rslider bounds(15, 135, 65, 65), text("LoDamp"), channel("preEqLoShelfGain"), range(-20, 0, -5) 
rslider bounds(80, 135, 65, 65), text("x_Fq"), channel("preEqHiShelfFq"), range(500, 5000, 2000, 0.35) 
rslider bounds(145, 135, 65, 65), text("HiBoost"), channel("preEqHiShelfGain"), range(0, 12, 5) 

label text("transients"), bounds(15, 210, 335, 12), colour(50, 50, 50, 255)
rslider bounds(80, 226, 65, 65), text("Thresh"), channel("amp_transientThresh"), range(0, 9, 2) 
rslider bounds(145, 226, 65, 65), text("DecThresh"), channel("amp_transientDecThresh"), range(0, 12, 5) 
rslider bounds(210, 226, 65, 65), text("DecTime"), channel("amp_transientDecTime"), range(0.1, 2.0, 0.5) 
rslider bounds(275, 226, 65, 65), text("DblLimit"), channel("amp_transientDoubleLimit"), range(0.1, 1.0, 0.2) 
label text("amp"), bounds(15, 230, 70, 12)
checkbox channel("transientDisplay"),bounds(42, 245, 15, 15), value(0)

rslider bounds(80, 295, 65, 65), text("Thresh"), channel("centr_transientThresh"), range(0.0, 1.0, 0.2) 
rslider bounds(145, 295, 65, 65), text("DecThresh"), channel("centr_transientDecThresh"), range(0.0, 1.0, 0.0) 
rslider bounds(210, 295, 65, 65), text("DecTime"), channel("centr_transientDecTime"), range(0.1, 2.0, 2.0) 
rslider bounds(275, 295, 65, 65), text("DblLimit"), channel("centr_doubleLimit"), range(0.1, 1.0, 0.1) 
label text("centroid"), bounds(15, 299, 70, 12)
checkbox channel("ctransientDisplay"),bounds(42, 315, 15, 15), value(0)

rslider bounds(80, 360, 65, 65), text("Thresh"), channel("kurt_transientThresh"), range(0.0, 6, 0.8) 
rslider bounds(145, 360, 65, 65), text("DecThresh"), channel("kurt_transientDecThresh"), range(0.0, 6, 0) 
rslider bounds(210, 360, 65, 65), text("DecTime"), channel("kurt_transientDecTime"), range(0.1, 2.0, 0.5) 
rslider bounds(275, 360, 65, 65), text("DblLimit"), channel("kurt_doubleLimit"), range(0.1, 1.0, 0.2) 
label text("kurtosis"), bounds(15, 364, 70, 12)
checkbox channel("ktransientDisplay"),bounds(42, 380, 15, 15), value(0)

rslider bounds(80, 425, 65, 65), text("Thresh"), channel("pitch_transientThresh"), range(0.0, 12.0, 1.5) 
rslider bounds(145, 425, 65, 65), text("DecThresh"), channel("pitch_transientDecThresh"), range(0.0, 12.0, 0.0) 
rslider bounds(210, 425, 65, 65), text("DecTime"), channel("pitch_transientDecTime"), range(0.1, 2.0, 0.5) 
rslider bounds(275, 425, 65, 65), text("DblLimit"), channel("pitch_doubleLimit"), range(0.1, 1.0, 0.7) 
label text("pitch"), bounds(12, 429, 70, 12)
checkbox channel("puptransientDisplay"),bounds(42, 445, 15, 15), value(0)
checkbox channel("pdwntransientDisplay"),bounds(42, 460, 15, 15), value(0)

gentable bounds(370,  5, 320, 240), identchannel("displays"), tablenumber(1), tablecolour("lightblue"), tablegridcolour(0,0,0,0), amprange(-.03,1,1), zoom(-1), samplerange(0,16)
image bounds(370, 5, 80,240), shape("sharp"), colour(175, 50,255, 40), identchannel("group1")	
label text("noisefloor"), bounds(385, 250, 200, 15), align("left"), rotate(1.5708, 0, 0)
label text("krms"), bounds(405, 250, 200, 15), align("left"), rotate(1.5708, 0, 0)
label text("cps"), bounds(425, 250, 200, 15), align("left"), rotate(1.5708, 0, 0)
label text("pitch"), bounds(445, 250, 200, 15), align("left"), rotate(1.5708, 0, 0)

image bounds(450, 5, 80,240), shape("sharp"), colour(75,255, 75, 40), identchannel("group2")	
label text("centroid"), bounds(465, 250, 200, 15), align("left"), rotate(1.5708, 0, 0)
label text("spread"), bounds(485, 250, 200, 15), align("left"), rotate(1.5708, 0, 0)
label text("skewness"), bounds(505, 250, 200, 15), align("left"), rotate(1.5708, 0, 0)
label text("kurtosis"), bounds(525, 250, 200, 15), align("left"), rotate(1.5708, 0, 0)

image bounds(530, 5, 60,240), shape("sharp"), colour(45, 45,255, 40), identchannel("group3")	
label text("flatness"), bounds(545, 250, 190, 15), align("left"), rotate(1.5708, 0, 0)
label text("crest"), bounds(565, 250, 165, 15), align("left"), rotate(1.5708, 0, 0)
label text("flux"), bounds(585, 250, 145, 15), align("left"), rotate(1.5708, 0, 0)

image bounds(590, 5, 100,240), shape("sharp"), colour(255,255, 50, 20), identchannel("group4")	
;label text("amp_t"), bounds(605, 250, 200, 15), align("left"), rotate(1.5708, 0, 0)
;label text("centr_t"), bounds(625, 250, 200, 15), align("left"), rotate(1.5708, 0, 0)
;label text("kurt_t"), bounds(645, 250, 200, 15), align("left"), rotate(1.5708, 0, 0)
;label text("pup_t"), bounds(665, 250, 200, 15), align("left"), rotate(1.5708, 0, 0)
;label text("pdwn_t"), bounds(685, 250, 200, 15), align("left"), rotate(1.5708, 0, 0)

label text("amp transient density"), bounds(370, 340, 200, 15), align("left")
label text("0"), bounds(370, 360, 200, 15), align("left"), identchannel("ampTransDensity")	
label text("centr transient density"), bounds(370, 380, 200, 15), align("left")
label text("0"), bounds(370, 400, 200, 15), align("left"), identchannel("cenTransDensity")	

csoundoutput bounds(5, 500, 290, 250), text("Output")
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-n -d -+rtmidi=NULL -M0 -m0d
;-iadc -odac
</CsOptions>
<CsInstruments>

        sr = 48000
        ksmps = 128
	nchnls = 2
	0dbfs = 1

        gi1     ftgen   1, 0, 16, -2, 0 
	giSine	ftgen	0, 0, 65536, 10, 1			; sine wave
	gifftsize 	= 1024
			chnset gifftsize, "fftsize"
	giFftTabSize	= (gifftsize / 2)+1
	gifna     	ftgen   1 ,0 ,giFftTabSize, 7, 0, giFftTabSize, 0   	; for pvs analysis
	gifnf     	ftgen   2 ,0 ,giFftTabSize, 7, 0, giFftTabSize, 0   	; for pvs analysis

        gifqToMelMax    = 16384
        gifqToMelTab    ftgen   0, 0, gifqToMelMax, 2, 0        ; table to be filled with mel frequency map
        gimelMax        = 1125*log(1+(gifqToMelMax/700))
        print gimelMax
        gi32melBands    ftgen   0, 0, 4096, -7, 0, gimelMax, 31, 4096-gimelMax, 31      ; divide mel scale evenly into 32 bands
        gi32melFqs      ftgen   0, 0, 32, -7, 0, 32, gimelMax                           ; mel frequencies for the 32 bands
        gi32melBandFqs  ftgen   0, 0, 32, 2, 0                                          ; to be filled with the frequencies for those 32 mel frequencies

        ;gifnTempomem    ftgen   0, 0, 128, 16, 1, 128, 1, 0

#include "analyze_udos.inc"

;**************************
	instr 1
#include "analyze_chn_init.inc"
	endin

;**************************
        instr 2
	a1,a2	        ins
;	                outs a1, a2
        kpause chnget "pause"
        if kpause > 0 kgoto skip

#include "analyze_audio.inc"

; analysis sig display in gui table
        tablew pow(ampdbfs(knoiseFloor_dB),0.4), 0, gi1 
        tablew pow(krms,0.4), 1, gi1 
        tablew kcps_n, 2, gi1 
        tablew kpitch_a, 3, gi1 
        tablew kcentroid_a, 4, gi1 
        tablew kspread_a, 5, gi1 
        tablew kskewness_a, 6, gi1 
        tablew kurtosis_a, 7, gi1 
        tablew kflatness_a, 8, gi1 
        tablew kcrest_a, 9, gi1 
        tablew kflux_a, 10, gi1 

/*        
        inormsize       = kr*4
        kNormArr[]      init inormsize
        kcalibrate      chnget "autocalibrate"
        kreset          trigger kcalibrate, 0.5, 0
        kval            = pow(krms,0.4);kcentroid_a
        kval_max        init 0
        kval_min        init 2^64
        kval_min_ok     init 0
        kval_max_ok     init 0
        kqMaxval        init 0
        kqMinval        init 2^64
        kqhighthresh    = 0.9
        kqlowthresh     = 0.1
        knormindex      init 0
        kcalibrated     init 0
        if kreset > 0 then
        kqMaxval        = 0
        kqMinval        = 2^64
        kqMax           = 0
        kqMin           = 2^64
        kval_max        = 0
        kval_min        = 2^64
        knormindex      = 0
        kcalibrated     = 0
        endif
        if kcalibrate > 0 then
          norm:
          kNormArr[knormindex] = kval
          knormindex += 1
          if knormindex == inormsize then
            knormindex2 = 0
            interquartile:
            kqMax,kqMaxIndx maxarray kNormArr
            kqMaxval max kqMaxval, kqMax
            if kqMax > kqMaxval*kqhighthresh then
              kNormArr[kqMaxIndx] = kqMax*kqhighthresh
            endif
            kqMin,kqMinIndx minarray kNormArr
            if kqMin < kqMaxval*kqlowthresh then ; lowthresh relative to max
              kNormArr[kqMinIndx] = kqMaxval*kqlowthresh
            endif
            knormindex2 += 1
            if knormindex2 < inormsize then
              kgoto interquartile
            endif
            kqMax maxarray kNormArr
            kqMin minarray kNormArr
            kcalibrated = 1
            knormindex = 0
          endif
        endif
        kval_max        max kval_max, kqMax
        kval_min        min kval_min, kqMin
        if kcalibrated > 0 then
        kval_min_ok     = kval_min
        kval_max_ok     = kval_max
        endif
 */       
        ;tablew kval_min_ok, 11, gi1
        ;tablew kval_max_ok, 12, gi1

        /*
        tablew kmelceps_b1, 11, gi1
        tablew kmelceps_b2, 12, gi1
        tablew kmelceps_b3, 13, gi1
        tablew kmelceps_b4, 14, gi1
        */
        kupd    metro 30
        kenableDisplay chnget "enableDisplay"
        if kupd*kenableDisplay > 0 then
 	chnset	"tablenumber(1)", "displays"	; update table display	
 	SatranD sprintfk "text(%.1f)", katransDensEnv
 	chnset	SatranD, "ampTransDensity"	; update gui	
 	SctranD sprintfk "text(%.1f)", kctransDensEnv
 	chnset	SctranD, "cenTransDensity"	; update gui	
        endif

        kchan           chnget "chan"
        kchanged        changed kchan
        if kchanged > 0 then
        reinit send
        endif
send:
#include "analyze_send.inc"
        kmonitor        chnget "pitchMonitor"
        atest           oscili interp(krms)*kmonitor, kcps, giSine
                        outs atest, atest
skip:
        endin

;**************************
        
</CsInstruments>
<CsScore>
#define SCORELEN #86400#
i1	0.1	1
i2	.1	$SCORELEN
e
</CsScore>
</CsoundSynthesizer>

