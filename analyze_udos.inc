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

        opcode RhythmRatio, kkkk, k
krel1 xin

; for rational approximation, we use table lookup since there are a lot of specific fractions sthat we do not want
; up to 1.0
iAfra1[] fillarray 0.0625, 0.125,  0.143,  0.1666, 0.2,    0.25,   0.333,  0.5,    0.6,    0.667,  0.75,   1.0
iAnom1[] fillarray 1,      1,      1,      1,      1,      1,      1,      1,      3,      2,      3,      1
iAden1[] fillarray 16,     8,      7,      6,      5,      4,      3,      2,      5,      3,      4,      1
; from 1.0 and up
iAfra2[] fillarray 1.0,    1.25,   1.33,   1.5,    1.67,   1.75,   2.0,    3.0,    4.0,    5.0,    6.0,    7.0,    8.0,    12.0,   16.0
iAnom2[] fillarray 1,      5,      4,      3,      5,      7,      2,      3,      4,      5,      6,      7,      8,      12,     16        
iAden2[] fillarray 1,      4,      3,      2,      3,      4,      1,      1,      1,      1,      1,      1,      1,      1,      1

ifra2max maxarray iAfra2
inomtablen = 512
; these will generate indices to look up values in the arrays above
indxtab1  ftgen 0, 0, inomtablen, -17,  0, 0,
                                     int((iAfra1[1]-((iAfra1[1]-iAfra1[0])/2))*inomtablen), 1,
                                     int((iAfra1[2]-((iAfra1[2]-iAfra1[1])/2))*inomtablen), 2,
                                     int((iAfra1[3]-((iAfra1[3]-iAfra1[2])/2))*inomtablen), 3,
                                     int((iAfra1[4]-((iAfra1[4]-iAfra1[3])/2))*inomtablen), 4,
                                     int((iAfra1[5]-((iAfra1[5]-iAfra1[4])/2))*inomtablen), 5,
                                     int((iAfra1[6]-((iAfra1[6]-iAfra1[5])/2))*inomtablen), 6,
                                     int((iAfra1[7]-((iAfra1[7]-iAfra1[6])/2))*inomtablen), 7,
                                     int((iAfra1[8]-((iAfra1[8]-iAfra1[7])/2))*inomtablen), 8,
                                     int((iAfra1[9]-((iAfra1[9]-iAfra1[8])/2))*inomtablen), 9,
                                     int((iAfra1[10]-((iAfra1[10]-iAfra1[9])/2))*inomtablen), 10,
                                     int((iAfra1[11]-((iAfra1[11]-iAfra1[10])/2))*inomtablen), 11


indxtab2  ftgen 0, 0, inomtablen, -17,  0, 0,
                                     int((iAfra2[1]-((iAfra2[1]-iAfra2[0])/2))*inomtablen/ifra2max), 1,
                                     int((iAfra2[2]-((iAfra2[2]-iAfra2[1])/2))*inomtablen/ifra2max), 2,
                                     int((iAfra2[3]-((iAfra2[3]-iAfra2[2])/2))*inomtablen/ifra2max), 3,
                                     int((iAfra2[4]-((iAfra2[4]-iAfra2[3])/2))*inomtablen/ifra2max), 4,
                                     int((iAfra2[5]-((iAfra2[5]-iAfra2[4])/2))*inomtablen/ifra2max), 5,
                                     int((iAfra2[6]-((iAfra2[6]-iAfra2[5])/2))*inomtablen/ifra2max), 6,
                                     int((iAfra2[7]-((iAfra2[7]-iAfra2[6])/2))*inomtablen/ifra2max), 7,
                                     int((iAfra2[8]-((iAfra2[8]-iAfra2[7])/2))*inomtablen/ifra2max), 8,
                                     int((iAfra2[9]-((iAfra2[9]-iAfra2[8])/2))*inomtablen/ifra2max), 9,
                                     int((iAfra2[10]-((iAfra2[10]-iAfra2[9])/2))*inomtablen/ifra2max), 10,
                                     int((iAfra2[11]-((iAfra2[11]-iAfra2[10])/2))*inomtablen/ifra2max), 11,
                                     int((iAfra2[12]-((iAfra2[12]-iAfra2[11])/2))*inomtablen/ifra2max), 12,
                                     int((iAfra2[13]-((iAfra2[13]-iAfra2[12])/2))*inomtablen/ifra2max), 13,
                                     int((iAfra2[14]-((iAfra2[14]-iAfra2[13])/2))*inomtablen/ifra2max), 14

; first rhythm relation
if krel1 <= 1 then
  kndx1 table krel1, indxtab1, 1
  knom1 = iAnom1[kndx1]
  kden1 = iAden1[kndx1]

  if kndx1 < (lenarray(iAnom1)-1) then
    knom1next = iAnom1[kndx1+1]
    kden1next = iAden1[kndx1+1]
  else
    knom1next = iAnom1[kndx1-1]
    kden1next = iAden1[kndx1-1]
  endif
  if kndx1 > 0 then
    knom1prev = iAnom1[kndx1-1]
    kden1prev = iAden1[kndx1-1]
  else
    knom1prev = iAnom1[1]
    kden1prev = iAden1[1]
  endif
  kfrom_one1 = 1-krel1
else
  kndx1 table krel1*(inomtablen/ifra2max), indxtab2
  knom1 = iAnom2[kndx1]
  kden1 = iAden2[kndx1]

  if kndx1 < lenarray(iAnom2)-1 then
    knom1next = iAnom2[kndx1+1]
    kden1next = iAden2[kndx1+1]
  else
    knom1next = iAnom2[kndx1-1]
    kden1next = iAden2[kndx1-1]
  endif
  
  kfrom_one1 = krel1/ifra2max
endif

kquant1 = knom1/kden1

if abs(krel1-kquant1) < 0 then
  kdevrange1 = abs((knom1/kden1)-(knom1prev/kden1prev))
else
  kdevrange1 = abs((knom1next/kden1next)-(knom1/kden1))
endif

kdev0 = abs(krel1-kquant1)
kdev1 = kdev0*(1/kdevrange1)

        xout knom1, kden1, kdev1, kfrom_one1
        endop

;***************************************************
opcode RhythmAutoCorr, k[]kkk, kkiii

kenv, ktrans, irhythm_sr, ifftsize, ioverlaps xin

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
kCeps[] init ifftsize*2
kAuto0[] init ifftsize*2
kAuto[] init ifftsize
kAutoPrev[] init ifftsize

ishapef ftgen 0, 0, ifftsize, -19, 0.25, 0.8, 270, 1
kShape[] init ifftsize
copyf2array kShape, ishapef

; we need to count the transients outside of the downsampled fft loop
; since transient triggers might happen in between clock ticks
; this makes the overlap indexing a little bit kludgy
kNumtransients[] init ioverlaps
knumtransients_out init 0
knewframe init 0
knewframetrig trigger knewframe, 0.5, 0
koverlap_indx init 0
koverlap_indx = (koverlap_indx + knewframetrig)%ioverlaps
kNumtransients[koverlap_indx] = kNumtransients[koverlap_indx]+ktrans


if krclock > 0 then 
  kArr[kcnt] = kenv
  kArr[kcnt+ifftsize] = kenv
  
  kcnt += 1
  kcnt = kcnt%ifftsize
  knewframe = (kcnt%ihopsize == 0 ? 1 : 0)
  if knewframe == 1 then

    reinit buffer
    buffer:
    kArrBuf slicearray kArr, i(kcnt), i(kcnt)+ifftsize-1   ; get current buffer 
    rireturn
    
    knumtransients_out sumarray kNumtransients
    kNumtransients[wrap((koverlap_indx-1), 0, ioverlaps)] = 0     ; zero previous transient counter
    
    ;kArrWin window kArrBuf                                 ; windowing
    kArrPad slicearray kArrBuf, 0, ifftsize-1              ; zero padding 
    
    ; autocorrelation
    kFFT[] rfft kArrPad                         ; fft
    ;kFFT = kFFT*(1/ifftsize)                    ; normalize
    kFFT2[] = pows(kFFT)                        ; square mags
    kFFT3[] slicearray kFFT2, 0, ifftsize-1      
    kFFT4[] r2c kFFT3                           ; SOMETHING OFF HERE IN 6.12
    kFFT4[0] = 0                                 ; zero first bin
    kAuto0[] rifft kFFT4                         ; and back
    kAuto0 = kAuto0 * kAuto0                    ; square result
    kAuto slicearray kAuto0, 0, ifftsize/2      ; take only the first half
    kbase = (kAuto[0]>0?kAuto[0]:1)
    kAuto = (kAuto/kbase)*kShape*(1/kShape[0])  ; normalize 
    kndx = 0

    while kndx <ifftsize -1 do
    if kAuto[kndx] > 0 then
    kAuto[kndx] = sqrt(kAuto[kndx])
    endif
    kndx += 1
    od

    kzeroX = 0                                  ; then zero the first few bins
    while kzeroX < 3 do
      kAuto[kzeroX] = 0
      kzeroX += 1
    od
    kndx2 = 1
    kcorrSum = 0
    kprevSum = 0
    kthisSum = 0

    while kndx2 < ifftsize-1 do
      kAutomax max kAuto[kndx2], kAuto[kndx2-1], kAuto[kndx2+1]
      kAutoPrevmax max kAutoPrev[kndx2], kAutoPrev[kndx2-1], kAutoPrev[kndx2+1]
      kcorrSum = kcorrSum + (kAutoPrevmax*kAutomax)
      kprevSum	= kprevSum + (kAutoPrevmax^2)
      kthisSum	= kthisSum + (kAutomax^2)
      kndx2 += 1
      od
    kAutoPrev = kAuto
    kflux_1 divz kcorrSum, (sqrt(kprevSum)*sqrt(kthisSum)), 1
    kflux = limit((1-kflux_1)*((ioverlaps+1)/2), 0, 1)

  endif

endif
knewframe trigger knewframe, 0.5, 0

xout kAuto, knewframe, knumtransients_out, kflux
endop



;***********
; >>>>>>> AUTOGENERATED TEXT: DO NOT EDIT THE FOLLOWING LINES >>>>>>>
opcode Plotchannel, k, Skkkkkkkkkkkkkkkkkkkkkkk
    Splotchan, krms, krms_dB_n, kamp_trans_hold, katransDensEnv_n, kenv_crest1, kenv_dyn, kpitch_n, kcentroid_n, kspread_n, kskewness_n, kurtosis_n, kflatness_n, kcrest_n, kflux_n, krhythm_irregularity, krhythm_consonance, krhythm_consonance_deviation, kra_flux, krhythm_ac_crest, kmfcc1, kmfcc2, kmfcc3, kmfccdiff xin 
    kplot_x chnget Splotchan
    Sparms[] fillarray "rms", "rms_dB", "transient", "trans_dens", "env_crest", "env_dyn", "pitch", 
                      "s_centroid", "s_spread", "s_skewness", "s_kurtosis", "s_flatness", "s_crest", "s_flux", 
                      "rhythm_irreg", "rhythm_cons", "rhyt_con_dev", "ra_flux", "ra_crest", 
                      "mfcc1", "mfcc2", "mfcc3", "mfcc_diff"
    Splot_x strcpyk Sparms[kplot_x-1]    
    kx init 0 
    kx = (strcmpk(Splot_x,"rms") == 0 ? krms : kx) 
    kx = (strcmpk(Splot_x,"rms_dB") == 0 ? krms_dB_n : kx) 
    kx = (strcmpk(Splot_x,"transient") == 0 ? kamp_trans_hold : kx) 
    kx = (strcmpk(Splot_x,"trans_dens") == 0 ? katransDensEnv_n : kx) 
    kx = (strcmpk(Splot_x,"env_crest") == 0 ? kenv_crest1 : kx) 
    kx = (strcmpk(Splot_x,"env_dyn") == 0 ? kenv_dyn : kx) 
    kx = (strcmpk(Splot_x,"pitch") == 0 ? kpitch_n : kx) 
    kx = (strcmpk(Splot_x,"s_centroid") == 0 ? kcentroid_n : kx) 
    kx = (strcmpk(Splot_x,"s_spread") == 0 ? kspread_n : kx) 
    kx = (strcmpk(Splot_x,"s_skewness") == 0 ? kskewness_n : kx) 
    kx = (strcmpk(Splot_x,"s_kurtosis") == 0 ? kurtosis_n : kx) 
    kx = (strcmpk(Splot_x,"s_flatness") == 0 ? kflatness_n : kx) 
    kx = (strcmpk(Splot_x,"s_crest") == 0 ? kcrest_n : kx) 
    kx = (strcmpk(Splot_x,"s_flux") == 0 ? kflux_n : kx) 
    kx = (strcmpk(Splot_x,"rhythm_irreg") == 0 ? krhythm_irregularity : kx) 
    kx = (strcmpk(Splot_x,"rhythm_cons") == 0 ? krhythm_consonance : kx) 
    kx = (strcmpk(Splot_x,"rhyt_con_dev") == 0 ? krhythm_consonance_deviation : kx) 
    kx = (strcmpk(Splot_x,"ra_flux") == 0 ? kra_flux : kx) 
    kx = (strcmpk(Splot_x,"ra_crest") == 0 ? krhythm_ac_crest : kx) 
    kx = (strcmpk(Splot_x,"mfcc1") == 0 ? kmfcc1 : kx) 
    kx = (strcmpk(Splot_x,"mfcc2") == 0 ? kmfcc2 : kx) 
    kx = (strcmpk(Splot_x,"mfcc3") == 0 ? kmfcc3 : kx) 
    kx = (strcmpk(Splot_x,"mfcc_diff") == 0 ? kmfccdiff : kx) 
    xout kx 
    endop 

; <<<<<<< AUTOGENERATED TEXT: DO NOT EDIT THE PRECEDING LINES <<<<<<<

;************
opcode makehistable,i,iii
        itabsize, inumbins, ibin xin 
        isegsize = round(itabsize/(inumbins-1))
        isegsize2 = round(itabsize/(inumbins-1)*(ibin-1))
        if ibin == 0 then ; **first segment
          itabnum ftgen 0, 0, itabsize, 7, 1, isegsize, 0
        elseif ibin == inumbins-1 then ; **last segment
          itabnum ftgen 0, 0, itabsize, 7, 0, isegsize2, 0, isegsize, 1
        elseif ibin == 1 && inumbins > 2 then
          itabnum ftgen 0, 0, itabsize, 7, 0, isegsize, 1, isegsize, 0
        elseif ibin == 2 && inumbins > 2 then
          itabnum ftgen 0, 0, itabsize, 7, 0, isegsize, 0, isegsize, 1, isegsize, 0
        elseif ibin > 2 && inumbins > 2 then
          itabnum ftgen 0, 0, itabsize, 7, 0, isegsize2, 0, isegsize, 1, isegsize, 0
        endif
        xout itabnum
        endop

opcode makehistables,i[],ii
        itabsize, inumbins xin 
        inumbins =(inumbins<2?2:inumbins); never less than 2 bins 
        iftabsA[] init inumbins
        ibin = 0
        while ibin < inumbins do
        itabnum makehistable itabsize, inumbins, ibin
        iftabsA[ibin] = itabnum
        ibin += 1
        od
        xout iftabsA
        endop
