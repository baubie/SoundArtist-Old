#include "SoundArtistMainWindow.h"SoundArtistMainWindow::SoundArtistMainWindow( wxWindow* parent ):MainWindow( parent ){	m_haveWaveForm = false;	m_haveSpectrogram = false;	initPropertyGrid();	initMainToolbar();	// Setup PNG Image Handler	wxInitAllImageHandlers();			this->Connect( wxID_REFRESH_WAVEFORM, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( SoundArtistMainWindow::OnWaveformRefresh ) );	this->Connect( wxID_REFRESH_SPECTROGRAM, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( SoundArtistMainWindow::OnSpectrogramRefresh ) );	this->Connect( wxID_SAVE_WAVEFORM, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( SoundArtistMainWindow::OnWaveformSave ) );	this->Connect( wxID_SAVE_SPECTROGRAM, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( SoundArtistMainWindow::OnSpectrogramSave ) );}SoundArtistMainWindow::~SoundArtistMainWindow(){	this->Disconnect( wxID_REFRESH_WAVEFORM, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( SoundArtistMainWindow::OnWaveformRefresh ) );	this->Disconnect( wxID_REFRESH_SPECTROGRAM, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( SoundArtistMainWindow::OnSpectrogramRefresh ) );	this->Disconnect( wxID_SAVE_WAVEFORM, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( SoundArtistMainWindow::OnWaveformSave ) );	this->Disconnect( wxID_SAVE_SPECTROGRAM, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( SoundArtistMainWindow::OnSpectrogramSave ) );}void SoundArtistMainWindow::initPropertyGrid(){    // Common Properties    m_pg->Append( new wxPropertyCategory(wxT("Common Properties")));    wxPGProperty* prop_start = m_pg->Append( new wxFloatProperty("Start (ms)", "StartTime", 0 ) );    prop_start->SetHelpString("The point in the file, in milliseconds, where the plots will begin.");    wxPGProperty* prop_end = m_pg->Append( new wxFloatProperty("End (ms)", "EndTime", 0 ) );    prop_end->SetHelpString("The point in the file, in milliseconds, where the plots will end.");    m_pg->Append( new wxBoolProperty("Start Time at Zero", "StartAtZero") );    wxPGProperty* prop_lwidth = m_pg->Append( new wxFloatProperty("Axes Line Width", "LineWidth", 0.05 ) );    prop_lwidth->SetHelpString("Sets the width of the lines.");    wxPGProperty* prop_fontsize = m_pg->Append( new wxFloatProperty("Font Size", "FontSize", 0.4 ) );    prop_lwidth->SetHelpString("Sets the size of the font.");     prop_lwidth->SetAttribute("Hint", "HINT: Sets the size of the font.");     m_pg->Append( new wxFloatProperty("Image Width (cm)", "ImageWidth", 14 ) );    m_pg->Append( new wxFloatProperty("Image Height (cm)", "ImageHeight", 8 ) );    wxArrayString arrTimeAxis;    arrTimeAxis.Add("Milliseconds (ms)");    arrTimeAxis.Add("Seconds (s)");    m_pg->Append( new wxEnumProperty( "Time Axis", "TimeAxis", arrTimeAxis ));    // Waveform Properties    m_pg->Append( new wxPropertyCategory(wxT("Wave Form Properties")));	wxArrayString arrQuality;	arrQuality.Add("Very Low");	arrQuality.Add("Low");	arrQuality.Add("Medium");	arrQuality.Add("High");	arrQuality.Add("Very High");	m_pg->Append( new wxEnumProperty( "Preview Quality", "WaveformQuality", arrQuality ));	m_pg->Append( new wxEnumProperty( "Export Quality", "WaveformExportQuality", arrQuality ));    m_pg->Append( new wxBoolProperty("Show Y-Values", "WaveformYLabels") );    m_pg->Append( new wxFloatProperty("Line Width", "WFLineWidth", 0.02 ) );    // Spectrogram Properties    m_pg->Append( new wxPropertyCategory(wxT("Spectrogram Properties")));    wxArrayString arrFreqAxis;    arrFreqAxis.Add("Hz");    arrFreqAxis.Add("kHz");	m_pg->Append( new wxEnumProperty( "Preview Quality", "SpectrogramQuality", arrQuality ));	m_pg->Append( new wxEnumProperty( "Export Quality", "SpectrogramExportQuality", arrQuality ));	wxArrayString arrPalette;	arrPalette.Add("Grayscale");	arrPalette.Add("Rainbow #1");	wxArrayInt arrPaletteIDs;	arrPaletteIDs.Add(0);	arrPaletteIDs.Add(100);    m_pg->Append( new wxEnumProperty( "Palette", "SpectrogramPalette", arrPalette, arrPaletteIDs, 0 ));	m_pg->Append( new wxBoolProperty( "Show Scale Bar", "ScaleBar"));    m_pg->Append( new wxEnumProperty( "Frequency Axis", "FreqAxis", arrFreqAxis ));    wxArrayString arrWindow;    arrWindow.Add("Flat Top");    arrWindow.Add("Hann");    arrWindow.Add("Hamming");    arrWindow.Add("Cosine");    arrWindow.Add("Blackman");    arrWindow.Add("None");	m_pg->Append( new wxEnumProperty( "Window Function", "Window", arrWindow ));    wxArrayString arrNWindowStr;    arrNWindowStr.Add("32");    arrNWindowStr.Add("64");    arrNWindowStr.Add("128");    arrNWindowStr.Add("256");    arrNWindowStr.Add("512");    arrNWindowStr.Add("1024");    wxArrayInt arrNWindow;    arrNWindow.Add(32);    arrNWindow.Add(64);    arrNWindow.Add(128);    arrNWindow.Add(256);    arrNWindow.Add(512);    arrNWindow.Add(1024);	m_pg->Append( new wxEnumProperty( "Window Size (samples)", "NWindow", arrNWindowStr, arrNWindow, 256 ));    m_pg->Append( new wxFloatProperty("Window Overlap (%)", "Overlap", 50 ) );    m_pg->Append( new wxFloatProperty("Amplitude Floor (0-1)", "Floor", 0 ) );}void SoundArtistMainWindow::initMainToolbar(){	m_toolBar->AddTool(wxID_OPEN, wxT("Open"), wxArtProvider::GetBitmap(wxART_FILE_OPEN, wxART_OTHER, wxSize(16,16)));	m_toolBar->Realize();	m_toolBarWaveForm->AddTool(wxID_REFRESH_WAVEFORM, wxT("Refresh"), wxArtProvider::GetBitmap(wxART_REDO, wxART_OTHER, wxSize(16,16)));	m_toolBarWaveForm->AddTool(wxID_SAVE_WAVEFORM, wxT("Save"), wxArtProvider::GetBitmap(wxART_FLOPPY, wxART_OTHER, wxSize(16,16)));	m_toolBarWaveForm->Realize();	m_toolBarSpectrogram->AddTool(wxID_REFRESH_SPECTROGRAM, wxT("Refresh"), wxArtProvider::GetBitmap(wxART_REDO, wxART_OTHER, wxSize(16,16)));	m_toolBarSpectrogram->AddTool(wxID_SAVE_SPECTROGRAM, wxT("Save"), wxArtProvider::GetBitmap(wxART_FLOPPY, wxART_OTHER, wxSize(16,16)));	m_toolBarSpectrogram->Realize();}void SoundArtistMainWindow::OnOpen( wxCommandEvent& event ){	wxFileDialog* OpenDialog = new wxFileDialog(			this, "Select a WAV file to open", wxEmptyString, wxEmptyString,			"WAV Files (*.wav)|*.wav",			wxFD_OPEN, wxDefaultPosition);	if (OpenDialog->ShowModal() == wxID_OK)	{		wxString CurrentDocPath = OpenDialog->GetPath();		if (m_wr.openFile(CurrentDocPath))		{			SetTitle(wxString("SoundArtist - ") << OpenDialog->GetFilename());			m_samplerateLabel->SetLabel(wxString::Format(wxT("%i Hz"),m_wr.samplerate));			m_durationLabel->SetLabel(wxString::Format(wxT("%f ms"),1000*m_wr.frames/(float)m_wr.samplerate));			m_pg->SetPropertyValue("EndTime", 1000*m_wr.frames/(float)m_wr.samplerate);			this->updateSpectrogram();			this->updateWaveForm();		}	}	OpenDialog->Destroy();}void SoundArtistMainWindow::OnQuit( wxCommandEvent& event ){    Close(true);}void SoundArtistMainWindow::OnWaveformRefresh( wxCommandEvent& event ){	updateWaveForm();}void SoundArtistMainWindow::OnSpectrogramRefresh( wxCommandEvent& event ){	updateSpectrogram();}void SoundArtistMainWindow::OnWaveformSave( wxCommandEvent& event ){	wxFileDialog* SaveDialog = new wxFileDialog(			this, "Select a PDF to save to.", wxEmptyString, wxEmptyString,			"PDF Files (*.pdf)|*.pdf",			wxFD_SAVE, wxDefaultPosition);	if (SaveDialog->ShowModal() == wxID_OK)	{		wxString CurrentDocPath = SaveDialog->GetPath();		this->updateWaveForm(CurrentDocPath);	}	SaveDialog->Destroy();}void SoundArtistMainWindow::OnSpectrogramSave( wxCommandEvent& event ){	wxFileDialog* SaveDialog = new wxFileDialog(			this, "Select a PDF to save to.", wxEmptyString, wxEmptyString,			"PDF Files (*.pdf)|*.pdf",			wxFD_SAVE, wxDefaultPosition);	if (SaveDialog->ShowModal() == wxID_OK)	{		wxString CurrentDocPath = SaveDialog->GetPath();		this->updateSpectrogram(CurrentDocPath);	}	SaveDialog->Destroy();}void SoundArtistMainWindow::updateWaveForm(wxString filename){	wxWindowDisabler disableAll;	wxBusyInfo wait("Please wait as I generate your wave form.", this);	WR_WFINFO info;	if (filename == "")	{		// Ensure image is resized		m_haveWaveForm = false;		// Clear current view		wxClientDC dc(m_panelWaveform);		dc.Clear();		dc.DrawBitmap(wxArtProvider::GetBitmap(wxART_MISSING_IMAGE, wxART_OTHER, wxSize(32,32)), 0, 0, false);		Refresh();		Update();		int w_mm, h_mm;		dc.GetSizeMM(&w_mm,&h_mm);		float w = w_mm/10;		float h = h_mm/10;		dc.GetSize(&m_wfWidth, &m_wfHeight);		info.width = w;		info.height = h;	if (m_pg->GetPropertyValueAsString("WaveformQuality") == "Very Low")		info.quality = 0.05;	if (m_pg->GetPropertyValueAsString("WaveformQuality") == "Low")		info.quality = 0.25;	if (m_pg->GetPropertyValueAsString("WaveformQuality") == "Medium")		info.quality = 0.5;	if (m_pg->GetPropertyValueAsString("WaveformQuality") == "High")		info.quality = 0.75;	if (m_pg->GetPropertyValueAsString("WaveformQuality") == "Very High")		info.quality = 1.0;	} else {		info.width = m_pg->GetPropertyValueAsDouble("ImageWidth");		info.height = m_pg->GetPropertyValueAsDouble("ImageHeight");		if (m_pg->GetPropertyValueAsString("WaveformExportQuality") == "Very Low")			info.quality = 0.05;		if (m_pg->GetPropertyValueAsString("WaveformExportQuality") == "Low")			info.quality = 0.25;		if (m_pg->GetPropertyValueAsString("WaveformExportQuality") == "Medium")			info.quality = 0.5;		if (m_pg->GetPropertyValueAsString("WaveformExportQuality") == "High")			info.quality = 0.75;		if (m_pg->GetPropertyValueAsString("WaveformExportQuality") == "Very High")			info.quality = 1.0;	}	if (m_pg->GetPropertyValueAsString("TimeAxis") == "Seconds (s)") {		info.timeaxis = WR_WFINFO::seconds;	} else {		info.timeaxis = WR_WFINFO::milliseconds;	}	info.ylabels = m_pg->GetPropertyValueAsBool("WaveformYLabels");	info.lwidth = m_pg->GetPropertyValueAsDouble("LineWidth");	info.wflwidth = m_pg->GetPropertyValueAsDouble("WFLineWidth");	info.fontsize = m_pg->GetPropertyValueAsDouble("FontSize");	info.start = m_pg->GetPropertyValueAsDouble("StartTime");	info.end = m_pg->GetPropertyValueAsDouble("EndTime");	info.startatzero = m_pg->GetPropertyValueAsBool("StartAtZero");	if (filename == "")	{		wxString tmpFile = wxFileName::CreateTempFileName("soundartist");		if (m_wr.makeWaveForm(tmpFile, &info, false))		{			if (wxFileName::FileExists(wxString(tmpFile) << ".png"))			{				m_waveformImage = wxImage(wxString(tmpFile) << ".png");				m_waveformBitmap = wxBitmap(m_waveformImage);				m_haveWaveForm = true;				Refresh();				Update();			}		}		cleanTmpFiles(tmpFile);	} else {		m_wr.makeWaveForm(filename, &info,true);	}}void SoundArtistMainWindow::updateSpectrogram(wxString filename){	wxWindowDisabler disableAll;	wxBusyInfo wait("Please wait as I generate your spectrogram.", this);	WR_SPECINFO info;	if (filename == "")	{		// Ensure image is resized		m_haveSpectrogram = false;		// Clear current view		wxClientDC dc(m_panelWaveform);		dc.Clear();		dc.DrawBitmap(wxArtProvider::GetBitmap(wxART_MISSING_IMAGE, wxART_OTHER, wxSize(32,32)), 0, 0, false);		Refresh();		Update();		int w_mm, h_mm;		dc.GetSizeMM(&w_mm,&h_mm);		float w = w_mm/10;		float h = h_mm/10;		dc.GetSize(&m_wfWidth, &m_wfHeight);		info.width = w;		info.height = h;		if (m_pg->GetPropertyValueAsString("SpectrogramQuality") == "Very Low")			info.quality = 0.05;		if (m_pg->GetPropertyValueAsString("SpectrogramQuality") == "Low")			info.quality = 0.25;		if (m_pg->GetPropertyValueAsString("SpectrogramQuality") == "Medium")			info.quality = 0.5;		if (m_pg->GetPropertyValueAsString("SpectrogramQuality") == "High")			info.quality = 0.75;		if (m_pg->GetPropertyValueAsString("SpectrogramQuality") == "Very High")			info.quality = 1.0;	} else {		info.width = m_pg->GetPropertyValueAsDouble("ImageWidth");		info.height = m_pg->GetPropertyValueAsDouble("ImageHeight");		if (m_pg->GetPropertyValueAsString("SpectrogramExportQuality") == "Very Low")			info.quality = 0.05;		if (m_pg->GetPropertyValueAsString("SpectrogramExportQuality") == "Low")			info.quality = 0.25;		if (m_pg->GetPropertyValueAsString("SpectrogramExportQuality") == "Medium")			info.quality = 0.5;		if (m_pg->GetPropertyValueAsString("SpectrogramExportQuality") == "High")			info.quality = 0.75;		if (m_pg->GetPropertyValueAsString("SpectrogramExportQuality") == "Very High")			info.quality = 1.0;	}	if (m_pg->GetPropertyValueAsString("TimeAxis") == "Seconds (s)") {		info.timeaxis = WR_SPECINFO::seconds;	} else {		info.timeaxis = WR_SPECINFO::milliseconds;	}	if (m_pg->GetPropertyValueAsString("FreqAxis") == "Hz") {		info.freqaxis = WR_SPECINFO::hz;	} else {		info.freqaxis = WR_SPECINFO::khz;	}	info.palette = m_pg->GetPropertyValueAsInt("SpectrogramPalette");	info.lwidth = m_pg->GetPropertyValueAsDouble("LineWidth");	info.fontsize = m_pg->GetPropertyValueAsDouble("FontSize");	info.start = m_pg->GetPropertyValueAsDouble("StartTime");	info.end = m_pg->GetPropertyValueAsDouble("EndTime");	info.floor = m_pg->GetPropertyValueAsDouble("Floor");	info.startatzero = m_pg->GetPropertyValueAsBool("StartAtZero");	info.scalebar = m_pg->GetPropertyValueAsBool("ScaleBar");	if (m_pg->GetPropertyValueAsString("Window") == "None") {		info.window = WR_SPECINFO::none;	} else if (m_pg->GetPropertyValueAsString("Window") == "Flat Top") {		info.window = WR_SPECINFO::flattop;	} else if (m_pg->GetPropertyValueAsString("Window") == "Cosine") {		info.window = WR_SPECINFO::cosine;	} else if (m_pg->GetPropertyValueAsString("Window") == "Blackman") {		info.window = WR_SPECINFO::blackman;	} else if (m_pg->GetPropertyValueAsString("Window") == "Hamming") {		info.window = WR_SPECINFO::hamming;	} else if (m_pg->GetPropertyValueAsString("Window") == "Hann") {		info.window = WR_SPECINFO::hann;	} else {		info.window = WR_SPECINFO::none;	}	info.nwindow = m_pg->GetPropertyValueAsInt("NWindow");	float overlap = m_pg->GetPropertyValueAsDouble("Overlap");	if (overlap >= 100) overlap = 99.9;	if (overlap < 0) overlap = 0;	int increment = info.nwindow * (100-overlap) * 0.01;	if (increment < 1) increment = 1;	info.increment = increment;	if (filename == "")	{		wxString tmpFile = wxFileName::CreateTempFileName("soundartist");		wxPuts(tmpFile);		if (m_wr.makeSpectrogram(tmpFile, &info, false))		{			if (wxFileName::FileExists(wxString(tmpFile) << ".png"))			{				m_spectrogramImage = wxImage(wxString(tmpFile) << ".png");				m_spectrogramBitmap = wxBitmap(m_spectrogramImage);				m_haveSpectrogram = true;				Refresh();				Update();			}		}		cleanTmpFiles(tmpFile);	} else {		m_wr.makeSpectrogram(filename, &info, true);	}}void SoundArtistMainWindow::OnWaveformResize( wxSizeEvent& event ){	Refresh();	event.Skip();}void SoundArtistMainWindow::OnSpectrogramResize( wxSizeEvent& event ){	Refresh();	event.Skip();}void SoundArtistMainWindow::OnWaveformPaint( wxPaintEvent & evt ){	if (!m_haveWaveForm) {		wxClientDC dc(m_panelWaveform);		dc.DrawBitmap(wxArtProvider::GetBitmap(wxART_MISSING_IMAGE, wxART_OTHER, wxSize(32,32)), 0, 0, false);		return;	}	wxClientDC dc(m_panelWaveform);	int neww, newh;	dc.GetSize( &neww, &newh );	if( neww != m_wfWidth || newh != m_wfHeight)	{		m_waveformBitmap = wxBitmap(m_waveformImage.Scale(neww, newh));		m_wfWidth = neww;		m_wfHeight = newh;		dc.DrawBitmap(m_waveformBitmap, 0, 0, false);	} else {		dc.DrawBitmap(m_waveformBitmap, 0, 0, false);	}}void SoundArtistMainWindow::OnSpectrogramPaint( wxPaintEvent & evt ){	if (!m_haveSpectrogram) {		wxClientDC dc(m_panelSpectrogram);		dc.DrawBitmap(wxArtProvider::GetBitmap(wxART_MISSING_IMAGE, wxART_OTHER, wxSize(32,32)), 0, 0, false);		return;	}	wxClientDC dc(m_panelSpectrogram);	int neww, newh;	dc.GetSize( &neww, &newh );	if( neww != m_wfWidth || newh != m_wfHeight)	{		m_spectrogramBitmap = wxBitmap(m_spectrogramImage.Scale(neww, newh));		m_wfWidth = neww;		m_wfHeight = newh;		dc.DrawBitmap(m_spectrogramBitmap, 0, 0, false);	} else {		dc.DrawBitmap(m_spectrogramBitmap, 0, 0, false);	}}void SoundArtistMainWindow::cleanTmpFiles(wxFileName filename){	// Clean up the temporary files we may have created.    wxRemoveFile(filename.GetFullPath());	    wxRemoveFile(filename.GetFullPath()<<".dat");	    wxRemoveFile(filename.GetFullPath()<<".gle");	    wxRemoveFile(filename.GetFullPath()<<".png");	    wxRemoveFile(filename.GetFullPath()<<".z");	}