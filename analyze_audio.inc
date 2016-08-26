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

; ***************
; pre-emphasis EQ for transient detection,
; allowing better sensitivity to utterances starting with a sibliant.
	kpreEqHiShelfFq	        chnget "preEqHiShelfFq"
	kpreEqHiShelfGain	chnget "preEqHiShelfGain"
	kpreEqLoShelfFq	        chnget "preEqLoShelfFq"
	kpreEqLoShelfGain	chnget "preEqLoShelfGain"
	
        a1preEq	                pareq a1, kpreEqHiShelfFq, ampdb(kpreEqHiShelfGain), 0.7,  2
        a1preEq	                pareq a1preEq, kpreEqHiShelfFq, ampdb(kpreEqLoShelfGain), 0.7,  1
        ;kautogain               = ampdbfs(-kpreEqHiShelfGain)
        ;a1preEq                 = a1preEq*kautogain
; ***************
; amplitude tracking
	krms_preEq	rms a1preEq			    	; simple level measure (with transient pre emphasis)
	krms_preEq      = krms_preEq*2
	krms		rms a1			    		; simple level measure 
	krms            = krms*2
        krms_dB         = dbfsamp(krms)
        krms_dB_n       = (krms_dB/96)+1

        icrestrate      = 4
        kcrestmetro     metro icrestrate
        krms_max        init 0
        krms_max        max krms_max, krms
        krms_max        = (kcrestmetro > 0 ? 0 : krms_max)
	icrestrise      = 1.5
	icrestfall      = 3
        kenv_crest0     divz krms_max, krms, 1
        icrestA         = 0.001^(1/(icrestrise*kr))
        icrestB         = 0.001^(1/(icrestfall*kr))
        kenv_crest      init 0
        kenv_crest      = (kenv_crest0>kenv_crest?(kenv_crest0+(icrestA*(kenv_crest-kenv_crest0))):(kenv_crest0+(icrestB*(kenv_crest-kenv_crest0))))
        kenv_crest1     = dbamp(kenv_crest)
        
	kAttack		= 0.001				        ; envelope follower attack
	kRelease        chnget "amp_transientDecTime"           ; envelope follower release
	a_env		follow2	a1preEq, kAttack, kRelease	; envelope follower
        k_env		downsamp a_env	

	knoiseFloor_dB	chnget "inputNoisefloor"
	kgate		= (krms_dB < knoiseFloor_dB ? 0 : 1)	;  gate when below noise floor (for sampholding centroid and pitch etc)
        isecond_dB      = 9
	kgate2		= (krms_dB < knoiseFloor_dB+isecond_dB ? 0 : 1)	;  gate when close to noise floor (for sampholding centroid and pitch etc)
        klowscaler      = limit(dbfsamp(krms)-knoiseFloor_dB, 0, isecond_dB)/isecond_dB      ; scaler to fade out different things towards the noise floor


; ***************
;analyze amplitude transients
	iresponse	        = 10 			                ; response time in milliseconds
	ktthresh	        chnget "amp_transientThresh"            ; transient trig threshold 
	klowThresh	        = knoiseFloor_dB                        ; lower threshold for transient detection
        kdoubleLimit            chnget "amp_transientDoubleLimit"       ; minimum duration between events, (double trig limit)
        kdecThresh	        chnget "amp_transientDecThresh"         ; retrig threshold, how much must the level decay from its local max before allowing new transient trig

	krms_tran0, kdiff	TransientDetect dbfsamp(k_env), iresponse, ktthresh, klowThresh, kdecThresh, kdoubleLimit

        ktime           timeinsts
        kprevaTranTime  init 0
        katranDensity   init 0
;Control segment for LEDs displaying transient detection:
        ktrigButn       trigger	krms_tran0, 0.5, 0
        if ktrigButn > 0 then
        katranTime      = ktime-kprevaTranTime
        kprevaTranTime  = ktime
        katranDensity_t divz 1, katranTime, 1           ; this will be accurate for slow tempi, but not if you play e.g. two fast consecutive transients followed by a puase
	                reinit lightTransient
        endif
lightTransient:
        kamp_trans      linseg 1,0.05,1,0,0,1,0
                        chnset kamp_trans, "transientDisplay"
rireturn

        katran1second   delayk krms_tran0, 1
        katranCount     init 0
        katranCount     += krms_tran0
        katranCount     -= katran1second                ; this will be accurate when playing an integer number of transient within each period  
                                                        ; but it will have jitter when playing at a tempo of e.g. 3.5 transients per second

        
        katranDensity   = (katranDensity_t < 1 ? katranDensity_t : katranCount)
        katranDensity   = (abs(katranDensity_t-katranCount) < 1 ? katranDensity_t : katranCount)
        iatranrise      = 0.5
	iatrandec       = 4
        itrandensA      = exp(-6.90775527898 * (1/iatranrise)/kr)
        itrandensB      = exp(-6.90775527898 * (1/iatrandec)/kr)
        katransDensEnv  init 0
        katransDensEnv   = (katranDensity>katransDensEnv?(katranDensity+(itrandensA*(katransDensEnv-katranDensity))):(katranDensity+(itrandensB*(katransDensEnv-katranDensity))))
        
; ***************
; rhythmic analysis

; one measure of rhythhmic irrregularity is the relative time ratios between conescutive transients
; we measure here the ratio between the currnet and the previous, the current and the next last, and the current and the third last deltatimes
; we multiply these three measures, as it will give a higher regularity measure if there is some short term repetition

; another measure of irregularity is the distance from a 1:1 ratio (regular beat), where large changes in tempo give high irregularity

ibufsize_ry = 8
kcnt_ry init 0
kArr_ry[] init ibufsize_ry

if krms_tran0 > 0 then
kArr_ry[kcnt_ry] = ktime
k_0 = kArr_ry[kcnt_ry]
k_1 = kArr_ry[wrap(kcnt_ry-1,0,ibufsize_ry)]
k_2 = kArr_ry[wrap(kcnt_ry-2,0,ibufsize_ry)]
k_3 = kArr_ry[wrap(kcnt_ry-3,0,ibufsize_ry)]
k_4 = kArr_ry[wrap(kcnt_ry-4,0,ibufsize_ry)]
kdist1 = (k_0 - k_1)
kdist2 = (k_1 - k_2)
kdist3 = (k_2 - k_3)
kdist4 = (k_3 - k_4)
krel1 divz kdist1, kdist2, 1
krel2 divz kdist1, kdist3, 1
krel3 divz kdist1, kdist4, 1

; for rational approximation, we use table lookup since there are a lot of specific fractions sthat we do not want
; up to 1.0
iAfra1[] fillarray 0.0625,  0.125,   0.1666,  0.25,    0.333,   0.5,     0.667,   0.75,    1.0
iAnom1[] fillarray 1,       1,       1,       1,       1,       1,       2,       3,       1
iAden1[] fillarray 16,      8,       6,       4,       3,       2,       3,       4,       1
; from 1.0 and up
iAfra2[] fillarray 1.0,     1.33,    1.5,     2.0,     3.0,     4.0,     6.0,     8.0,     12.0,    16.0
iAnom2[] fillarray 1,       4,       3,       2,       3,       4,       6,       8,       12,      16        
iAden2[] fillarray 1,       3,       2,       1,       1,       1,       1,       1,       1,       1

ifra2max maxarray iAfra2
inomtablen = 512

inomtab1  ftgen 0, 0, inomtablen, -17,  0, iAnom1[0],
                                     int((iAfra1[1]-((iAfra1[1]-iAfra1[0])/2))*inomtablen), iAnom1[1],
                                     int((iAfra1[2]-((iAfra1[2]-iAfra1[1])/2))*inomtablen), iAnom1[2],
                                     int((iAfra1[3]-((iAfra1[3]-iAfra1[2])/2))*inomtablen), iAnom1[3],
                                     int((iAfra1[4]-((iAfra1[4]-iAfra1[3])/2))*inomtablen), iAnom1[4],
                                     int((iAfra1[5]-((iAfra1[5]-iAfra1[4])/2))*inomtablen), iAnom1[5],
                                     int((iAfra1[6]-((iAfra1[6]-iAfra1[5])/2))*inomtablen), iAnom1[6],
                                     int((iAfra1[7]-((iAfra1[7]-iAfra1[6])/2))*inomtablen), iAnom1[7],
                                     int((iAfra1[8]-((iAfra1[8]-iAfra1[7])/2))*inomtablen), iAnom1[8]

identab1  ftgen 0, 0, inomtablen, -17,  0, iAden1[0],
                                     int((iAfra1[1]-((iAfra1[1]-iAfra1[0])/2))*inomtablen), iAden1[1],
                                     int((iAfra1[2]-((iAfra1[2]-iAfra1[1])/2))*inomtablen), iAden1[2],
                                     int((iAfra1[3]-((iAfra1[3]-iAfra1[2])/2))*inomtablen), iAden1[3],
                                     int((iAfra1[4]-((iAfra1[4]-iAfra1[3])/2))*inomtablen), iAden1[4],
                                     int((iAfra1[5]-((iAfra1[5]-iAfra1[4])/2))*inomtablen), iAden1[5],
                                     int((iAfra1[6]-((iAfra1[6]-iAfra1[5])/2))*inomtablen), iAden1[6],
                                     int((iAfra1[7]-((iAfra1[7]-iAfra1[6])/2))*inomtablen), iAden1[7],
                                     int((iAfra1[8]-((iAfra1[8]-iAfra1[7])/2))*inomtablen), iAden1[8]


inomtab2  ftgen 0, 0, inomtablen, -17,  0, iAnom2[0],
                                     int((iAfra2[1]-((iAfra2[1]-iAfra2[0])/2))*inomtablen/ifra2max), iAnom2[1],
                                     int((iAfra2[2]-((iAfra2[2]-iAfra2[1])/2))*inomtablen/ifra2max), iAnom2[2],
                                     int((iAfra2[3]-((iAfra2[3]-iAfra2[2])/2))*inomtablen/ifra2max), iAnom2[3],
                                     int((iAfra2[4]-((iAfra2[4]-iAfra2[3])/2))*inomtablen/ifra2max), iAnom2[4],
                                     int((iAfra2[5]-((iAfra2[5]-iAfra2[4])/2))*inomtablen/ifra2max), iAnom2[5],
                                     int((iAfra2[6]-((iAfra2[6]-iAfra2[5])/2))*inomtablen/ifra2max), iAnom2[6],
                                     int((iAfra2[7]-((iAfra2[7]-iAfra2[6])/2))*inomtablen/ifra2max), iAnom2[7],
                                     int((iAfra2[8]-((iAfra2[8]-iAfra2[7])/2))*inomtablen/ifra2max), iAnom2[8],
                                     int((iAfra2[9]-((iAfra2[9]-iAfra2[8])/2))*inomtablen/ifra2max), iAnom2[9]

                                     
identab2  ftgen 0, 0, inomtablen, -17,  0, iAden2[0],
                                     int((iAfra2[1]-((iAfra2[1]-iAfra2[0])/2))*inomtablen/ifra2max), iAden2[1],
                                     int((iAfra2[2]-((iAfra2[2]-iAfra2[1])/2))*inomtablen/ifra2max), iAden2[2],
                                     int((iAfra2[3]-((iAfra2[3]-iAfra2[2])/2))*inomtablen/ifra2max), iAden2[3],
                                     int((iAfra2[4]-((iAfra2[4]-iAfra2[3])/2))*inomtablen/ifra2max), iAden2[4],
                                     int((iAfra2[5]-((iAfra2[5]-iAfra2[4])/2))*inomtablen/ifra2max), iAden2[5],
                                     int((iAfra2[6]-((iAfra2[6]-iAfra2[5])/2))*inomtablen/ifra2max), iAden2[6],
                                     int((iAfra2[7]-((iAfra2[7]-iAfra2[6])/2))*inomtablen/ifra2max), iAden2[7],
                                     int((iAfra2[8]-((iAfra2[8]-iAfra2[7])/2))*inomtablen/ifra2max), iAden2[8],
                                     int((iAfra2[9]-((iAfra2[9]-iAfra2[8])/2))*inomtablen/ifra2max), iAden2[9]

if krel1 <= 1 then
knom1 table krel1, inomtab1, 1
kden1 table krel1, identab1, 1
kfrom_one1 = 1-krel1
else
knom1 table krel1*(inomtablen/ifra2max), inomtab2
kden1 table krel1*(inomtablen/ifra2max), identab2
kfrom_one1 = krel1/ifra2max
endif
kquant1 = knom1/kden1
kdev1 = abs(krel1-kquant1)

if krel2 <= 1 then
knom2 table krel2, inomtab1, 1
kden2 table krel2, identab1, 1
kfrom_one2 = 1-krel2
else
knom2 table krel2*(inomtablen/ifra2max), inomtab2
kden2 table krel2*(inomtablen/ifra2max), identab2
kfrom_one2 = krel2/ifra2max
endif
kquant2 = knom2/kden2
kdev2 = abs(krel2-kquant2)

if krel3 <= 1 then
knom3 table krel3, inomtab1, 1
kden3 table krel3, identab1, 1
kfrom_one3 = 1-krel3
else
knom3 table krel3*(inomtablen/ifra2max), inomtab2
kden3 table krel3*(inomtablen/ifra2max), identab2
kfrom_one3 = krel3/ifra2max
endif
kquant3 = knom3/kden3
kdev3 = abs(krel3-kquant3)

krhythm_consonance_deviation = (kdev1);+kdev2+kdev3)/3

;Benedetti height
kben1 = (knom1*kden1)
kben2 = (knom2*kden2)
kben3 = (knom3*kden3)

;Tenney height is log2(Benedetti height)
kten1 = (log(knom1*kden1)/3)+0.2
kten2 = (log(knom2*kden2)/3)+0.2
kten3 = (log(knom3*kden3)/3)+0.2

; multiply the last three tenney heights
; it will allow for low values when there is some short term repetition
krhythm_cons_x = kten1*kten2*kten3

;fir
krhythm_cons_x0 init 0
krhythm_cons_x = (krhythm_cons_x+krhythm_cons_x0)/2
krhythm_cons_x0 = krhythm_cons_x
;iir
krhythm_cons_y init 0
ircfilter_a = 0.4
ircfilter_b = 0.6
krhythm_cons_y = (krhythm_cons_x*ircfilter_a) + (krhythm_cons_y*ircfilter_b)
krhythm_consonance = krhythm_cons_y

; filter the "distance from one" measure
; multiply the two highest out of the last three
kfrom_ox = max(kfrom_one1,kfrom_one2)*max(kfrom_one2,kfrom_one3)
; fir
kfrom_ox0 init 0
kfrom_ox = (kfrom_ox+kfrom_ox0)/2
kfrom_ox0 = kfrom_ox
; iir
kfrom_oy init 0
kfrom_oy = (kfrom_ox*ircfilter_a) + (kfrom_oy*ircfilter_b)
krhythmFromOne = kfrom_oy

krhythm_ratio1 = knom1/kden1
krhythm_ratio2 = knom2/kden2
krhythm_ratio3 = knom3/kden3

kcnt_ry wrap (kcnt_ry+1), 0, ibufsize_ry
endif


;*************************************
; ** rhythm autocorrelation

; sinoid trigger envelope, allows (soft) rhythmic deviation from strict pattern
ishapedur = 0.1
if krms_tran0 == 1 then
reinit triggershape
endif
triggershape:
kra_env linseg 0, ishapedur*0.5, 1, ishapedur*0.5, 0, 1, 0
kra_env = kra_env * krms * kgate
rireturn

; autocorrelation, done in layers of different fftsize to reduce latency
irhythm_sr = 60
ira_fftsize = 512
ira_overlaps = 2
kRhythmAuto[], krnewframe, knumtransients RhythmAutoCorr kra_env, krms_tran0, irhythm_sr, ira_fftsize, ira_overlaps
kRhythmAuto2[], krnewframe2, knumtransients2 RhythmAutoCorr kra_env, krms_tran0, irhythm_sr, ira_fftsize/2, ira_overlaps
kRhythmAuto4[], krnewframe4, knumtransients4 RhythmAutoCorr kra_env, krms_tran0, irhythm_sr, ira_fftsize/4, ira_overlaps
;kRhythmAuto slicearray kRhythmAuto2, 0, ira_fftsize/8 ; take only the first quarter (as the second quarter will already be tapering off)
;kRhythmAuto slicearray kRhythmAuto4, 0, ira_fftsize/16 ; as above, since this is 1/4 the larger fftsize, 1/16 is a quarter of this autocorr output

kra_peak_delta chnget "rhythmAutocorrPeakDelta" ; peak picking delta 

; WE MIGHT WANT TO DO THIS FOR ALL 3 LAYERS OF AC ANALYSIS, THEN SELECT ONE (MAX, MIN, AVERAGE?)
kra_max1_i, kra_max2_i, kra_max3_i, kra_max1_v, kra_max2_v, kra_max3_v,
kra_first_i, kra_first_v,
kra_closest_i, kra_closest_v,
krdenom_first, krdenom_max2, krdenom_max3, krdenom_closest,
kgrid_affirmativity, kgrid_subdiv AutoCorrPeaks kRhythmAuto, krnewframe, kra_peak_delta, ira_fftsize

kra_first_time =  kra_first_i/irhythm_sr
kra_max1_time =  kra_max1_i/irhythm_sr
kra_max2_time =  kra_max2_i/irhythm_sr
kra_max3_time =  kra_max3_i/irhythm_sr
kra_closest_time =  kra_closest_i/irhythm_sr

krhythm_ac_rms = sqrt(sumarray(kRhythmAuto*kRhythmAuto)/ira_fftsize)
krhythm_ac_max = maxarray(kRhythmAuto*kRhythmAuto)
krhythm_ac_crest0 divz krhythm_ac_max, krhythm_ac_rms, 1
krhythm_ac_crest = (krhythm_ac_rms/(log2(knumtransients+1)+1))*100

; ***************
; spectral analysis

	iwtype 			= 1
	fsin 			pvsanal	a1, gifftsize, gifftsize/4, gifftsize, iwtype
	kflag   		pvsftw	fsin,gifna,gifnf          	; export  amps  and freqs to table,

	kupdateRate		= 500
	kmetro			metro kupdateRate
	kdoflag			init 0
	kdoflag			= (kdoflag + kmetro);*kgate

	; copy pvs data from table to array
	; analyze spectral features
	kArrA[]  		init    giFftTabSize
	kArrAprev[]  		init    giFftTabSize
	kArrF[]  		init    giFftTabSize
	kArrCorr[]  		init    giFftTabSize
	kflatness		init 0

if (kdoflag > 0) && (kflag > 0) then
	kArrAprev[]		= kArrA
        			copyf2array kArrA, gifna
        			copyf2array kArrF, gifnf	
	kindx 			= 0
	kcentroid		= 0
	ksumAmp			sumarray kArrA
	kmaxAmp			maxarray kArrA
        kflatlogsum		= 0
	kcorrSum		= 0
	kthisSum2		= 0
	kprevSum2		= 0

        kcentroid               pvscent fsin
        
; loop for spread, skewness, kurtosis (depend on centroid being previously calculated) 
	kindx 			= 0
	kspread			= 0
	kskewness		= 0
	kurtosis		= 0
	kmean_geo               = 1
 process2:
	kArrCorr[kindx]		= kArrA[kindx]*kArrAprev[kindx]
	knormAmp		divz kArrA[kindx], ksumAmp, 0
	kspread			= kspread+ (((kArrF[kindx] - kcentroid)^2)*knormAmp)
	kskewness		= kskewness + ((kArrF[kindx] - kcentroid)^3)*knormAmp
	kurtosis		= kurtosis + ((kArrF[kindx] - kcentroid)^4)*knormAmp
	kflatlogsum		= (kArrA[kindx] !=0 ? kflatlogsum + log(kArrA[kindx]) : kflatlogsum)
	kcorrSum		= kcorrSum + (kArrAprev[kindx]*kArrA[kindx])
	kprevSum2		= kprevSum2 + (kArrAprev[kindx]^2)
	kthisSum2		= kthisSum2 + (kArrA[kindx]^2)
	kindx 			= kindx + 1
  if kindx < giFftTabSize then
  kgoto process2
  endif
	kspread			= kspread^0.5
	kskewness		divz kskewness, kspread^3, 0
	kurtosis		divz kurtosis, kspread^4, 0
	kflatness		divz exp(kflatlogsum/giFftTabSize),  (ksumAmp/giFftTabSize), 0
        kcrest			divz kmaxAmp, ksumAmp/giFftTabSize, 1
	kflux_1			divz kcorrSum, (sqrt(kprevSum2)*sqrt(kthisSum2)), 1
	kflux			= (1-kflux_1)
	kdoflag 		= 0

; ** filter hack to keep spectral signals at the value analyzed while sound level above noise floor
	kcentroid		samphold kcentroid, kgate
	kcentroid2		samphold kcentroid, kgate2
        kcentroid               = (kgate2 > 0 ? kcentroid : kcentroid2-((kcentroid-kcentroid2)*((1-klowscaler)*0.25)))
	kspread		        samphold kspread, kgate
	kspread2		samphold kspread, kgate2
        kspread                 = (kgate2 > 0 ? kspread : kspread2-((kspread-kspread2)*((1-klowscaler)*0.25)))
	kskewness		samphold kskewness, kgate
	kskewness2		samphold kskewness, kgate2
        kskewness               = (kgate2 > 0 ? kskewness : kskewness2-((kskewness-kskewness2)*((1-klowscaler)*0.25)))
	kurtosis		samphold kurtosis, kgate
	kurtosis2		samphold kurtosis, kgate2
        kurtosis                = (kgate2 > 0 ? kurtosis : kurtosis2-((kurtosis-kurtosis2)*((1-klowscaler)*0.25)))
	kflatness		samphold kflatness, kgate
	kflatness2		samphold kflatness, kgate2
        kflatness               = (kgate2 > 0 ? kflatness : kflatness2-((kflatness-kflatness2)*((1-klowscaler)*0.25)))
	kcrest		        samphold kcrest, kgate
	kcrest2		        samphold kcrest, kgate2
        kcrest                   = (kgate2 > 0 ? kcrest : kcrest2-((kcrest-kcrest2)*((1-klowscaler)*0.25)))
	kflux		        samphold kflux, kgate
	kflux2		        samphold kflux, kgate2
        kflux                   = (kgate2 > 0 ? kflux : kflux2-((kflux-kflux2)*((1-klowscaler)*0.25)))

endif

	kautocorr		sumarray kArrCorr
	krmsA			sumarray kArrA
	krmsAprev		sumarray kArrAprev
	kautocorr		divz kautocorr*2, krmsA*krmsAprev , 0

; post filtering of spectral tracks
        kcentroidf              tonek kcentroid, 20
        kfluxf                  tonek kflux, 20
        ;kcentroidf              mediank kcentroidf, 11, 11
        ;kurtosis                mediank kurtosis, 11, 11

; ***************
; cepstrum, mfcc

 ibins init gifftsize/2
 kIn[] init gifftsize
 kcnt init 0
 kIn shiftin a1
 kcnt += ksmps
 if kcnt == gifftsize then
  kFFT[] = rfft(kIn)
  kPows[] = pows(kFFT)
  kMFB[] = log(mfb(kPows,300,8000,32))
  kmfcc[] = dct(kMFB)
  kcnt = 0
 endif




; ***************
; pitch tracking
; using four different methods
; ptrack may be better for polyphonic signals
; plltrack probably better for monophonic signals
; pitchamdf is also a good alternative, all depending on the material to be sensed
; pitchtracking based on epoch filtering is effective for vocal signals and speech but may also work ok in other contexts

        kpitchmethod    chnget "pitchmethod"
        kpitch_low      chnget "pitch_low"
        kpitch_high     chnget "pitch_high"
        kpitch_low      init 100
        kpitch_high     init 1000
         
        ; reinit pitchamdf when pitch range changes
        kp_range_change changed kpitch_low, kpitch_high
        if kp_range_change > 0 then
        reinit pamdf
        endif
        
if kpitchmethod == 1 then
	kcps 		init 0	
	ihopsize	= 512
	kcps, ka0 	ptrack a1, ihopsize
        
elseif kpitchmethod == 2 then
	kd 		= 0.1
	kloopf		= 20
	kloopq		= 0.3
	acps, alockp	plltrack a1, kd, kloopf, kloopq, kpitch_low, kpitch_high, ampdbfs(knoiseFloor_dB-8)
	kcps		downsamp acps

elseif kpitchmethod == 3 then
pamdf:
        kcps,k0	        pitchamdf a1, i(kpitch_low), i(kpitch_high), i(kpitch_low), 0 , 1 ,i(kpitch_low) ,0
        rireturn

elseif kpitchmethod == 4 then
	kcps    	EpochCps a1     ; pitch analysis by epoch filtering and zero cross count

endif
	kmedianSize	chnget "pitchFilterSize"
	kcps	        mediank	kcps, kmedianSize, 256
        kcps            limit kcps, kpitch_low, kpitch_high
        kcps            tonek kcps, 50

	kcps		samphold kcps, kgate2
        ksemitone       = limit:k((log2(kcps/440)*12)+69, 0, 127)
        

; ***************
; normalization
        kcps_n          = divz(kcps, kpitch_high, 1)
        kpitch_n        = limit(divz(kcps-kpitch_low, kpitch_high-kpitch_low, 1), 0, 1)    ; normalized and offset
        kcentroid_n     = kcentroidf / (sr*0.15)
        kspread_n       = kspread / (sr*0.2)
        kskewness_n     = kskewness / 20
        kurtosis_n      = kurtosis / 250
        kflatness_n     = kflatness * 3
        kcrest_n        = kcrest / 250
        kflux_n         = kfluxf * 4
        kautocorr_n     = kautocorr * 2
        imfccscale      = 1/200
        kmax            maxarray kmfcc
        kmin            minarray kmfcc
        kmfcc1          samphold kmfcc[1]*imfccscale, kgate
        kmfcc2          samphold kmfcc[2]*imfccscale, kgate
        kmfcc3          samphold kmfcc[3]*imfccscale, kgate
        kmfcc4          samphold kmfcc[4]*imfccscale, kgate
        kmfcc5          samphold kmfcc[5]*imfccscale, kgate
        kmfcc6          samphold kmfcc[6]*imfccscale, kgate
        kmfccdiff       = (abs((kmfcc1-kmfcc2)*0.5)+
                          abs(kmfcc2-kmfcc3)+
                          abs(kmfcc3-kmfcc4)+
                          abs(kmfcc4-kmfcc5)+
                          abs(kmfcc5-kmfcc6))
        kmfccdiff       mediank kmfccdiff, 60, 60
        kmfccdiff       tonek kmfccdiff, 1

; auto-normalize
        kcalibrate      chnget "autocalibrate"
        kreset          trigger kcalibrate, 0.5, 0
        knormalize      chnget "autonormalize"
        iperiod         = 4
        ;krms           AutoNormalize krms, kreset, kcalibrate, knormalize, iperiod
        ;krms_preEq     AutoNormalize krms_preEq, kreset, kcalibrate, knormalize, iperiod
        kpitch_a        AutoNormalize kpitch_n, kreset, kcalibrate, knormalize, iperiod
        kcentroid_a     AutoNormalize kcentroid_n, kreset, kcalibrate, knormalize, iperiod
        kspread_a       AutoNormalize kspread_n, kreset, kcalibrate, knormalize, iperiod
        kskewness_a     AutoNormalize kskewness_n, kreset, kcalibrate, knormalize, iperiod
        kurtosis_a      AutoNormalize kurtosis_n, kreset, kcalibrate, knormalize, iperiod
        kflatness_a     AutoNormalize kflatness_n, kreset, kcalibrate, knormalize, iperiod
        kcrest_a        AutoNormalize kcrest_n, kreset, kcalibrate, knormalize, iperiod
        kflux_a         AutoNormalize kflux_n, kreset, kcalibrate, knormalize, iperiod

;        kmfcc1          AutoNormalize kmfcc[1], kreset, kcalibrate, knormalize, iperiod
;        kmfcc2          AutoNormalize kmfcc[2], kreset, kcalibrate, knormalize, iperiod
;        kmfcc3          AutoNormalize kmfcc[3], kreset, kcalibrate, knormalize, iperiod
;        kmfcc4          AutoNormalize kmfcc[4], kreset, kcalibrate, knormalize, iperiod
;        kmfcc5          AutoNormalize kmfcc[5], kreset, kcalibrate, knormalize, iperiod

        krms            limit krms, 0, 1
        krms_dB_n       limit krms_dB_n, 0, 1
        krms_preEq      limit krms_preEq, 0, 1
        kpitch_a        limit kpitch_a, 0, 1
        kcentroid_a     limit kcentroid_a, 0, 1
        kspread_a       limit kspread_a, 0, 1
        kskewness_a     limit kskewness_a, 0, 1
        kurtosis_a      limit kurtosis_a, 0, 1
        kflatness_a     limit kflatness_a, 0, 1
        kcrest_a        limit kcrest_a, 0, 1
        kflux_a         limit kflux_a, 0, 1
        ;kmfcc1          limit kmfcc1, 0, 1
        ;kmfcc2          limit kmfcc2, 0, 1
        ;kmfcc3          limit kmfcc3, 0, 1
        ;kmfcc4          limit kmfcc4, 0, 1
        ;kmfcc5          limit kmfcc5, 0, 1
