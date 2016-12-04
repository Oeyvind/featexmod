#!/usr/bin/python
# -*- coding: latin-1 -*-

import sys
import numpy as np
import scipy.stats as sp
import math

class SpectralAnalysis:

    def __init__(self): 
        self.gifna = '' # csound table number
        self.gifnf = '' # csound table number
        self.amps = np.ones(0) # ctcsound will create pointer to csound table and hold data in array of appropriate size
        self.freqs = np.zeros(0)# as above
        self.amps_norm = np.zeros(0) 
        self.previous_amps = np.zeros(0)
        self.centroid = 0
        self.spread = 0
        self.skewness = 0
        self.kurtosis = 0
        self.flatness = 0
        self.crest = 0
        self.flux = 0

    def initialize(self, cs):
        self.gifna = cs.controlChannel("gifna")
        self.gifnf = cs.controlChannel("gifnf")
        self.amps = cs.table(int(self.gifna[0]))
        self.amps_norm = np.zeros(np.size(self.amps))
        self.previous_amps = np.zeros(np.size(self.amps))
        self.freqs = cs.table(int(self.gifnf[0]))

    def basics(self):
        self.amps_norm = self.amps/np.sum(self.amps)

    def get_centroid(self):
        self.centroid = np.sum(self.amps_norm*self.freqs)
        return self.centroid
    
    def get_spread(self):
        self.spread = math.pow(np.sum(np.power((self.freqs-self.centroid),2)*self.amps_norm),0.5)
        return self.spread
        
    def get_skewness(self):
        return np.sum(np.power((self.freqs-self.centroid),3)*self.amps_norm)/math.pow(self.spread, 3)

    def get_kurtosis(self):
        return np.sum(np.power((self.freqs-self.centroid),4)*self.amps_norm)/math.pow(self.spread, 4)
        
    def get_flatness(self):
        return sp.gmean(self.amps)/np.mean(self.amps)
        
    def get_crest(self):
        return np.max(self.amps)/np.mean(self.amps)
        
    def get_flux(self):
        flux = 1-(np.sum(self.previous_amps*self.amps)/(np.sqrt(np.sum(np.power(self.previous_amps,2)))*np.sqrt(np.sum(np.power(self.amps,2)))))
        self.previous_amps = np.copy(self.amps)
        return flux

    def get_spectrals(self, bogus=0):
        return self.get_centroid(), self.get_spread(), self.get_skewness(), \
               self.get_kurtosis(), self.get_flatness(), self.get_crest(), self.get_flux()

if __name__=="__main__":
    s = SpectralAnalysis()
    s.centroid()
    s.spread()