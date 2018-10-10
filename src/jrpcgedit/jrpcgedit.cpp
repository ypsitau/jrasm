//=============================================================================
// jrpcgedit.cpp
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// AppMain
//-----------------------------------------------------------------------------
class AppMain: public wxApp
{
public:
	virtual bool OnInit();
};

//-----------------------------------------------------------------------------
// FrameMain
//-----------------------------------------------------------------------------
class FrameMain: public wxFrame
{
public:
	enum {
		ID_Hello = 1
	};
public:
	FrameMain(const wxString &title, const wxPoint &pos, const wxSize &size);
private:
    void OnHello(wxCommandEvent &event);
    void OnExit(wxCommandEvent &event);
    void OnAbout(wxCommandEvent &event);
    wxDECLARE_EVENT_TABLE();
};

//-----------------------------------------------------------------------------
// AppMain
//-----------------------------------------------------------------------------
wxIMPLEMENT_APP(AppMain);

bool AppMain::OnInit()
{
	FrameMain *pFrame = new FrameMain(wxT("Hello World"), wxPoint(50, 50), wxSize(450, 340));
	pFrame->Show(true);
	return true;
}

//-----------------------------------------------------------------------------
// FrameMain
//-----------------------------------------------------------------------------
FrameMain::FrameMain(const wxString& title, const wxPoint& pos, const wxSize& size)
        : wxFrame(NULL, wxID_ANY, title, pos, size)
{
	wxMenuBar *pMenuBar = new wxMenuBar();
	SetMenuBar(pMenuBar);
	do {
		wxMenu *pMenu = new wxMenu();
		pMenuBar->Append(pMenu, wxT("&File"));
		pMenu->Append(ID_Hello, wxT("&Hello...\tCtrl-H"),
					  wxT("Help string shown in status bar for this menu item"));
		pMenu->AppendSeparator();
		pMenu->Append(wxID_EXIT);
	} while (0);
	do {
		wxMenu *pMenu = new wxMenu();
		pMenuBar->Append(pMenu, wxT("&Help"));
		pMenu->Append(wxID_ABOUT);
	} while (0);
	CreateStatusBar();
	SetStatusText(wxT("Welcome to wxWidgets!"));
}

wxBEGIN_EVENT_TABLE(FrameMain, wxFrame)
EVT_MENU(ID_Hello,		FrameMain::OnHello)
EVT_MENU(wxID_EXIT,		FrameMain::OnExit)
EVT_MENU(wxID_ABOUT,	FrameMain::OnAbout)
wxEND_EVENT_TABLE()

void FrameMain::OnExit(wxCommandEvent &event)
{
	Close(true);
}

void FrameMain::OnAbout(wxCommandEvent &event)
{
	wxMessageBox(wxT("This is a wxWidgets' Hello world sample"),
				 wxT("About Hello World"), wxOK | wxICON_INFORMATION);
}

void FrameMain::OnHello(wxCommandEvent &event)
{
	wxLogMessage(wxT("Hello world from wxWidgets!"));
}
