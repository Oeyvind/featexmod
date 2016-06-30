#!/usr/bin/python
# -*- coding: latin-1 -*-

""" 
Peak detection wrapper to be used from Csound

@author: Øyvind Brandtsegg
@contact: obrandts@gmail.com
"""

import peakdetect
import numpy as np

class PeakDetector:

    def __init__(self): 
        self.peaks = np.zeros([])
        self.peakndx = -1
        self.v = np.zeros(1)
        
    def fill_array(self, val, indx, size):
        if indx == 0:
            self.v = np.zeros(size)
        self.v[indx] = val
    
    def analyze_peaks(self, delta, x=None):
        self.peaks, vals_ = peakdetect.peakdet(self.v, delta, x)
        #print 'Peaks:', self.peaks
        
    def get_peaks(self, bogus=None):
        self.peakndx += 1
        if self.peakndx > len(self.peaks)-1:
            self.peakndx = -1
            return float(-1), float(-1)
        i,v = self.peaks[self.peakndx]
        return float(i),float(v)

if __name__=="__main__":
    data = [0,0,1,2,1,1,3,4,3,1,1,4,5,4,1,1,1]
    pd = PeakDetector()
    i = 0
    for d in data:
        pd.fill_array(d, i, len(data))
        i += 1
    pd.analyze_peaks(1)
    i = 0
    while i != -1:
        i,v = pd.get_peaks()
        print i,v
