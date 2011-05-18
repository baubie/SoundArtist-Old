import sys
import gtk
import wave
from numpy import fromstring
import tempfile
from GLE import *

class SoundArtist:

    def on_window_destroy(self, widget, data=None):
        gtk.main_quit()


    def quit(self, widget, data=None):
        gtk.main_quit()


    def openWaveFile(self, widget, data=None):
        chooser = gtk.FileChooserDialog(title="Open WAV File", 
                                        action=gtk.FILE_CHOOSER_ACTION_OPEN,
                                        buttons=(gtk.STOCK_CANCEL,gtk.RESPONSE_CANCEL,gtk.STOCK_OPEN,gtk.RESPONSE_OK))

        filter = gtk.FileFilter()
        filter.set_name("All files")
        filter.add_pattern("*")
        chooser.add_filter(filter)
        filter = gtk.FileFilter()
        filter.set_name("WAV Files")
        filter.add_pattern("*.wav")
        chooser.add_filter(filter)

        response = chooser.run()

        if response == gtk.RESPONSE_OK:
            filename = chooser.get_filename()
            self.builder.get_object("lblFilename").set_label(filename.split("/")[-1])
            spf = wave.open(filename,'r')
            sound_info = spf.readframes(-1)
            self.wavfile = fromstring(sound_info, 'Int16')
            self.framerate = spf.getframerate()
            spf.close()
            self.builder.get_object("lblSampleRate").set_label("Sampling Rate: "+str(self.framerate)+" Hz")
            self.builder.get_object("lblDuration").set_label("Duration: "+str(float(len(self.wavfile))/self.framerate)+" s")
            self.refreshWaveForm()

        chooser.destroy()


    def refreshWaveForm(self):
        tmpname = tempfile.mkstemp()
        self.GLE.waveform(tmpname[1], self.wavfile, self.framerate)




    def __init__(self):

        self.GLE = GLE()

        self.builder = gtk.Builder()
        self.builder.add_from_file("gui.glade")

        self.window = self.builder.get_object("MainWindow")
        self.builder.connect_signals(self)



if __name__ == "__main__":
    app = SoundArtist()
    app.window.show()
    gtk.main()
