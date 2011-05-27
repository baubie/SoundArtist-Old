import sys
import gtk
import wave
from numpy import fromstring
import tempfile
from GLE import *
from glib import child_watch_add
from os import remove

class SoundArtist:

    def on_window_destroy(self, widget, data=None):
        cleantemp()
        gtk.main_quit()

    def cleanTemp(self,filename):
	try:
	    remove(filename)
	except OSError,error:
	    pass

	try:
	    remove(filename+"_SA_.z")
	except OSError,error:
	    pass

	try:
	    remove(filename+"_SA_.dat")
	except OSError,error:
	    pass

	try:
	    remove(filename+"_SA_.gle")
	except OSError,error:
	    pass

	try:
	    remove(filename+"_SA_.png")
	except OSError,error:
	    pass

    def quit(self, widget, data=None):
        gtk.main_quit()


    def clearEvents(self):
        while gtk.events_pending():
            gtk.main_iteration(False)


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
            chooser.destroy()
            self.builder.get_object("txtStart").set_text("0")
            self.builder.get_object("txtEnd").set_text(str(1000*float(len(self.wavfile))/self.framerate))
            self.clearEvents()
	    self.refreshWaveForm(data={"type":"preview"})
            self.refreshSpectrogram(data={"type":"preview"})
        else:
            chooser.destroy()


    def WaveFormSave(self, widget=None, data=None):
        print "Save Waveform"

    def SpectrogramSave(self, widget=None, data=None):
        print "Save Spectrogram"

    def updateWaveFormImage(self, pid, condition, data=None):
        self.builder.get_object("WaveFormImage").set_from_file(data+"_SA_.png")
	self.cleanTemp(data)

    def updateSpectrogramImage(self, pid, condition, data=None):
        self.builder.get_object("SpectrogramImage").set_from_file(data+"_SA_.png")
	self.cleanTemp(data)

    def refreshWaveForm(self, widget=None, data=None):
        self.builder.get_object("WaveFormImage").set_from_stock(gtk.STOCK_REFRESH, gtk.ICON_SIZE_LARGE_TOOLBAR)
        self.clearEvents()
        tmpname = tempfile.mkstemp()
	starttime = float(self.builder.get_object("txtStart").get_text())
	endtime = float(self.builder.get_object("txtEnd").get_text())

	msmode = True
	if self.cbTimeAxis.get_active() == 0:
	    starttime = starttime / 1000.0
	    endtime = endtime / 1000.0
	    msmode = False

	gle = GLE()
	if data == None or data["type"] == "preview":
	    gle.format = 'png' 

        pid = gle.waveform(tmpname[1], self.wavfile, self.framerate, msmode=msmode, minX=starttime, maxX=endtime)
        watch = child_watch_add(pid, self.updateWaveFormImage, data=tmpname[1])
        self.clearEvents()

    def refreshSpectrogram(self, widget=None, data=None):
        self.builder.get_object("SpectrogramImage").set_from_stock(gtk.STOCK_REFRESH, gtk.ICON_SIZE_LARGE_TOOLBAR)
        self.clearEvents()
        tmpname = tempfile.mkstemp()
	starttime = float(self.builder.get_object("txtStart").get_text())
	endtime = float(self.builder.get_object("txtEnd").get_text())

	msmode = True
	if self.cbTimeAxis.get_active() == 0:
	    starttime = starttime / 1000.0
	    endtime = endtime / 1000.0
	    msmode = False

	hzmode = True
	if self.cbFreqAxis.get_active() == 1:
	    hzmode = False

	gle = GLE()
	if data == None or data["type"] == "preview":
	    gle.format = 'png' 

        pid = gle.spectrogram(tmpname[1], self.wavfile, self.framerate, msmode=msmode, hzmode=hzmode, minX=starttime, maxX=endtime)
        watch = child_watch_add(pid, self.updateSpectrogramImage, data=tmpname[1])
        self.clearEvents()


    def __init__(self):

        self.builder = gtk.Builder()
        self.builder.add_from_file("gui.glade")

        self.window = self.builder.get_object("MainWindow")
        self.builder.connect_signals(self)

        self.builder.get_object("WaveFormImage").clear()
        self.builder.get_object("SpectrogramImage").clear()

	self.cbTimeAxis = gtk.combo_box_new_text()
        self.builder.get_object("boxTimeAxis").pack_start(self.cbTimeAxis)
        self.cbTimeAxis.append_text("Seconds (s)")
        self.cbTimeAxis.append_text("Milliseconds (ms)")
	self.cbTimeAxis.set_active(0)
	self.cbTimeAxis.show()

	self.cbFreqAxis = gtk.combo_box_new_text()
	self.builder.get_object("boxFrequencyAxis").pack_start(self.cbFreqAxis)
	self.cbFreqAxis.append_text("Hz")
	self.cbFreqAxis.append_text("kHz")
	self.cbFreqAxis.set_active(0)
	self.cbFreqAxis.show()


if __name__ == "__main__":
    app = SoundArtist()
    app.window.show()
    gtk.main()
