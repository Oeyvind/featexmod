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

<Cabbage>
form size(720, 1010), caption("VST_MIDIator"), pluginID("vmid")
;image bounds(0, 0, 575, 710), file("background.jpg"), shape("round")
label text("port"), bounds(10, 10, 80, 12), identchannel("port_id")
;combobox channel("presets"), bounds(250, 6, 90, 20), populate("*.snaps")
;filebutton bounds(350, 6, 90, 20), channel("presetsave"), text("Save"), populate("*.snaps")

groupbox bounds(5, 30, 710, 81), plant("plant_parm1"), linethickness("0"){ 
combobox channel("source1_parm1"), bounds(10, 12, 90, 20), items("rms", "rms_preEq", "cps", "pitch", "centroid", "spread", "skewness", "kurtosis", "flatness", "crest", "flux", "amp_trans", "amp_t_dens", "centr_trans", "centr_t_dens", "kurt_trans", "pitchup_trans", "pitchdown_trans", "cps_raw"), value(1), channeltype("string")
combobox channel("chan1_parm1"), bounds(103, 12, 50, 20), items("1", "2", "3", "4"), value(1)
numberbox bounds(158, 14, 35, 15), channel("rise1_parm1"), range(0.01, 30.0, 0.01)
numberbox bounds(196, 14, 35, 15), channel("fall1_parm1"), range(0.01, 30.0, 0.5)
hslider bounds(233, 12, 86, 20), channel("scale1_parm1"), range(-1.0, 1.0, 0, 1, 0.01)
button bounds(320, 12, 29, 19), channel("scale1_x_parm1"), text("x 1","x 10"), 
hslider bounds(349, 12, 86, 20), channel("curve1_parm1"), range(-5.0, 5.0, 0)

combobox channel("source2_parm1"), bounds(10, 34, 90, 20), items("rms", "rms_preEq", "cps", "pitch", "centroid", "spread", "skewness", "kurtosis", "flatness", "crest", "flux", "amp_trans", "amp_t_dens", "centr_trans", "centr_t_dens", "kurt_trans", "pitchup_trans", "pitchdown_trans", "cps_raw"), value(1), channeltype("string")
combobox channel("chan2_parm1"), bounds(103, 34, 50, 20), items("1", "2", "3", "4"), value(1)
numberbox bounds(158, 36, 35, 15), channel("rise2_parm1"), range(0.01, 30.0, 0.01)
numberbox bounds(196, 36, 35, 15), channel("fall2_parm1"), range(0.01, 30.0, 0.5)
hslider bounds(233, 34, 86, 20), channel("scale2_parm1"), range(-1.0, 1.0, 0, 1, 0.01)
button bounds(320, 34, 29, 19), channel("scale2_x_parm1"), text("x 1","x 10"), 
hslider bounds(349, 34, 86, 20), channel("curve2_parm1"), range(-5.0, 5.0, 0)

label bounds(10, 58, 90, 12), text("source"), colour(20,20,20,255)
label bounds(103, 58, 50, 12), text("chan"), colour(20,20,20,255)
label bounds(156, 58, 76, 12), text("rise/fall"), colour(20,20,20,255)
label bounds(236, 58, 110, 12), text("scale"), colour(20,20,20,255)
label bounds(352, 58, 81, 12), text("curve"), colour(20,20,20,255)

rslider bounds(433, 12, 62, 62), text("offset"), channel("offset_parm1"), range(0.0, 1.0, 0.5, 1, 0.001) 
combobox bounds(433, 1, 55, 12), channel("offsetx_parm1"), items("-1", "Nornm", "+1"), , value(2), channeltype("string")

rslider bounds(494, 8, 66, 66), text("parm1"), channel("parm1"), range(0.0, 1.0, 0.5, 1, 0.001)

label bounds(570, 8, 55, 12), text("midi"), colour(20,20,20,255)
checkbox bounds(632, 8, 12, 12), text("enable"), channel("enable_parm1"), value(1)
numberbox bounds(570, 25, 55, 15), channel("midich_parm1"), range(1, 16, 1)
numberbox bounds(570, 42, 55, 15), channel("ctrlnum_parm1"), range(1, 127, 1)
label bounds(632, 25, 70, 12), text("channel"), colour(20,20,20,255)
label bounds(632, 42, 70, 12), text("ctrl"), colour(20,20,20,255)

}

groupbox bounds(5, 115, 710, 81), plant("plant_parm2"), linethickness("0"){ 
combobox channel("source1_parm2"), bounds(10, 12, 90, 20), items("rms", "rms_preEq", "cps", "pitch", "centroid", "spread", "skewness", "kurtosis", "flatness", "crest", "flux", "amp_trans", "amp_t_dens", "centr_trans", "centr_t_dens", "kurt_trans", "pitchup_trans", "pitchdown_trans", "cps_raw"), value(1), channeltype("string")
combobox channel("chan1_parm2"), bounds(103, 12, 50, 20), items("1", "2", "3", "4"), value(1)
numberbox bounds(158, 14, 35, 15), channel("rise1_parm2"), range(0.01, 30.0, 0.01)
numberbox bounds(196, 14, 35, 15), channel("fall1_parm2"), range(0.01, 30.0, 0.5)
hslider bounds(233, 12, 86, 20), channel("scale1_parm2"), range(-1.0, 1.0, 0, 1, 0.01)
button bounds(320, 12, 29, 19), channel("scale1_x_parm2"), text("x 1","x 10"), 
hslider bounds(349, 12, 86, 20), channel("curve1_parm2"), range(-5.0, 5.0, 0)

combobox channel("source2_parm2"), bounds(10, 34, 90, 20), items("rms", "rms_preEq", "cps", "pitch", "centroid", "spread", "skewness", "kurtosis", "flatness", "crest", "flux", "amp_trans", "amp_t_dens", "centr_trans", "centr_t_dens", "kurt_trans", "pitchup_trans", "pitchdown_trans", "cps_raw"), value(1), channeltype("string")
combobox channel("chan2_parm2"), bounds(103, 34, 50, 20), items("1", "2", "3", "4"), value(1)
numberbox bounds(158, 36, 35, 15), channel("rise2_parm2"), range(0.01, 30.0, 0.01)
numberbox bounds(196, 36, 35, 15), channel("fall2_parm2"), range(0.01, 30.0, 0.5)
hslider bounds(233, 34, 86, 20), channel("scale2_parm2"), range(-1.0, 1.0, 0, 1, 0.01)
button bounds(320, 34, 29, 19), channel("scale2_x_parm2"), text("x 1","x 10"), 
hslider bounds(349, 34, 86, 20), channel("curve2_parm2"), range(-5.0, 5.0, 0)

label bounds(10, 58, 90, 12), text("source"), colour(20,20,20,255)
label bounds(103, 58, 50, 12), text("chan"), colour(20,20,20,255)
label bounds(156, 58, 76, 12), text("rise/fall"), colour(20,20,20,255)
label bounds(236, 58, 110, 12), text("scale"), colour(20,20,20,255)
label bounds(352, 58, 81, 12), text("curve"), colour(20,20,20,255)

rslider bounds(433, 12, 62, 62), text("offset"), channel("offset_parm2"), range(0.0, 1.0, 0.5, 1, 0.001) 
combobox bounds(433, 1, 55, 12), channel("offsetx_parm2"), items("-1", "Nornm", "+1"), , value(2), channeltype("string")

rslider bounds(494, 8, 66, 66), text("parm2"), channel("parm2"), range(0.0, 1.0, 0.5, 1, 0.001)

label bounds(570, 8, 55, 12), text("midi"), colour(20,20,20,255)
checkbox bounds(632, 8, 12, 12), text("enable"), channel("enable_parm2"), value(1)
numberbox bounds(570, 25, 55, 15), channel("midich_parm2"), range(1, 16, 1)
numberbox bounds(570, 42, 55, 15), channel("ctrlnum_parm2"), range(1, 127, 1)
label bounds(632, 25, 70, 12), text("channel"), colour(20,20,20,255)
label bounds(632, 42, 70, 12), text("ctrl"), colour(20,20,20,255)

}

groupbox bounds(5, 200, 710, 81), plant("plant_parm3"), linethickness("0"){ 
combobox channel("source1_parm3"), bounds(10, 12, 90, 20), items("rms", "rms_preEq", "cps", "pitch", "centroid", "spread", "skewness", "kurtosis", "flatness", "crest", "flux", "amp_trans", "amp_t_dens", "centr_trans", "centr_t_dens", "kurt_trans", "pitchup_trans", "pitchdown_trans", "cps_raw"), value(1), channeltype("string")
combobox channel("chan1_parm3"), bounds(103, 12, 50, 20), items("1", "2", "3", "4"), value(1)
numberbox bounds(158, 14, 35, 15), channel("rise1_parm3"), range(0.01, 30.0, 0.01)
numberbox bounds(196, 14, 35, 15), channel("fall1_parm3"), range(0.01, 30.0, 0.5)
hslider bounds(233, 12, 86, 20), channel("scale1_parm3"), range(-1.0, 1.0, 0, 1, 0.01)
button bounds(320, 12, 29, 19), channel("scale1_x_parm3"), text("x 1","x 10"), 
hslider bounds(349, 12, 86, 20), channel("curve1_parm3"), range(-5.0, 5.0, 0)

combobox channel("source2_parm3"), bounds(10, 34, 90, 20), items("rms", "rms_preEq", "cps", "pitch", "centroid", "spread", "skewness", "kurtosis", "flatness", "crest", "flux", "amp_trans", "amp_t_dens", "centr_trans", "centr_t_dens", "kurt_trans", "pitchup_trans", "pitchdown_trans", "cps_raw"), value(1), channeltype("string")
combobox channel("chan2_parm3"), bounds(103, 34, 50, 20), items("1", "2", "3", "4"), value(1)
numberbox bounds(158, 36, 35, 15), channel("rise2_parm3"), range(0.01, 30.0, 0.01)
numberbox bounds(196, 36, 35, 15), channel("fall2_parm3"), range(0.01, 30.0, 0.5)
hslider bounds(233, 34, 86, 20), channel("scale2_parm3"), range(-1.0, 1.0, 0, 1, 0.01)
button bounds(320, 34, 29, 19), channel("scale2_x_parm3"), text("x 1","x 10"), 
hslider bounds(349, 34, 86, 20), channel("curve2_parm3"), range(-5.0, 5.0, 0)

label bounds(10, 58, 90, 12), text("source"), colour(20,20,20,255)
label bounds(103, 58, 50, 12), text("chan"), colour(20,20,20,255)
label bounds(156, 58, 76, 12), text("rise/fall"), colour(20,20,20,255)
label bounds(236, 58, 110, 12), text("scale"), colour(20,20,20,255)
label bounds(352, 58, 81, 12), text("curve"), colour(20,20,20,255)

rslider bounds(433, 12, 62, 62), text("offset"), channel("offset_parm3"), range(0.0, 1.0, 0.5, 1, 0.001) 
combobox bounds(433, 1, 55, 12), channel("offsetx_parm3"), items("-1", "Nornm", "+1"), , value(2), channeltype("string")

rslider bounds(494, 8, 66, 66), text("parm3"), channel("parm3"), range(0.0, 1.0, 0.5, 1, 0.001)

label bounds(570, 8, 55, 12), text("midi"), colour(20,20,20,255)
checkbox bounds(632, 8, 12, 12), text("enable"), channel("enable_parm3"), value(1)
numberbox bounds(570, 25, 55, 15), channel("midich_parm3"), range(1, 16, 1)
numberbox bounds(570, 42, 55, 15), channel("ctrlnum_parm3"), range(1, 127, 1)
label bounds(632, 25, 70, 12), text("channel"), colour(20,20,20,255)
label bounds(632, 42, 70, 12), text("ctrl"), colour(20,20,20,255)

}

groupbox bounds(5, 285, 710, 81), plant("plant_parm4"), linethickness("0"){ 
combobox channel("source1_parm4"), bounds(10, 12, 90, 20), items("rms", "rms_preEq", "cps", "pitch", "centroid", "spread", "skewness", "kurtosis", "flatness", "crest", "flux", "amp_trans", "amp_t_dens", "centr_trans", "centr_t_dens", "kurt_trans", "pitchup_trans", "pitchdown_trans", "cps_raw"), value(1), channeltype("string")
combobox channel("chan1_parm4"), bounds(103, 12, 50, 20), items("1", "2", "3", "4"), value(1)
numberbox bounds(158, 14, 35, 15), channel("rise1_parm4"), range(0.01, 30.0, 0.01)
numberbox bounds(196, 14, 35, 15), channel("fall1_parm4"), range(0.01, 30.0, 0.5)
hslider bounds(233, 12, 86, 20), channel("scale1_parm4"), range(-1.0, 1.0, 0, 1, 0.01)
button bounds(320, 12, 29, 19), channel("scale1_x_parm4"), text("x 1","x 10"), 
hslider bounds(349, 12, 86, 20), channel("curve1_parm4"), range(-5.0, 5.0, 0)

combobox channel("source2_parm4"), bounds(10, 34, 90, 20), items("rms", "rms_preEq", "cps", "pitch", "centroid", "spread", "skewness", "kurtosis", "flatness", "crest", "flux", "amp_trans", "amp_t_dens", "centr_trans", "centr_t_dens", "kurt_trans", "pitchup_trans", "pitchdown_trans", "cps_raw"), value(1), channeltype("string")
combobox channel("chan2_parm4"), bounds(103, 34, 50, 20), items("1", "2", "3", "4"), value(1)
numberbox bounds(158, 36, 35, 15), channel("rise2_parm4"), range(0.01, 30.0, 0.01)
numberbox bounds(196, 36, 35, 15), channel("fall2_parm4"), range(0.01, 30.0, 0.5)
hslider bounds(233, 34, 86, 20), channel("scale2_parm4"), range(-1.0, 1.0, 0, 1, 0.01)
button bounds(320, 34, 29, 19), channel("scale2_x_parm4"), text("x 1","x 10"), 
hslider bounds(349, 34, 86, 20), channel("curve2_parm4"), range(-5.0, 5.0, 0)

label bounds(10, 58, 90, 12), text("source"), colour(20,20,20,255)
label bounds(103, 58, 50, 12), text("chan"), colour(20,20,20,255)
label bounds(156, 58, 76, 12), text("rise/fall"), colour(20,20,20,255)
label bounds(236, 58, 110, 12), text("scale"), colour(20,20,20,255)
label bounds(352, 58, 81, 12), text("curve"), colour(20,20,20,255)

rslider bounds(433, 12, 62, 62), text("offset"), channel("offset_parm4"), range(0.0, 1.0, 0.5, 1, 0.001) 
combobox bounds(433, 1, 55, 12), channel("offsetx_parm4"), items("-1", "Nornm", "+1"), , value(2), channeltype("string")

rslider bounds(494, 8, 66, 66), text("parm4"), channel("parm4"), range(0.0, 1.0, 0.5, 1, 0.001)

label bounds(570, 8, 55, 12), text("midi"), colour(20,20,20,255)
checkbox bounds(632, 8, 12, 12), text("enable"), channel("enable_parm4"), value(1)
numberbox bounds(570, 25, 55, 15), channel("midich_parm4"), range(1, 16, 1)
numberbox bounds(570, 42, 55, 15), channel("ctrlnum_parm4"), range(1, 127, 1)
label bounds(632, 25, 70, 12), text("channel"), colour(20,20,20,255)
label bounds(632, 42, 70, 12), text("ctrl"), colour(20,20,20,255)

}

groupbox bounds(5, 370, 710, 81), plant("plant_parm5"), linethickness("0"){ 
combobox channel("source1_parm5"), bounds(10, 12, 90, 20), items("rms", "rms_preEq", "cps", "pitch", "centroid", "spread", "skewness", "kurtosis", "flatness", "crest", "flux", "amp_trans", "amp_t_dens", "centr_trans", "centr_t_dens", "kurt_trans", "pitchup_trans", "pitchdown_trans", "cps_raw"), value(1), channeltype("string")
combobox channel("chan1_parm5"), bounds(103, 12, 50, 20), items("1", "2", "3", "4"), value(1)
numberbox bounds(158, 14, 35, 15), channel("rise1_parm5"), range(0.01, 30.0, 0.01)
numberbox bounds(196, 14, 35, 15), channel("fall1_parm5"), range(0.01, 30.0, 0.5)
hslider bounds(233, 12, 86, 20), channel("scale1_parm5"), range(-1.0, 1.0, 0, 1, 0.01)
button bounds(320, 12, 29, 19), channel("scale1_x_parm5"), text("x 1","x 10"), 
hslider bounds(349, 12, 86, 20), channel("curve1_parm5"), range(-5.0, 5.0, 0)

combobox channel("source2_parm5"), bounds(10, 34, 90, 20), items("rms", "rms_preEq", "cps", "pitch", "centroid", "spread", "skewness", "kurtosis", "flatness", "crest", "flux", "amp_trans", "amp_t_dens", "centr_trans", "centr_t_dens", "kurt_trans", "pitchup_trans", "pitchdown_trans", "cps_raw"), value(1), channeltype("string")
combobox channel("chan2_parm5"), bounds(103, 34, 50, 20), items("1", "2", "3", "4"), value(1)
numberbox bounds(158, 36, 35, 15), channel("rise2_parm5"), range(0.01, 30.0, 0.01)
numberbox bounds(196, 36, 35, 15), channel("fall2_parm5"), range(0.01, 30.0, 0.5)
hslider bounds(233, 34, 86, 20), channel("scale2_parm5"), range(-1.0, 1.0, 0, 1, 0.01)
button bounds(320, 34, 29, 19), channel("scale2_x_parm5"), text("x 1","x 10"), 
hslider bounds(349, 34, 86, 20), channel("curve2_parm5"), range(-5.0, 5.0, 0)

label bounds(10, 58, 90, 12), text("source"), colour(20,20,20,255)
label bounds(103, 58, 50, 12), text("chan"), colour(20,20,20,255)
label bounds(156, 58, 76, 12), text("rise/fall"), colour(20,20,20,255)
label bounds(236, 58, 110, 12), text("scale"), colour(20,20,20,255)
label bounds(352, 58, 81, 12), text("curve"), colour(20,20,20,255)

rslider bounds(433, 12, 62, 62), text("offset"), channel("offset_parm5"), range(0.0, 1.0, 0.5, 1, 0.001) 
combobox bounds(433, 1, 55, 12), channel("offsetx_parm5"), items("-1", "Nornm", "+1"), , value(2), channeltype("string")

rslider bounds(494, 8, 66, 66), text("parm5"), channel("parm5"), range(0.0, 1.0, 0.5, 1, 0.001)

label bounds(570, 8, 55, 12), text("midi"), colour(20,20,20,255)
checkbox bounds(632, 8, 12, 12), text("enable"), channel("enable_parm5"), value(1)
numberbox bounds(570, 25, 55, 15), channel("midich_parm5"), range(1, 16, 1)
numberbox bounds(570, 42, 55, 15), channel("ctrlnum_parm5"), range(1, 127, 1)
label bounds(632, 25, 70, 12), text("channel"), colour(20,20,20,255)
label bounds(632, 42, 70, 12), text("ctrl"), colour(20,20,20,255)

}

groupbox bounds(5, 455, 710, 81), plant("plant_parm6"), linethickness("0"){ 
combobox channel("source1_parm6"), bounds(10, 12, 90, 20), items("rms", "rms_preEq", "cps", "pitch", "centroid", "spread", "skewness", "kurtosis", "flatness", "crest", "flux", "amp_trans", "amp_t_dens", "centr_trans", "centr_t_dens", "kurt_trans", "pitchup_trans", "pitchdown_trans", "cps_raw"), value(1), channeltype("string")
combobox channel("chan1_parm6"), bounds(103, 12, 50, 20), items("1", "2", "3", "4"), value(1)
numberbox bounds(158, 14, 35, 15), channel("rise1_parm6"), range(0.01, 30.0, 0.01)
numberbox bounds(196, 14, 35, 15), channel("fall1_parm6"), range(0.01, 30.0, 0.5)
hslider bounds(233, 12, 86, 20), channel("scale1_parm6"), range(-1.0, 1.0, 0, 1, 0.01)
button bounds(320, 12, 29, 19), channel("scale1_x_parm6"), text("x 1","x 10"), 
hslider bounds(349, 12, 86, 20), channel("curve1_parm6"), range(-5.0, 5.0, 0)

combobox channel("source2_parm6"), bounds(10, 34, 90, 20), items("rms", "rms_preEq", "cps", "pitch", "centroid", "spread", "skewness", "kurtosis", "flatness", "crest", "flux", "amp_trans", "amp_t_dens", "centr_trans", "centr_t_dens", "kurt_trans", "pitchup_trans", "pitchdown_trans", "cps_raw"), value(1), channeltype("string")
combobox channel("chan2_parm6"), bounds(103, 34, 50, 20), items("1", "2", "3", "4"), value(1)
numberbox bounds(158, 36, 35, 15), channel("rise2_parm6"), range(0.01, 30.0, 0.01)
numberbox bounds(196, 36, 35, 15), channel("fall2_parm6"), range(0.01, 30.0, 0.5)
hslider bounds(233, 34, 86, 20), channel("scale2_parm6"), range(-1.0, 1.0, 0, 1, 0.01)
button bounds(320, 34, 29, 19), channel("scale2_x_parm6"), text("x 1","x 10"), 
hslider bounds(349, 34, 86, 20), channel("curve2_parm6"), range(-5.0, 5.0, 0)

label bounds(10, 58, 90, 12), text("source"), colour(20,20,20,255)
label bounds(103, 58, 50, 12), text("chan"), colour(20,20,20,255)
label bounds(156, 58, 76, 12), text("rise/fall"), colour(20,20,20,255)
label bounds(236, 58, 110, 12), text("scale"), colour(20,20,20,255)
label bounds(352, 58, 81, 12), text("curve"), colour(20,20,20,255)

rslider bounds(433, 12, 62, 62), text("offset"), channel("offset_parm6"), range(0.0, 1.0, 0.5, 1, 0.001) 
combobox bounds(433, 1, 55, 12), channel("offsetx_parm6"), items("-1", "Nornm", "+1"), , value(2), channeltype("string")

rslider bounds(494, 8, 66, 66), text("parm6"), channel("parm6"), range(0.0, 1.0, 0.5, 1, 0.001)

label bounds(570, 8, 55, 12), text("midi"), colour(20,20,20,255)
checkbox bounds(632, 8, 12, 12), text("enable"), channel("enable_parm6"), value(1)
numberbox bounds(570, 25, 55, 15), channel("midich_parm6"), range(1, 16, 1)
numberbox bounds(570, 42, 55, 15), channel("ctrlnum_parm6"), range(1, 127, 1)
label bounds(632, 25, 70, 12), text("channel"), colour(20,20,20,255)
label bounds(632, 42, 70, 12), text("ctrl"), colour(20,20,20,255)

}

groupbox bounds(5, 540, 710, 81), plant("plant_parm7"), linethickness("0"){ 
combobox channel("source1_parm7"), bounds(10, 12, 90, 20), items("rms", "rms_preEq", "cps", "pitch", "centroid", "spread", "skewness", "kurtosis", "flatness", "crest", "flux", "amp_trans", "amp_t_dens", "centr_trans", "centr_t_dens", "kurt_trans", "pitchup_trans", "pitchdown_trans", "cps_raw"), value(1), channeltype("string")
combobox channel("chan1_parm7"), bounds(103, 12, 50, 20), items("1", "2", "3", "4"), value(1)
numberbox bounds(158, 14, 35, 15), channel("rise1_parm7"), range(0.01, 30.0, 0.01)
numberbox bounds(196, 14, 35, 15), channel("fall1_parm7"), range(0.01, 30.0, 0.5)
hslider bounds(233, 12, 86, 20), channel("scale1_parm7"), range(-1.0, 1.0, 0, 1, 0.01)
button bounds(320, 12, 29, 19), channel("scale1_x_parm7"), text("x 1","x 10"), 
hslider bounds(349, 12, 86, 20), channel("curve1_parm7"), range(-5.0, 5.0, 0)

combobox channel("source2_parm7"), bounds(10, 34, 90, 20), items("rms", "rms_preEq", "cps", "pitch", "centroid", "spread", "skewness", "kurtosis", "flatness", "crest", "flux", "amp_trans", "amp_t_dens", "centr_trans", "centr_t_dens", "kurt_trans", "pitchup_trans", "pitchdown_trans", "cps_raw"), value(1), channeltype("string")
combobox channel("chan2_parm7"), bounds(103, 34, 50, 20), items("1", "2", "3", "4"), value(1)
numberbox bounds(158, 36, 35, 15), channel("rise2_parm7"), range(0.01, 30.0, 0.01)
numberbox bounds(196, 36, 35, 15), channel("fall2_parm7"), range(0.01, 30.0, 0.5)
hslider bounds(233, 34, 86, 20), channel("scale2_parm7"), range(-1.0, 1.0, 0, 1, 0.01)
button bounds(320, 34, 29, 19), channel("scale2_x_parm7"), text("x 1","x 10"), 
hslider bounds(349, 34, 86, 20), channel("curve2_parm7"), range(-5.0, 5.0, 0)

label bounds(10, 58, 90, 12), text("source"), colour(20,20,20,255)
label bounds(103, 58, 50, 12), text("chan"), colour(20,20,20,255)
label bounds(156, 58, 76, 12), text("rise/fall"), colour(20,20,20,255)
label bounds(236, 58, 110, 12), text("scale"), colour(20,20,20,255)
label bounds(352, 58, 81, 12), text("curve"), colour(20,20,20,255)

rslider bounds(433, 12, 62, 62), text("offset"), channel("offset_parm7"), range(0.0, 1.0, 0.5, 1, 0.001) 
combobox bounds(433, 1, 55, 12), channel("offsetx_parm7"), items("-1", "Nornm", "+1"), , value(2), channeltype("string")

rslider bounds(494, 8, 66, 66), text("parm7"), channel("parm7"), range(0.0, 1.0, 0.5, 1, 0.001)

label bounds(570, 8, 55, 12), text("midi"), colour(20,20,20,255)
checkbox bounds(632, 8, 12, 12), text("enable"), channel("enable_parm7"), value(1)
numberbox bounds(570, 25, 55, 15), channel("midich_parm7"), range(1, 16, 1)
numberbox bounds(570, 42, 55, 15), channel("ctrlnum_parm7"), range(1, 127, 1)
label bounds(632, 25, 70, 12), text("channel"), colour(20,20,20,255)
label bounds(632, 42, 70, 12), text("ctrl"), colour(20,20,20,255)

}

groupbox bounds(5, 625, 710, 81), plant("plant_parm8"), linethickness("0"){ 
combobox channel("source1_parm8"), bounds(10, 12, 90, 20), items("rms", "rms_preEq", "cps", "pitch", "centroid", "spread", "skewness", "kurtosis", "flatness", "crest", "flux", "amp_trans", "amp_t_dens", "centr_trans", "centr_t_dens", "kurt_trans", "pitchup_trans", "pitchdown_trans", "cps_raw"), value(1), channeltype("string")
combobox channel("chan1_parm8"), bounds(103, 12, 50, 20), items("1", "2", "3", "4"), value(1)
numberbox bounds(158, 14, 35, 15), channel("rise1_parm8"), range(0.01, 30.0, 0.01)
numberbox bounds(196, 14, 35, 15), channel("fall1_parm8"), range(0.01, 30.0, 0.5)
hslider bounds(233, 12, 86, 20), channel("scale1_parm8"), range(-1.0, 1.0, 0, 1, 0.01)
button bounds(320, 12, 29, 19), channel("scale1_x_parm8"), text("x 1","x 10"), 
hslider bounds(349, 12, 86, 20), channel("curve1_parm8"), range(-5.0, 5.0, 0)

combobox channel("source2_parm8"), bounds(10, 34, 90, 20), items("rms", "rms_preEq", "cps", "pitch", "centroid", "spread", "skewness", "kurtosis", "flatness", "crest", "flux", "amp_trans", "amp_t_dens", "centr_trans", "centr_t_dens", "kurt_trans", "pitchup_trans", "pitchdown_trans", "cps_raw"), value(1), channeltype("string")
combobox channel("chan2_parm8"), bounds(103, 34, 50, 20), items("1", "2", "3", "4"), value(1)
numberbox bounds(158, 36, 35, 15), channel("rise2_parm8"), range(0.01, 30.0, 0.01)
numberbox bounds(196, 36, 35, 15), channel("fall2_parm8"), range(0.01, 30.0, 0.5)
hslider bounds(233, 34, 86, 20), channel("scale2_parm8"), range(-1.0, 1.0, 0, 1, 0.01)
button bounds(320, 34, 29, 19), channel("scale2_x_parm8"), text("x 1","x 10"), 
hslider bounds(349, 34, 86, 20), channel("curve2_parm8"), range(-5.0, 5.0, 0)

label bounds(10, 58, 90, 12), text("source"), colour(20,20,20,255)
label bounds(103, 58, 50, 12), text("chan"), colour(20,20,20,255)
label bounds(156, 58, 76, 12), text("rise/fall"), colour(20,20,20,255)
label bounds(236, 58, 110, 12), text("scale"), colour(20,20,20,255)
label bounds(352, 58, 81, 12), text("curve"), colour(20,20,20,255)

rslider bounds(433, 12, 62, 62), text("offset"), channel("offset_parm8"), range(0.0, 1.0, 0.5, 1, 0.001) 
combobox bounds(433, 1, 55, 12), channel("offsetx_parm8"), items("-1", "Nornm", "+1"), , value(2), channeltype("string")

rslider bounds(494, 8, 66, 66), text("parm8"), channel("parm8"), range(0.0, 1.0, 0.5, 1, 0.001)

label bounds(570, 8, 55, 12), text("midi"), colour(20,20,20,255)
checkbox bounds(632, 8, 12, 12), text("enable"), channel("enable_parm8"), value(1)
numberbox bounds(570, 25, 55, 15), channel("midich_parm8"), range(1, 16, 1)
numberbox bounds(570, 42, 55, 15), channel("ctrlnum_parm8"), range(1, 127, 1)
label bounds(632, 25, 70, 12), text("channel"), colour(20,20,20,255)
label bounds(632, 42, 70, 12), text("ctrl"), colour(20,20,20,255)

}

;next x position available below plants is 710
csoundoutput bounds(5, 710, 290, 200), text("Output")

</Cabbage>
<CsoundSynthesizer>
<CsOptions>
;-n -d -+rtmidi=portmidi -Q5 -M0 -m0d
-n -d -+rtmidi=NULL -Q0 -M0 -m0d
</CsOptions>
<CsInstruments>

        sr = 48000
	ksmps = 64
	nchnls = 2
	0dbfs = 1

	giSine	ftgen	0, 0, 65536, 10, 1			; sine wave
        gkhandle init -1
        gkport init -1
        ; list of source channels, used for checking which signals we are currently listening to
        giSourceActive  ftgen 0, 0, 32, 2, 0    ; store the channel numbers (analysis instances) currently listened to

#include "vst_mediator_parameter_ranges.inc"           ; instr 1
#include "subscriber_connect.inc"                   ; instr 2,3
#include "subscriber_receiver.inc"                  ; instr 4,5,6,7,8

instr 98
ain     inch 1
kmetro metro 1
ktime timeinsts
printk2 kmetro*ktime
a0 = 0
outs a0, a0
endin
;****************************************************************
; MIDI output, send mixed and scaled parameter values as midi control messages

	instr 99
        kparm1          chnget "parm1"
        kparm2          chnget "parm2"
        kparm3          chnget "parm3"
        kparm4          chnget "parm4"
        kparm5          chnget "parm5"
        kparm6          chnget "parm6"
        kparm7          chnget "parm7"
        kparm8          chnget "parm8"

        kparm1q         = int(kparm1*127) ; quantize to limit send speed 
        kparm2q         = int(kparm2*127) 
        kparm3q         = int(kparm3*127) 
        kparm4q         = int(kparm4*127) 
        kparm5q         = int(kparm5*127) 
        kparm6q         = int(kparm6*127) 
        kparm7q         = int(kparm7*127) 
        kparm8q         = int(kparm8*127) 
        kchanged1       changed kparm1q
        kchanged2       changed kparm2q
        kchanged3       changed kparm3q
        kchanged4       changed kparm4q
        kchanged5       changed kparm5q
        kchanged6       changed kparm6q
        kchanged7       changed kparm7q
        kchanged8       changed kparm8q

        kchan1          chnget "midich_parm1"
        kchan2          chnget "midich_parm2"
        kchan3          chnget "midich_parm3"
        kchan4          chnget "midich_parm4"
        kchan5          chnget "midich_parm5"
        kchan6          chnget "midich_parm6"
        kchan7          chnget "midich_parm7"
        kchan8          chnget "midich_parm8"
        kctrl1          chnget "ctrlnum_parm1"
        kctrl2          chnget "ctrlnum_parm2"
        kctrl3          chnget "ctrlnum_parm3"
        kctrl4          chnget "ctrlnum_parm4"
        kctrl5          chnget "ctrlnum_parm5"
        kctrl6          chnget "ctrlnum_parm6"
        kctrl7          chnget "ctrlnum_parm7"
        kctrl8          chnget "ctrlnum_parm8"
        kenable1        chnget "enable_parm1"
        kenable2        chnget "enable_parm2"
        kenable3        chnget "enable_parm3"
        kenable4        chnget "enable_parm4"
        kenable5        chnget "enable_parm5"
        kenable6        chnget "enable_parm6"
        kenable7        chnget "enable_parm7"
        kenable8        chnget "enable_parm8"


        if (kchanged1>0) && (kenable1>0) then
        midiout 176, kchan1, kctrl1, kparm1q
        endif
        if (kchanged2>0) && (kenable2>0) then
        midiout 176, kchan2, kctrl2, kparm2q
        endif
        if (kchanged3>0) && (kenable3>0) then
        midiout 176, kchan3, kctrl3, kparm3q
        endif
        if (kchanged4>0) && (kenable4>0) then
        midiout 176, kchan4, kctrl4, kparm4q
        endif
        if (kchanged5>0) && (kenable5>0) then
        midiout 176, kchan5, kctrl5, kparm5q
        endif
        if (kchanged6>0) && (kenable6>0) then
        midiout 176, kchan6, kctrl6, kparm6q
        endif
        if (kchanged7>0) && (kenable7>0) then
        midiout 176, kchan7, kctrl7, kparm7q
        endif
        if (kchanged8>0) && (kenable8>0) then
        midiout 176, kchan8, kctrl8, kparm8q
        endif

	endin


</CsInstruments>
<CsScore>
f0 0.1 ; workaround so that the host VST scanning will not fail upon trying to open the same network port in several plugins
s
#define SCORELEN #86400#
i1      0.1     1               ; init arrays
i2	0.1	$SCORELEN
#include "vst_mediator_score_events.inc"
i99     0.2     $SCORELEN       ; MIDI out
;i 98 0 $SCORELEN
e
</CsScore>
</CsoundSynthesizer>

