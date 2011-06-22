#ifndef __WavRead__
#define __WavRead__
#include <wx/wx.h>
#include <wx/filename.h>
#include <sndfile.h>

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

	private:
		wxString m_filename;

};

#endif
