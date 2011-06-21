#include "mainwindow.h"

class MyApp: public wxApp
{
    public:
	virtual bool OnInit();
};

bool MyApp::OnInit()
{
    mainwindow *mw = new mainwindow(NULL);

    mw->Show(true);
    SetTopWindow(mw);
    return true;
} 


IMPLEMENT_APP(MyApp)
