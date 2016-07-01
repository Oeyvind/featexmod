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

        ichan           chnget "chan"
 	ibaseport	= 7001
        SoscName        sprintf {{/%s%i}}, "analysis", ichan
	kwhen           init 0
	kcountwhen      init 0
	kcountwhen      = (kcountwhen+1)%2   ; send on every Nth k-rate period
	kwhen           = (kcountwhen == 1 ? kwhen+1 : kwhen)
#define PUBLISH(N) #
	iport$N.	= ibaseport + $N.
        OSCsend         kwhen, "127.0.0.1", iport$N., SoscName, "fffffffffffffffffffffffff", krms, krms_preEq, kcps_n, kpitch_a, kcentroid_a, kspread_a, 
                             kskewness_a, kurtosis_a, kflatness_a, kcrest_a, kflux_a, 
                             kamp_trans, katransDensEnv, krhythm_consonance, krhythm_ratio1, krhythm_ratio2, krhythm_ac1, krhythm_ac2,  
                             krhythm_ac1time, krhythm_ac2time, kmfcc1, kmfcc2, kmfcc3, kmfcc4, kcps ; (the kcps value is raw, not normalized!)

#
$PUBLISH(0)

        
        