#ifndef __WavRead__
#define __WavRead__
#include <wx/wx.h>
#include <wx/filename.h>
#include <wx/utils.h>
#include <sndfile.h>

struct WR_WFINFO {
	float quality;
	enum {
		milliseconds,
		seconds
	} timeaxis;
	bool ylabels;
	float lwidth;
	float width;
	float height;
	float start;
	float end;
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
	float lwidth;
	float width;
	float height;
	float start;
	float end;
	bool color;
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
		bool makeWaveForm(wxFileName filename, WR_WFINFO* info);

	private:
		wxFileName m_filename;

};

#endif
