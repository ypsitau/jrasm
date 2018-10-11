//=============================================================================
// PanelMain.cpp
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// PanelMain
//-----------------------------------------------------------------------------
PanelMain::PanelMain(wxWindow *pParent) : wxPanel(pParent, wxID_ANY)
{
	wxBoxSizer *pOuterBox = new wxBoxSizer(wxVERTICAL);
	SetSizer(pOuterBox);
	do {
		wxNotebook *pNotebook = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNB_BOTTOM);
		pOuterBox->Add(pNotebook, wxSizerFlags(1).Expand());
		pNotebook->AddPage(new Page(pNotebook), wxT("Page1"));
		pNotebook->AddPage(new Page(pNotebook), wxT("Page2"));
		pNotebook->AddPage(new Page(pNotebook), wxT("Page3"));
		pNotebook->AddPage(new Page(pNotebook), wxT("Page4"));
	} while (0);
}

//-----------------------------------------------------------------------------
// PanelMain::Page
//-----------------------------------------------------------------------------
PanelMain::Page::Page(wxWindow *pParent) : wxPanel(pParent, wxID_ANY)
{
	wxBoxSizer *pOuterBox = new wxBoxSizer(wxVERTICAL);
	SetSizer(pOuterBox);
	wxBoxSizer *pHBox = new wxBoxSizer(wxHORIZONTAL);
	pOuterBox->Add(pHBox, wxSizerFlags(1).Expand());
	do {
		PatternEditor *pPatternEditor = new PatternEditor(this);
		pHBox->Add(pPatternEditor, wxSizerFlags(1).Expand());
	} while (0);
	do {
		ImageBrowser *pImageBrowser = new ImageBrowser(this);
		pHBox->Add(pImageBrowser, wxSizerFlags(1).Expand());
	} while (0);
}

//-----------------------------------------------------------------------------
// Event Handler
//-----------------------------------------------------------------------------
wxBEGIN_EVENT_TABLE(PanelMain, wxPanel)
wxEND_EVENT_TABLE()
