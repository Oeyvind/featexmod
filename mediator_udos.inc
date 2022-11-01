opcode OscLearnReaper, 0, S[]ii
  Smess[], ichan, ilabelinstr xin
  Sdaw = "Reaper"
  Sadr strcpyk Smess[k(0)]  
  ;Sparm_val strcpyk, Smess[k(2)]
         
  kfx strindexk Sadr, "fx/"
  kfxmaster strindexk Sadr, "fxparam/"
  ksend strindexk Sadr, "send"
  if kfx > -1 then
    Stest strcpyk Smess[k(9)]
    ktest strindexk Stest, "name"
    if ktest>0 then
      Sparm_name strcpyk Smess[k(11)]
    endif
  elseif kfxmaster > -1 then
    Sadr strcpyk Smess[k(3)]
    ;Sparm_val strcpyk Smess[k(5)]
    Sparm_name strcpyk Smess[k(26)]
    Sname1 strcpyk Smess[k(12)]
    Sname2 strcpyk Smess[k(21)]
    if strcmpk(Sname1, "/fxparam/last_touched/name") == 0 then
      Sparm_name strcpyk Smess[k(14)]
    elseif strcmpk(Sname2, "/fxparam/last_touched/name") == 0 then
      Sparm_name strcpyk Smess[k(23)]
    endif
  elseif ksend > 0 then
    Sendnum strsubk Sadr, ksend+5, ksend+6
    Sparm_name sprintfk "Send %s", Sendnum
    Sadr strcpyk Smess[k(3)]
    ;Sparm_val strcpyk, Smess[k(5)]
  else
    kdb strindexk Sadr, "db"
    if kdb > 0 then ; special case for track volume
      Sadr strcpyk, Smess[k(6)]
      ;Sparm_val strcpyk, Smess[k(8)]
    endif
    kparm strrindexk Sadr, "/" ; normal case for non-fx
    Sparm_name strsubk Sadr, kparm+1, -1
  endif
    
  ; track num display
  kistrack strindexk Sadr, "track"
  if kistrack > -1 then
    S2_ strsubk Sadr, kistrack+6, -1
    knumtest strindexk S2_, "/"
    Strackname strsubk Sadr, kistrack+6, kistrack+6+knumtest
    if knumtest == -1 then
      Strackname = "mstr"
    endif
  endif
  if strindexk(Sadr,"master")>-1 then
    Strackname strsubk Sadr, 7,8
  endif
  
  ; output
  knew0 changed Smess[k(0)] 
  knew1 changed Smess[k(1)]
  knew2 changed Smess[k(2)]
  knew = knew0+knew1+knew2
  if knew>0 then
    Sscoreline sprintfk {{i %i 0 0.1 %i "%s" "%s" "%s" "%s" "%s" "%s" "%s"}}, ilabelinstr, ichan, Sadr, Strackname, Sparm_name, "", "", "", Sdaw
    scoreline Sscoreline, 1
  endif
endop

opcode OscLearnLive, 0, S[]ii
  Smess[], ichan, ilabelinstr xin
  Sdaw = "Live"
  Sadr strcpyk Smess[k(0)]
  Stype strcpyk Smess[k(1)]
  Stype strsubk Stype, 1, -1
  Strack strcpyk Smess[k(2)]
  knew0 changed Smess[k(0)] 
  knew1 changed Smess[k(1)]
  knew2 changed Smess[k(2)]
  knew3 changed Smess[k(3)]
  knew4 changed Smess[k(4)]
  knew5 changed Smess[k(5)]
  knew6 changed Smess[k(6)]
  knew = knew0+knew1+knew2+knew3+knew4+knew5+knew6

  kfx strindexk Sadr, "device"
  if kfx < 0 then
    kparm strrindexk Sadr, "/"
    Sparm_name strsubk Sadr, kparm+1, -1
    ksend strindexk Sadr, "send"
    if ksend < 0 then
      Sparm_val strcpyk Smess[k(3)]
    else
      Sparm_num strcpyk Smess[k(3)]
      Sparm_val strcpyk Smess[k(4)]
    endif
  else
    ; type: iiifs
    ; track device, parm_num, val, name
    Sdevice strcpyk Smess[k(3)]
    Sparm_num strcpyk Smess[k(4)]
    Sparm_val strcpyk Smess[k(5)]
    Sparm_name strcpyk Smess[k(6)]
  endif
  
  ; track num/name display
  if strindexk(Sadr,"master")>-1 then
    Strackname strcpyk "master"
  elseif strindexk(Sadr,"return")>-1 then
    Strackname sprintfk "return%s", Strack 
  else
    Strackname sprintfk "track%s", Strack
  endif
  
  ; set labels
  if knew>0 then
    Sscoreline sprintfk {{i %i 0 0.1 %i "%s" "%s" "%s" "%s" "%s" "%s" "%s"}}, ilabelinstr, ichan, Sadr, Strackname, Sparm_name, Sdevice, Strack, Sparm_num, Sdaw
    scoreline Sscoreline, 1
  endif
endop

;inputs Sdevice, Strack, Sparm_num, Sadr, kData1
opcode OscControlLive, 0, SSSSk
  Sdevice, Strack, Sparm_num, Sadr, kData xin
  ktrigd changed Sdevice 
  ktrigt changed Strack 
  ktrigp changed Sparm_num 
  ktriga changed Sadr
  ktrig = ktrigd+ktrigt+ktrigp+ktriga 
  printk2 ktrig, 10
  if ktrig > 0 then
    reinit stringupdate
  endif
  stringupdate:
  if strlen(Sdevice)==0 then
    Sdevice strcpy "123"
  endif
  if strlen(Strack)==0 then
    Strack strcpy "123"
  endif
  if strlen(Sparm_num)==0 then
    Sparm_num strcpy "123"
  endif

  kwhen changed kData

  if strindex(Sadr,"master")>-1 then
    if strindex(Sadr,"device")>-1 then
      Sadr_d strsub Sadr, 0, strindex(Sadr,"param")-1 ; device on master still does not work, but this *should* be correct...
      OSCsend kwhen+1, "127.0.0.1",9000, Sadr_d, "iif", strtod(Sdevice), strtod(Sparm_num), kData
    elseif strindex(Sadr,"pan")>-1 then
      OSCsend kwhen+1, "127.0.0.1",9000, Sadr, "f", (kData*2)-1
    else 
      OSCsend kwhen+1, "127.0.0.1",9000, Sadr, "f", kData
    endif
  elseif strindex(Sadr,"mute")>-1 then
    OSCsend kwhen+1, "127.0.0.1",9000, Sadr, "ii", strtod(Strack), kData
  elseif strindex(Sadr,"pan")>-1 then
    OSCsend kwhen+1, "127.0.0.1",9000, Sadr, "if", strtod(Strack), (kData*2)-1
  elseif strindex(Sadr,"vol")>-1 then
    OSCsend kwhen+1, "127.0.0.1",9000, Sadr, "if", strtod(Strack), kData
  elseif strindex(Sadr,"send")>-1 then
    OSCsend kwhen+1, "127.0.0.1",9000, Sadr, "iif", strtod(Strack), strtod(Sparm_num), kData
  elseif strindex(Sadr,"device")>-1 then
    Sadr_d strsub Sadr, 0, strindex(Sadr,"param")-1
    OSCsend kwhen+1, "127.0.0.1",9000, Sadr_d, "iiif", strtod(Strack), strtod(Sdevice), strtod(Sparm_num), kData
  endif
  rireturn
  /*
  mute ii tracknum, val
  vol/pan if tracknum, val
  send iif tracknum, sendnum, val
  dev iiifs tracknum, devnum, parmnum, val, name
  master vol f val
  master dev iifs devnum, parmnum, val, name
  */
endop

opcode SendOscToDaw, 0, kSkSSSSk
kenable, Sdaw, klearn_off, Sdevice, Strack, Sparm_num, Sadr, kdata  xin
kData[][] init 10,1
Sdest[] init 10
Stype[] fillarray "f","f","f","f","f","f","f","f","f","f"
Sdest_empty[] fillarray "","","","","","","","","",""    
ktrig metro chnget:k("oscrate")
kindex init 0

if strlen(Sdaw)==0 then
  Sdaw strcpy "123"
endif
; reduce send speed
kdata = int(kdata*1000)/1000
if (kenable>0) then
  if strcmpk(Sdaw,"Live") == 0 then ; ** Live as DAW
    if klearn_off > 0 then 
      reinit livecontrol
    endif
    livecontrol:
    OscControlLive Sdevice, Strack, Sparm_num, Sadr, kdata
    rireturn
  else ; ** Reaper as DAW
/* single messages (non-bundle), enable these two lines and disable the next if-paragraph
    kwhen changed kdata
    OSCsend kwhen+1, "127.0.0.1",9000, Sadr, "f", kdata
*/

    kwhen changed kdata
    if kwhen > 0 then
      Sdest[kindex] strcpyk Sadr
      kData[kindex][0] = kdata
      kindex += 1	

      if kindex > lenarray(Sdest)-1 || ktrig > 0 then
      ;if kindex > lenarray(Sdest)-1 then
        OSCbundle  rnd31:k(1,1)+2, "127.0.0.1", 9000, Sdest, Stype, kData
        ;printarray(Sdest)
        ;printarray(kData)
        Sdest = Sdest_empty
        kData *= 0
        kindex = 0
      endif
    endif

  endif
endif
endop

    