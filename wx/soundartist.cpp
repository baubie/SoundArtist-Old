#include <wx/app.h>
#include "SoundArtistMainWindow.h"

class SoundArtist : public wxApp
{
    public:
		virtual bool OnInit();
		
		WavRead m_wr;
	
};

bool SoundArtist::OnInit()
{
    SoundArtistMainWindow *mw = new SoundArtistMainWindow(NULL);

    mw->Show(true);
    SetTopWindow(mw);
    return true;
} 


IMPLEMENT_APP(SoundArtist)
