#include "wx/wx.h" 
#include "gui.h"

class MyApp: public wxApp
{
    virtual bool OnInit();
};

bool MyApp::OnInit()
{
    MainWindow *mw = new MainWindow(NULL, wxID_ANY, _("Sound Artist"), wxPoint(50, 50),
                                  wxSize(850,540) );
    mw->Show(true);
    SetTopWindow(mw);
    return true;
} 

void MainWindow::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    Close(true);
}


IMPLEMENT_APP(MyApp)
