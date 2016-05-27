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
	kAttack		= 0.001				        ; envelope follower attack
	kRelease        chnget "amp_transientDecTime"           ; envelope follower release
	a_env		follow2	a1preEq, kAttack, kRelease	; envelope follower
        k_env		downsamp a_env	

	knoiseFloor_dB	chnget "inputNoisefloor"
	kgate		= (dbfsamp(krms) < knoiseFloor_dB ? 0 : 1)	;  gate when below noise floor (for sampholding centroid and pitch etc)
        isecond_dB      = 9
	kgate0		= (dbfsamp(krms) < knoiseFloor_dB+isecond_dB ? 0 : 1)	;  gate when far below noise floor (for sampholdingp pitch etc)
	kgate2		= (dbfsamp(krms) < knoiseFloor_dB+isecond_dB ? 0 : 1)	;  gate when close to noise floor (for sampholding centroid and pitch etc)
        klowscaler      = limit(dbfsamp(krms)-knoiseFloor_dB, 0, isecond_dB)/isecond_dB      ; scaler to fade out different things towards the noise floor

; ** tempo estimation
;  iprd = 0.01
;  imindur = 0.1
;  imemdur = 3
;  ihp = 1
;  ithresh = ampdbfs(-40)
;  ihtim = 0.005
;  ixfdbak = 0.05
;  istartempo = 110
;  k1 tempest krms, iprd, imindur, imemdur, ihp, ithresh, ihtim, ixfdbak, istartempo, gifnTempomem
;printk2 k1
        
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
        ;kamp_trans init 0
        if ktrigButn > 0 then
        katranTime      = ktime-kprevaTranTime
        kprevaTranTime  = ktime
        ;printk2 katranTime
        katranDensity   divz 1, katranTime, 1
	                reinit lightTransient
        endif
lightTransient:
        kamp_trans      linseg 1,0.15,1,0,0,1,0
                        chnset kamp_trans, "transientDisplay"
rireturn

        katran1second   delayk ktrigButn, 1
        katran05second  delayk ktrigButn, 0.5
        katranCount     init 0
        katranCount     += ktrigButn
        katranCount     -= (katran1second+katran05second)*0.5
        
        katranDensity0   init 0
        katranDensity    = katranDensity-(katranDensity0*1/kr) ; hipass
        katranDensity0   = tonek(katranDensity, 1) ; lowpass
        katranDensity1   = katranDensity*1.1
        iatranrise       = 10000
	iatrandec        = 2
        iA               = exp(-6.90775527898 * (1/iatranrise)/kr)
        iB               = exp(-6.90775527898 * (1/iatrandec)/kr)
        katransDensEnv   init 0
        katransDensEnv   = (katranDensity1>katransDensEnv?(katranDensity1+(iA*(katransDensEnv-katranDensity1))):(katranDensity1+(iB*(katransDensEnv-katranDensity1))))

        ;katranCount     tonek katranCount, 1
        katransDensEnv1 init 0
        iA               = exp(-6.90775527898 * (1/kr))
        katransDensEnv  = katranCount-(katransDensEnv1*iA)
        katransDensEnv1 delayk katransDensEnv, 1/kr
        katransDensEnv  tonek katransDensEnv, 0.6
        ;katransDensEnv   = (katranCount+(iA*(katransDensEnv-katranCount)))
        
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
	kMelAmps[]              init    33              ; amps for 32 mel freq bands

 process1:
	kArrCorr[kindx]		= kArrA[kindx]*kArrAprev[kindx]
	knormAmp		divz kArrA[kindx], ksumAmp, 0
	kcentroid		= kcentroid + (kArrF[kindx]*knormAmp)
	kmelFq                  table kArrF[kindx], gifqToMelTab        ; get mel freq of this band
	kmelBand                table kmelFq, gi32melBands              ; get the mel band where this freq belongs (fractional value)
        kmelBand1               = floor(kmelBand)
	kMelAmps[kmelBand1]     = kMelAmps[kmelBand1]+(kArrA[kindx]*(1-frac(kmelBand)))
	kMelAmps[kmelBand1+1]   = kMelAmps[kmelBand1+1]+(kArrA[kindx]*frac(kmelBand))
	kindx 			= kindx + 1
  if kindx < giFftTabSize then
  kgoto process1
  endif
        kMelCepsInput[]		init    128
        kindx                   = 0

fillCeps:
        kindx                   = kindx + 1
        kmelBandFq              table kindx, gi32melBandFqs
        kMelCepsInput[kindx*2]  = (kMelAmps[kindx])
        kMelCepsInput[(kindx*2)+1]  = kmelBandFq
  if kindx < 31 then
  kgoto fillCeps         
  endif
        kMelCeps[]		init    128
        kMelCepsIn[]		init    128
        kMelCepsIn2[]		init    128
/*
        kMelCepsIn              ceps mags(rfft(kMelCepsInput)), 20

        printk 0.4, kMelCepsInput[2]
        kmelceps_b1             = kMelCepsIn[2];sqrt((kMelCeps[2]^2)+(kMelCeps[3]^2))
        kmelceps_b2             = kMelCepsIn[4];sqrt((kMelCeps[4]^2)+(kMelCeps[5]^2))
        kmelceps_b3             = kMelCepsIn[6];sqrt((kMelCeps[6]^2)+(kMelCeps[7]^2))
        kmelceps_b4             = kMelCepsIn[8];sqrt((kMelCeps[8]^2)+(kMelCeps[9]^2))
*/
        
; separate loop for spread, skewness, kurtosis (as they depend on centroid being previously calculated) 
	kindx 			= 0
	kspread			= 0
	kskewness		= 0
	kurtosis		= 0
	kmean_geo               = 1
 process2:
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
; cepstrum
        kCeps[]  		init    giFftTabSize
        kCeps                   pvsceps fsin
        kceps_b1                = sqrt((kCeps[0]^2)+(kCeps[1]^2))
        kceps_b2                = sqrt((kCeps[2]^2)+(kCeps[3]^2))
        kceps_b3                = sqrt((kCeps[4]^2)+(kCeps[5]^2))
        kceps_b4                = sqrt((kCeps[6]^2)+(kCeps[7]^2))
        kceps_b100              = sqrt((kCeps[198]^2)+(kCeps[199]^2))
        
; ***************
;analyze centroid transients

	icresponse	= 10 		                        ; response time in milliseconds
	kctthresh	chnget "centro_transientThresh"	        ; transient trig threshold (in octaves)
	kclowThresh	= 1		                        ; lower threshold for transient detection (in octaves)
	kcdoubleLimit	chnget "centro_transientDoubleLimit"	; minimum duration between events, (double trig limit)
	kcdecTime	chnget "centro_transientDecTime" 	; envelope follower decay time
	kcdecThresh	chnget "centro_transientDecThresh" 	; retrig threshold, how much must the level decay from its local max before allowing new transient trig
        icrise          = 0.001
	icrise          limit icrise, 1/kr, 10
	kcdecTime       limit kcdecTime, 1/kr, 10
        iA              = exp(-6.90775527898 * (1/icrise)/kr)
        kB              = exp(-6.90775527898 * (1/kcdecTime)/kr)
        kcentro_env     init 0
        kcentro_env     = (kcentroid>kcentro_env?(kcentroid+(iA*(kcentro_env-kcentroid))):(kcentroid+(kB*(kcentro_env-kcentroid))))
	kcentroct	= log2(kcentro_env)
	;printk 0.4, kcentroct

	kcentr_tran0, kcdiff	TransientDetect kcentroct, icresponse, kctthresh, kclowThresh, kcdecThresh, kcdoubleLimit

        kprevcTranTime  init 0
        kctranDensity   init 0
;Control segment for LEDs displaying transient detection:
        kctrigButn	trigger	kcentr_tran0, 0.5, 0
        ;kcentr_trans    init 0
        if kctrigButn > 0 then
        kctranTime      = ktime-kprevcTranTime
        kprevcTranTime  = ktime
        kctranDensity   divz 1, kctranTime, 1
	                reinit clightTransient
        endif
clightTransient:
        kcentr_trans    linseg 1,0.15,1,0,0,1,0
                        chnset kcentr_trans, "ctransientDisplay"
rireturn

        kctranDensity0   init 0
        kctranDensity    = kctranDensity-(kctranDensity0*5/kr) ; hipass
        kctranDensity0   = kctranDensity
        kctranDensity1   = kctranDensity*1.1
        ictranrise       = 0.001
	ictrandec        = 2
        iA               = exp(-6.90775527898 * (1/ictranrise)/kr)
        iB               = exp(-6.90775527898 * (1/ictrandec)/kr)
        kctransDensEnv   init 0
        kctransDensEnv   = (kctranDensity1>kctransDensEnv?(kctranDensity1+(iA*(kctransDensEnv-kctranDensity1))):(kctranDensity1+(iB*(kctransDensEnv-kctranDensity1))))

; ***************
;analyze kurtosis transients
	ikresponse	= 10 		                        ; response time in milliseconds
	kktthresh	chnget "kurt_transientThresh"	        ; transient trig threshold 
	kklowThresh	= 1		                        ; lower threshold for transient detection
	kkdoubleLimit	chnget "kurt_transientDoubleLimit"	; minimum duration between events, (double trig limit)
	kkdecTime	chnget "kurt_transientDecTime" 	; envelope follower decay time
	kkdecThresh	chnget "kurt_transientDecThresh" 	; retrig threshold, how much must the level decay from its local max before allowing new transient trig

        ikrise          = 0.001
	ikrise          limit ikrise, 1/kr, 10
	kkdecTime       limit kkdecTime, 1/kr, 10
        ikA             = exp(-6.90775527898 * (1/ikrise)/kr)
        kkB             = exp(-6.90775527898 * (1/kkdecTime)/kr)
        kkurt_env       init 0
        kkurt_env     = (kurtosis>kkurt_env?(kurtosis+(ikA*(kkurt_env-kurtosis))):(kurtosis+(kkB*(kkurt_env-kurtosis))))

	kkurt_tran0, kkdiff	TransientDetect kkurt_env, ikresponse, kktthresh, kklowThresh, kkdecThresh, kkdoubleLimit

;Control segment for LEDs displaying transient detection:
        kktrigButn	changed	kkurt_tran0
        kkurt_trans     init 0
        if kktrigButn > 0 then
	                reinit sklightTransient
        endif
sklightTransient:
        kkurt_trans     linseg 1,0.15,1,0,0,1,0
                        chnset kkurt_trans, "ktransientDisplay"
rireturn

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

	kcps		samphold kcps, kgate0
        ksemitone       = limit:k((log2(kcps/440)*12)+69, 0, 127)
        
; ***************
;analyze pitch transients

	ipresponse	= 15 		                        ; response time in milliseconds
	kptthresh	chnget "pitch_transientThresh"	        ; transient trig threshold (in semitones)
	kplowThresh	= 24		                        ; lower threshold for transient detection
	kpdoubleLimit	chnget "pitch_transientDoubleLimit"	; minimum duration between events, (double trig limit)
	kpdecTime	chnget "pitch_transientDecTime" 	; envelope follower decay time
	kpdecThresh	chnget "pitch_transientDecThresh" 	; retrig threshold, how much must the level decay from its local max before allowing new transient trig

        iprise          = 0.001
	iprise          limit iprise, 1/kr, 10
	kpdecTime       limit kpdecTime, 1/kr, 10
        ipA             = exp(-6.90775527898 * (1/iprise)/kr)
        kpB             = exp(-6.90775527898 * (1/kpdecTime)/kr)
        kpitchup_env    init 0
        kpitchup_env    = (ksemitone>kpitchup_env?(ksemitone+(ipA*(kpitchup_env-ksemitone))):(ksemitone+(kpB*(kpitchup_env-ksemitone))))
        kpitchdwn_env   init 0
        kpitchdwn_env   = ((127-ksemitone)>kpitchdwn_env?((127-ksemitone)+(ipA*(kpitchdwn_env-(127-ksemitone)))):((127-ksemitone)+(kpB*(kpitchdwn_env-(127-ksemitone)))))


	kpitchup_tran0, kpdiff	TransientDetect kpitchup_env, ipresponse, kptthresh, kplowThresh, kpdecThresh, kpdoubleLimit
	kpitchdwn_tran0, kpdiff	TransientDetect kpitchdwn_env, ipresponse, kptthresh, kplowThresh, kpdecThresh, kpdoubleLimit
        
;Control segment for LEDs displaying transient detection:
        kpuptrigButn	changed	kpitchup_tran0
        kpitchup_trans  init 0
        if kpuptrigButn > 0 then
	                reinit puplightTransient
        endif
puplightTransient:
        kpitchup_trans  linseg 1,0.15,1,0,0,1,0
                        chnset kpitchup_trans, "puptransientDisplay"
rireturn

        kpdwntrigButn	changed	kpitchdwn_tran0
        kpitchdwn_trans init 0
        if kpdwntrigButn > 0 then
	                reinit pdwnlightTransient
        endif
pdwnlightTransient:
        kpitchdwn_trans linseg 1,0.15,1,0,0,1,0
                        chnset kpitchdwn_trans, "pdwntransientDisplay"
rireturn
; ***************
; normalization
        kcps_n          = divz(kcps, kpitch_high, 1)
        kpitch_n        = limit(divz(kcps-kpitch_low, kpitch_high-kpitch_low, 1), 0, 1)    ; normalized and offset
        kcentroid_n     = kcentroidf / (sr*0.2)
        kspread_n       = kspread / (sr*0.25)
        kskewness_n     = kskewness / 7
        kurtosis_n      = kurtosis / 15
        kcrest_n        = kcrest / 250
        kflux_n         = kfluxf * 4
        kautocorr_n     = kautocorr * 2

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
        kflatness_a     AutoNormalize kflatness, kreset, kcalibrate, knormalize, iperiod
        kcrest_a        AutoNormalize kcrest_n, kreset, kcalibrate, knormalize, iperiod
        kflux_a         AutoNormalize kflux_n, kreset, kcalibrate, knormalize, iperiod

        krms            limit krms, 0, 1
        krms_preEq      limit krms_preEq, 0, 1
        kpitch_a        limit kpitch_a, 0, 1
        kcentroid_a     limit kcentroid_a, 0, 1
        kspread_a       limit kspread_a, 0, 1
        kskewness_a     limit kskewness_a, 0, 1
        kurtosis_a      limit kurtosis_a, 0, 1
        kflatness_a     limit kflatness_a, 0, 1
        kcrest_a        limit kcrest_a, 0, 1
        kflux_a         limit kflux_a, 0, 1
