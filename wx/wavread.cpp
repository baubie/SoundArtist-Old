#include "wavread.h"
#include <iostream>

bool WavRead::fileOpen()
{
	return m_filename.FileExists();
}

bool WavRead::openFile( wxString& filename )
{
	if (!wxFileName::FileExists(filename)) return false;

	SF_INFO info;
	SNDFILE* sf = sf_open(filename.char_str(), SFM_READ, &info);
	if (sf == NULL) {
		wxMessageBox("Error reading in file.", "Error", wxICON_ERROR | wxOK, NULL);
		return false;
	}
	if (info.channels != 1) {
		wxMessageBox("Wav file has more than 1 channel. File must have only 1 channel.", "Error", wxICON_ERROR | wxOK, NULL);
		return false;
	}

	frames = info.frames;
	samplerate = info.samplerate;
	channels = info.channels;
	format = info.format;
	sections = info.sections;
	seekable = info.seekable;
	m_filename = wxFileName(filename);
	sf_close(sf);

	return true;
}

bool WavRead::makeWaveForm(wxFileName filename, WR_WFINFO* wfinfo)
{
	if (!this->fileOpen()) return false;

	/*
	 * Create the Wave Form data file.
	 */
	wxPuts(filename.GetFullPath());
	wxFile dataf(filename.GetFullPath()<<".dat", wxFile::write);
	if (!dataf.IsOpened()) return false;

	SF_INFO info;
	SNDFILE* sf = sf_open(m_filename.GetFullPath().char_str(), SFM_READ, &info);

	// Read in the sound file
	int *buf;
	int num;
	buf = (int *) malloc(info.frames*sizeof(int));
	num = sf_read_int(sf,buf,info.frames);
	sf_close(sf);

	int timescale = 1;
	if (wfinfo->timeaxis == WR_WFINFO::milliseconds) timescale = 1000;
	// Write wavefile data to file
	for (int i = 0; i < info.frames; i+=(int)(1.0/wfinfo->quality))
	{
		dataf.Write(wxString::Format(wxT("%f"), (float)(timescale*(float)i/(float)info.samplerate)) 
									 << "," 
									 << wxString::Format(wxT("%i"),buf[i])<<"\n");
	}
	free(buf);
	dataf.Close();

	/*
	 * Create the GLE file.
	 */
	wxFile glef(filename.GetFullPath()<<".gle", wxFile::write);

	glef.Write(wxString("size ") << wxString::Format(wxT("%f"), wfinfo->width) << " " << wxString::Format(wxT("%f"), wfinfo->height) << "\n");
	glef.Write("set font psh"); glef.Write("\n");
	glef.Write("set hei 0.4"); glef.Write("\n");
	glef.Write("begin graph"); glef.Write("\n");
	glef.Write("nobox"); glef.Write("\n");
	glef.Write("x2axis off"); glef.Write("\n");
	glef.Write("y2axis off"); glef.Write("\n");

	if (wfinfo->timeaxis == WR_WFINFO::milliseconds) {
	    glef.Write("xtitle \"Time (ms)\""); glef.Write("\n");
	}
	else {
	    glef.Write("xtitle \"Time (s)\""); glef.Write("\n");
	}
	glef.Write("ytitle \" \""); glef.Write("\n");

	glef.Write("xticks length -0.1"); glef.Write("\n");
	glef.Write("yticks length -0.1"); glef.Write("\n");
	glef.Write("title \"\""); glef.Write("\n");
	glef.Write(wxString::Format("xaxis min %f max %f", wfinfo->start, wfinfo->end*timescale/1000)); glef.Write("\n");

	glef.Write(wxString("data \"") << filename.GetFullName() << ".dat\""); glef.Write("\n");

	glef.Write("d1 line"); glef.Write("\n");
	glef.Write("end graph"); glef.Write("\n");
	glef.Close();

	// Generate PNG file
	wxShell(wxString("gle -d png ") << filename.GetFullPath() << ".gle");	

	return true;
}
