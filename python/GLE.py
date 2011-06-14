from os import remove,access,F_OK,mkdir,rename
from math import floor
from glib import spawn_async, SPAWN_DO_NOT_REAP_CHILD
from numpy import abs,fft,linspace
from scipy.signal import flattop,hamming

def transposed(lists):
   if not lists: return []
   return map(lambda *row: list(row), *lists)

class GLE:

    def __init__(self):
        self.format = 'png'
        self.width = 17
        self.height = 7
        self.resolution = 250.0

    def waveform(self, filename, wavfile, framerate, msmode=False, minX=None,maxX=None,minY=None,maxY=None):

        numPoints = len(wavfile)
        rawResolution = 0.2*numPoints/self.width
        skip = 1
        if rawResolution > self.resolution:
            skip = int(floor(float(rawResolution)/self.resolution))
	if skip < 1:
	    skip = 1
	skip = 1

	timescale = 1
	if msmode:
	    timescale = 1000

        FILE = open(filename+'_SA_.dat', 'w')
        count = 0
        for i in wavfile:
            if count % skip == 0:
                FILE.write(str(timescale*count/float(framerate))+",")
                FILE.write(str(i)+"\n")
            count = count + 1
        FILE.close()

        if minX != None and maxX != None: 
            if minX >= maxX:
                maxX = minX+1

	if minX == None:
	    minX = 0
	if maxX == None:
	    maxX = float(len(wavfile))/framerate
        
        s = ['size '+str(self.width)+' '+str(self.height)]
        s.append('set font psh')
        s.append('set hei 0.4')
        s.append('begin graph')
        s.append('nobox')
        s.append('x2axis off')
        s.append('y2axis off')

	if msmode:
	    s.append('xtitle "Time (ms)"')
	else:
	    s.append('xtitle "Time (s)"')

	s.append('ytitle " "')

        s.append('xticks length -0.1')
        s.append('yticks length -0.1')
        s.append('title ""')

	s.append('xaxis min '+str(minX)+' max '+str(maxX))


        s.append('data "'+filename.rsplit('/')[-1]+'_SA_.dat"')
        s.append('d1 line')
        s.append('end graph')

        FILE = open(filename+'_SA_.gle', 'w')
        FILE.writelines('\n'.join(s))
        FILE.close()

        glecall = ['/usr/bin/gle', '-device', self.format, '-output', filename+'_SA_.'+self.format, filename+'_SA_.gle']
        pid,stdin,stdout,stderr = spawn_async(glecall, flags=SPAWN_DO_NOT_REAP_CHILD)
        return pid

    # Window := Number of samples per window
    # Increment := Distance to slide window
    # Coeff := 128
    def spectrogram(self, filename, wavfile, framerate, msmode=False, hzmode=False, minX=None,maxX=None,minY=None,maxY=None):

        increment = 128
        nwindow =  256
        window = flattop(nwindow)
        nfft = nwindow

        # Convert wavfile into array
        
        wavfile_array = []

        for i in wavfile:
            wavfile_array.append(i)

        numPoints = len(wavfile)

	spec = []

        for i in range(0,len(wavfile_array)-nwindow,increment):
            buf = wavfile_array[i:i+nwindow]
            N = nwindow
            X = abs(fft.fft(window*buf, nfft))
            f = linspace(0,framerate,N+1)[:N] 
            Nout = N/2 + 1
	    spec.append(X[Nout:])

	timescale = 1
	if msmode:
	    timescale = 1000

	freqscale = 0.001
	if hzmode:
	    freqscale = 1


	scale = 1
	spec = transposed(spec)

        FILE = open(filename+'_SA_.z', 'w')
	FILE.write('! nx '+str(len(spec[0]))+' ny '+str(len(spec)/scale)+' xmin 0 xmax '+str(timescale*len(wavfile_array)/float(framerate))+' ymin 0 ymax '+str(freqscale*framerate/2.0)+"\n")

	maxval = -100
	for y in range(0,len(spec),scale):
	    for x in range(0,len(spec[y]),1):
		if spec[y][x] > maxval:
		    maxval = spec[y][x]

	maxval = float(maxval)

	for y in range(0,len(spec),scale):
	    for x in range(0,len(spec[y]),1):
                FILE.write(str(1-spec[len(spec)-y-1][x]/maxval)+" ")
	    FILE.write("\n") 
        FILE.close()


	if minX == None:
	    minX = 0
	if maxX == None:
	    maxX = float(len(wavfile))/framerate

	if minY == None:
	    minY = 0
	if maxY == None:
	    maxY = float(framerate)/2
        
        s = ['size '+str(self.width)+' '+str(self.height)]
        s.append('include "color.gle"')
        s.append('set font psh')
        s.append('set hei 0.4')
        s.append('begin graph')
        s.append('nobox')
        s.append('x2axis off')
        s.append('y2axis off')
	if msmode:
	    s.append('xtitle "Time (ms)"')
	else:
	    s.append('xtitle "Time (s)"')


	if hzmode:
	    s.append('ytitle "Frequency (Hz)"')
	else:
	    s.append('ytitle "Frequency (kHz)"')

        s.append('xticks length -0.1')
        s.append('yticks length -0.1')
        s.append('title ""')
	s.append('xaxis min '+str(minX)+' max '+str(maxX))
	s.append('yaxis min '+str(minY*freqscale)+' max '+str(maxY*freqscale))
        s.append('colormap "'+filename.rsplit('/')[-1]+'_SA_.z" 300 200 color')
        s.append('end graph')

        FILE = open(filename+'_SA_.gle', 'w')
        FILE.writelines('\n'.join(s))
        FILE.close()

        glecall = ['/usr/bin/gle', '-device', self.format, '-output', filename+'_SA_.'+self.format, filename+'_SA_.gle']
        pid,stdin,stdout,stderr = spawn_async(glecall, flags=SPAWN_DO_NOT_REAP_CHILD)
        return pid



