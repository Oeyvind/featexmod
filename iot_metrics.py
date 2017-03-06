#!/usr/bin/python
# -*- coding: latin-1 -*-
import numpy

class Iot_metrics:

    def __init__(self): 
        self.data = []
        self.time_window = 5
        self.cs_tabsize = 128
        self.iot_hist = numpy.array(self.cs_tabsize)

    def update_iot_buffer(self, t):
        self.data.append(t)
        delete_list = []
        for i in range(len(self.data)):
            if self.data[i] < t-self.time_window:
                delete_list.append(self.data[i])
            else: break
        for item in delete_list:
            print item
            self.data.remove(item)
        dist = self.get_distances(self.data, [])
        #print('**distances {}'.format(dist))
        h = self.get_histogram(dist,self.cs_tabsize)
        #print h
        hist = h[0]
        if hist.max() == 0: max_hist = 1
        else: max_hist = hist.max()
        self.iot_hist = hist*(1.0/max_hist)
    
    def get_distances(self, data, distances):
        #print('data {}\ndistance {}'.format(data,distances))
        if len(data) == 1:
            return distances
        else:
            for i in range(1,len(data)):
                distances.append(data[i]-data[0])
            data = data[1:]
            return self.get_distances(data,distances)

    def get_histogram(self, data, bins):
        return numpy.histogram(data, bins=[x/float(bins/self.time_window) for x in range(bins+1)])
'''
# make kde from observations
 kde = KernelDensity(kernel='gaussian', bandwidth=0.05).fit(X[:,np.newaxis])
# represent the kde over some space
 log_dens = kde.score_samples(X_space10[:,np.newaxis])
# linear and normalized
 k=np.exp(log_dens)/np.exp(log_dens).max()
 '''
 
if __name__=="__main__":
    iot = Iot_distances()
    #data = [100.0, 102.0, 102.5, 103.0, 104.0]
    import sys
    test = int(sys.argv[1])
    if test == 1:
        iot.update_iot_buffer(1)
        iot.update_iot_buffer(2)
        iot.update_iot_buffer(3)
        #iot.update_iot_buffer(4)
    if test == 2:
        data = [1,2,3]
        d = iot.get_distances(data, [])
        print d
        h = iot.get_histogram(d,10)
        print h
        n = h[0]*(1.0/h[0].max())
        print n
