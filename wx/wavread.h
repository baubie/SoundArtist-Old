#ifndef __WavRead__
#define __WavRead__
#include <wx/wx.h>
#include <wx/filename.h>
#include <wx/utils.h>
#include <sndfile.h>
#include <fftw3.h>

struct WR_WFINFO {
	float quality;
	enum {
		milliseconds,
		seconds
	} timeaxis;
	bool ylabels;
	float lwidth;
	float wflwidth; 
	float fontsize;
	float width;
	float height;
	float start;
	float end;
	bool startatzero;
};

struct WR_SPECINFO {
	float quality;
	enum {
		milliseconds,
		seconds
	} timeaxis;
	enum {
		hz,
		khz
	} freqaxis;
	enum {
		none,
		flattop,
		hann,
		hamming,
		cosine,
		blackman
	} window;
	float lwidth;
	float fontsize;
	float width;
	float height;
	float start;
	float end;
	float floor;
	int nwindow;
	int increment;
	bool color;
	int palette;
	bool startatzero;
	bool scalebar;
};


class WavRead {

	public:
		bool fileOpen();
		bool openFile( wxString& filename );
		uint frames;
		int samplerate;
		int channels;
		int format;
		int sections;
		int seekable;

		// Graphing Functions
		bool makeWaveForm(wxFileName filename, WR_WFINFO* wfinfo, bool pdf);
		bool makeSpectrogram(wxFileName filename, WR_SPECINFO* specinfo, bool pdf);

	private:
		wxFileName m_filename;
		void window_flattop(const int N, float* window);
		void window_hann(const int N, float* window);
		void window_hamming(const int N, float* window);
		void window_cosine(const int N, float* window);
		void window_blackman(const int N, float* window);
		void window_none(const int N, float* window);

};

#endif
