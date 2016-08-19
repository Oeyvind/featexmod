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

;***************************************************
; Transient detection udo

	opcode TransientDetect, kk,kikkkk
	kin, iresponse, ktthresh, klowThresh, kdecThresh, kdoubleLimit xin 
        ;setksmps 32
/*
	iresponse	= 10 		; response time in milliseconds
	ktthresh	= 6		; transient trig threshold 
	klowThresh	= -60		; lower threshold for transient detection
	kdoubleLimit	= 0.02		; minimum duration between events, (double trig limit)
	kdecThresh	= 6		; retrig threshold, how much must the level decay from its local max before allowing new transient trig
*/	
	kinDel		delayk	kin, iresponse/1000		; delay with response time for comparision of levels
	ktrig		= ((kin > kinDel + ktthresh) ? 1 : 0) 	; if current rms plus threshold is larger than previous rms, set trig signal to current rms
	klowGate	= (kin < klowThresh? 0 : 1)		; gate to remove transient of low level signals
	ktrig		= ktrig * klowGate			; activate gate on trig signal
	ktransLev	init 0
	ktransLev	samphold kin, 1-ktrig			; read amplitude at transient
	
	kreGate		init 1					; retrigger gate, to limit transient double trig before signal has decayed (decThresh) from its local max
	ktrig		= ktrig*kreGate				; activate gate
	kmaxAmp		init -99999
	kmaxAmp		max kmaxAmp, kin			; find local max amp
	kdiff		= kmaxAmp-kin				; how much the signal has decayed since its local max value
	kreGate		limit kreGate-ktrig, 0, 1		; mute when trig detected
	kreGate		= (kdiff > kdecThresh ? 1 : kreGate)	; re-enable gate when signal has decayed sufficiently
	kmaxAmp		= (kreGate == 1 ? -99999 : kmaxAmp)	; reset max amp gauge

	; avoid closely spaced transient triggers (first trig priority)
	kdouble		init 1
	ktrig		= ktrig*kdouble
	if ktrig > 0 then
	reinit double
	endif
double:
        idoubleLimit  = i(kdoubleLimit)	
        idoubleLimit    limit idoubleLimit, 1/kr, 5
        kdouble		linseg	0, idoubleLimit, 0, 0, 1, 1, 1
	rireturn

	xout ktrig, kdiff
	endop

;***************************************************
; epoch filtering udo
	opcode EpochCps, k,a
	a1              xin 
	                setksmps 8
	a20		butterbp a1, 20, 5
	a20		dcblock2 a20*40
	aepochSig	butlp a20, 200
	kepochSig	downsamp aepochSig
	kepochRms	rms aepochSig

; count epoch zero crossings
	ktime		times	
	kZC		trigger kepochSig, 0, 0		; zero cross
	kprevZCtim	init 0
	kinterval1	init 0
	kinterval2	init 0
	kinterval3	init 0
	kinterval4	init 0
	if kZC > 0 then
	kZCtim	 	= ktime				; get time between zero crossings
	kinterval4	= kinterval3
	kinterval3	= kinterval2
	kinterval2	= kinterval1
	kinterval1	= kZCtim-kprevZCtim
	kprevZCtim	= kZCtim
	endif
	kmax		max kinterval1, kinterval2, kinterval3, kinterval4
	kmin		min kinterval1, kinterval2, kinterval3, kinterval4
	kZCmedi		= (kinterval1+kinterval2+kinterval3+kinterval4-kmax-kmin)/2
	kepochZCcps	divz 1, kZCmedi, 1
	kepochZCcps     mediank kepochZCcps, 40, 40
	                xout kepochZCcps
	endop

;***************************************************
; auto normalize udo
        opcode AutoNormalize0, k, kkkk
        kval, kreset, kcalibrate, knormalize xin 
        kval_max        init 0
        kval_min        init 2^64
        if kreset > 0 then
        kval_max        = 0
        kval_min        = 2^64
        endif
        if kcalibrate > 0 then
        kval_max        max kval_max, kval
        kval_min        min kval_min, kval
        endif
        if knormalize > 0 then
        kval_n          = (kval-kval_min)*divz(1,(kval_max-kval_min), 1)
        else
        kval_n          = kval
        endif
        xout kval_n
        endop

        opcode AutoNormalize, k, kkkki
        kval, kreset, kcalibrate, knormalize, iperiod xin 
        inormsize       = kr*iperiod
        kNormArr[]      init inormsize
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
        if knormalize > 0 then
        kval_n          = (kval-kval_min_ok)*divz(1,(kval_max_ok-kval_min_ok), 1)
        else
        kval_n          = kval
        endif
        xout kval_n
        endop

;***************************************************

opcode RhythmAutoCorr, k[]k[]k, kkkiii

ktrans, krms, kpeak_delta, irhythm_sr, ifftsize, ioverlaps xin

; sinoid trigger envelope, allows (soft) rhythmic deviation from strict pattern
ishapedur = 0.05
if ktrans == 1 then
reinit triggershape
endif
triggershape:
kenv linseg 1, ishapedur, 0, 1, 0
kenv = kenv * krms
rireturn

; downsampling
krclock metro irhythm_sr
kenv samphold kenv, krclock

; fft
ihopsize = ifftsize/ioverlaps
kcnt init 0
kArr[] init ifftsize*2
kArrBuf[] init ifftsize
kArrWin[] init ifftsize
kArrPad[] init ifftsize*2

kFFT[] init ifftsize*2
kAuto[] init ifftsize*2

kAutoPeaks[] init ifftsize*2
kAutoPeaksIndx[] init ifftsize*2
kAutoPeaksOut[][] init 2, ifftsize/2
kZeros[] init ifftsize
kZeros2[] init ifftsize*2

if krclock > 0 then 
  kArr[kcnt] = kenv
  kArr[kcnt+ifftsize] = kenv
  
  kcnt += 1
  knewframe = (kcnt%ihopsize == 0 ? 1 : 0)
  if knewframe == 1 then

    reinit buffer
    buffer:
    kArrBuf slicearray kArr, i(kcnt), i(kcnt)+ifftsize-1   ; get current buffer 
    rireturn

    kArrWin window kArrBuf                                 ; windowing
    kArrPad slicearray kArrWin, 0, ifftsize-1              ; zero padding 
    
    ; autocorrelation
    kFFT fft kArrPad                            ; fft
    kFFT = kFFT*(1/ifftsize)                    ; normalize
    kFFT =  kFFT * kFFT                         ; square mags
    kFFT[0] = 0                                 ; zero first bin
    kAuto fftinv kFFT                           ; and back
    kAuto = kAuto * kAuto                       ; square result
    kzeroX = 0                                  ; zero the first few
    while kzeroX < 6 do
      kAuto[kzeroX] = 0
      kzeroX += 1
    od
    /*
    kzeroX = ifftsize/2                         ; and zero the second half
    while kzeroX < ifftsize do
      kAuto[kzeroX] = 0
      kzeroX += 1
    od
    */
    kmax maxarray kAuto                         ; get max, for normalization
    kmax = (kmax == 0 ? 1 : kmax)               ; (divz)
    kAuto = kAuto/kmax
    
    ; peak picker
    kAutoPeaks = kZeros2
    kAutoPeaksIndx = kZeros2
    kcnt_fill = 0
    while kcnt_fill < ifftsize/2 do
      pycall "p.fill_array", kAuto[kcnt_fill], kcnt_fill, ifftsize/2
      kcnt_fill += 1
    od
    pycall "p.analyze_peaks", kpeak_delta
    kcnt2 = 0
    kval = 0

    while kval >= 0 do
      kindx, kval pycall2 "p.get_peaks", 0
      if kval >= 0 then
        kAutoPeaks[kcnt2] = kval
        kAutoPeaksIndx[kcnt2] = kindx
      endif
      kcnt2 += 1
    od  

    kAutoPeaksOut setrow kAutoPeaks, 0
    kAutoPeaksOut setrow kAutoPeaksIndx, 1
    
  endif
kcnt = kcnt%ifftsize
endif
xout kAuto, kAutoPeaksOut, knewframe
endop

;***********
opcode Plotchannel, k, Skkkkkkkkkkkkkkkkkkkkkkkkk
        Splotchan, krms, kcps_n, kpitch_a, kcentroid_a, kspread_a, kskewness_a, kurtosis_a, kflatness_a, kcrest_a, kflux_a, kamp_trans, katransDensEnv, krhythm_consonance, krhythmFromOne, krhythm_ratio1, krhythm_ratio2, krhythm_ac1, krhythm_ac2, krhythm_ac1time, krhythm_ac2time, kmfcc1, kmfcc2, kmfcc3, kmfcc4, kmfcc5 xin
        Splot_x chnget Splotchan
        kx init 0
        kx = (strcmpk(Splot_x,"rms") == 0 ? krms : kx)
        kx = (strcmpk(Splot_x,"cps_n") == 0 ? kcps_n : kx)
        kx = (strcmpk(Splot_x,"pitch_a") == 0 ? kpitch_a : kx)
        kx = (strcmpk(Splot_x,"centroid_a") == 0 ? kcentroid_a : kx)
        kx = (strcmpk(Splot_x,"spread_a") == 0 ? kspread_a : kx)
        kx = (strcmpk(Splot_x,"skewness_a") == 0 ? kskewness_a : kx)
        kx = (strcmpk(Splot_x,"kurtosis_a") == 0 ? krms : kurtosis_a)
        kx = (strcmpk(Splot_x,"flatness_a") == 0 ? kflatness_a : kx)
        kx = (strcmpk(Splot_x,"crest_a") == 0 ? kcrest_a : kx)
        kx = (strcmpk(Splot_x,"flux_a") == 0 ? kflux_a : kx)
        kx = (strcmpk(Splot_x,"amp_trans") == 0 ? kamp_trans : kx)
        kx = (strcmpk(Splot_x,"atransDensEnv") == 0 ? katransDensEnv : kx)
        kx = (strcmpk(Splot_x,"rhythm_consonance") == 0 ? krhythm_consonance : kx)
        kx = (strcmpk(Splot_x,"rhythmFromOne") == 0 ? krhythmFromOne : kx)
        kx = (strcmpk(Splot_x,"rhythm_ratio1") == 0 ? krhythm_ratio1 : kx)
        kx = (strcmpk(Splot_x,"rhythm_ratio2") == 0 ? krhythm_ratio2 : kx)
        kx = (strcmpk(Splot_x,"rhythm_ac1") == 0 ? krhythm_ac1 : kx)
        kx = (strcmpk(Splot_x,"rhythm_ac2") == 0 ? krhythm_ac2 : kx)
        kx = (strcmpk(Splot_x,"rhythm_ac1time") == 0 ? krhythm_ac1time : kx)
        kx = (strcmpk(Splot_x,"rhythm_ac2time") == 0 ? krhythm_ac2time : kx)
        kx = (strcmpk(Splot_x,"mfcc1") == 0 ? kmfcc1 : kx)
        kx = (strcmpk(Splot_x,"mfcc2") == 0 ? kmfcc2 : kx)
        kx = (strcmpk(Splot_x,"mfcc3") == 0 ? kmfcc3 : kx)
        kx = (strcmpk(Splot_x,"mfcc4") == 0 ? kmfcc4 : kx)
        kx = (strcmpk(Splot_x,"mfcc5") == 0 ? kmfcc5 : kx)
        xout kx
        endop
