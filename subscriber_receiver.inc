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
        Srms            sprintf {{rms%i}}, isource_chan
        Srms_preEq      sprintf {{rms_preEq%i}}, isource_chan
        Scps            sprintf {{cps%i}}, isource_chan
        Spitch          sprintf {{pitch%i}}, isource_chan
        Scentroid       sprintf {{centroid%i}}, isource_chan
        Sflatness       sprintf {{flatness%i}}, isource_chan
        Sspread         sprintf {{spread%i}}, isource_chan
        Sskewness       sprintf {{skewness%i}}, isource_chan
        Skurtosis       sprintf {{kurtosis%i}}, isource_chan
        Screst          sprintf {{crest%i}}, isource_chan
        Sflux           sprintf {{flux%i}}, isource_chan
        Sautocorr       sprintf {{autocorr%i}}, isource_chan
        Samp_trans      sprintf {{amp_trans%i}}, isource_chan
        Samp_transdens  sprintf {{amp_t_dens%i}}, isource_chan
        Scentr_trans    sprintf {{centr_trans%i}}, isource_chan
        Scentr_transdens sprintf {{centr_t_dens%i}}, isource_chan
        Skurt_trans     sprintf {{kurt_trans%i}}, isource_chan
        Spitchup_trans  sprintf {{pitchup_trans%i}}, isource_chan
        Spitchdown_trans sprintf {{pitchdown_trans%i}}, isource_chan
        Scps_raw        sprintf {{cps_raw%i}}, isource_chan

        SoscName        sprintf {{/%s%i}}, "analysis", isource_chan
        ;print p1
        krms            init 0
        krms_preEq      init 0
        kcps            init 0
        kpitch          init 0
        kcentroid       init 0
        kspread         init 0
        kskewness       init 0
        kurtosis        init 0
        kflatness       init 0
        kcrest          init 0
        kflux           init 0
        kautocorr       init 0
        kamp_trans      init 0
        katransDensEnv  init 0
        kcentr_trans    init 0
        kctransDensEnv  init 0
        kkurt_trans     init 0
        kpitchup_trans  init 0
        kpitchdwn_trans init 0
        kcps_raw        init 0
        
nxt_val:
    	kk		OSClisten gihandle, SoscName, "fffffffffffffffffff", krms, krms_preEq, kcps, kpitch, kcentroid, kspread, kskewness, kurtosis, kflatness, kcrest, kflux, kamp_trans, katransDensEnv, kcentr_trans, kctransDensEnv, kkurt_trans, kpitchup_trans, kpitchdwn_trans, kcps_raw
                	chnset krms, Srms               	
                	chnset krms_preEq, Srms_preEq
                        chnset kcps, Scps            
                        chnset kpitch, Spitch            
                        chnset kcentroid, Scentroid       
                	chnset kspread, Sspread         
                	chnset kskewness, Sskewness       
                	chnset kurtosis, Skurtosis       
                	chnset kflatness, Sflatness       
                	chnset kcrest, Screst          
                	chnset kflux, Sflux           
                        chnset kamp_trans, Samp_trans          
                        chnset katransDensEnv, Samp_transdens          
                        chnset kcentr_trans, Scentr_trans        
                        chnset kctransDensEnv, Scentr_transdens          
                        chnset kkurt_trans, Skurt_trans
                        chnset kpitchup_trans, Spitchup_trans
                        chnset kpitchdwn_trans, Spitchdown_trans
                        chnset kcps_raw, Scps_raw   

if (kk == 0) goto ex_val
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
        kval_0          chnget SsourceChan
        ;Source_val      sprintfk "param %s instr %f source %s val%f", Sparam, p1, SsourceChan, kval
        ;puts Source_val, kval

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
        itrigSource     strindex Ssource, "_trans"
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
