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


import sys


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
start_x_pos = 30
start_y_pos = 5
plant_height = 85
analysis_parms = '''"rms", "rms_preEq", "cps", "pitch", "centroid", "spread", "skewness", "kurtosis", "flatness", "crest", "flux", 
                    "amp_trans", "amp_t_dens", "rhythm_cons", "rhythm_ratio1", "rhythm_ratio2", "rhythm_ac", "rhythm_ac1", "rhythm_ac2",  
                     "rhythm_ac1t", "rhythm_ac2t", "mfcc1", "mfcc2", "mfcc3", "mfcc4", "cps_raw"'''


plant = '''groupbox bounds({start_y}, {start_x}, 564, 81), plant("plant_{pname}"), linethickness("0"){{ 
combobox channel("source1_{pname}"), bounds(10, 12, 90, 20), items({analysis_p}), value(1), channeltype("string")
combobox channel("chan1_{pname}"), bounds(103, 12, 50, 20), items("1", "2", "3", "4"), value(1)
numberbox bounds(158, 14, 35, 15), channel("rise1_{pname}"), range(0.01, 10.0, 0.01)
numberbox bounds(196, 14, 35, 15), channel("fall1_{pname}"), range(0.01, 10.0, 0.5)
hslider bounds(233, 12, 86, 20), channel("scale1_{pname}"), range(-1.0, 1.0, 0, 1, 0.01)
button bounds(320, 12, 29, 19), channel("scale1_x_{pname}"), text("x 1","x 10"), 
hslider bounds(349, 12, 86, 20), channel("curve1_{pname}"), range(-5.0, 5.0, 0)

combobox channel("source2_{pname}"), bounds(10, 34, 90, 20), items({analysis_p}), value(1), channeltype("string")
combobox channel("chan2_{pname}"), bounds(103, 34, 50, 20), items("1", "2", "3", "4"), value(1)
numberbox bounds(158, 36, 35, 15), channel("rise2_{pname}"), range(0.01, 10.0, 0.01)
numberbox bounds(196, 36, 35, 15), channel("fall2_{pname}"), range(0.01, 10.0, 0.5)
hslider bounds(233, 34, 86, 20), channel("scale2_{pname}"), range(-1.0, 1.0, 0, 1, 0.01)
button bounds(320, 34, 29, 19), channel("scale2_x_{pname}"), text("x 1","x 10"), 
hslider bounds(349, 34, 86, 20), channel("curve2_{pname}"), range(-5.0, 5.0, 0)

label bounds(10, 58, 90, 12), text("source"), colour(20,20,20,255)
label bounds(103, 58, 50, 12), text("chan"), colour(20,20,20,255)
label bounds(156, 58, 76, 12), text("rise/fall"), colour(20,20,20,255)
label bounds(236, 58, 110, 12), text("scale"), colour(20,20,20,255)
label bounds(352, 58, 81, 12), text("curve"), colour(20,20,20,255)

rslider bounds(433, 12, 62, 62), text("offset"), channel("offset_{pname}"), range({p_min}, {p_max}, {p_default}, {p_skew}, {p_incr}) 
combobox bounds(433, 1, 55, 12), channel("offsetx_{pname}"), items("-1", "Nornm", "+1"), , value(2), channeltype("string")

rslider bounds(494, 8, 66, 66), text("{pname}"), channel("{pname}"), range({p_min}, {p_max}, {p_default}, {p_skew}, {p_incr})
}}

'''
plantMIDI = '''groupbox bounds({start_y}, {start_x}, 710, 81), plant("plant_{pname}"), linethickness("0"){{ 
combobox channel("source1_{pname}"), bounds(10, 12, 90, 20), items({analysis_p}), value(1), channeltype("string")
combobox channel("chan1_{pname}"), bounds(103, 12, 50, 20), items("1", "2", "3", "4"), value(1)
numberbox bounds(158, 14, 35, 15), channel("rise1_{pname}"), range(0.01, 10.0, 0.01)
numberbox bounds(196, 14, 35, 15), channel("fall1_{pname}"), range(0.01, 10.0, 0.5)
hslider bounds(233, 12, 86, 20), channel("scale1_{pname}"), range(-1.0, 1.0, 0, 1, 0.01)
button bounds(320, 12, 29, 19), channel("scale1_x_{pname}"), text("x 1","x 10"), 
hslider bounds(349, 12, 86, 20), channel("curve1_{pname}"), range(-5.0, 5.0, 0)

combobox channel("source2_{pname}"), bounds(10, 34, 90, 20), items({analysis_p}), value(1), channeltype("string")
combobox channel("chan2_{pname}"), bounds(103, 34, 50, 20), items("1", "2", "3", "4"), value(1)
numberbox bounds(158, 36, 35, 15), channel("rise2_{pname}"), range(0.01, 10.0, 0.01)
numberbox bounds(196, 36, 35, 15), channel("fall2_{pname}"), range(0.01, 10.0, 0.5)
hslider bounds(233, 34, 86, 20), channel("scale2_{pname}"), range(-1.0, 1.0, 0, 1, 0.01)
button bounds(320, 34, 29, 19), channel("scale2_x_{pname}"), text("x 1","x 10"), 
hslider bounds(349, 34, 86, 20), channel("curve2_{pname}"), range(-5.0, 5.0, 0)

label bounds(10, 58, 90, 12), text("source"), colour(20,20,20,255)
label bounds(103, 58, 50, 12), text("chan"), colour(20,20,20,255)
label bounds(156, 58, 76, 12), text("rise/fall"), colour(20,20,20,255)
label bounds(236, 58, 110, 12), text("scale"), colour(20,20,20,255)
label bounds(352, 58, 81, 12), text("curve"), colour(20,20,20,255)

rslider bounds(433, 12, 62, 62), text("offset"), channel("offset_{pname}"), range({p_min}, {p_max}, {p_default}, {p_skew}, {p_incr}) 
combobox bounds(433, 1, 55, 12), channel("offsetx_{pname}"), items("-1", "Nornm", "+1"), , value(2), channeltype("string")

rslider bounds(494, 8, 66, 66), text("{pname}"), channel("{pname}"), range({p_min}, {p_max}, {p_default}, {p_skew}, {p_incr})

label bounds(570, 8, 55, 12), text("midi"), colour(20,20,20,255)
checkbox bounds(632, 8, 12, 12), text("enable"), channel("enable_{pname}"), value(1)
numberbox bounds(570, 25, 55, 15), channel("midich_{pname}"), range(1, 16, 1)
numberbox bounds(570, 42, 55, 15), channel("ctrlnum_{pname}"), range(1, 127, 1)
label bounds(632, 25, 70, 12), text("channel"), colour(20,20,20,255)
label bounds(632, 42, 70, 12), text("ctrl"), colour(20,20,20,255)

}}

'''

if effectname == 'vst_MIDIator': plant = plantMIDI

guifile = open(effectname+'_gui_scratchpad.inc', 'w')
x_pos = start_x_pos
x_pos1 = start_x_pos
y_pos = start_y_pos
for i in range(len(parameters)):
    parm = parameters[i]
    guifile.write(plant.format(start_x=x_pos, start_y=y_pos, pname=parm[0], analysis_p=analysis_parms,p_min=parm[1][0], p_max=parm[1][1], p_default=parm[1][2], p_skew=parm[1][3], p_incr=parm[1][4]))
    x_pos+=plant_height
guifile.write(';next x position available below plants is {}'.format(max([x_pos,x_pos1])))