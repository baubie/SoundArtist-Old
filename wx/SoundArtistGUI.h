///////////////////////////////////////////////////////////////////////////// C++ code generated with wxFormBuilder (version Sep 12 2010)// http://www.wxformbuilder.org///// PLEASE DO "NOT" EDIT THIS FILE!///////////////////////////////////////////////////////////////////////////#ifndef __SoundArtistGUI__#define __SoundArtistGUI__#include <wx/string.h>#include <wx/bitmap.h>#include <wx/image.h>#include <wx/icon.h>#include <wx/menu.h>#include <wx/gdicmn.h>#include <wx/font.h>#include <wx/colour.h>#include <wx/settings.h>#include <wx/toolbar.h>#include <wx/stattext.h>#include <wx/sizer.h>#include <wx/propgrid/propgrid.h>#include "wavread.h"#include <wx/statline.h>#include <wx/panel.h>#include <wx/frame.h>///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Class MainWindow///////////////////////////////////////////////////////////////////////////////class MainWindow : public wxFrame {	private:		protected:		wxMenuBar* m_menubar;		wxMenu* m_file;		wxToolBar* m_toolBar;		wxStaticText* m_staticText3;		wxStaticText* m_samplerateLabel;		wxStaticText* m_staticText5;		wxStaticText* m_durationLabel;		wxPropertyGrid *m_pg;		wxStaticLine* m_staticline1;		wxToolBar* m_toolBarWaveForm;		wxPanel* m_panelWaveform;		wxToolBar* m_toolBarSpectrogram;		wxPanel* m_panelSpectrogram;				// Virtual event handlers, overide them in your derived class		virtual void OnOpen( wxCommandEvent& event ) { event.Skip(); }		virtual void OnQuit( wxCommandEvent& event ) { event.Skip(); }		virtual void OnWaveformPaint( wxPaintEvent& event ) { event.Skip(); }		virtual void OnWaveformResize( wxSizeEvent& event ) { event.Skip(); }		virtual void OnSpectrogramPaint( wxPaintEvent& event ) { event.Skip(); }		virtual void OnSpectrogramSize( wxSizeEvent& event ) { event.Skip(); }				public:				MainWindow( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Sound Artist"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 800,600 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );		~MainWindow();	};#endif //__SoundArtistGUI__