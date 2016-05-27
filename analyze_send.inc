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

        ichan           chnget "chan"
 	ibaseport	= 7001
        SoscName        sprintf {{/%s%i}}, "analysis", ichan
	kwhen           init 0
	kcountwhen      init 0
	kcountwhen      = (kcountwhen+1)%4   ; send on every Nth k-rate period
	kwhen           = (kcountwhen == 1 ? kwhen+1 : kwhen)
#define PUBLISH(N) #
	iport$N.	= ibaseport + $N.
        OSCsend         kwhen, "127.0.0.1", iport$N., SoscName, "fffffffffffffffffff", krms, krms_preEq, kcps_n, kpitch_a, kcentroid_a, kspread_a, kskewness_a, kurtosis_a, kflatness_a, kcrest_a, kflux_a, kamp_trans, katransDensEnv, kcentr_trans, kctransDensEnv, kkurt_trans, kpitchup_trans, kpitchdwn_trans, kcps ; (the kcps value is raw, not normalized!)
#
$PUBLISH(0)
$PUBLISH(1)
$PUBLISH(2)
$PUBLISH(3)
$PUBLISH(4)
$PUBLISH(5)
$PUBLISH(6)
$PUBLISH(7)
$PUBLISH(8)
$PUBLISH(9)

        
        