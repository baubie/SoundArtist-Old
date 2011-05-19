from subprocess import call
from os import remove,access,F_OK,mkdir,rename
from math import floor

class GLE:

    def __init__(self):
        self.format = 'eps'
        self.width = 8
        self.height = 5
        self.resolution = 500.0


    def waveform(self, filename, wavfile, framerate, minX=None,maxX=None,minY=None,maxY=None):

        numPoints = len(wavfile)
        rawResolution = numPoints/self.width
        skip = 1
        if rawResolution > self.resolution:
            skip = int(floor(float(rawResolution) / self.resolution))
        skip = 1


        FILE = open(filename+'_data', 'w')
        count = 0
        for i in wavfile:
            if count % skip == 0:
                FILE.write(str(count/float(framerate))+",")
                FILE.write(str(i)+"\n")
            count = count + 1
        FILE.close()


        
        s = ['size '+str(self.width)+' '+str(self.height)]
        s.append('set font psh')
        s.append('set hei 0.3')
        s.append('begin graph')
        s.append('nobox')
        s.append('x2axis off')
        s.append('y2axis off')
        s.append('scale auto')
        s.append('xtitle "Time (s)"')

        s.append('xticks length -0.1')
        s.append('yticks length -0.1')
        s.append('title ""')

        if minX != None and maxX != None: 
            if minX >= maxX:
                maxX = minX+1
            s.append('xaxis min '+str(minX)+' max '+str(maxX))


        s.append('data "'+filename.rsplit('/')[-1]+'_data"')
        s.append('d1 line')
        s.append('end graph')

        FILE = open(filename+'.gle', 'w')
        FILE.writelines('\n'.join(s))
        FILE.close()

        glecall = ['gle', '-device', self.format, '-output', filename+'.'+self.format, filename+'.gle']
        print ' '.join(glecall)
        call(glecall)


