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

<Cabbage>
form size(575, 710), caption("VST_mediator"), pluginID("vmed")
;image bounds(0, 0, 575, 710), file("background.jpg"), shape("round")
label text("port"), bounds(10, 10, 80, 12), identchannel("port_id")
label text("output_port"), bounds(120, 10, 80, 12)
combobox channel("output_port"), bounds(200, 6, 90, 20), items("7021", "7022", "7023", "7024", "7025"), value(1), channeltype("string")

groupbox bounds(5, 30, 564, 81), plant("plant_parm1"), linethickness("0"){ 
combobox channel("source1_parm1"), bounds(10, 12, 90, 20), items("rms", "rms_preEq", "cps", "pitch", "centroid", "spread", "skewness", "kurtosis", "flatness", "crest", "flux", "amp_trans", "amp_t_dens", "centr_trans", "centr_t_dens", "kurt_trans", "pitchup_trans", "pitchdown_trans", "cps_raw"), value(1), channeltype("string")
combobox channel("chan1_parm1"), bounds(103, 12, 50, 20), items("1", "2", "3", "4"), value(1)
numberbox bounds(158, 14, 35, 15), channel("rise1_parm1"), range(0.01, 10.0, 0.01)
numberbox bounds(196, 14, 35, 15), channel("fall1_parm1"), range(0.01, 10.0, 0.5)
hslider bounds(233, 12, 86, 20), channel("scale1_parm1"), range(-1.0, 1.0, 0, 1, 0.01)
button bounds(320, 12, 29, 19), channel("scale1_x_parm1"), text("x 1","x 10"), 
hslider bounds(349, 12, 86, 20), channel("curve1_parm1"), range(-5.0, 5.0, 0)

combobox channel("source2_parm1"), bounds(10, 34, 90, 20), items("rms", "rms_preEq", "cps", "pitch", "centroid", "spread", "skewness", "kurtosis", "flatness", "crest", "flux", "amp_trans", "amp_t_dens", "centr_trans", "centr_t_dens", "kurt_trans", "pitchup_trans", "pitchdown_trans", "cps_raw"), value(1), channeltype("string")
combobox channel("chan2_parm1"), bounds(103, 34, 50, 20), items("1", "2", "3", "4"), value(1)
numberbox bounds(158, 36, 35, 15), channel("rise2_parm1"), range(0.01, 10.0, 0.01)
numberbox bounds(196, 36, 35, 15), channel("fall2_parm1"), range(0.01, 10.0, 0.5)
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
}

groupbox bounds(5, 115, 564, 81), plant("plant_parm2"), linethickness("0"){ 
combobox channel("source1_parm2"), bounds(10, 12, 90, 20), items("rms", "rms_preEq", "cps", "pitch", "centroid", "spread", "skewness", "kurtosis", "flatness", "crest", "flux", "amp_trans", "amp_t_dens", "centr_trans", "centr_t_dens", "kurt_trans", "pitchup_trans", "pitchdown_trans", "cps_raw"), value(1), channeltype("string")
combobox channel("chan1_parm2"), bounds(103, 12, 50, 20), items("1", "2", "3", "4"), value(1)
numberbox bounds(158, 14, 35, 15), channel("rise1_parm2"), range(0.01, 10.0, 0.01)
numberbox bounds(196, 14, 35, 15), channel("fall1_parm2"), range(0.01, 10.0, 0.5)
hslider bounds(233, 12, 86, 20), channel("scale1_parm2"), range(-1.0, 1.0, 0, 1, 0.01)
button bounds(320, 12, 29, 19), channel("scale1_x_parm2"), text("x 1","x 10"), 
hslider bounds(349, 12, 86, 20), channel("curve1_parm2"), range(-5.0, 5.0, 0)

combobox channel("source2_parm2"), bounds(10, 34, 90, 20), items("rms", "rms_preEq", "cps", "pitch", "centroid", "spread", "skewness", "kurtosis", "flatness", "crest", "flux", "amp_trans", "amp_t_dens", "centr_trans", "centr_t_dens", "kurt_trans", "pitchup_trans", "pitchdown_trans", "cps_raw"), value(1), channeltype("string")
combobox channel("chan2_parm2"), bounds(103, 34, 50, 20), items("1", "2", "3", "4"), value(1)
numberbox bounds(158, 36, 35, 15), channel("rise2_parm2"), range(0.01, 10.0, 0.01)
numberbox bounds(196, 36, 35, 15), channel("fall2_parm2"), range(0.01, 10.0, 0.5)
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
}

groupbox bounds(5, 200, 564, 81), plant("plant_parm3"), linethickness("0"){ 
combobox channel("source1_parm3"), bounds(10, 12, 90, 20), items("rms", "rms_preEq", "cps", "pitch", "centroid", "spread", "skewness", "kurtosis", "flatness", "crest", "flux", "amp_trans", "amp_t_dens", "centr_trans", "centr_t_dens", "kurt_trans", "pitchup_trans", "pitchdown_trans", "cps_raw"), value(1), channeltype("string")
combobox channel("chan1_parm3"), bounds(103, 12, 50, 20), items("1", "2", "3", "4"), value(1)
numberbox bounds(158, 14, 35, 15), channel("rise1_parm3"), range(0.01, 10.0, 0.01)
numberbox bounds(196, 14, 35, 15), channel("fall1_parm3"), range(0.01, 10.0, 0.5)
hslider bounds(233, 12, 86, 20), channel("scale1_parm3"), range(-1.0, 1.0, 0, 1, 0.01)
button bounds(320, 12, 29, 19), channel("scale1_x_parm3"), text("x 1","x 10"), 
hslider bounds(349, 12, 86, 20), channel("curve1_parm3"), range(-5.0, 5.0, 0)

combobox channel("source2_parm3"), bounds(10, 34, 90, 20), items("rms", "rms_preEq", "cps", "pitch", "centroid", "spread", "skewness", "kurtosis", "flatness", "crest", "flux", "amp_trans", "amp_t_dens", "centr_trans", "centr_t_dens", "kurt_trans", "pitchup_trans", "pitchdown_trans", "cps_raw"), value(1), channeltype("string")
combobox channel("chan2_parm3"), bounds(103, 34, 50, 20), items("1", "2", "3", "4"), value(1)
numberbox bounds(158, 36, 35, 15), channel("rise2_parm3"), range(0.01, 10.0, 0.01)
numberbox bounds(196, 36, 35, 15), channel("fall2_parm3"), range(0.01, 10.0, 0.5)
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
}

groupbox bounds(5, 285, 564, 81), plant("plant_parm4"), linethickness("0"){ 
combobox channel("source1_parm4"), bounds(10, 12, 90, 20), items("rms", "rms_preEq", "cps", "pitch", "centroid", "spread", "skewness", "kurtosis", "flatness", "crest", "flux", "amp_trans", "amp_t_dens", "centr_trans", "centr_t_dens", "kurt_trans", "pitchup_trans", "pitchdown_trans", "cps_raw"), value(1), channeltype("string")
combobox channel("chan1_parm4"), bounds(103, 12, 50, 20), items("1", "2", "3", "4"), value(1)
numberbox bounds(158, 14, 35, 15), channel("rise1_parm4"), range(0.01, 10.0, 0.01)
numberbox bounds(196, 14, 35, 15), channel("fall1_parm4"), range(0.01, 10.0, 0.5)
hslider bounds(233, 12, 86, 20), channel("scale1_parm4"), range(-1.0, 1.0, 0, 1, 0.01)
button bounds(320, 12, 29, 19), channel("scale1_x_parm4"), text("x 1","x 10"), 
hslider bounds(349, 12, 86, 20), channel("curve1_parm4"), range(-5.0, 5.0, 0)

combobox channel("source2_parm4"), bounds(10, 34, 90, 20), items("rms", "rms_preEq", "cps", "pitch", "centroid", "spread", "skewness", "kurtosis", "flatness", "crest", "flux", "amp_trans", "amp_t_dens", "centr_trans", "centr_t_dens", "kurt_trans", "pitchup_trans", "pitchdown_trans", "cps_raw"), value(1), channeltype("string")
combobox channel("chan2_parm4"), bounds(103, 34, 50, 20), items("1", "2", "3", "4"), value(1)
numberbox bounds(158, 36, 35, 15), channel("rise2_parm4"), range(0.01, 10.0, 0.01)
numberbox bounds(196, 36, 35, 15), channel("fall2_parm4"), range(0.01, 10.0, 0.5)
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
}

groupbox bounds(5, 370, 564, 81), plant("plant_parm5"), linethickness("0"){ 
combobox channel("source1_parm5"), bounds(10, 12, 90, 20), items("rms", "rms_preEq", "cps", "pitch", "centroid", "spread", "skewness", "kurtosis", "flatness", "crest", "flux", "amp_trans", "amp_t_dens", "centr_trans", "centr_t_dens", "kurt_trans", "pitchup_trans", "pitchdown_trans", "cps_raw"), value(1), channeltype("string")
combobox channel("chan1_parm5"), bounds(103, 12, 50, 20), items("1", "2", "3", "4"), value(1)
numberbox bounds(158, 14, 35, 15), channel("rise1_parm5"), range(0.01, 10.0, 0.01)
numberbox bounds(196, 14, 35, 15), channel("fall1_parm5"), range(0.01, 10.0, 0.5)
hslider bounds(233, 12, 86, 20), channel("scale1_parm5"), range(-1.0, 1.0, 0, 1, 0.01)
button bounds(320, 12, 29, 19), channel("scale1_x_parm5"), text("x 1","x 10"), 
hslider bounds(349, 12, 86, 20), channel("curve1_parm5"), range(-5.0, 5.0, 0)

combobox channel("source2_parm5"), bounds(10, 34, 90, 20), items("rms", "rms_preEq", "cps", "pitch", "centroid", "spread", "skewness", "kurtosis", "flatness", "crest", "flux", "amp_trans", "amp_t_dens", "centr_trans", "centr_t_dens", "kurt_trans", "pitchup_trans", "pitchdown_trans", "cps_raw"), value(1), channeltype("string")
combobox channel("chan2_parm5"), bounds(103, 34, 50, 20), items("1", "2", "3", "4"), value(1)
numberbox bounds(158, 36, 35, 15), channel("rise2_parm5"), range(0.01, 10.0, 0.01)
numberbox bounds(196, 36, 35, 15), channel("fall2_parm5"), range(0.01, 10.0, 0.5)
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
}

groupbox bounds(5, 455, 564, 81), plant("plant_parm6"), linethickness("0"){ 
combobox channel("source1_parm6"), bounds(10, 12, 90, 20), items("rms", "rms_preEq", "cps", "pitch", "centroid", "spread", "skewness", "kurtosis", "flatness", "crest", "flux", "amp_trans", "amp_t_dens", "centr_trans", "centr_t_dens", "kurt_trans", "pitchup_trans", "pitchdown_trans", "cps_raw"), value(1), channeltype("string")
combobox channel("chan1_parm6"), bounds(103, 12, 50, 20), items("1", "2", "3", "4"), value(1)
numberbox bounds(158, 14, 35, 15), channel("rise1_parm6"), range(0.01, 10.0, 0.01)
numberbox bounds(196, 14, 35, 15), channel("fall1_parm6"), range(0.01, 10.0, 0.5)
hslider bounds(233, 12, 86, 20), channel("scale1_parm6"), range(-1.0, 1.0, 0, 1, 0.01)
button bounds(320, 12, 29, 19), channel("scale1_x_parm6"), text("x 1","x 10"), 
hslider bounds(349, 12, 86, 20), channel("curve1_parm6"), range(-5.0, 5.0, 0)

combobox channel("source2_parm6"), bounds(10, 34, 90, 20), items("rms", "rms_preEq", "cps", "pitch", "centroid", "spread", "skewness", "kurtosis", "flatness", "crest", "flux", "amp_trans", "amp_t_dens", "centr_trans", "centr_t_dens", "kurt_trans", "pitchup_trans", "pitchdown_trans", "cps_raw"), value(1), channeltype("string")
combobox channel("chan2_parm6"), bounds(103, 34, 50, 20), items("1", "2", "3", "4"), value(1)
numberbox bounds(158, 36, 35, 15), channel("rise2_parm6"), range(0.01, 10.0, 0.01)
numberbox bounds(196, 36, 35, 15), channel("fall2_parm6"), range(0.01, 10.0, 0.5)
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
}

groupbox bounds(5, 540, 564, 81), plant("plant_parm7"), linethickness("0"){ 
combobox channel("source1_parm7"), bounds(10, 12, 90, 20), items("rms", "rms_preEq", "cps", "pitch", "centroid", "spread", "skewness", "kurtosis", "flatness", "crest", "flux", "amp_trans", "amp_t_dens", "centr_trans", "centr_t_dens", "kurt_trans", "pitchup_trans", "pitchdown_trans", "cps_raw"), value(1), channeltype("string")
combobox channel("chan1_parm7"), bounds(103, 12, 50, 20), items("1", "2", "3", "4"), value(1)
numberbox bounds(158, 14, 35, 15), channel("rise1_parm7"), range(0.01, 10.0, 0.01)
numberbox bounds(196, 14, 35, 15), channel("fall1_parm7"), range(0.01, 10.0, 0.5)
hslider bounds(233, 12, 86, 20), channel("scale1_parm7"), range(-1.0, 1.0, 0, 1, 0.01)
button bounds(320, 12, 29, 19), channel("scale1_x_parm7"), text("x 1","x 10"), 
hslider bounds(349, 12, 86, 20), channel("curve1_parm7"), range(-5.0, 5.0, 0)

combobox channel("source2_parm7"), bounds(10, 34, 90, 20), items("rms", "rms_preEq", "cps", "pitch", "centroid", "spread", "skewness", "kurtosis", "flatness", "crest", "flux", "amp_trans", "amp_t_dens", "centr_trans", "centr_t_dens", "kurt_trans", "pitchup_trans", "pitchdown_trans", "cps_raw"), value(1), channeltype("string")
combobox channel("chan2_parm7"), bounds(103, 34, 50, 20), items("1", "2", "3", "4"), value(1)
numberbox bounds(158, 36, 35, 15), channel("rise2_parm7"), range(0.01, 10.0, 0.01)
numberbox bounds(196, 36, 35, 15), channel("fall2_parm7"), range(0.01, 10.0, 0.5)
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
}

groupbox bounds(5, 625, 564, 81), plant("plant_parm8"), linethickness("0"){ 
combobox channel("source1_parm8"), bounds(10, 12, 90, 20), items("rms", "rms_preEq", "cps", "pitch", "centroid", "spread", "skewness", "kurtosis", "flatness", "crest", "flux", "amp_trans", "amp_t_dens", "centr_trans", "centr_t_dens", "kurt_trans", "pitchup_trans", "pitchdown_trans", "cps_raw"), value(1), channeltype("string")
combobox channel("chan1_parm8"), bounds(103, 12, 50, 20), items("1", "2", "3", "4"), value(1)
numberbox bounds(158, 14, 35, 15), channel("rise1_parm8"), range(0.01, 10.0, 0.01)
numberbox bounds(196, 14, 35, 15), channel("fall1_parm8"), range(0.01, 10.0, 0.5)
hslider bounds(233, 12, 86, 20), channel("scale1_parm8"), range(-1.0, 1.0, 0, 1, 0.01)
button bounds(320, 12, 29, 19), channel("scale1_x_parm8"), text("x 1","x 10"), 
hslider bounds(349, 12, 86, 20), channel("curve1_parm8"), range(-5.0, 5.0, 0)

combobox channel("source2_parm8"), bounds(10, 34, 90, 20), items("rms", "rms_preEq", "cps", "pitch", "centroid", "spread", "skewness", "kurtosis", "flatness", "crest", "flux", "amp_trans", "amp_t_dens", "centr_trans", "centr_t_dens", "kurt_trans", "pitchup_trans", "pitchdown_trans", "cps_raw"), value(1), channeltype("string")
combobox channel("chan2_parm8"), bounds(103, 34, 50, 20), items("1", "2", "3", "4"), value(1)
numberbox bounds(158, 36, 35, 15), channel("rise2_parm8"), range(0.01, 10.0, 0.01)
numberbox bounds(196, 36, 35, 15), channel("fall2_parm8"), range(0.01, 10.0, 0.5)
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
}

;next x position available below plants is 710
;csoundoutput bounds(5, 710, 290, 200), text("Output")

</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-n -d -+rtmidi=NULL -M0 -m0d
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

;****************************************************************
; set OSC output port
        instr 98
        Soutport        chnget "output_port"
        koutport        strtodk Soutport
        printk2 koutport
        kchanged        changed koutport
        if kchanged > 0 then
                        event "i", -99, 0.1, 1, koutport
                        event "i", 99, 0.2, -1, koutport
        endif
        endin

;****************************************************************
; Mediator, send mixed and scaled parameter values as single values via OSC

	instr 99
        kparm1          chnget "parm1"
        kparm2          chnget "parm2"
        kparm3          chnget "parm3"
        kparm4          chnget "parm4"
        kparm5          chnget "parm5"
        kparm6          chnget "parm6"
        kparm7          chnget "parm7"
        kparm8          chnget "parm8"

        kparm1q         = int(kparm1*100)/100 ; quantize to limit OSC send speed (receiver may choke)
        kparm2q         = int(kparm2*100)/100 
        kparm3q         = int(kparm3*100)/100 
        kparm4q         = int(kparm4*100)/100 
        kparm5q         = int(kparm5*100)/100 
        kparm6q         = int(kparm6*100)/100 
        kparm7q         = int(kparm7*100)/100 
        kparm8q         = int(kparm8*100)/100 
        kchanged1       changed kparm1q
        kchanged2       changed kparm2q
        kchanged3       changed kparm3q
        kchanged4       changed kparm4q
        kchanged5       changed kparm5q
        kchanged6       changed kparm6q
        kchanged7       changed kparm7q
        kchanged8       changed kparm8q

        ktrig           metro 10
 	ioutport	= p4
 	Soutport        sprintf "using output port %i", ioutport
 	                puts Soutport, 1
 	                
        OSCsend         kchanged1, "127.0.0.1", ioutport, "parm1", "f", kparm1
        OSCsend         kchanged2, "127.0.0.1", ioutport, "parm2", "f", kparm2
        OSCsend         kchanged3, "127.0.0.1", ioutport, "parm3", "f", kparm3
        OSCsend         kchanged4, "127.0.0.1", ioutport, "parm4", "f", kparm4
        OSCsend         kchanged5, "127.0.0.1", ioutport, "parm5", "f", kparm5
        OSCsend         kchanged6, "127.0.0.1", ioutport, "parm6", "f", kparm6
        OSCsend         kchanged7, "127.0.0.1", ioutport, "parm7", "f", kparm7
        OSCsend         kchanged8, "127.0.0.1", ioutport, "parm8", "f", kparm8

	endin

</CsInstruments>
<CsScore>
f0 0.1 ; workaround so that the host VST scanning will not fail upon trying to open the same network port in several plugins
s
#define SCORELEN #86400#
i1      0.1     1               ; init arrays
i2	0.1	$SCORELEN
#include "vst_mediator_score_events.inc"
i98     0.2       $SCORELEN       ; OSC output port selector, starts instr 99
e
</CsScore>
</CsoundSynthesizer>

