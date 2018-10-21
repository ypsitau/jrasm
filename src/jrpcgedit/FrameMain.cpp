//=============================================================================
// FrameMain.cpp
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// FrameMain
//-----------------------------------------------------------------------------
FrameMain::FrameMain(wxWindow *pParent, const wxPoint &pos, const wxSize &size) :
	wxFrame(pParent, wxID_ANY, wxEmptyString, pos, size), _pDocument(new Document())
{
	wxMenuBar *pMenuBar = new wxMenuBar();
	SetMenuBar(pMenuBar);
	do {
		wxMenu *pMenu = new wxMenu();
		pMenuBar->Append(pMenu, _("&File"));
		pMenu->Append(wxID_OPEN, _("&Open...\tCtrl-O"), _("Open document"));
		pMenu->Append(wxID_SAVE, _("&Save...\tCtrl-S"), _("Save document"));
		pMenu->Append(wxID_SAVEAS, _("&Save As...\tCtrl-A"), _("Save document with another name"));
		pMenu->AppendSeparator();
		pMenu->Append(wxID_EXIT);
	} while (0);
	do {
		wxMenu *pMenu = new wxMenu();
		pMenuBar->Append(pMenu, _("&Help"));
		pMenu->Append(wxID_ABOUT);
	} while (0);
	CreateStatusBar();
	//SetStatusText(_("Ready"));
	_pPanelMain = new PanelMain(this, _pDocument->Reference());
	UpdateTitle();
}

bool FrameMain::Destroy()
{
	if (!IsMaximized()) {
		SavePositionAndSize();
	}
	return wxFrame::Destroy();
}

void FrameMain::SavePositionAndSize()
{
	Config &cfg = Config::GetInst();
	wxRect rc = GetScreenRect();
	cfg.FrameMain.x = rc.x;
	cfg.FrameMain.y = rc.y;
	cfg.FrameMain.width = rc.width;
	cfg.FrameMain.height = rc.height;
}

void FrameMain::UpdateTitle()
{
	wxString title;
	if (_pDocument->IsNoName()) {
		title = _("*noname*");
	} else {
		String fileName;
		SplitFileName(_pDocument->GetPathName(), nullptr, &fileName);
		title = fileName.c_str();
	}
	SetTitle(title);
}

void FrameMain::OpenFile()
{
	wxFileDialog dlg(this, _("Open JR-200 Assembler Source"),
					 wxEmptyString, wxEmptyString,
					 "ASM files (*.asm)|*.asm", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if (dlg.ShowModal() == wxID_CANCEL) return;
	wxString pathName = dlg.GetPath();
	if (_pDocument->ReadFile(pathName)) {
		_pPanelMain->UpdateDocument();
		UpdateTitle();
	} else {
		ErrorDialog dlg(this);
		dlg.ShowModal();
	}
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
	String dirName, fileName;
	SplitFileName(_pDocument->GetPathName(), &dirName, &fileName);
	wxFileDialog dlg(this, _("Save JR-200 Assembler Source"),
					 dirName.c_str(), fileName.c_str(),
					 "ASM files (*.asm)|*.asm", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
	if (dlg.ShowModal() == wxID_CANCEL) return;
	wxString pathName = dlg.GetPath();
	if (_pDocument->WriteFile(pathName)) {
		UpdateTitle();
	} else {
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
	EVT_MENU(wxID_SAVEAS,	FrameMain::OnSaveAs)
	EVT_MENU(wxID_EXIT,		FrameMain::OnExit)
	EVT_MENU(wxID_ABOUT,	FrameMain::OnAbout)
	EVT_SIZE(FrameMain::OnSize)
wxEND_EVENT_TABLE()

void FrameMain::OnExit(wxCommandEvent &event)
{
	Close(true);
}

void FrameMain::OnAbout(wxCommandEvent &event)
{
	//wxMessageBox(_("This is a wxWidgets' Hello world sample"),
	//			 _("About Hello World"), wxOK | wxICON_INFORMATION);
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

void FrameMain::OnSize(wxSizeEvent &event)
{
	event.Skip();
}
