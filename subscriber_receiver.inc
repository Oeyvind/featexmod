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
        instr_8         = 8+frac(p1)
        Sscoreline      sprintf {{i %f 0 -1 "%s"}}, instr_8, Sparam
                        scoreline_i Sscoreline
        Ssourcechn1     sprintf "source1_%s", Sparam
        ;puts Ssourcechn1, 1
        Ssourcechn2     sprintf "source2_%s", Sparam
        Schanchn1       sprintf "chan1_%s", Sparam
        Schanchn2       sprintf "chan2_%s", Sparam
        Source_old1     strcpy ""
        Source_old2     strcpy ""
        Source1         chnget Ssourcechn1
        Source2         chnget Ssourcechn2
        ksourcechanged1 strcmpk Source1, Source_old1
        ksourcechanged2 strcmpk Source2, Source_old2
        Source_old1     strcpyk Source1
        Source_old2     strcpyk Source2
        kchan1          chnget Schanchn1
        kchan2          chnget Schanchn2
        ;printk2 kchan1
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
        print inumActive, p1
        Sscoreline      sprintf {{i %f 0.1 -1  %i}}, 6+frac(p1)+(isource_chan*0.1), isource_chan
        puts Sscoreline, 1
        if inumActive == 0 then
                        scoreline_i Sscoreline
        endif
                        tableiw inumActive+1, isource_chan, giSourceActive

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
Srhyt_ratio1 sprintf {{rhyt_ratio1%i}}, isource_chan 
Srhyt_ratio2 sprintf {{rhyt_ratio2%i}}, isource_chan 
Srhyt_ratio3 sprintf {{rhyt_ratio3%i}}, isource_chan 
Srac_1st_v sprintf {{rac_1st_v%i}}, isource_chan 
Srac_clos_v sprintf {{rac_clos_v%i}}, isource_chan 
Srac_max_v sprintf {{rac_max_v%i}}, isource_chan 
Srac_max2_v sprintf {{rac_max2_v%i}}, isource_chan 
Srac_max3_v sprintf {{rac_max3_v%i}}, isource_chan 
Srac_1st_tim sprintf {{rac_1st_tim%i}}, isource_chan 
Srac_clos_tim sprintf {{rac_clos_tim%i}}, isource_chan 
Srac_max_tim sprintf {{rac_max_tim%i}}, isource_chan 
Srac_max2_tim sprintf {{rac_max2_tim%i}}, isource_chan 
Srac_max3_tim sprintf {{rac_max3_tim%i}}, isource_chan 
Ssubdiv_1st sprintf {{subdiv_1st%i}}, isource_chan 
Ssubdiv_clos sprintf {{subdiv_clos%i}}, isource_chan 
Ssubdiv_max2 sprintf {{subdiv_max2%i}}, isource_chan 
Ssubdiv_max3 sprintf {{subdiv_max3%i}}, isource_chan 
Sgrid_subdiv sprintf {{grid_subdiv%i}}, isource_chan 
Sgridness sprintf {{gridness%i}}, isource_chan 
Sra_flux sprintf {{ra_flux%i}}, isource_chan 
Sra_crest sprintf {{ra_crest%i}}, isource_chan 
Smfcc1 sprintf {{mfcc1%i}}, isource_chan 
Smfcc2 sprintf {{mfcc2%i}}, isource_chan 
Smfcc3 sprintf {{mfcc3%i}}, isource_chan 
Smfcc_diff sprintf {{mfcc_diff%i}}, isource_chan 
SoscName_a sprintf {{/%s_a_%i}}, "analysis", isource_chan 
SoscName_b sprintf {{/%s_b_%i}}, "analysis", isource_chan 
krms init 0 
krms_dB_n init 0 
kamp_trans_hold init 0 
katransDensEnv_n init 0 
kenv_crest1 init 0 
kpitch_n init 0 
kcentroid_n init 0 
kspread_n init 0 
kskewness_n init 0 
kurtosis_n init 0 
kflatness_n init 0 
kcrest_n init 0 
kflux_n init 0 
krhythm_irregularity init 0 
krhythm_consonance init 0 
krhythm_consonance_deviation init 0 
krhythm_ratio1 init 0 
krhythm_ratio2 init 0 
krhythm_ratio3 init 0 
kra_first_v init 0 
kra_closest_v init 0 
kra_max1_v init 0 
kra_max2_v init 0 
kra_max3_v init 0 
kra_first_time init 0 
kra_closest_time init 0 
kra_max1_time init 0 
kra_max2_time init 0 
kra_max3_time init 0 
krdenom_first init 0 
krdenom_closest init 0 
krdenom_max2 init 0 
krdenom_max3 init 0 
kgrid_subdiv init 0 
kgridness init 0 
kra_flux init 0 
krhythm_ac_crest init 0 
kmfcc1 init 0 
kmfcc2 init 0 
kmfcc3 init 0 
kmfccdiff init 0 
nxt_val: 
kk1 OSClisten gihandle, SoscName_a, "fffffffffffffffffffffffff", krms, krms_dB_n, kamp_trans_hold, katransDensEnv_n, kenv_crest1, kpitch_n, kcentroid_n, kspread_n, kskewness_n, kurtosis_n, kflatness_n, kcrest_n, kflux_n, krhythm_irregularity, krhythm_consonance, krhythm_consonance_deviation, krhythm_ratio1, krhythm_ratio2, krhythm_ratio3, kra_first_v, kra_closest_v, kra_max1_v, kra_max2_v, kra_max3_v, kra_first_time 
kk2 OSClisten gihandle, SoscName_b, "ffffffffffffffff", kra_closest_time, kra_max1_time, kra_max2_time, kra_max3_time, krdenom_first, krdenom_closest, krdenom_max2, krdenom_max3, kgrid_subdiv, kgridness, kra_flux, krhythm_ac_crest, kmfcc1, kmfcc2, kmfcc3, kmfccdiff 
chnset krms, Srms 
chnset krms_dB_n, Srms_dB 
chnset kamp_trans_hold, Stransient 
chnset katransDensEnv_n, Strans_dens 
chnset kenv_crest1, Senv_crest 
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
chnset krhythm_ratio1, Srhyt_ratio1 
chnset krhythm_ratio2, Srhyt_ratio2 
chnset krhythm_ratio3, Srhyt_ratio3 
chnset kra_first_v, Srac_1st_v 
chnset kra_closest_v, Srac_clos_v 
chnset kra_max1_v, Srac_max_v 
chnset kra_max2_v, Srac_max2_v 
chnset kra_max3_v, Srac_max3_v 
chnset kra_first_time, Srac_1st_tim 
chnset kra_closest_time, Srac_clos_tim 
chnset kra_max1_time, Srac_max_tim 
chnset kra_max2_time, Srac_max2_tim 
chnset kra_max3_time, Srac_max3_tim 
chnset krdenom_first, Ssubdiv_1st 
chnset krdenom_closest, Ssubdiv_clos 
chnset krdenom_max2, Ssubdiv_max2 
chnset krdenom_max3, Ssubdiv_max3 
chnset kgrid_subdiv, Sgrid_subdiv 
chnset kgridness, Sgridness 
chnset kra_flux, Sra_flux 
chnset krhythm_ac_crest, Sra_crest 
chnset kmfcc1, Smfcc1 
chnset kmfcc2, Smfcc2 
chnset kmfcc3, Smfcc3 
chnset kmfccdiff, Smfcc_diff 

; <<<<<<< AUTOGENERATED TEXT: DO NOT EDIT THE PRECEDING LINES <<<<<<<

if (kk1 == 0) && (kk2 == 0) goto ex_val
    	kgoto nxt_val
ex_val:
        endin
  
; *********************
; read (OSC) data from chn bus, update parameter channel and gui
        instr 7
        Sparam          strget p4
        Ssource         strget p5
        isource_chan     = p6
        idest_mix_chn   = p7    ; for mixing from several sources
        SsourceChan     sprintf {{%s%i}}, Ssource, isource_chan
        puts Sparam,1
        puts SsourceChan,1
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
        if kscale_x_param == 1 then
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
                                                
        endin
        
        instr 8
        Sparam          strget p4
        Stemp1          sprintf "temp1_%s", Sparam
        Stemp2          sprintf "temp2_%s", Sparam
        Soffsetchn      sprintf "offset_%s", Sparam
        Soffsetxchn     sprintf "offsetx_%s", Sparam
        Sparam_min      sprintf "%s_min", Sparam
        Sparam_max      sprintf "%s_max", Sparam
        kval1           chnget Stemp1
        kval2           chnget Stemp2
        koffset         chnget Soffsetchn
        Soffsetx        chnget Soffsetxchn
        imin            chnget Sparam_min
        imax            chnget Sparam_max
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
        ;printk2 kraw1
        ;printk2 kraw2, 10
        if (kraw1 == 1) && (kraw2 == 0) then
        kval            limit kval1+(kval2*(imax-imin))+koffset1, imin, imax  ; do not autoscale raw values for source 1
        elseif (kraw2 == 1) && (kraw1 == 0) then
        kval            limit (kval1*(imax-imin))+kval2+koffset1, imin, imax  ; do not autoscale raw values for source 2
        elseif (kraw2 == 1) && (kraw1 == 1) then
        kval            limit kval1+kval2+koffset1, imin, imax                ; do not autoscale raw values for either source
        else
        kval            limit ((kval1+kval2)*(imax-imin))+koffset1, imin, imax  ; autoascale normalized values
        endif
        ;printks "raw1 %i, val1 %f, raw2 %i, val2 %f, outval %f \n", 1, kraw1, kval1, kraw2, kval2, kval
                        chnset kval, Sparam
	endin
