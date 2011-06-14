import sys
import wave
from numpy import fromstring

def wavinfo(file):
    spf = wave.open(file,'r')
    sound_info = spf.readframes(-1)
    sound_info = fromstring(sound_info, 'Int16')
    f = spf.getframerate()
    spf.close()

    print "******************************"
    print file
    print "******************************"
    print "Sample Rate: ", str(f), "Hz"
    print "Frames: ", str(len(sound_info))
    print "Duration: ", str(float(len(sound_info))/f)
    print "******************************"

file = sys.argv[1]
wavinfo(file)
