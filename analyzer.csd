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
form size(1740, 760), caption("Analyzer"), pluginID("anlz"), guirefresh(1) 
image bounds(0, 0, 740, 760), file("background.jpg"), shape("round")

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
rslider bounds(275, 226, 65, 65), text("DblLimit"), channel("amp_transientDoubleLimit"), range(0.02, 1.0, 0.05, 0.35, 0.01) 
label text("amp"), bounds(15, 230, 70, 12)
checkbox channel("transientDisplay"),bounds(42, 245, 15, 15), value(0)

label text("plot"), bounds(5, 295, 40, 15), align("left")
button bounds(40, 295, 50, 15), channel("plotenable"), text("not","plot"), value(0), colour("gray"), fontcolour("blue")
button bounds(90, 295, 50, 15), channel("plotclear"), text("clear","-"), value(0), colour("gray"), fontcolour("blue"), latched(0)
label text("update"), bounds(160, 295, 100, 15), align("left")
combobox channel("plotupdatemethod"), bounds(215, 295, 70, 15), items("metro", "transient"), value(1)
numberbox bounds(290, 295, 40, 15), channel("plotmetrorate"), range(1, 50, 10)

label text("x"), bounds(270, 315, 15, 15), align("left")
combobox channel("plot_x"), bounds(270, 332, 90, 18), items("rms", "cps_n", "pitch_a", "centroid_a", "spread_a", "skewness_a", "kurtosis_a", "flatness_a", "crest_a", "flux_a", "amp_trans", "atransDensEnv", "rhythm_consonance", "rhythmFromOne", "rhythm_ratio1", "rhythm_ratio2", "rhythm_ac1", "rhythm_ac2", "rhythm_ac1time", "rhythm_ac2time", "mfcc1", "mfcc2", "mfcc3", "mfcc4", "mfcc5"), value(1), channeltype("string")
label text("scale  offset"), bounds(270, 352, 90, 15), align("left")
numberbox bounds(270, 375, 40, 15), channel("plot_x_scale"), range(0.0, 99.0, 1.0)
numberbox bounds(320, 375, 40, 15), channel("plot_x_offset"), range(-1.0, 1.0, 0.0)

label text("y"), bounds(270, 395, 15, 15), align("left")
combobox channel("plot_y"), bounds(270, 412, 90, 18), items("rms", "cps_n", "pitch_a", "centroid_a", "spread_a", "skewness_a", "kurtosis_a", "flatness_a", "crest_a", "flux_a", "amp_trans", "atransDensEnv", "rhythm_consonance", "rhythmFromOne", "rhythm_ratio1", "rhythm_ratio2", "rhythm_ac1", "rhythm_ac2", "rhythm_ac1time", "rhythm_ac2time", "mfcc1", "mfcc2", "mfcc3", "mfcc4", "mfcc5"), value(1), channeltype("string")
label text("scale  offset"), bounds(270, 432, 90, 15), align("left")
numberbox bounds(270, 455, 40, 15), channel("plot_y_scale"), range(0.0, 99, 1.0)
numberbox bounds(320, 455, 40, 15), channel("plot_y_offset"), range(-1.0, 1.0, 0.0)

label text("c"), bounds(270, 475, 15, 15), align("left")
combobox channel("plot_c"), bounds(270, 492, 90, 18), items("rms", "cps_n", "pitch_a", "centroid_a", "spread_a", "skewness_a", "kurtosis_a", "flatness_a", "crest_a", "flux_a", "amp_trans", "atransDensEnv", "rhythm_consonance", "rhythmFromOne", "rhythm_ratio1", "rhythm_ratio2", "rhythm_ac1", "rhythm_ac2", "rhythm_ac1time", "rhythm_ac2time", "mfcc1", "mfcc2", "mfcc3", "mfcc4", "mfcc5"), value(1), channeltype("string")
label text("scale  offset"), bounds(270, 512, 90, 15), align("left")
numberbox bounds(270, 535, 40, 15), channel("plot_c_scale"), range(0.0, 99, 1.0)
numberbox bounds(320, 535, 40, 15), channel("plot_c_offset"), range(-1.0, 1.0, 0.0)

image bounds(5, 315, 260, 260), shape("sharp"), colour(0, 0, 0)
image bounds(-100, -100, 1, 1), widgetarray("plot", 200), shape("ellipse"), colour(255, 255, 255)

csoundoutput bounds(5, 585, 360, 145), text("Output")



gentable bounds(370,  5, 320, 240), identchannel("displays"), tablenumber(1), tablecolour("lightblue"), tablegridcolour(0,0,0,0), amprange(-.03,1,1), zoom(-1), samplerange(0,20)

image bounds(370, 5, 64,240), shape("sharp"), colour(175, 50,255, 40), identchannel("group_amp")	
label text("noisefloor"), bounds(386, 250, 200, 15), align("left"), rotate(1.5708, 0, 0)
label text("krms"), bounds(402, 250, 200, 15), align("left"), rotate(1.5708, 0, 0)
label text("trans dens"), bounds(418, 250, 200, 15), align("left"), rotate(1.5708, 0, 0)
label text("env crest"), bounds(434, 250, 200, 15), align("left"), rotate(1.5708, 0, 0)

image bounds(434, 5, 32 ,240), shape("sharp"), colour(75,255, 75, 40), identchannel("group_pitch")	
label text("cps"), bounds(450, 250, 200, 15), align("left"), rotate(1.5708, 0, 0)
label text("pitch"), bounds(466, 250, 200, 15), align("left"), rotate(1.5708, 0, 0)

image bounds(466, 5, 64,240), shape("sharp"), colour(45, 45,255, 40), identchannel("group_spectral1")	
label text("centroid"), bounds(482, 250, 200, 15), align("left"), rotate(1.5708, 0, 0)
label text("spread"), bounds(498, 250, 200, 15), align("left"), rotate(1.5708, 0, 0)
label text("skewness"), bounds(514, 250, 200, 15), align("left"), rotate(1.5708, 0, 0)
label text("kurtosis"), bounds(530, 250, 200, 15), align("left"), rotate(1.5708, 0, 0)

image bounds(530, 5, 48,240), shape("sharp"), colour(255,255, 50, 20), identchannel("group_spectral2")	
label text("flatness"), bounds(546, 250, 190, 15), align("left"), rotate(1.5708, 0, 0)
label text("crest"), bounds(562, 250, 165, 15), align("left"), rotate(1.5708, 0, 0)
label text("flux"), bounds(578, 250, 145, 15), align("left"), rotate(1.5708, 0, 0)

image bounds(578, 5, 96,240), shape("sharp"), colour(50,200, 255, 20), identchannel("group_mfcc")	
label text("mfcc1"), bounds(594, 250, 145, 15), align("left"), rotate(1.5708, 0, 0)
label text("2"), bounds(610, 250, 145, 15), align("left"), rotate(1.5708, 0, 0)
label text("3"), bounds(626, 250, 145, 15), align("left"), rotate(1.5708, 0, 0)
label text("4"), bounds(642, 250, 145, 15), align("left"), rotate(1.5708, 0, 0)
label text("5"), bounds(658, 250, 145, 15), align("left"), rotate(1.5708, 0, 0)
label text("6"), bounds(674, 250, 145, 15), align("left"), rotate(1.5708, 0, 0)
label text("mfccdiff"), bounds(690, 250, 145, 15), align("left"), rotate(1.5708, 0, 0)

label text("transient dens"), bounds(370, 340, 120, 15), align("left")
label text("0"), bounds(370, 360, 200, 15), align("left"), identchannel("ampTransDensity")	
label text("env crest"), bounds(490, 340, 120, 15), align("left")
label text("0"), bounds(490, 360, 200, 15), align("left"), identchannel("envCrest")	


label text("rhythm consonance"), bounds(370, 430, 200, 15), align("left")
label text("0"), bounds(370, 450, 40, 15), align("left"), identchannel("rhythmFromOne")	
label text("0"), bounds(410, 450, 40, 15), align("left"), identchannel("rhythmConsonance")	
label text("0"), bounds(450, 450, 40, 15), align("left"), identchannel("rhythmConsonanceDeviation")
label text("dev"), bounds(495, 450, 200, 15), align("left")
gentable bounds(370, 470, 160, 140), identchannel("rhythm_consonance"), tablenumber(5,6,7), tablecolour("lightblue", "red", "darkolivegreen"), tablegridcolour(0,0,0,0), amprange(0,1,5), amprange(0,1,6), amprange(0,1,7), zoom(-1), samplerange(0,32)
label text("latest rhythm ratios"), bounds(370, 620, 200, 15), align("left")
label text("0"), bounds(370, 640, 100, 15), align("left"), identchannel("rhythmratio1")	
label text("0"), bounds(420, 640, 100, 15), align("left"), identchannel("rhythmratio2")	
label text("0"), bounds(470, 640, 100, 15), align("left"), identchannel("rhythmratio3")	

label text("rhythm autocorr"), bounds(540, 430, 200, 15), align("left")
gentable bounds(540, 470, 1160, 140), identchannel("rhythm_autocorr"), tablenumber(10), tablecolour("lightblue"), tablegridcolour(0,0,0,0), amprange(0,1,10), zoom(-1), samplerange(0,1024)
;numberbox bounds(540, 450, 40, 15), channel("rhythmAutocorrEnvShape"), range(0.001, 0.20, 0.05)
;label text("ra env"), bounds(585, 450, 200, 15), align("left")
numberbox bounds(540, 620, 40, 15), channel("rhythmAutocorrPeakDelta"), range(0.01, 0.50, 0.1)
label text("ra thresh"), bounds(585, 620, 200, 15), align("left")

label text("autocorr peaks"), bounds(540, 640, 200, 15), align("left")
label text("0"), bounds(540, 660, 100, 15), align("left"), identchannel("rhythmauto1indx")	
label text("0"), bounds(590, 660, 100, 15), align("left"), identchannel("rhythmauto2indx")	
label text("0"), bounds(640, 660, 100, 15), align("left"), identchannel("rhythmauto3indx")	
label text("0"), bounds(540, 680, 100, 15), align("left"), identchannel("rhythmauto1")	
label text("0"), bounds(590, 680, 100, 15), align("left"), identchannel("rhythmauto2")	
label text("0"), bounds(640, 680, 100, 15), align("left"), identchannel("rhythmauto3")	

label text("autocorr crest"), bounds(540, 700, 200, 15), align("left")
label text("0"), bounds(540, 720, 100, 15), align("left"), identchannel("rhythmautocrest")	


</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-n -d -+rtmidi=NULL -M0 -m0d
;-iadc -odac
</CsOptions>
<CsInstruments>

        sr = 48000
        ksmps = 64
	nchnls = 2
	0dbfs = 1	

        gi1     ftgen   1, 0, 32, -2, 0  ; analysis signal display
        gi5     ftgen   5, 0, 32, -2, 0  ; rhythm consonance display
        gi6     ftgen   6, 0, 32, -2, 0  ; rhythm consonance display
        gi7     ftgen   7, 0, 32, -2, 0  ; rhythm consonance display
        gi10    ftgen   10, 0, 1024, -2, 0 ; rhythm autocorr display
	giSine	ftgen	0, 0, 65536, 10, 1			; sine wave
	gifftsize 	= 1024
			chnset gifftsize, "fftsize"
	giFftTabSize	= (gifftsize / 2)+1
	gifna     	ftgen   1 ,0 ,giFftTabSize, 7, 0, giFftTabSize, 0   	; for pvs analysis
	gifnf     	ftgen   2 ,0 ,giFftTabSize, 7, 0, giFftTabSize, 0   	; for pvs analysis

	giSinEnv        ftgen   0, 0, 8192, 19, 1, 0.5, 270, 0.5        ; sinoid transient envelope shape for autocorr

        ;gifnTempomem    ftgen   0, 0, 128, 16, 1, 128, 1, 0

        ; colourspace index 0-96 (6*16)
        gicolourindxlength = 72 ; (use only red-to-blue colur spectrum, skip the purple so we know that blue is max value for plot display)
        giRed   ftgen 0, 0, 256, -7, 255, 16, 255, 16, 0, 32, 0, 16, 255, 16, 255
        giGreen ftgen 0, 0, 256, -7,   0, 16, 255, 32, 255, 16, 0, 32, 0
        giBlue  ftgen 0, 0, 256, -7,   0, 32, 0, 16, 255, 32, 255, 16, 0


#include "analyze_udos.inc"


;**************************
	instr 1
	pyinit
        pyruni "import sys"
	pyruni "import os"
	
	pyruni "sys.path.append(os.getcwd())"	; needed for OSX
        pyruni "sys.path.append('c:\\python27\\DLLs')" ; needed for Win
        
	; needed when reloading a saved project in a DAW, as the cwd will have changed
	Spath 	chnget "CSD_PATH"
	Schdir	strcat "os.chdir('", Spath
	Schdir2	strcat Schdir, "')"
	;pyruni Schdir2
	pyruni "sys.path.append(os.getcwd())"
	
        pyruni "import peakdetect_wrapper"
        pyruni "p = peakdetect_wrapper.PeakDetector()"
        pyruni "import rational_approx as r"

;#include "analyze_chn_init.inc"
	endin

;**************************
	instr 2
#include "analyze_chn_init.inc"
	endin

;**************************
        instr 3
	a1,a2	        ins

;	                outs a1, a2
        kpause chnget "pause"
        if kpause > 0 kgoto skip

#include "analyze_audio.inc"

; analysis sig display in gui table
        tablew pow(ampdbfs(knoiseFloor_dB),0.4), 0, gi1 
        tablew pow(krms,0.4), 1, gi1 
        tablew katransDensEnv/8, 2, gi1 
        tablew kenv_crest1/35, 3, gi1 
        tablew kcps_n, 4, gi1 
        tablew kpitch_a, 5, gi1 
        tablew kcentroid_a, 6, gi1 
        tablew kspread_a, 7, gi1 
        tablew kskewness_a, 8, gi1 
        tablew kurtosis_a, 9, gi1 
        tablew kflatness_a, 10, gi1 
        tablew kcrest_a, 11, gi1 
        tablew kflux_a, 12, gi1 
        imfcc_min = -0.3
        imfcc_max = 1.0
        imfcc_range = imfcc_max-imfcc_min
        tablew (kmfcc1*imfcc_range)-imfcc_min, 13, gi1
        tablew (kmfcc2*imfcc_range)-imfcc_min, 14, gi1
        tablew (kmfcc3*imfcc_range)-imfcc_min, 15, gi1
        tablew (kmfcc4*imfcc_range)-imfcc_min, 16, gi1
        tablew (kmfcc5*imfcc_range)-imfcc_min, 17, gi1
        tablew (kmfcc6*imfcc_range)-imfcc_min, 18, gi1
        tablew kmfccdiff, 19, gi1

        kupd    metro 100
        kenableDisplay chnget "enableDisplay"
        if kupd*kenableDisplay > 0 then
 	chnset	"tablenumber(1)", "displays"	; update table display	
 	SatranD sprintfk "text(%.1f)", katransDensEnv
 	chnset	SatranD, "ampTransDensity"	; update gui	
 	SacrestD sprintfk "text(%.1f)", kenv_crest1
 	chnset	SacrestD, "envCrest"	; update gui	
        endif

        if krnewframe*kenableDisplay > 0 then
        copya2ftab kRhythmAuto, gi10
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

 	SrautocrestD sprintfk "text(%i)", krhythm_ac_crest
 	chnset	SrautocrestD, "rhythmautocrest"	; update gui	

        endif

        if krms_tran0>0 then
        krc_indx init 0
 	SrcomplexD sprintfk "text(%.2f)", krhythm_consonance
        chnset	SrcomplexD, "rhythmConsonance"

 	SrcomplexdevD sprintfk "text(%.2f)", krhythm_consonance_deviation
        chnset	SrcomplexdevD, "rhythmConsonanceDeviation"

 	SrhythmFromOneD sprintfk "text(%.2f)", krhythmFromOne
        chnset	SrhythmFromOneD, "rhythmFromOne"


 	Srratio1D sprintfk "text(%.2f)", krhythm_ratio1
 	Srratio2D sprintfk "text(%.2f)", krhythm_ratio2
 	Srratio3D sprintfk "text(%.2f)", krhythm_ratio3
        chnset	Srratio1D, "rhythmratio1"
        chnset	Srratio2D, "rhythmratio2"
        chnset	Srratio3D, "rhythmratio3"
                
        krc_indx = (krc_indx+1)%32
        tablew krhythmFromOne, krc_indx, gi5
        tablew krhythm_consonance, krc_indx, gi6              ; 
        tablew 1, krc_indx, gi7                                 ; set (display) locator into table
        tablew 0, krc_indx-1, gi7, 0, 0, 1                      ; reset previous locator (with wrap)
 	chnset	"tablenumber(5,6,7)", "rhythm_consonance" ; update table display	
        endif
        
        kplotcnt init 0
        kplotenable chnget "plotenable" 
        kplotmetrorate chnget "plotmetrorate"
        kplotmetro metro kplotmetrorate
        kplot_trans vdel_k krms_tran0, kplotmetrorate/1000, 0.1
        kplotupdatemeth chnget "plotupdatemethod"
        kplotupdate = 0
        kplotupdate = (kplotupdatemeth == 1 ? kplotmetro*kplotenable : kplotupdate)
        kplotupdate = (kplotupdatemeth == 2 ? kplot_trans*kplotenable : kplotupdate)
        iplotlength = 200

        iplotmin_x = 5
        iplotrange_x = 260
        iplotmin_y = 315
        iplotrange_y = 260

        if kplotupdate > 0 then        
        kx Plotchannel "plot_x", krms, kcps_n, kpitch_a, kcentroid_a, kspread_a, kskewness_a, kurtosis_a, kflatness_a, kcrest_a, kflux_a, kamp_trans, katransDensEnv, krhythm_consonance, krhythmFromOne, krhythm_ratio1, krhythm_ratio2, krhythm_ac1, krhythm_ac2, krhythm_ac1time, krhythm_ac2time, kmfcc1, kmfcc2, kmfcc3, kmfcc4, kmfcc5
        ky Plotchannel "plot_y", krms, kcps_n, kpitch_a, kcentroid_a, kspread_a, kskewness_a, kurtosis_a, kflatness_a, kcrest_a, kflux_a, kamp_trans, katransDensEnv, krhythm_consonance, krhythmFromOne, krhythm_ratio1, krhythm_ratio2, krhythm_ac1, krhythm_ac2, krhythm_ac1time, krhythm_ac2time, kmfcc1, kmfcc2, kmfcc3, kmfcc4, kmfcc5
        kc Plotchannel "plot_c", krms, kcps_n, kpitch_a, kcentroid_a, kspread_a, kskewness_a, kurtosis_a, kflatness_a, kcrest_a, kflux_a, kamp_trans, katransDensEnv, krhythm_consonance, krhythmFromOne, krhythm_ratio1, krhythm_ratio2, krhythm_ac1, krhythm_ac2, krhythm_ac1time, krhythm_ac2time, kmfcc1, kmfcc2, kmfcc3, kmfcc4, kmfcc5
        kplot_x_scale chnget "plot_x_scale"
        kplot_y_scale chnget "plot_y_scale"
        kplot_c_scale chnget "plot_c_scale"
        kplot_x_offset chnget "plot_x_offset"
        kplot_y_offset chnget "plot_y_offset"
        kplot_c_offset chnget "plot_c_offset"
        kx = limit((kx*kplot_x_scale)+kplot_x_offset, 0, 1)
        ky = 1-limit((ky*kplot_y_scale)+kplot_y_offset, 0, 1)
        kc = limit((kc*kplot_c_scale)+kplot_c_offset, 0, 1)
        
        kx = (kx*iplotrange_x)+iplotmin_x
        ky = (ky*iplotrange_y)+iplotmin_y
        kcoulourindx = (kc*gicolourindxlength)
        kred table kcoulourindx, giRed
        kgreen table kcoulourindx, giGreen
        kblue table kcoulourindx, giBlue
        S1 sprintfk "pos(%d, %d), size(4, 4), colour(%d,%d,%d)", kx, ky, kred, kgreen, kblue
        S2 sprintfk "plot_ident%d", kplotcnt+1
        chnset S1, S2
        kplotcnt = (kplotcnt+1)%iplotlength
        endif

        kplotclear chnget "plotclear"
        if kplotclear > 0 then
        kclearcnt = 0
        while kclearcnt < iplotlength do
        S1 sprintfk "pos(%d, %d), size(4, 4), colour(%d,%d,%d)", 0,0,0,0,0
        S2 sprintfk "plot_ident%d", kclearcnt+1
        chnset S1, S2
        kclearcnt += 1
        od
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
i1	0.1	1               ; init python, must be run in an instr because of chnget for CSD_PATH
;i2	0.1	1               ; init gui values, for running without Cabbage
i3	.1	$SCORELEN
e
</CsScore>
</CsoundSynthesizer>

