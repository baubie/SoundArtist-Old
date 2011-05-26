from os import remove,access,F_OK,mkdir,rename
from math import floor
from glib import spawn_async, SPAWN_DO_NOT_REAP_CHILD

class GLE:

    def __init__(self):
        self.format = 'png'
        self.width = 17
        self.height = 6
        self.resolution = 250.0


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

        glecall = ['/usr/bin/gle', '-device', self.format, '-output', filename+'.'+self.format, filename+'.gle']
        pid,stdin,stdout,stderr = spawn_async(glecall, flags=SPAWN_DO_NOT_REAP_CHILD)
        return pid


