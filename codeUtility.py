#!/usr/bin/python
# -*- coding: latin-1 -*-

#    Copyright 2015 Oeyvind Brandtsegg 
#
#    This file is part of the Signal Interaction Toolkit
#
#    The Signal Interaction Toolkit is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License version 3 
#    as published by the Free Software Foundation.
#
#    The Signal Interaction Toolkit is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with The Signal Interaction Toolkit.  
#    If not, see <http://www.gnu.org/licenses/>.

import sys

if sys.argv[1] == 'template':
    effectname = 'template'
    parameters = [('Vol', (0.0, 1.0, 0.5, 0.25, 0.00001))] 
                # pName, (min, max, default, skew, increment)
                # where skew is a dynamic adjustment of exp/lin/log translation if the GUI widget
                # and increment is the smallest change allowed by the GUI widget

if sys.argv[1] == 'stereopan':
    effectname = 'stereopan'
    parameters = [('Pan', (0.0, 1.0, 0.5, 1, 0.001)),
                  ('Mix', (0.0, 1.0, 0.5, 1, 0.001))] 

if sys.argv[1] == 'tremolam':
    effectname = 'tremolam'
    parameters = [('Depth', (0.0, 1.0, 0.5, 0.25, 0.001)),
                  ('RateLow', (0.0, 10.0, 0.5, 0.25, 0.001)),
                  ('RateHigh', (0.0, 500.0, 0.5, 0.25, 0.001))] 

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

if sys.argv[1] == 'stereodelay':
    effectname = 'stereodelay'
    parameters = [('delaytime', (0.0008, 2.0, 0.5, 0.25, 0.00001)), 
                  ('filt_fq', (100, 10000, 1000, 0.35, 1)),
                  ('feedback', (0.0, 0.9999, 0.3, 1.9, 0.0001))
                  ]
              
if sys.argv[1] == 'pluck':
    effectname = 'pluck'
    parameters = [('inlevel', (0, 1.0, 1, 0.3, 0.01)), 
                  ('freq', (1, 1450, 400, 0.3, 0.01)), 
                  ('filt_fq', (1000, 16000, 7000, 0.35, 1)),
                  ('feedback', (0.8, 0.9999, 0.95, 1.9, 0.0001)),
                  ('mix', (0, 1.0, 1, 0.3, 0.01))
                  ]

if sys.argv[1] == 'lpf18dist':
    effectname = 'lpf18dist'
    parameters = [('Drive', (1, 12, 2, 1, 0.1)), 
                  ('Freq', (20, 10000, 3000, 0.35, 1)),
                  ('Resonance', (0.001, 0.95, 0.3, 1, 0.001)),
                  ('Dist', (0.001, 10, 0.2, 0.5, 0.001)),
                  ('Mix', (0.0, 1.0, 1.0, 1, 0.01)),
                  ]

if sys.argv[1] == 'screverb':
    effectname = 'screverb'
    parameters = [('InLevel', (0, 1.0, 0.2, 0.3, 0.01)), 
                  ('Feed', (0.0, 1.0, 0.85, 1.2, 0.01)), 
                  ('FiltFq', (100, 14000, 7000, 0.6, 1)), 
                  ('PitchMod', (0.0, 4.0, 0.9, 1, 0.01)), 
                  ('PreDly', (0.0, 500, 120, 1, 1)), 
                  ('LfRoll', (20, 500, 90, 1, 1)), 
                  ('Mix', (0.0, 1.0, 1.0, 1, 0.01))
                  ]

if sys.argv[1] == 'freeverb':
    effectname = 'freeverb'
    parameters = [('inlevel', (0, 1.0, 1.0, 0.3, 0.01)), 
                  ('reverbtime', (0.0, 8.0, 1.5, 0.4, 0.01)), 
                  ('reverbdamp', (0.0, 1.0, 0.25, 0.6, 0.01)), 
                  ('reverbmix', (0.0, 1.0, 0.7, 1, 0.01))
                  ]

if sys.argv[1] == 'mincertime':
    effectname = 'mincertime'
    parameters = [('inlevel', (0, 1.0, 1, 0.3, 0.01)), 
                  ('timpoint', (0, 0.99, 0.1, 0.4, 0.001)), 
                  ('pitch', (0.0, 2.0, 1.0, 1, 0.01)),
                  ('feedback', (0.0, 1.0, 0.0, 1, 0.01)),
                  ('mix', (0, 1.0, 1, 0.3, 0.01))
                  ]

if sys.argv[1] == 'plucktremlpfverb':
    effectname = 'plucktremlpfverb'
    parameters = [('inlevel', (0, 1.0, 1, 0.3, 0.01)), 
                  ('pluckfreq', (1, 1450, 400, 0.3, 0.01)), 
                  ('pluckfilt', (1000, 16000, 7000, 0.35, 1)),
                  ('pluckfeed', (0.8, 0.9999, 0.95, 1.9, 0.0001)),
                  ('pluckmix', (0, 1.0, 1, 0.3, 0.01)),
                  ('tremDepth', (0.0, 1.0, 0.5, 0.25, 0.001)),
                  ('tRateLow', (0.0, 10.0, 0.5, 0.25, 0.001)),
                  ('tRateHigh', (0.0, 500.0, 0.5, 0.25, 0.001)),  
                  ('lpfDrive', (1, 12, 2, 1, 0.1)), 
                  ('lpfFreq', (20, 10000, 3000, 0.35, 1)),
                  ('lpfResonance', (0.001, 0.95, 0.3, 1, 0.001)),
                  ('lpfDist', (0.001, 10, 0.2, 0.5, 0.001)),
                  ('lpfMix', (0.0, 1.0, 1.0, 1, 0.01)),
                  ('reverbtime', (0.0, 8.0, 1.5, 0.4, 0.01)), 
                  ('reverbdamp', (0.0, 1.0, 0.25, 0.6, 0.01)), 
                  ('reverbmix', (0.0, 1.0, 0.7, 1, 0.01))
                  ]

if sys.argv[1] == 'mincerpanverb':
    effectname = 'mincerpanverb'
    parameters = [('inlevel', (0, 1.0, 1, 0.3, 0.01)), 
                  ('mincertime', (0, 0.99, 0.1, 0.4, 0.001)), 
                  ('mincerpitch', (0.0, 2.0, 1.0, 1, 0.01)),
                  ('mincerfeed', (0.0, 1.0, 0.0, 1, 0.01)),
                  ('mincermix', (0, 1.0, 1, 0.3, 0.01)),
                  ('Pan', (0.0, 1.0, 0.5, 1, 0.001)),
                  ('panMix', (0.0, 1.0, 0.5, 1, 0.001)),
                  ('reverbtime', (0.0, 8.0, 1.5, 0.4, 0.01)), 
                  ('reverbdamp', (0.0, 1.0, 0.25, 0.6, 0.01)), 
                  ('reverbmix', (0.0, 1.0, 0.7, 1, 0.01))
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
analysis_parms = '"rms", "rms_preEq", "cps", "pitch", "centroid", "spread", "skewness", "kurtosis", "flatness", "crest", "flux", "amp_trans", "amp_t_dens", "centr_trans", "centr_t_dens", "kurt_trans", "pitchup_trans", "pitchdown_trans", "cps_raw"'


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
    if (effectname == 'plucktremlpfverb') and (parm[0] == 'lpfDrive'): 
        x_pos1 = x_pos
        x_pos = start_x_pos
        y_pos = 575
    guifile.write(plant.format(start_x=x_pos, start_y=y_pos, pname=parm[0], analysis_p=analysis_parms,p_min=parm[1][0], p_max=parm[1][1], p_default=parm[1][2], p_skew=parm[1][3], p_incr=parm[1][4]))
    x_pos+=plant_height
guifile.write(';next x position available below plants is {}'.format(max([x_pos,x_pos1])))