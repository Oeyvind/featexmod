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
	;kpreEqLoShelfFq	        chnget "preEqLoShelfFq"
	;kpreEqLoShelfGain	chnget "preEqLoShelfGain"
	
        a1preEq	                pareq a1, kpreEqHiShelfFq, ampdb(kpreEqHiShelfGain), 0.7,  2
        ;a1preEq	                pareq a1preEq, kpreEqHiShelfFq, ampdb(kpreEqLoShelfGain), 0.7,  1
        ;kautogain               = ampdbfs(-kpreEqHiShelfGain)
        ;a1preEq                 = a1preEq*kautogain
; ***************
; amplitude tracking
	krms_preEq	rms a1preEq			    	; simple level measure (with transient pre emphasis)
	krms_preEq      = krms_preEq*2
	krms		rms a1			    		; simple level measure 
	krms            = krms*2
        krms_dB         = dbfsamp(krms)
                
	kAttack		= 0.001				        ; envelope follower attack
	kRelease        = 0.5;chnget "amp_transientDecTime"           ; envelope follower release
	a_env		follow2	a1preEq, kAttack, kRelease	; envelope follower
        k_env		downsamp a_env	

	knoiseFloor_dB	chnget "inputNoisefloor"
	kgate		= (krms_dB < knoiseFloor_dB ? 0 : 1)	;  gate when below noise floor (for sampholding centroid and pitch etc)
        isecond_dB      = 9
	kgate2		= (krms_dB < knoiseFloor_dB+isecond_dB ? 0 : 1)	;  gate when close to noise floor (for sampholding centroid and pitch etc)
        klowscaler      = limit(dbfsamp(krms)-knoiseFloor_dB, 0, isecond_dB)/isecond_dB      ; scaler to fade out different things towards the noise floor
        krms_dB_n       = (krms_dB/abs(knoiseFloor_dB))+1

        icrestrate      = 10
        kcrestmetro     metro icrestrate
        krms_max        init 0
        krms_max        max krms_max, krms
        krms_max        = (kcrestmetro > 0 ? 0 : krms_max)
        kcrestindex     init 0
        kcrestindex     = (kcrestindex+kcrestmetro)%2
        kcrestArr[]     init 2
        kcrestArr[kcrestindex] = krms_max
        kcrest_max      maxarray kcrestArr ; get max out of N last values
        kenv_crest0     divz kcrest_max, krms, 1
	kcrestrise      = (kgate2 > 0 ? 1 : 1) ; STATIC .. was slow response when signal is low
	kcrestfall      = (kgate2 > 0 ? 3 : 3) ; STATIC .. was slow response when signal is low
        kcrestA         = 0.001^(1/(kcrestrise*kr))
        kcrestB         = 0.001^(1/(kcrestfall*kr))
        kenv_crest      init 0
        kenv_crest      = (kenv_crest0>kenv_crest?(kenv_crest0+(kcrestA*(kenv_crest-kenv_crest0))):(kenv_crest0+(kcrestB*(kenv_crest-kenv_crest0))))
        kenv_crest1     = (dbamp(kenv_crest))/50



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
        
; transient hold, statistics etc 
        ktrigButn       trigger	krms_tran0, 0.5, 0
        if ktrigButn > 0 then
        katranTime      = ktime-kprevaTranTime
        kprevaTranTime  = ktime
        katranDensity_t divz 1, katranTime, 1           ; this will be accurate for slow tempi, but not if you play e.g. two fast consecutive transients followed by a pause
	reinit holdTransient
        endif
holdTransient:
        kamp_trans_hold linseg 1,0.05,1,0,0,1,0
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
        katransDensEnv_n = katransDensEnv/14
                
        ; transient dynamics
        kenv_dyn_count  init 0
        isize_env_dyn   = 5
        kenv_dyn_Arr[]  init isize_env_dyn
        
        krms_tran0_del delayk krms_tran0, 50/1000
        if krms_tran0 > 0 then
        krms_this       = 0
        endif
        krms_this       max krms, krms_this ; max amp during 50 ms after transient detected
        if krms_tran0_del > 0 then
        kenv_dyn_count  = (kenv_dyn_count+krms_tran0_del)%isize_env_dyn
        kenv_dyn_Arr[kenv_dyn_count] = krms_this
        endif
        kenv_dyn_min5   minarray kenv_dyn_Arr
        kenv_dyn_min2   min kenv_dyn_Arr[kenv_dyn_count], kenv_dyn_Arr[wrap(kenv_dyn_count-1,0,isize_env_dyn-1)]
        kenv_dyn_max5   maxarray kenv_dyn_Arr
        kenv_dyn_max2   max kenv_dyn_Arr[kenv_dyn_count], kenv_dyn_Arr[wrap(kenv_dyn_count-1,0,isize_env_dyn-1)]
        kenv_dyn_min    = (kenv_dyn_min5+kenv_dyn_min2)/2
        kenv_dyn_max    = (kenv_dyn_max5+kenv_dyn_max2)/2
        ;kenv_dyn        limit log10(divz(kenv_dyn_max, kenv_dyn_min, -1))/3, 0, 1
        kenv_dyn        = 1-limit(divz(dbfsamp(kenv_dyn_max)-5, dbfsamp(kenv_dyn_min)-5, -1), 0, 1)

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

; ***
knom1, kden1, kdev1, kfrom_one1 RhythmRatio krel1
knom2, kden2, kdev2, kfrom_one2 RhythmRatio krel2
knom3, kden3, kdev3, kfrom_one3 RhythmRatio krel3

krhythm_consonance_deviation tonek limit((kdev1+kdev2+kdev3)/3, 0, 1), kr/8

;Benedetti height
kben1 = (knom1*kden1)
kben2 = (knom2*kden2)
kben3 = (knom3*kden3)

;Tenney height is log2(Benedetti height)
kten1 = (log(kben1)/3)+0.2
kten2 = (log(kben2)/3)+0.2
kten3 = (log(kben3)/3)+0.2

; multiply the last three tenney heights
; it will allow for low values when there is some short term repetition (low values at this stage means high consonance)
krhythm_cons_x = (kten1*kten2*kten3)
; low pass filter and invert
krhythm_consonance tonek limit(krhythm_cons_x, 0, 1), kr/8 
krhythm_consonance = 1-krhythm_consonance


; the "distance from one" measure (irregularity)
; multiply the two highest out of the last three
kfrom_one limit max(kfrom_one1,kfrom_one2)*max(kfrom_one2,kfrom_one3), 0, 1
; low pass filter and log
krhythm_irregularity tonek log10((kfrom_one*9/10)+0.1)+1, kr/8 

krhythm_ratio1 = knom1/kden1
krhythm_ratio2 = knom2/kden2
krhythm_ratio3 = knom3/kden3

kcnt_ry wrap (kcnt_ry+1), 0, ibufsize_ry
endif


;*************************************
; ** rhythm autocorrelation
/*
; sinoid trigger envelope, allows (soft) rhythmic deviation from strict pattern
ishapedur = 0.1
;ktest metro 1
;if ktest == 1 then
if krms_tran0 == 1 then
reinit triggershape
endif
triggershape:
kra_env linseg 0, ishapedur*0.5, 1, ishapedur*0.5, 0, 1, 0
;kra_env = kra_env * krms * kgate
rireturn

; autocorrelation, done in layers of different fftsize to reduce latency
irhythm_sr = 24.8
ira_fftsize = 256
ira_overlaps = 4
kRhythmAuto[], krnewframe, knumtransients, kra_flux RhythmAutoCorr kra_env, krms_tran0, irhythm_sr, ira_fftsize, ira_overlaps
kRhythmAuto2[], krnewframe2, knumtransients2, kra_flux2 RhythmAutoCorr kra_env, krms_tran0, irhythm_sr, ira_fftsize/2, ira_overlaps
kRhythmAuto4[], krnewframe4, knumtransients4, kra_flux4 RhythmAutoCorr kra_env, krms_tran0, irhythm_sr, ira_fftsize/4, ira_overlaps

kcombocount init 0
kcombocount = (krnewframe > 0 ? 0 : kcombocount)
kcombocount += krnewframe4
knewcombo changed kcombocount
if krnewframe > 0 then
kRhythmAutoCombo[] = kRhythmAuto
endif
if knewcombo > 0 then
if ((kcombocount == 1) || (kcombocount == 2)) then 
kRA1[] slicearray kRhythmAuto, 0, ira_fftsize/4
kRA2[] = (kRhythmAuto2*0.333)+(kRA1*0.667)
kcombotransients = (knumtransients/2)+(((knumtransients2/2)*0.333)+((knumtransients/2)*0.667))
kRhythmAutoCombo slicearray kRA2, 0, (ira_fftsize/4)
endif
if ((kcombocount == 3) || (kcombocount == 4)) then 
kRA1[] slicearray kRhythmAuto, 0, ira_fftsize/4
kRA2[] = (kRhythmAuto2*0.667)+(kRA1*0.333)
kcombotransients = (knumtransients/2)+(((knumtransients2/2)*0.667)+((knumtransients/2)*0.333))
kRhythmAutoCombo slicearray kRA2, 0, (ira_fftsize/4)
endif
if ((kcombocount == 1) || (kcombocount == 3)) then 
kRA3[] slicearray kRA2, 0, ira_fftsize/8
kRA4[] = (kRhythmAuto4*0.333)+(kRA3*0.667)
kcombotransients = (kcombotransients*0.833)+((knumtransients4/2)*0.333)
kRhythmAutoCombo slicearray kRA4, 0, (ira_fftsize/8)
endif
if ((kcombocount == 2) || (kcombocount == 4)) then 
kRA3[] slicearray kRA2, 0, ira_fftsize/8
kRA4[] = (kRhythmAuto4*0.667)+(kRA3*0.333)
kcombotransients = (kcombotransients*0.666)+((knumtransients4/2)*0.667)
kRhythmAutoCombo slicearray kRA4, 0, (ira_fftsize/8)
endif

kndxfluxcombo = 1
kcorrcomboSum = 0
kprevcomboSum = 0
kthiscomboSum = 0
while kndxfluxcombo < ira_fftsize-1 do
kRhythmAutoComboPrev[] init ira_fftsize        
 kcombomax max kRhythmAutoCombo[kndxfluxcombo], kRhythmAutoCombo[kndxfluxcombo-1], kRhythmAutoCombo[kndxfluxcombo+1]
 kcomboPrevmax max kRhythmAutoComboPrev[kndxfluxcombo], kRhythmAutoComboPrev[kndxfluxcombo-1], kRhythmAutoComboPrev[kndxfluxcombo+1]
 kcorrcomboSum = kcorrcomboSum + (kcomboPrevmax*kcombomax)
 kprevcomboSum	= kprevcomboSum + (kcomboPrevmax^2)
 kthiscomboSum	= kthiscomboSum + (kcombomax^2)
 kndxfluxcombo += 1
od

kRhythmAutoComboPrev = kRhythmAutoCombo
kfluxcombo_1 divz kcorrcomboSum, (sqrt(kprevcomboSum)*sqrt(kthiscomboSum)), 1
kra_fluxcombo = limit((1-kfluxcombo_1)*((ira_overlaps+1)/2), 0, 1)

endif

kra_flux = (kra_flux2+kra_fluxcombo)/2 ; use the stepwise updated combo, but combine it with the relatively stable middle layer (middle sized window)

; removed, test
;kra_fluxsum = (kra_flux+kra_flux2+kra_flux4)/3 ; test, not used



krhythm_ac_rms = sqrt(sumarray(kRhythmAuto*kRhythmAuto)/ira_fftsize)
krhythm_ac_max = maxarray(kRhythmAuto)
krhythm_ac_crest1 = limit((krhythm_ac_rms/(log2(knumtransients+1)+1))*20, 0, 1) ; some empirical adjustment here... (!)

krhythm_ac_rms2 = sqrt(sumarray(kRhythmAuto2*kRhythmAuto2)/ira_fftsize)
krhythm_ac_max2 = maxarray(kRhythmAuto2)
krhythm_ac_crest2 = limit((krhythm_ac_rms2/(log2(knumtransients2+1)+1))*20, 0, 1) ; some empirical adjustment here... (!)

krhythm_ac_rms4 = sqrt(sumarray(kRhythmAuto4*kRhythmAuto4)/ira_fftsize)
krhythm_ac_max4 = maxarray(kRhythmAuto4)
krhythm_ac_crest4 = limit((krhythm_ac_rms4/(log2(knumtransients4+1)+1))*20, 0, 1) ; some empirical adjustment here... (!)

krhythm_ac_crest = (krhythm_ac_crest1+krhythm_ac_crest2+krhythm_ac_crest4)/3
*/


; ***************
; rhythm IOT histogram analysis

iot_size = 4
kiotArr[] init iot_size
kiot_index init 0
if krms_tran0 == 1 then
;kiotArr[kiot_index] = kdist1 ; kdist comes from previous rhythm analysis (ratios, above)
kiotArr[kiot_index] = knom1 ; nominator comes from previous rhythmic ratio analysis (ratios, above)
kiot_index = (kiot_index+1)%iot_size
inumbins = 64
imax = 8
itabsize = 8192
iftabsA[] makehistables itabsize, inumbins
kHist[] init inumbins
kindex = 0
while kindex < lenarray(kiotArr) do
  khistindex = 0
  while khistindex < inumbins do
    kHist[khistindex] = kHist[khistindex]+tablekt(kiotArr[kindex]/imax, iftabsA[khistindex], 1)
    khistindex += 1
  od
kindex += 1
od
endif

kiot_fade metro 4
if kiot_fade > 0 then
kHist = kHist*0.95
endif

;************
iot_size2 = 4
kiotArr2[] init iot_size2
kiot_index2 init 0
if krms_tran0 == 1 then
kiotArr2[kiot_index2] = kden1 ; denominator comes from previous rhythmic ratio analysis (ratios, above)
kiot_index2 = (kiot_index2+1)%iot_size2
inumbins2 = 64
imax2 = 8
itabsize2 = 8192
iftabsA2[] makehistables itabsize2, inumbins2
kHist2[] init inumbins2
kindex2 = 0
while kindex2 < lenarray(kiotArr2) do
  khistindex2 = 0
  while khistindex2 < inumbins2 do
    kHist2[khistindex2] = kHist2[khistindex2]+tablekt(kiotArr2[kindex2]/imax2, iftabsA2[khistindex2], 1)
    khistindex2 += 1
  od
kindex2 += 1
od
endif

kiot_fade2 metro 4
if kiot_fade2 > 0 then
kHist2 = kHist2*0.95
endif
;************
;printf "%i %i \n", krms_tran0, knom1, kden1

; ***************
; spectral analysis

	iwtype 			= 1
	fsin 			pvsanal	a1, gifftsize, gifftsize/2, gifftsize, iwtype
	kflag   		pvsftw	fsin,gifna,gifnf          	; export  amps  and freqs to table,
        
	kupdateRate		= 200
	kmetro			metro kupdateRate
	kdoflag			init 0
	kdoflag			= (kdoflag + kmetro);*kgate

	; copy pvs data from table to array
	; analyze spectral features
	kArrA[]  		init    giFftTabSize
	kArrAprev[]  		init    giFftTabSize
	kArrAnorm[]  		init    giFftTabSize
	kArrF[]  		init    giFftTabSize
	kArrCorr[]  		init    giFftTabSize
	kflatness		init 0

if (kdoflag > 0) && (kflag > 0) then

	kArrAprev[]		= kArrA
        			copyf2array kArrA, gifna
        			copyf2array kArrF, gifnf	
	ksumAmp			sumarray kArrA
	kmaxAmp			maxarray kArrA
	ksumAmp                 = (ksumAmp == 0 ? 1 : ksumAmp)
        kArrAnorm               = kArrA/ksumAmp
        kcentroid               pvscent fsin
	kArrCorr		= kArrA*kArrAprev
	kspread		        = sumarray(((kArrF+(kcentroid*-1))^2)*kArrAnorm)^0.5
	kskewness	        divz sumarray(((kArrF+(kcentroid*-1))^3)*kArrAnorm), kspread^3, 1
	kurtosis	        divz sumarray(((kArrF+(kcentroid*-1))^4)*kArrAnorm), kspread^4, 1	
        kcrest			divz kmaxAmp, ksumAmp/giFftTabSize, 1  
        kArrAlog[]              = kArrA
        kArrAlog[0]             = 1
        kArrAlog[1]             = 0
        klogmin minarray kArrAlog
        while klogmin == 0 do
        klogmin,klogndx minarray kArrAlog
        kArrAlog[klogndx] = 1
        od
      	kflatness		divz exp(sumarray(log(kArrAlog))/giFftTabSize),  (ksumAmp/giFftTabSize), 0
	kflux                   = 1-(divz(sumarray(kArrCorr),(sqrt(sumarray(kArrA^2))*sqrt(sumarray(kArrAprev^2))),0))
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

; post filtering of spectral tracks
        kcentroidf              tonek kcentroid, 20
        kfluxf                  tonek kflux, 20

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
        kspread_n       = kspread / (sr*0.15)
        kskewness_n     = kskewness / (sr*0.0005)
        kurtosis_n      = kurtosis / (sr*0.006)
        kflatness_n     = kflatness * 3
        kcrest_n        = kcrest / 250
        kflux_n         = kfluxf * 4
        ;kautocorr_n     = kautocorr * 2
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
