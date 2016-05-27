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
; try find an available port and connect to it (instr 2 and 3)
        instr 2
        kmetro          metro 1
        if (gkhandle == -1) && (kmetro == 1) then
        gkport          += 1
                        event "i", -3, 0.1, 1, gkport
                        event "i", 3, 0.2, -1, gkport
        endif
        endin

        instr 3
        iport           = p4
        iport           = 7001+iport
        print iport
	gihandle        OSCinit iport
	gkhandle = gihandle
	Sport  sprintf {{text("port %i")}}, iport
	                chnset Sport, "port_id"
        endin


