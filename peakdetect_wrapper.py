#!/usr/bin/python
# -*- coding: latin-1 -*-

""" 
Peak detection wrapper to be used from Csound

@author: Øyvind Brandtsegg
@contact: obrandts@gmail.com
"""

import peakdetect
import numpy as np
import rational_approx as ra

class PeakDetector:

    def __init__(self): 
        self.peaks = np.zeros([])
        self.peakndx = -1
        self.v = np.zeros(1)
        self.size = 1
        
    def fill_array(self, val, indx, size):
        if indx == 0:
            if size != self.size:
                self.v = np.zeros(size)
                self.size = size
        self.v[indx] = val
    
    def analyze_peaks(self, delta, x=None):
        self.peaks, vals_ = peakdetect.peakdet(self.v, delta, x)
        #print('Peaks:', self.peaks)
        
    def get_peaks(self, bogus=None):
        self.peakndx += 1
        if self.peakndx > len(self.peaks)-1:
            self.peakndx = -1
            return float(-1), float(-1)
        i,v = self.peaks[self.peakndx]
        return float(i),float(v)

    def get_3maxpeaks(self, bogus=None):
        maxindices = []
        maxvals = []
        if len(self.peaks) > 0:
            vals = [el[1] for el in self.peaks]
            indices = [el[0] for el in self.peaks]
            for i in range(3):
                m = vals.index(max(vals))
                maxindices.append(indices[m])
                maxvals.append(vals[m])
                vals[m] = -1
            # if we would want to rearrange which of these three is the actual max, do it here
        else: 
            maxindices = [0,0,0]
            maxvals = [0,0,0]
        print(float(maxindices[0]), float(maxindices[1]), float(maxindices[2]), float(maxvals[0]), float(maxvals[1]), float(maxvals[2]))
        return float(maxindices[0]), float(maxindices[1]), float(maxindices[2]), float(maxvals[0]), float(maxvals[1]), float(maxvals[2])

    def get_firstpeak(self, bogus=None):
        if len(self.peaks) > 0:
            i,v = self.peaks[0]
        else: 
            i,v = (0,0)
        return float(i), float(v)

    def get_closest_to_maxpeak(self, maxpeak=None):
        if len(self.peaks) > 1: # must be more than 1 peak to look for the next closest one
            vals = [el[1] for el in self.peaks]
            indices = [el[0] for el in self.peaks]
            if maxpeak in [None, 0]:
                m = vals.index(max(vals))
            else: 
                m = indices.index(maxpeak)
            m_cur = indices[m]
            if m < (len(indices)-1): m_next = indices[m+1]
            else: m_next = 9999
            if m>0: m_prev = indices[m-1]
            else: m_prev = -9999
            if m_next-m_cur>m_cur-m_prev:
                closest_i = m_prev
                closest_val = vals[m-1]
            else:
                closest_i = m_next
                closest_val = vals[m+1]
        else:
            closest_i = 0
            closest_val = 0
        return float(closest_i), float(closest_val)
    '''
    def distance_to_max(self, bogus=None):
        # distance from first peak to max (strongest) peak
        # distance from strongest to second strongest
        # distance from strongest to third strongest
        firstpeak_i, _ = self.get_firstpeak()
        maxpeak_i, maxpeak2_i, maxpeak3_i, _, _, _ = self.get_3maxpeaks()
        closest_max_i, _ = self.get_closest_to_maxpeak(maxpeak_i)
        return float(maxpeak_i-firstpeak_i), float(abs(maxpeak_i-maxpeak2_i)), float(abs(maxpeak_i-maxpeak3_i)), float(abs(maxpeak_i-closest_max_i)), float(maxpeak_i)
    '''    
    def relative_distance(self, bogus=None):
        # distance (0 to N) as ratio to (0 to max) index
        # e.g. first peak index as ratio to max peak index
        firstpeak_i, _ = self.get_firstpeak()
        maxpeak_i, maxpeak2_i, maxpeak3_i, _, _, _ = self.get_3maxpeaks()
        closest_max_i, _ = self.get_closest_to_maxpeak(maxpeak_i)
        if len(self.peaks) == 0: maxpeak_i = 1 # divz
        return float(firstpeak_i/maxpeak_i), float(maxpeak2_i/maxpeak_i), float(maxpeak3_i/maxpeak_i), float(closest_max_i/maxpeak_i), float(maxpeak_i)
        
    def relative_distance_denom(self, bogus=None):
        # distance (max to N) ratio reduced to simplest fraction, then return just denominator
        r_first_max, r_max_2, r_max_3, r_max_closest, maxpeak_i = self.relative_distance()
        max_subdiv = 16 # highest denominator for our grid
        deviation = 2.0/self.size # N frames deviation when calculating grid
        _, first_denom = ra.farey(r_first_max, max_subdiv, deviation)
        _, m2_denom = ra.farey(r_max_2, max_subdiv, deviation)
        _, m3_denom = ra.farey(r_max_3, max_subdiv, deviation)
        _, closest_denom = ra.farey(r_max_closest, max_subdiv, deviation)
        denoms = [first_denom, m2_denom, m3_denom, closest_denom]
        for i in range(len(denoms)):
            if denoms[i] in [9,10,11,13,14,15]: # avoid the unwanted denoms)
                denoms[i] = 1
        return float(denoms[0]), float(denoms[1]), float(denoms[2]), float(denoms[3])

    def gridness(self, bogus=None):
        # use relative distance to lay a grid
        # one noe of the grid is at max index
        # subdivisions of the grid set by relative distance
        # we may try several grids, and output the highest grid affirmativity (gridness) found
        # also output the grid (subdiv) that yields the highest affirmativity
        indices = []
        if len(self.peaks) > 0:
            gridoptions = {} # the possible fractions on which to build grids, and their associated grids
            denoms = self.relative_distance_denom()
            maxpeak_i, _, _, _, _, _ = self.get_3maxpeaks()
            for denom in denoms:
                if denom > 0: stepsize = maxpeak_i/float(denom)
                else: stepsize = self.size/2
                numsteps = int(self.size/stepsize)
                thisgrid= []
                for i in range(1, numsteps+1):
                    thisgrid.append(round(i*stepsize))
                gridoptions.setdefault(denom, thisgrid)
            indices = [el[0] for el in self.peaks]
            max_gridmatch = 0
            best_grid = -1
            outlier = -1
            for denom, grid in gridoptions.iteritems():
                gm = []
                for indx in indices:
                    for grindx in grid:
                        if ((grindx >= indx-2) and (grindx <= indx+2)):
                            gm.append(indx)
                            break
                if len(gm) > max_gridmatch: 
                    max_gridmatch = len(gm)
                    best_grid = denom
            gridness = max_gridmatch/float(len(indices))
        else: gridness, denom = (0,1)
        return float(gridness), float(denom), float(len(indices))

if __name__=="__main__":
    data = [0,0,0,0,1,2.1,1,1,3,4,3,1,1,4,5,4,1,1,1,1,1,1,6,8,7,1,1,1,1,1,1,3,4,3,1,1]
    data = [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,]
    p = PeakDetector()
    i = 0
    for d in data:
        p.fill_array(d, i, len(data))
        if d > 0:
            print i
        i += 1
    p.analyze_peaks(.05)
    i = 0
    while i != -1:
        i,v = p.get_peaks()
        print(i,v)
    print('Maxindices:{}'.format(p.get_3maxpeaks()))
    print('Firstpeak:{}'.format(p.get_firstpeak()))
    print('Closest to max:{}'.format(p.get_closest_to_maxpeak()))
    print('relative_distance:{}'.format(p.relative_distance_denom()))
    print('gridness:{}'.format(p.gridness()))

