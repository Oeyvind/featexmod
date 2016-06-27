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
opcode RhythmAutoCorr, k[]k[]k[], kkkiii
ktrans, kshapedur, ksort_threshold, irhythm_sr, ifftsize, ioverlaps xin

; sinoid trigger envelope, allows (soft) rhythmic deviation from strict pattern
if ktrans == 1 then
reinit triggershape
endif
triggershape:
kenvindx linseg 0, i(kshapedur), 1, 1, 1
kenv table kenvindx, giSinEnv, 1
rireturn

; downsampling
kRclock metro irhythm_sr
kenv samphold kenv, kRclock

; fft
ihopsize = ifftsize/ioverlaps

kcnt init 0
knumtransients[] init ioverlaps
kArr[][] init ioverlaps, ifftsize
kArrPad[] init ifftsize*2
kFFT2[] init ifftsize*2
kAuto_temp[] init ifftsize*2
kAuto[][] init ioverlaps, ifftsize*2
kAuto2[][] init ioverlaps, ifftsize*2
kAutoSort[][] init ioverlaps, ifftsize*2
kAutoSortIndx[][] init ioverlaps, ifftsize*2
kZeros[] init ifftsize
kZeros2[] init ifftsize*2
knumtransients += ktrans
ktest maxarray knumtransients

if kRclock > 0 then 
kArr[0][kcnt] = kenv
kArr[1][wrap(kcnt-ihopsize,0,ifftsize)] = kenv
kArr[2][wrap(kcnt-(ihopsize*2),0,ifftsize)] = kenv
kArr[3][wrap(kcnt-(ihopsize*3),0,ifftsize)] = kenv
/*
kauto = (kAuto[0][kcnt]+
        kAuto[1][(kcnt+ihopsize)%ifftsize]+
        kAuto[2][(kcnt+(ihopsize*2))%ifftsize]+
        kAuto[3][(kcnt+(ihopsize*3))%ifftsize])*(1/ioverlaps)

kauto = kAuto[0][kcnt]
*/
;ktest init 0
;ktest = (kcnt == 0 ? ktest +1 : ktest)
;printk2 ktest
        
kcnt += 1

if kcnt%ihopsize == 0 then
klayer = 0
klayer = (kcnt > ihopsize ? 1 : klayer)
klayer = (kcnt > ihopsize*2 ? 2 : klayer)
klayer = (kcnt > ihopsize*3 ? 3 : klayer)

; autocorrelation
if knumtransients[klayer] == 0 then
kAuto setrow kZeros2, klayer
else
kArrPad init ifftsize*2
kArrPad slicearray window(getrow(kArr,klayer),0,0), 0, ifftsize-1; zero padded and windowed
kFFT2 fft kArrPad                                      ; fft
kFFT2 = kFFT2*(1/ifftsize)                   ; normalize
kFFT2 =  kFFT2 * kFFT2                          ; square mags
kFFT2[0] = 0                                    ; zero first bin
kAuto_temp fftinv kFFT2                               ; and back
kbase = kAuto_temp[0]
kAuto_temp = kAuto_temp/kbase                             ; normalize
kAuto_temp = kAuto_temp * kAuto_temp                          ; square result
kAuto_temp = (kAuto_temp*4)/log2(knumtransients[klayer]+1)         ; (attempt to) normalize with regards to the number of pulses in this window
kAuto_temp = kAuto_temp*.8
kAuto_temp[0] = 0
kAuto_temp[1] = 0
kAuto_temp[2] = 0
kAuto_temp[3] = 0
kAuto_temp[4] = 0
kAuto_temp[5] = 0
kAuto_temp[6] = 0
kAuto setrow kAuto_temp, klayer

; peak picker
kcnt2 = 0
kmax maxarray kAuto_temp
ksort_thresh = kmax*ksort_threshold
kcomp = ksort_thresh
ksortindx = 0
ksortindx_inc = 0
kAutoSort setrow kZeros2, klayer
kAutoSortIndx setrow kZeros2, klayer
while kcnt2 < ifftsize/2 do
  if kAuto_temp[kcnt2] < ksort_thresh then
        kcomp = ksort_thresh
  endif
  if kAuto_temp[kcnt2] > kcomp then
    ksortindx_inc = 0
    kAutoSort[klayer][ksortindx] = kAuto_temp[kcnt2]
    kAutoSortIndx[klayer][ksortindx] = kcnt2
    kcomp = kAuto_temp[kcnt2]
  else
    ksortindx_inc += 1
      if ksortindx_inc == 1 then
      ksortindx += 1
      endif
  endif
  kcnt2 += 1
od
/*
kAuto[klayer][0] = -kAutoSort[klayer][0]
kAuto[klayer][1] = -kAutoSort[klayer][1]
kAuto[klayer][2] = -kAutoSort[klayer][2]
kAuto[klayer][3] = -kAutoSort[klayer][3]
kAuto[klayer][4] = -kAutoSort[klayer][4]
kAuto[klayer][5] = -kAutoSort[klayer][5]
*/
knumtransients[klayer] = 0
endif
endif
kraAuto1[] = (getrow(kAuto,0)+getrow(kAuto,1)+getrow(kAuto,2)+getrow(kAuto,3))*0.25
endif
kcnt = kcnt%ifftsize

        xout kraAuto1, kAutoSort, kAutoSortIndx
endop
