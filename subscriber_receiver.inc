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

; *********************
; ; *********************
; check if the source or chan (gui) selectors have changed
; instantiate other control instruments as necessary

        instr 4

        Sparam          strget p4
        imod_num        = frac(p1)*100
        instr_8         = 8+frac(p1)
        Sscoreline      sprintf {{i %f 0 -1 "%s" %i}}, instr_8, Sparam, imod_num
                        scoreline_i Sscoreline
        Ssourcechn1     sprintf "source1_%s", Sparam
        Ssourcechn2     sprintf "source2_%s", Sparam
        Schanchn1       sprintf "chan1_%s", Sparam
        Schanchn2       sprintf "chan2_%s", Sparam
        Sources[]       fillarray "rms", "rms_dB", "transient", "trans_dens", "env_crest", "env_dyn", "pitch", 
                                  "s_centroid", "s_spread", "s_skewness", "s_kurtosis", "s_flatness", "s_crest", "s_flux", 
                                  "rhythm_irreg", "rhythm_cons", "rhyt_con_dev", "ra_flux", "ra_crest", 
                                  "mfcc1", "mfcc2", "mfcc3", "mfcc_diff"
        ksource1         chnget Ssourcechn1
        ksource2         chnget Ssourcechn2
        Source1 strcpyk Sources[ksource1-1]
        Source2 strcpyk Sources[ksource2-1]
        ksourcechanged1 changed ksource1
        puts Source1, ksourcechanged1+1
        ksourcechanged2 changed Source2
        kchan1          chnget Schanchn1
        kchan2          chnget Schanchn2
        kchanchanged1   changed kchan1
        kchanchanged2   changed kchan2
        kanychanged1    = kchanchanged1 + abs(ksourcechanged1)
        kanychanged2    = kchanchanged2 + abs(ksourcechanged2)
        khandleOK       trigger gkhandle, -1, 0
        instr5_1        = frac(p1) + 0.0001 + 5
        instr5_2        = frac(p1) + 0.0002 + 5
        instr7_1        = frac(p1) + 0.0001 + 7
        instr7_2        = frac(p1) + 0.0002 + 7
        ;print p1, frac(p1), instr5_1, instr5_2
        if khandleOK == 1 then 
        Sscoreline      sprintfk {{i %f 0.1 -1 %i}}, instr5_1, kchan1
                        scoreline Sscoreline, 1
        Sscoreline      sprintfk {{i %f 0.1 -1 %i}}, instr5_2, kchan2
                        scoreline Sscoreline, 1
        Sscoreline      sprintfk {{i %f 0.1 -1 "%s" "%s" %i 1}}, instr7_1, Sparam, Source1, kchan1
                        scoreline Sscoreline, 1
        Sscoreline      sprintfk {{i %f 0.1 -1 "%s" "%s" %i 2}}, instr7_2, Sparam, Source2, kchan2
                        scoreline Sscoreline, 1
        endif
        if (gkhandle != -1) && (kchanchanged1 > 0) then
        Sscoreline      sprintfk {{i -%f 0 1 %i}}, instr5_1, kchan1
                        scoreline Sscoreline, 1
        Sscoreline      sprintfk {{i %f 0.1 -1 %i}}, instr5_1, kchan1
                        scoreline Sscoreline, 1
        endif
        if (gkhandle != -1) && (kanychanged1 > 0) then
        Sscoreline      sprintfk {{i -%f 0 1 "%s" "%s" %i 1}}, instr7_1, Sparam, Source1, kchan1
                        scoreline Sscoreline, 1
        Sscoreline      sprintfk {{i %f 0.1 -1 "%s" "%s" %i 1}}, instr7_1, Sparam, Source1, kchan1
                        scoreline Sscoreline, 1
        endif
        if (gkhandle != -1) && (kchanchanged2 > 0) then
        Sscoreline      sprintfk {{i -%f 0 1 %i}}, instr5_2, kchan2
                        scoreline Sscoreline, 1
        Sscoreline      sprintfk {{i %f 0.1 -1 %i}}, instr5_2, kchan2
                        scoreline Sscoreline, 1
        endif
        if (gkhandle != -1) && (kanychanged2 > 0) then
        Sscoreline      sprintfk {{i -%f 0 1 "%s" "%s" %i 2}}, instr7_2, Sparam, Source2, kchan2
                        scoreline Sscoreline, 1
        Sscoreline      sprintfk {{i %f 0.1 -1 "%s" "%s" %i 2}}, instr7_2, Sparam, Source2, kchan2
                        scoreline Sscoreline, 1
        endif
        endin

; *********************
; check if we are already listening to this source_channel, if not we will instantiate an OSC listener
; if no one is using a source_channel, we release the OSC listener for that signal
	instr 5
        isource_chan     = p4
        inumActive      table isource_chan, giSourceActive
        ;print inumActive, p1
        Sscoreline      sprintf {{i %f 0.1 -1  %i}}, 6+frac(p1)+(isource_chan*0.1), isource_chan
        ;puts Sscoreline, 1
        if inumActive == 0 then
                        scoreline_i Sscoreline
        endif
                        tablew inumActive+1, isource_chan, giSourceActive

                        xtratim 1/kr
        krelease        release
        if krelease > 0 then
        knumActive      table isource_chan, giSourceActive
                        tablew knumActive-1, isource_chan, giSourceActive
        if knumActive == 1 then
        Sscoreline      sprintfk {{i %f 0 -1 %i}}, -(6+frac(p1)+(isource_chan*0.1)), isource_chan
                        scoreline Sscoreline, 1
        endif
        endif
        endin
        
; *********************
; OSC listener control, write data to chn bus
        instr 6
        isource_chan     = p4

; >>>>>>> AUTOGENERATED TEXT: DO NOT EDIT THE FOLLOWING LINES >>>>>>>
Srms sprintf {{rms%i}}, isource_chan 
Srms_dB sprintf {{rms_dB%i}}, isource_chan 
Stransient sprintf {{transient%i}}, isource_chan 
Strans_dens sprintf {{trans_dens%i}}, isource_chan 
Senv_crest sprintf {{env_crest%i}}, isource_chan 
Senv_dyn sprintf {{env_dyn%i}}, isource_chan 
Spitch sprintf {{pitch%i}}, isource_chan 
Ss_centroid sprintf {{s_centroid%i}}, isource_chan 
Ss_spread sprintf {{s_spread%i}}, isource_chan 
Ss_skewness sprintf {{s_skewness%i}}, isource_chan 
Ss_kurtosis sprintf {{s_kurtosis%i}}, isource_chan 
Ss_flatness sprintf {{s_flatness%i}}, isource_chan 
Ss_crest sprintf {{s_crest%i}}, isource_chan 
Ss_flux sprintf {{s_flux%i}}, isource_chan 
Srhythm_irreg sprintf {{rhythm_irreg%i}}, isource_chan 
Srhythm_cons sprintf {{rhythm_cons%i}}, isource_chan 
Srhyt_con_dev sprintf {{rhyt_con_dev%i}}, isource_chan 
Sra_flux sprintf {{ra_flux%i}}, isource_chan 
Sra_crest sprintf {{ra_crest%i}}, isource_chan 
Smfcc1 sprintf {{mfcc1%i}}, isource_chan 
Smfcc2 sprintf {{mfcc2%i}}, isource_chan 
Smfcc3 sprintf {{mfcc3%i}}, isource_chan 
Smfcc_diff sprintf {{mfcc_diff%i}}, isource_chan 
SoscName_a sprintf {{/%s_a_%i}}, "analysis", isource_chan 


Smess[] init 30
nxt_val:
Smess, ka OSCraw i(gkhandle)
Sadr strcpyk Smess[k(0)]
if strcmpk(Sadr, SoscName_a) > -1 then
  Skrms strcpyk Smess[k(2)] 
  Skrms_dB_n strcpyk Smess[k(3)] 
  Skamp_trans_hold strcpyk Smess[k(4)] 
  SkatransDensEnv_n strcpyk Smess[k(5)] 
  Skenv_crest1 strcpyk Smess[k(6)] 
  Skenv_dyn strcpyk Smess[k(7)] 
  Skpitch_n strcpyk Smess[k(8)] 
  Skcentroid_n strcpyk Smess[k(9)] 
  Skspread_n strcpyk Smess[k(10)] 
  Skskewness_n strcpyk Smess[k(11)] 
  Skurtosis_n strcpyk Smess[k(12)] 
  Skflatness_n strcpyk Smess[k(13)] 
  Skcrest_n strcpyk Smess[k(14)] 
  Skflux_n strcpyk Smess[k(15)] 
  Skrhythm_irregularity strcpyk Smess[k(16)] 
  Skrhythm_consonance strcpyk Smess[k(17)] 
  Skrhythm_consonance_deviation strcpyk Smess[k(18)] 
  Skra_flux strcpyk Smess[k(19)] 
  Skrhythm_ac_crest strcpyk Smess[k(20)] 
  Skmfcc1 strcpyk Smess[k(21)] 
  Skmfcc2 strcpyk Smess[k(22)] 
  Skmfcc3 strcpyk Smess[k(23)] 
  Skmfccdiff strcpyk Smess[k(24)] 

Skrms init "0"
Skrms init "0" 
Skrms_dB_n init "0" 
Skamp_trans_hold init "0" 
SkatransDensEnv_n init "0" 
Skenv_crest1 init "0" 
Skenv_dyn init "0" 
Skpitch_n init "0" 
Skcentroid_n init "0" 
Skspread_n init "0" 
Skskewness_n init "0" 
Skurtosis_n init "0" 
Skflatness_n init "0" 
Skcrest_n init "0" 
Skflux_n init "0" 
Skrhythm_irregularity init "0" 
Skrhythm_consonance init "0" 
Skrhythm_consonance_deviation init "0" 
Skra_flux init "0" 
Skrhythm_ac_crest init "0" 
Skmfcc1 init "0" 
Skmfcc2 init "0" 
Skmfcc3 init "0" 
Skmfccdiff init "0"

krms strtodk Skrms
  krms_dB_n strtodk Skrms_dB_n
  kamp_trans_hold strtodk Skamp_trans_hold
  katransDensEnv_n strtodk SkatransDensEnv_n
  kenv_crest1 strtodk Skenv_crest1
  kenv_dyn strtodk Skenv_dyn
  kpitch_n strtodk Skpitch_n
  kcentroid_n strtodk Skcentroid_n
  kspread_n strtodk Skspread_n
  kskewness_n strtodk Skskewness_n
  kurtosis_n strtodk Skurtosis_n
  kflatness_n strtodk Skflatness_n
  kcrest_n strtodk Skcrest_n
  kflux_n strtodk Skflux_n
  krhythm_irregularity strtodk Skrhythm_irregularity
  krhythm_consonance strtodk Skrhythm_consonance
  krhythm_consonance_deviation strtodk Skrhythm_consonance_deviation
  kra_flux strtodk Skra_flux
  krhythm_ac_crest strtodk Skrhythm_ac_crest
  kmfcc1 strtodk Skmfcc1
  kmfcc2 strtodk Skmfcc2
  kmfcc3 strtodk Skmfcc3
  kmfccdiff strtodk Skmfccdiff

  chnset krms, Srms 
  chnset krms_dB_n, Srms_dB 
  chnset kamp_trans_hold, Stransient 
  chnset katransDensEnv_n, Strans_dens 
  chnset kenv_crest1, Senv_crest 
  chnset kenv_dyn, Senv_dyn 
  chnset kpitch_n, Spitch 
  chnset kcentroid_n, Ss_centroid 
  chnset kspread_n, Ss_spread 
  chnset kskewness_n, Ss_skewness 
  chnset kurtosis_n, Ss_kurtosis 
  chnset kflatness_n, Ss_flatness 
  chnset kcrest_n, Ss_crest 
  chnset kflux_n, Ss_flux 
  chnset krhythm_irregularity, Srhythm_irreg 
  chnset krhythm_consonance, Srhythm_cons 
  chnset krhythm_consonance_deviation, Srhyt_con_dev 
  chnset kra_flux, Sra_flux 
  chnset krhythm_ac_crest, Sra_crest 
  chnset kmfcc1, Smfcc1 
  chnset kmfcc2, Smfcc2 
  chnset kmfcc3, Smfcc3 
  chnset kmfccdiff, Smfcc_diff 
endif  
; <<<<<<< AUTOGENERATED TEXT: DO NOT EDIT THE PRECEDING LINES <<<<<<<

if (ka > 0) kgoto nxt_val
endin
  
; *********************
; read (OSC) data from chn bus, update parameter channel and gui
        instr 7
        Sparam          strget p4
        Ssource         strget p5
        isource_chan     = p6
        idest_mix_chn   = p7    ; for mixing from several sources
        ;if strlenk(Ssource) == 0 then ; hack for combobox string channel first time init
        ;Ssource = "rms"
        ;endif
        SsourceChan     sprintf {{%s%i}}, Ssource, isource_chan
        ;puts Sparam,1
        ;puts SsourceChan,1
        kval_0          chnget SsourceChan
        ;Source_val      sprintfk "param %s instr %f source %s val%f", Sparam, p1, SsourceChan, kval_0
        ;puts Source_val, kval_0+1

        kraw            strindexk Ssource, "_raw"
        ;kdens           strindexk Ssource, "transdens"
        ;Stest           sprintfk "%s : %i", Ssource, kraw
        ;                puts Stest, kraw+10
        ;krawflag        = (kraw+kdens>0?1:0)
        krawflag        = (kraw>0?1:0)
        Srawflag        sprintf "%s_raw_flag%i", Sparam, idest_mix_chn
        ;puts Srawflag, 1
                        chnset krawflag, Srawflag

        ; if source is a transient trigger, make an envelope
        itrigSource     strindex Ssource, "transient"
        kvaltrig        trigger kval_0, 0.5, 0
        if kvaltrig > 0 then
        reinit transEnvelope
        endif
        Srisechn        sprintf "rise%i_%s", idest_mix_chn, Sparam
        Sfallchn        sprintf "fall%i_%s", idest_mix_chn, Sparam
        krise           chnget Srisechn
        kfall           chnget Sfallchn

transEnvelope:
        kenv            linseg 0, i(krise), 1, i(kfall), 0, 1, 0
        rireturn
        if itrigSource > 0 then
        kval            = kenv
        else
        ; if not envelope triggering, then envelope follow filtering
	krise   limit krise, 1/kr, 10
	kfall   limit kfall, 1/kr, 10
        kA      = 0.001^(1/(krise*kr))
        kB      = 0.001^(1/(kfall*kr))
        kfilt   init 0
        kfilt   = (kval_0>kfilt?(kval_0+(kA*(kfilt-kval_0))):(kval_0+(kB*(kfilt-kval_0))))
        kval    = kfilt
        endif

        Sscalechn       sprintf "scale%i_%s", idest_mix_chn, Sparam
        Sscalexchn      sprintf "scale%i_x_%s", idest_mix_chn, Sparam
        kscale_param    chnget Sscalechn
        kscale_x_param  chnget Sscalexchn
        if kscale_x_param == 2 then
        kscale_param    *= 2
        elseif kscale_x_param == 3 then
        kscale_param    *= 10
        endif
        ; curve/map, then write to temporary chn
        Scurvechn       sprintf "curve%i_%s", idest_mix_chn, Sparam
        kcurve          chnget Scurvechn
        if kcurve == 0 then
        kcurved         = kval
        else
        kcurved         = (1 - exp(kval*kcurve) ) / (1 - exp(kcurve))
        endif
        kcurved         = kcurved*kscale_param
        Stemp           sprintf "temp%i_%s", idest_mix_chn, Sparam
                        chnset kcurved, Stemp
        Sto_gate        sprintf "to_gate%i_%s", idest_mix_chn, Sparam
                        chnset kval_0, Sto_gate
        endin
        
        instr 8
        Sparam          strget p4
        imod_num        = p5 ; module number, for linking sample and hold gates
        Stemp1          sprintf "temp1_%s", Sparam
        Stemp2          sprintf "temp2_%s", Sparam
        Soffsetchn      sprintf "offset_%s", Sparam
        Soffsetxchn     sprintf "offsetx_%s", Sparam
        ;Sparam_min      sprintf "%s_min", Sparam
        ;Sparam_max      sprintf "%s_max", Sparam
        Sto_gate1       sprintf "to_gate1_%s", Sparam
        Sto_gate2       sprintf "to_gate2_%s", Sparam
        Sthresh1        sprintf "thresh1_%s", Sparam
        Sthresh2        sprintf "thresh2_%s", Sparam
        Slowpass1       sprintf "lowpass1_%s", Sparam
        Slowpass2       sprintf "lowpass2_%s", Sparam
        Srise           sprintf "outrise_%s", Sparam
        Sfall           sprintf "outfall_%s", Sparam
        Strigtype1      sprintf "triggertype1_%s", Sparam
        Strigtype2      sprintf "triggertype2_%s", Sparam
        Sdisplay1       sprintf "display1_%s", Sparam
        Sdisplay2       sprintf "display2_%s", Sparam
        kgate           init 0
        kval1           chnget Stemp1
        kval2           chnget Stemp2
        koffset         chnget Soffsetchn
        koffsetx        chnget Soffsetxchn
        Soffsets[]      fillarray "-1", "Norm", "+1"
        Soffsetx        strcpyk Soffsets[koffsetx-1]
        imin            = 0;chnget Sparam_min
        imax            = 1;chnget Sparam_max
        if strcmpk(Soffsetx, "-1") == 0 then
        koffset1        = koffset-(imax-imin)
        elseif strcmpk(Soffsetx, "+1") == 0 then
        koffset1        = koffset+(imax-imin)
        else
        koffset1        = koffset
        endif
        Srawflag1        sprintf "%s_raw_flag1", Sparam
        Srawflag2        sprintf "%s_raw_flag2", Sparam
        kraw1           chnget Srawflag1
        kraw2           chnget Srawflag2
        Smixmeth        sprintf "method_%s", Sparam
        kmixmeth        chnget Smixmeth
        Smixmethods[]   fillarray "add", "abs_diff", "gate"
        Smixmethod      strcpyk Smixmethods[kmixmeth-1]
        if (strcmpk(Smixmethod, "add") == 0) then 
          if (kraw1 == 1) && (kraw2 == 0) then
            kval limit kval1+(kval2*(imax-imin))+koffset1, imin, imax  ; do not autoscale raw values for source 1
          elseif (kraw2 == 1) && (kraw1 == 0) then
            kval limit (kval1*(imax-imin))+kval2+koffset1, imin, imax  ; do not autoscale raw values for source 2
          elseif (kraw2 == 1) && (kraw1 == 1) then
            kval limit kval1+kval2+koffset1, imin, imax                ; do not autoscale raw values for either source
          else
            kval limit ((kval1+kval2)*(imax-imin))+koffset1, imin, imax  ; autoscale normalized values
          endif
        elseif strcmpk(Smixmethod, "abs_diff") == 0 then
          if (kraw1 == 1) && (kraw2 == 0) then
            kval limit abs(kval1-(kval2*(imax-imin)))+koffset1, imin, imax  ; do not autoscale raw values for source 1
          elseif (kraw2 == 1) && (kraw1 == 0) then
            kval limit abs((kval1*(imax-imin))-kval2)+koffset1, imin, imax  ; do not autoscale raw values for source 2
          elseif (kraw2 == 1) && (kraw1 == 1) then
            kval limit abs(kval1-kval2)+koffset1, imin, imax                ; do not autoscale raw values for either source
          else
            kval limit (abs(kval1-kval2)*(imax-imin))+koffset1, imin, imax  ; autoscale normalized values
          endif
        elseif strcmpk(Smixmethod, "gate") == 0 then
          kto_gate1 chnget Sto_gate1
          kto_gate2 chnget Sto_gate2
          klopass1 chnget Slowpass1
          klopass2 chnget Slowpass2
          kto_gate1 tonek kto_gate1, klopass1
          kto_gate2 tonek kto_gate2, klopass2
          kthresh1 chnget Sthresh1
          kthresh2 chnget Sthresh2
          ktrigtype1 chnget Strigtype1
          ktrigtype1 limit ktrigtype1, 1, 2 ; to allow old saved projects to load (if the plugin version did not contain this widget)
          ktrigtype2 chnget Strigtype2
          ktrigtype2 limit ktrigtype2, 1, 2
          chnset kto_gate1, Sdisplay1
          chnset kto_gate2, Sdisplay2
          ktrig1 trigger kto_gate1, kthresh1, ktrigtype1-1
          ktrig2 trigger kto_gate2, kthresh2, ktrigtype2-1
          kgate = (ktrig1 > 0 ? 1 : kgate)
          kgate = (ktrig2 > 0 ? 0 : kgate)
          krise chnget Srise
          kfall chnget Sfall
          knofilter = (krise+kfall == 0 ? 1 : 0)
	  krise limit krise, 1/kr, 10
	  kfall limit kfall, 1/kr, 10
          kA = 0.001^(1/(krise*kr))
          kB = 0.001^(1/(kfall*kr))
          kfilt init 0
          kfilt = (kgate>kfilt?(kgate+(kA*(kfilt-kgate))):(kgate+(kB*(kfilt-kgate))))
          kval = (knofilter == 1 ? kgate : kfilt)
        endif
        SgateOut        sprintf "gate_parm%i", imod_num
                        chnset kgate, SgateOut
        SgateIn         sprintf "gate_parm%i", imod_num-1
        kgate_in        chnget SgateIn
        SampleHold      sprintf "sh_parm%i", imod_num-1
        ksh             chnget SampleHold
        klearnactive chnget "learn_active"
        if klearnactive == 0 then
          if ksh == 0 then
                        chnset kval, Sparam
          else
            if kgate_in > 0 then
                        chnset kval, Sparam
            endif
          endif
        endif
	endin
