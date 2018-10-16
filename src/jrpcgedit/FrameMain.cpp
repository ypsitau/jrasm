//=============================================================================
// FrameMain.cpp
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// FrameMain
//-----------------------------------------------------------------------------
FrameMain::FrameMain(wxWindow *pParent, const wxString &title, const wxPoint &pos, const wxSize &size) :
	wxFrame(pParent, wxID_ANY, title, pos, size), _pDocument(new Document())
{
	wxMenuBar *pMenuBar = new wxMenuBar();
	SetMenuBar(pMenuBar);
	do {
		wxMenu *pMenu = new wxMenu();
		pMenuBar->Append(pMenu, wxT("&File"));
		pMenu->Append(wxID_SAVE, wxT("&Save...\tCtrl-S"), wxT("Save the document"));
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
#if 1
	if (!_pDocument->ReadFile("test.asm")) {
		ErrorLog::Print(stderr);
	}
#endif
	_pPanelMain = new PanelMain(this, _pDocument->Reference());
}

//-----------------------------------------------------------------------------
// Event Handler
//-----------------------------------------------------------------------------
wxBEGIN_EVENT_TABLE(FrameMain, wxFrame)
	EVT_MENU(wxID_SAVE,		FrameMain::OnSave)
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

void FrameMain::OnSave(wxCommandEvent &event)
{
	_pDocument->WriteFile("test.asm");
}
