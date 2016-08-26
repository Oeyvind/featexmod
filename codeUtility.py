#!/usr/bin/python
# -*- coding: latin-1 -*-

#    Copyright 2016 Oeyvind Brandtsegg 
#
#    This file is part of the Feature-Extract-Modulator package
#
#    The Feature-Extract-Modulator is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License version 3 
#    as published by the Free Software Foundation.
#
#    The Feature-Extract-Modulator is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with The Feature-Extract-Modulator package.  
#    If not, see <http://www.gnu.org/licenses/>.


import sys, re


if sys.argv[1] == 'vst_mediator':
    effectname = 'vst_mediator'
    parameters = [('parm1', (0.0, 1.0, 0.5, 1, 0.001)), 
                  ('parm2', (0.0, 1.0, 0.5, 1, 0.001)), 
                  ('parm3', (0.0, 1.0, 0.5, 1, 0.001)), 
                  ('parm4', (0.0, 1.0, 0.5, 1, 0.001)), 
                  ('parm5', (0.0, 1.0, 0.5, 1, 0.001)), 
                  ('parm6', (0.0, 1.0, 0.5, 1, 0.001)), 
                  ('parm7', (0.0, 1.0, 0.5, 1, 0.001)), 
                  ('parm8', (0.0, 1.0, 0.5, 1, 0.001))
                  ]
                # pName, (min, max, default, skew, increment)
                # where skew is a dynamic adjustment of exp/lin/log translation if the GUI widget
                # and increment is the smallest change allowed by the GUI widget

if sys.argv[1] == 'vst_MIDIator':
    effectname = 'vst_MIDIator'
    parameters = [('parm1', (0.0, 1.0, 0.5, 1, 0.001)), 
                  ('parm2', (0.0, 1.0, 0.5, 1, 0.001)), 
                  ('parm3', (0.0, 1.0, 0.5, 1, 0.001)), 
                  ('parm4', (0.0, 1.0, 0.5, 1, 0.001)), 
                  ('parm5', (0.0, 1.0, 0.5, 1, 0.001)), 
                  ('parm6', (0.0, 1.0, 0.5, 1, 0.001)), 
                  ('parm7', (0.0, 1.0, 0.5, 1, 0.001)), 
                  ('parm8', (0.0, 1.0, 0.5, 1, 0.001))
                  ]


#
scorefile = open(effectname+'_score_events.inc', 'w')
fractionalinstr = 0
for p in parameters:
    fractionalinstr += 1
    scorefile.write('i4.{fracinstr:02d}	3.1	$SCORELEN "{pname}"\n'.format(fracinstr=fractionalinstr, pname=p[0]))

#

chn_init_file = open(effectname+'_parameter_ranges.inc', 'w')
instr_template = '''
        instr 1
; list of min and max for the mappable parameters
{}                        
        endin
'''
parameter_ranges = ''
for i in range(len(parameters)):
    parm = parameters[i]
    parameter_ranges += '   chnset {}, "{}_min" \n'.format(parm[1][0], parm[0])
    parameter_ranges += '   chnset {}, "{}_max" \n'.format(parm[1][1], parm[0])
chn_init_file.write(instr_template.format(parameter_ranges))

#

### These need to be updated when we add or remove parameters, then autogenerate new code and all relevant files should be updated with the new paramter set

analysis_parms = ["rms", "rms_dB", "transient", "trans_dens", "env_crest", "pitch", 
"s_centroid", "s_spread", "s_skewness", "s_kurtosis", "s_flatness", "s_crest", "s_flux", "s_autocorr",
"rhythm_irreg", "rhythm_cons", "rhyt_con_dev", "rhyt_ratio1", "rhyt_ratio2", "rhyt_ratio3", 
"rac_1st_v", "rac_clos_v", "rac_max_v", "rac_max2_v", "rac_max3_v", "rac_1st_tim", "rac_clos_tim", "rac_max_tim", "rac_max2_tim", "rac_max3_tim", 
"subdiv_1st", "subdiv_clos", "subdiv_max2", "subdiv_max3", "grid_subdiv", "gridness", "rac_crest",
"mfcc1", "mfcc2", "mfcc3", "mfcc4", "mfcc_diff"] 

analysis_parms_string = ''''''
for item in analysis_parms: analysis_parms_string += '\"'+item+'" ,'
print analysis_parms_string

csound_parms = ["krms", "krms_dB_n", "krms_tran0", "katransDensEnv", "kenv_crest1", "kpitch_n", 
"kcentroid_n", "kspread_n", "kskewness_n", "kurtosis_n", "kflatness_n", "kcrest_n", "kflux_n", "kautocorr_n", 
"krhythm_irregularity", "krhythm_consonance", "krhythm_consonance_deviation", "krhythm_ratio1", "krhythm_ratio2", "krhythm_ratio3", 
"kra_first_v", "kra_closest_v", "kra_max1_v", "kra_max2_v", "kra_max3_v", "kra_first_time", "kra_closest_time", "kra_max1_time", "kra_max2_time", "kra_max3_time",
"krdenom_first", "krdenom_closest", "krdenom_max2", "krdenom_max3", "kgrid_subdiv", "kgridness", "krhythm_ac_crest", 
"kmfcc1", "kmfcc2", "kmfcc3", "kmfcc4", "kmfccdiff"]

#######################

#
start_x_pos = 30
start_y_pos = 5
plant_height = 85

plant = '''groupbox bounds({start_y}, {start_x}, 574, 81), plant("plant_{pname}"), linethickness("0"){{ 
combobox channel("source1_{pname}"), bounds(10, 12, 100, 20), items({analysis_p}), value(1), channeltype("string")
combobox channel("chan1_{pname}"), bounds(113, 12, 50, 20), items("1", "2", "3", "4"), value(1)
numberbox bounds(168, 14, 35, 15), channel("rise1_{pname}"), range(0.01, 10.0, 0.01)
numberbox bounds(206, 14, 35, 15), channel("fall1_{pname}"), range(0.01, 10.0, 0.5)
hslider bounds(243, 12, 86, 20), channel("scale1_{pname}"), range(-1.0, 1.0, 0, 1, 0.001)
button bounds(330, 12, 29, 19), channel("scale1_x_{pname}"), text("x 1","x 10")
hslider bounds(359, 12, 86, 20), channel("curve1_{pname}"), range(-5.0, 5.0, 0)

combobox channel("source2_{pname}"), bounds(10, 34, 100, 20), items({analysis_p}), value(1), channeltype("string")
combobox channel("chan2_{pname}"), bounds(113, 34, 50, 20), items("1", "2", "3", "4"), value(1)
numberbox bounds(168, 36, 35, 15), channel("rise2_{pname}"), range(0.01, 10.0, 0.01)
numberbox bounds(206, 36, 35, 15), channel("fall2_{pname}"), range(0.01, 10.0, 0.5)
hslider bounds(243, 34, 86, 20), channel("scale2_{pname}"), range(-1.0, 1.0, 0, 1, 0.001)
button bounds(330, 34, 29, 19), channel("scale2_x_{pname}"), text("x 1","x 10") 
hslider bounds(359, 34, 86, 20), channel("curve2_{pname}"), range(-5.0, 5.0, 0)

label bounds(10, 58, 100, 12), text("source"), colour(20,20,20,255)
label bounds(113, 58, 50, 12), text("chan"), colour(20,20,20,255)
label bounds(166, 58, 76, 12), text("rise/fall"), colour(20,20,20,255)
label bounds(246, 58, 110, 12), text("scale"), colour(20,20,20,255)
label bounds(362, 58, 81, 12), text("curve"), colour(20,20,20,255)

rslider bounds(443, 12, 62, 62), text("offset"), channel("offset_{pname}"), range({p_min}, {p_max}, {p_default}, {p_skew}, {p_incr}) 
combobox bounds(443, 1, 55, 12), channel("offsetx_{pname}"), items("-1", "Nornm", "+1"), , value(2), channeltype("string")

rslider bounds(504, 8, 66, 66), text("{pname}"), channel("{pname}"), range({p_min}, {p_max}, {p_default}, {p_skew}, {p_incr})
}}

'''
plantMIDI = '''groupbox bounds({start_y}, {start_x}, 720, 81), plant("plant_{pname}"), linethickness("0"){{ 
combobox channel("source1_{pname}"), bounds(10, 12, 100, 20), items({analysis_p}), value(1), channeltype("string")
combobox channel("chan1_{pname}"), bounds(113, 12, 50, 20), items("1", "2", "3", "4"), value(1)
numberbox bounds(168, 14, 35, 15), channel("rise1_{pname}"), range(0.01, 10.0, 0.01)
numberbox bounds(206, 14, 35, 15), channel("fall1_{pname}"), range(0.01, 10.0, 0.5)
hslider bounds(243, 12, 86, 20), channel("scale1_{pname}"), range(-1.0, 1.0, 0, 1, 0.001)
button bounds(330, 12, 29, 19), channel("scale1_x_{pname}"), text("x 1","x 10"), 
hslider bounds(359, 12, 86, 20), channel("curve1_{pname}"), range(-5.0, 5.0, 0)

combobox channel("source2_{pname}"), bounds(10, 34, 100, 20), items({analysis_p}), value(1), channeltype("string")
combobox channel("chan2_{pname}"), bounds(113, 34, 50, 20), items("1", "2", "3", "4"), value(1)
numberbox bounds(168, 36, 35, 15), channel("rise2_{pname}"), range(0.01, 10.0, 0.01)
numberbox bounds(206, 36, 35, 15), channel("fall2_{pname}"), range(0.01, 10.0, 0.5)
hslider bounds(243, 34, 86, 20), channel("scale2_{pname}"), range(-1.0, 1.0, 0, 1, 0.001)
button bounds(330, 34, 29, 19), channel("scale2_x_{pname}"), text("x 1","x 10"), 
hslider bounds(359, 34, 86, 20), channel("curve2_{pname}"), range(-5.0, 5.0, 0)

label bounds(10, 58, 100, 12), text("source"), colour(20,20,20,255)
label bounds(113, 58, 50, 12), text("chan"), colour(20,20,20,255)
label bounds(166, 58, 76, 12), text("rise/fall"), colour(20,20,20,255)
label bounds(246, 58, 110, 12), text("scale"), colour(20,20,20,255)
label bounds(362, 58, 81, 12), text("curve"), colour(20,20,20,255)

rslider bounds(443, 12, 62, 62), text("offset"), channel("offset_{pname}"), range({p_min}, {p_max}, {p_default}, {p_skew}, {p_incr}) 
combobox bounds(443, 1, 55, 12), channel("offsetx_{pname}"), items("-1", "Nornm", "+1"), , value(2), channeltype("string")

rslider bounds(504, 8, 66, 66), text("{pname}"), channel("{pname}"), range({p_min}, {p_max}, {p_default}, {p_skew}, {p_incr})

label bounds(580, 8, 55, 12), text("midi"), colour(20,20,20,255)
checkbox bounds(642, 8, 12, 12), text("enable"), channel("enable_{pname}"), value(1)
numberbox bounds(580, 25, 55, 15), channel("midich_{pname}"), range(1, 16, 1)
numberbox bounds(580, 42, 55, 15), channel("ctrlnum_{pname}"), range(1, 127, 1)
label bounds(642, 25, 70, 12), text("channel"), colour(20,20,20,255)
label bounds(642, 42, 70, 12), text("ctrl"), colour(20,20,20,255)

}}

'''

if effectname == 'vst_MIDIator': plant = plantMIDI

guifile = open(effectname+'_gui_scratchpad.inc', 'w')
x_pos = start_x_pos
x_pos1 = start_x_pos
y_pos = start_y_pos
for i in range(len(parameters)):
    parm = parameters[i]
    guifile.write(plant.format(start_x=x_pos, start_y=y_pos, pname=parm[0], analysis_p=analysis_parms_string, p_min=parm[1][0], p_max=parm[1][1], p_default=parm[1][2], p_skew=parm[1][3], p_incr=parm[1][4]))
    x_pos+=plant_height
guifile.write(';next x position available below plants is {}'.format(max([x_pos,x_pos1])))
guifile.close()

processor_csd_file = open('vst_mediator_interprocess.csd', 'r')
processor_csd_txt = processor_csd_file.read()
#updated_txt = re.sub('; >>>>>>> AUTOGENERATED TEXT: DO NOT EDIT THE FOLLOWING LINES >>>>>>>(.*?); <<<<<<< AUTOGENERATED TEXT: DO NOT EDIT THE PRECEDING LINES <<<<<<<',
#                     '', processor_csd_txt, flags=re.DOTALL)
processor_csd_file.close()
processor_csd_file = open('vst_mediator_interprocess.csd', 'w')
updated_txt = 'Hello'
print '***'
print updated_txt
processor_csd_file.write(updated_txt)
processor_csd_file.close()
