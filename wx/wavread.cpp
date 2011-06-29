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

void WavRead::window_none(const int N, float* window)
{
	for(int i=0; i < N; ++i) {
		window[i] = 1.0;
	}
}

void WavRead::window_flattop(const int N, float* window)
{
	// Got this formula from 
	// http://en.wikipedia.org/wiki/Window_function#Flat_top_window
#define PI 3.14159265
	for(int i=0; i < N; ++i) {
		window[i] = 0.355768 - 0.487396*cos(2*PI*i/(N-1)) + 0.144232*cos(4*PI*i/(N-1)) - 0.012604*cos(6*PI*i/(N-1));
	}
}

bool WavRead::makeSpectrogram(wxFileName filename, WR_SPECINFO* specinfo, bool pdf)
{
	if (!this->fileOpen()) return false;


	/*
	 * Create the Spectrogram data file.
	 */

	// Make sure our settings are valid
    if (specinfo->floor < 0) specinfo->floor = 0;	
    if (specinfo->floor > 1) specinfo->floor = 1;

	wxFile dataf(filename.GetFullPath()<<".z", wxFile::write);
	if (!dataf.IsOpened()) return false;

	SF_INFO info;
	SNDFILE* sf = sf_open(m_filename.GetFullPath().char_str(), SFM_READ, &info);

	// Read in the sound file
	int *buf;
	int num;
	buf = (int *)malloc(info.frames*sizeof(int));
	num = sf_read_int(sf,buf,info.frames);
	sf_close(sf);

	int timescale = 1;
	if (specinfo->timeaxis == WR_SPECINFO::milliseconds) timescale = 1000;

	float freqscale = 0.001;
	if (specinfo->freqaxis == WR_SPECINFO::hz) freqscale = 1;

	int N = specinfo->nwindow;


	// Get the window
	float window[N];
	if (specinfo->window == WR_SPECINFO::none) {
		window_none(N, window);
	} else if (specinfo->window == WR_SPECINFO::flattop) {
		window_flattop(N, window);
	}


	int nx = floor((frames-N)/specinfo->increment)+1;
	int ny = N/2.0+1;
	dataf.Write("! nx ");
	dataf.Write(wxString::Format("%i", nx));
	dataf.Write(" ny ");
	dataf.Write(wxString::Format("%i", ny)); 
	dataf.Write(" xmin 0 xmax ");
	dataf.Write(wxString::Format("%f", (float)timescale*frames/samplerate));
	dataf.Write(" ymin 0 ymax ");
	dataf.Write(wxString::Format("%f", freqscale*samplerate/2.0));
	dataf.Write("\n");

	// Write spectrogram data to file
	
	// Create out 2D array on the heap (due to size)
	double **spec = new double*[nx];
	for (int j = 0; j < nx; ++j) spec[j] = new double[ny];

	int curX = 0;
	double maxVal = 0.0001;
	double *in;
	double *out;
	in = (double*) fftw_malloc(sizeof(double)*N);
	out = (double*) fftw_malloc(sizeof(double)*N);
	fftw_plan p = fftw_plan_r2r_1d(N, in, out, FFTW_DHT, FFTW_ESTIMATE);

	for (int i = 0; i < info.frames-N; i+=specinfo->increment)
	{
		for (int j = 0; j < N; ++j)
		{
			in[j] = buf[i+j]*window[j];	
		}

		fftw_execute(p);

		// Save results
		for (int j = 0; j < ny; ++j)
		{
			double val = 10*log10(out[j]*out[j]);
			spec[curX][j] = val;
			maxVal = maxVal < spec[curX][j] ? spec[curX][j] : maxVal;
		}

		curX++;
	}
	fftw_destroy_plan(p);
	fftw_free(in); 
	fftw_free(out);

	// Write the data to file
	double floor = 1-specinfo->floor;
	for (int y = 0; y < ny; ++y) {
		for (int x = 0; x < nx; ++x) {
			double val = 1-spec[x][y]/maxVal;

			// This is our floor function.  Right now we shoot everything to 1.
			// Should have something more gradual.
			if (val > floor) val = 1;
			else val = 1-pow((val-floor)/(floor),2);

			dataf.Write(wxString::Format("%f ", val));
		}
		dataf.Write("\n");
	}
	free(buf);
	dataf.Close();

	// Delete the 2D array on the heap
	for (int j = 0; j < nx; ++j) delete [] spec[j];
	delete[] spec;



	// Create the GLE file.
	wxFile glef(filename.GetFullPath() << ".gle", wxFile::write);

	glef.Write(wxString("size ") << wxString::Format(wxT("%f"), specinfo->width) << " " << wxString::Format(wxT("%f"), specinfo->height) << "\n");
	glef.Write("include \"color.gle\""); glef.Write("\n");
	glef.Write("set font psh"); glef.Write("\n");
	glef.Write("set hei 0.4"); glef.Write("\n");
	glef.Write("begin graph"); glef.Write("\n");
	glef.Write("nobox"); glef.Write("\n");
	glef.Write("x2axis off"); glef.Write("\n");
	glef.Write("y2axis off"); glef.Write("\n");

	if (specinfo->timeaxis == WR_SPECINFO::milliseconds) {
	    glef.Write("xtitle \"Time (ms)\""); glef.Write("\n");
	} else {
	    glef.Write("xtitle \"Time (s)\""); glef.Write("\n");
	}

	if (specinfo->freqaxis == WR_SPECINFO::hz) {
		glef.Write("ytitle \"Frequency (Hz)\""); glef.Write("\n");
	} else {
		glef.Write("ytitle \"Frequency (kHz)\""); glef.Write("\n");
	}

	glef.Write("xticks length -0.1"); glef.Write("\n");
	glef.Write("yticks length -0.1"); glef.Write("\n");
	glef.Write("title \"\""); glef.Write("\n");
	glef.Write(wxString::Format("xaxis min %f max %f", specinfo->start*timescale/1000, specinfo->end*timescale/1000)); glef.Write("\n");
	glef.Write(wxString::Format("yaxis min %f max %f", 0.0, freqscale*samplerate/2)); glef.Write("\n");

    glef.Write(wxString("colormap \"") << filename.GetFullName() << ".z\" "<< nx << " " << ny << " color"); glef.Write("\n");

	glef.Write("end graph"); glef.Write("\n");
	glef.Close();

	if (!pdf)
	{
		// Generate PNG file
		wxShell(wxString("gle -d png ") << filename.GetFullPath() << ".gle");	
	} else {
		// Generate PDF file
		wxShell(wxString("gle -d pdf ") << filename.GetFullPath() << ".gle");	
	}

	return true;
}

bool WavRead::makeWaveForm(wxFileName filename, WR_WFINFO* wfinfo, bool pdf)
{
	if (!this->fileOpen()) return false;


	/*
	 * Create the Wave Form data file.
	 */
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
		float t = (float)(timescale*(float)i/(float)info.samplerate);
		dataf.Write(wxString::Format(wxT("%f"), t) 
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

	if (wfinfo->ylabels == false)
	{
		glef.Write("ylabels off"); glef.Write("\n");
	}

	glef.Write("xticks length -0.1"); glef.Write("\n");
	glef.Write("yticks length -0.1"); glef.Write("\n");
	glef.Write("title \"\""); glef.Write("\n");
	glef.Write(wxString::Format("xaxis min %f max %f", wfinfo->start*timescale/1000, wfinfo->end*timescale/1000)); glef.Write("\n");

	glef.Write(wxString("data \"") << filename.GetFullName() << ".dat\""); glef.Write("\n");

	glef.Write("d1 line"); glef.Write("\n");
	glef.Write("end graph"); glef.Write("\n");
	glef.Close();

	if (!pdf)
	{
		// Generate PNG file
		wxShell(wxString("gle -d png ") << filename.GetFullPath() << ".gle");	
	} else {
		// Generate PDF file
		wxShell(wxString("gle -d pdf ") << filename.GetFullPath() << ".gle");	
	}

	return true;
}
