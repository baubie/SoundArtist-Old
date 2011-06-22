#include "wavread.h"

bool WavRead::fileOpen()
{
	return !m_filename.empty();
}

bool WavRead::openFile( wxString& filename )
{
	if (!wxFileName::FileExists(filename)) return false;

	SF_INFO *info;
	SNDFILE* sf = sf_open(filename.char_str(), SFM_READ, info);
	frames = info->frames;
	samplerate = info->samplerate;
	channels = info->channels;
	format = info->format;
	sections = info->sections;
	seekable = info->seekable;
	sf_close(sf);

	return true;
}
