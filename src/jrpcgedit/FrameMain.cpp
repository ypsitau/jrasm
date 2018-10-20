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
		pMenu->Append(wxID_OPEN, wxT("&Open...\tCtrl-O"), wxT("Open document"));
		pMenu->Append(wxID_SAVE, wxT("&Save...\tCtrl-S"), wxT("Save document"));
		pMenu->Append(wxID_SAVEAS, wxT("&Save As...\tCtrl-A"), wxT("Save document with another name"));
		pMenu->AppendSeparator();
		pMenu->Append(wxID_EXIT);
	} while (0);
	do {
		wxMenu *pMenu = new wxMenu();
		pMenuBar->Append(pMenu, wxT("&Help"));
		pMenu->Append(wxID_ABOUT);
	} while (0);
	CreateStatusBar();
	//SetStatusText(wxT("Ready"));
	_pPanelMain = new PanelMain(this, _pDocument->Reference());
}

void FrameMain::OpenFile()
{
	wxFileDialog dlg(this, _("Open JR-200 Assembler Source"), "", "",
					 "ASM files (*.asm)|*.asm", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if (dlg.ShowModal() == wxID_CANCEL) return;
	wxString pathName = dlg.GetPath();
	if (!_pDocument->ReadFile(pathName)) {
		ErrorDialog dlg(this);
		dlg.ShowModal();
		return;
	}
	_pPanelMain->UpdateDocument();
}

void FrameMain::SaveFile()
{
	if (_pDocument->IsNoName()) {
		SaveFileAs();
	} else if (!_pDocument->WriteFile(_pDocument->GetPathName())) {
		ErrorDialog dlg(this);
		dlg.ShowModal();
		return;
	}
}

void FrameMain::SaveFileAs()
{
	wxFileDialog dlg(this, _("Save JR-200 Assembler Source"), "", "",
					 "ASM files (*.asm)|*.asm", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
	if (dlg.ShowModal() == wxID_CANCEL) return;
	wxString pathName = dlg.GetPath();
	if (!_pDocument->WriteFile("test.asm")) {
		ErrorDialog dlg(this);
		dlg.ShowModal();
	}
}

//-----------------------------------------------------------------------------
// Event Handler
//-----------------------------------------------------------------------------
wxBEGIN_EVENT_TABLE(FrameMain, wxFrame)
	EVT_MENU(wxID_OPEN,		FrameMain::OnOpen)
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
	//wxMessageBox(wxT("This is a wxWidgets' Hello world sample"),
	//			 wxT("About Hello World"), wxOK | wxICON_INFORMATION);
}

void FrameMain::OnOpen(wxCommandEvent &event)
{
	OpenFile();
}

void FrameMain::OnSave(wxCommandEvent &event)
{
	SaveFile();
}

void FrameMain::OnSaveAs(wxCommandEvent &event)
{
	SaveFileAs();
}
