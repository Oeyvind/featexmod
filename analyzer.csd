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

<Cabbage>
form size(740, 750), caption("Analyzer"), pluginID("anlz"), guirefresh(1) 
image bounds(0, 0, 740, 500), file("background.jpg"), shape("round")

label text("channel"), bounds(15, 16, 75, 12)
combobox channel("chan"), bounds(80, 12, 60, 25), items("1", "2", "3", "4"), value(1)

rslider bounds(15, 40, 65, 65), text("Noisefloor"), channel("inputNoisefloor"), range(-96, 0, -40) 
label text("pitch track method"), bounds(90, 50, 120, 12)
combobox channel("pitchmethod"), bounds(90, 68, 120, 15), items("ptrack", "plltrack", "pitchamdf", "epoch"), value(1)
button channel("pitchMonitor"),bounds(90, 88, 120, 15), text("pitch monitor"), colour:0("black"), colour:1("green"), latching(1)

checkbox channel("pause"),bounds(220, 5, 15, 15), value(0)
label bounds(170, 5, 50, 12), text("pause"),  colour(50, 50, 50, 255)
checkbox channel("enableDisplay"),bounds(345, 5, 15, 15), value(1)
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
rslider bounds(145, 226, 65, 65), text("DecThresh"), channel("amp_transientDecThresh"), range(0, 12, 4) 
rslider bounds(210, 226, 65, 65), text("DecTime"), channel("amp_transientDecTime"), range(0.1, 2.0, 0.5) 
rslider bounds(275, 226, 65, 65), text("DblLimit"), channel("amp_transientDoubleLimit"), range(0.1, 1.0, 0.1) 
label text("amp"), bounds(15, 230, 70, 12)
checkbox channel("transientDisplay"),bounds(42, 245, 15, 15), value(0)

gentable bounds(370,  5, 320, 240), identchannel("displays"), tablenumber(1), tablecolour("lightblue"), tablegridcolour(0,0,0,0), amprange(-.3,1,1), zoom(-1), samplerange(0,16)
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

label text("amp transient density"), bounds(370, 340, 200, 15), align("left")
label text("0"), bounds(370, 360, 200, 15), align("left"), identchannel("ampTransDensity")	

label text("rhythm consonance"), bounds(370, 430, 200, 15), align("left")
label text("0"), bounds(370, 450, 200, 15), align("left"), identchannel("rhythmConsonance")	
numberbox bounds(435, 450, 40, 15), channel("rhythmConsonanceDeviation"), range(0.00, 0.90, 0.1)
label text("rd dev"), bounds(480, 450, 200, 15), align("left")
gentable bounds(370, 470, 160, 140), identchannel("rhythm_consonance"), tablenumber(5), tablecolour("lightblue"), tablegridcolour(0,0,0,0), amprange(0,1,5), zoom(-1), samplerange(0,32)
label text("latest rhythm ratios"), bounds(370, 620, 200, 15), align("left")
label text("0"), bounds(370, 640, 100, 15), align("left"), identchannel("rhythmratio1")	
label text("0"), bounds(420, 640, 100, 15), align("left"), identchannel("rhythmratio2")	
label text("0"), bounds(470, 640, 100, 15), align("left"), identchannel("rhythmratio3")	

label text("rhythm autocorr"), bounds(540, 430, 200, 15), align("left")
gentable bounds(540, 470, 160, 140), identchannel("rhythm_autocorr"), tablenumber(10), tablecolour("lightblue"), tablegridcolour(0,0,0,0), amprange(0,1,10), zoom(-1), samplerange(0,128)
numberbox bounds(540, 450, 40, 15), channel("rhythmAutocorrEnvShape"), range(0.001, 0.20, 0.05)
label text("ra env"), bounds(585, 450, 200, 15), align("left")
numberbox bounds(540, 620, 40, 15), channel("rhythmAutocorrThresh"), range(0.01, 0.50, 0.1)
label text("ra thresh"), bounds(585, 620, 200, 15), align("left")

label text("autocorr peaks"), bounds(540, 640, 200, 15), align("left")
label text("0"), bounds(540, 660, 100, 15), align("left"), identchannel("rhythmauto1indx")	
label text("0"), bounds(590, 660, 100, 15), align("left"), identchannel("rhythmauto2indx")	
label text("0"), bounds(640, 660, 100, 15), align("left"), identchannel("rhythmauto3indx")	
label text("0"), bounds(540, 680, 100, 15), align("left"), identchannel("rhythmauto1")	
label text("0"), bounds(590, 680, 100, 15), align("left"), identchannel("rhythmauto2")	
label text("0"), bounds(640, 680, 100, 15), align("left"), identchannel("rhythmauto3")	



csoundoutput bounds(5, 500, 290, 250), text("Output")
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-n -d -+rtmidi=NULL -M0 -m0d
;-iadc -odac
</CsOptions>
<CsInstruments>

        sr = 48000
        ksmps = 32
	nchnls = 2
	0dbfs = 1
	
	pyinit
        pyruni "import sys"
	pyruni "import os"
	pyruni "sys.path.append(os.getcwd())"	; needed for OSX
        pyruni "if sys.platform.startswith('win'): sys.path.append('c:\\python27\\DLLs')" ; needed for Win
        pyruni "import peakdetect_wrapper"
        pyruni "p = peakdetect_wrapper.PeakDetector()"
        pyruni "import rational_approx as r"

        gi1     ftgen   1, 0, 16, -2, 0  ; analysis signal display
        gi5     ftgen   5, 0, 32, -2, 0  ; rhythm consonance display
        gi10    ftgen   10, 0, 256, -2, 0 ; rhythm autocorr display
	giSine	ftgen	0, 0, 65536, 10, 1			; sine wave
	gifftsize 	= 1024
			chnset gifftsize, "fftsize"
	giFftTabSize	= (gifftsize / 2)+1
	gifna     	ftgen   1 ,0 ,giFftTabSize, 7, 0, giFftTabSize, 0   	; for pvs analysis
	gifnf     	ftgen   2 ,0 ,giFftTabSize, 7, 0, giFftTabSize, 0   	; for pvs analysis

	giSinEnv        ftgen   0, 0, 8192, 19, 1, 0.5, 270, 0.5        ; sinoid transient envelope shape for autocorr

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
        tablew kmfcc1, 11, gi1
        tablew kmfcc2, 12, gi1
        tablew kmfcc3, 13, gi1
        tablew kmfcc4, 14, gi1
        tablew kmfcc5, 15, gi1

        kupd    metro 100
        kenableDisplay chnget "enableDisplay"
        if kupd*kenableDisplay > 0 then
 	chnset	"tablenumber(1)", "displays"	; update table display	
 	SatranD sprintfk "text(%.1f)", katransDensEnv
 	chnset	SatranD, "ampTransDensity"	; update gui	
        endif

        if krnewframe*kenableDisplay > 0 then
        copya2ftab kRhythmAuto, 10
 	chnset	"tablenumber(10)", "rhythm_autocorr"	; update table display	
 	Srauto1D sprintfk "text(%.2f)", krhythm_ac1
 	Srauto2D sprintfk "text(%.2f)", krhythm_ac2
 	Srauto3D sprintfk "text(%.2f)", krhythm_ac3
 	Srauto1indxD sprintfk "text(%i)", krhythm_ac1indx;krhythm_ac1time
 	Srauto2indxD sprintfk "text(%i)", krhythm_ac2indx
 	Srauto3indxD sprintfk "text(%i)", krhythm_ac3indx
 	chnset	Srauto1D, "rhythmauto1"	; update gui	
 	chnset	Srauto2D, "rhythmauto2"	; update gui	
 	chnset	Srauto3D, "rhythmauto3"	; update gui	
 	chnset	Srauto1indxD, "rhythmauto1indx"	; update gui	
 	chnset	Srauto2indxD, "rhythmauto2indx"	; update gui	
 	chnset	Srauto3indxD, "rhythmauto3indx"	; update gui	

        endif

        if krms_tran0>0 then
        krc_indx init 0
 	SrcomplexD sprintfk "text(%.3f)", krhythm_consonance
        chnset	SrcomplexD, "rhythmConsonance"

 	Srratio1D sprintfk "text(%.2f)", krhythm_ratio1
 	Srratio2D sprintfk "text(%.2f)", krhythm_ratio2
 	Srratio3D sprintfk "text(%.2f)", krhythm_ratio3
        chnset	Srratio1D, "rhythmratio1"
        chnset	Srratio2D, "rhythmratio2"
        chnset	Srratio3D, "rhythmratio3"
        krc_indx = (krc_indx+1)%32
        tablew krhythm_consonance, krc_indx, gi5
 	chnset	"tablenumber(5)", "rhythm_consonance" ; update table display	
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
;i1	0.1	1               ; init gui values, for running without Cabbage
i2	.1	$SCORELEN
e
</CsScore>
</CsoundSynthesizer>

