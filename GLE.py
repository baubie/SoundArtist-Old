from subprocess import call
from os import remove,access,F_OK,mkdir,rename

class GLE:

    def __init__(self):
        self.format = 'eps'
        self.width = 8
        self.height = 5


    def waveform(self, filename, wavfile, framerate, minX=None,maxX=None,minY=None,maxY=None):

        FILE = open(filename+'_data', 'w')
        for i in wavfile:
            FILE.write(str(i)+"\n")
        FILE.close()

        
        s = ['size '+str(self.width)+' '+str(self.height)]
        s.append('set font psh')
        s.append('set hei 0.3')
        s.append('begin graph')
        s.append('nobox')
        s.append('x2axis off')
        s.append('y2axis off')
        s.append('scale auto')
        s.append('xtitle "Time (ms)"')

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


