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
	_pDocument.reset(new Document());
	_pDocument->ReadFile("pcgsimple.asm");
	do {
		wxNotebook *pNotebook = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNB_BOTTOM);
		pOuterBox->Add(pNotebook, wxSizerFlags(1).Expand());
		pNotebook->AddPage(new Page(pNotebook, new PageInfo()), wxT("Page1"));
		pNotebook->AddPage(new Page(pNotebook, new PageInfo()), wxT("Page2"));
		pNotebook->AddPage(new Page(pNotebook, new PageInfo()), wxT("Page3"));
		pNotebook->AddPage(new Page(pNotebook, new PageInfo()), wxT("Page4"));
	} while (0);
}

//-----------------------------------------------------------------------------
// Event Handler for PanelMain
//-----------------------------------------------------------------------------
wxBEGIN_EVENT_TABLE(PanelMain, wxPanel)
wxEND_EVENT_TABLE()

//-----------------------------------------------------------------------------
// PanelMain::Page
//-----------------------------------------------------------------------------
PanelMain::Page::Page(wxWindow *pParent, PageInfo *pPageInfo) :
	wxPanel(pParent, wxID_ANY), _pPageInfo(pPageInfo)
{
	wxBoxSizer *pOuterBox = new wxBoxSizer(wxVERTICAL);
	SetSizer(pOuterBox);
	do {
		wxSashLayoutWindow *pSash = new wxSashLayoutWindow(
			this, ID_SASH_Left, wxDefaultPosition, wxDefaultSize,
			wxNO_BORDER | wxSW_3D | wxCLIP_CHILDREN);
		pSash->SetDefaultSize(wxSize(300, 1000));
		pSash->SetOrientation(wxLAYOUT_VERTICAL);
		pSash->SetAlignment(wxLAYOUT_RIGHT);
		pSash->SetSashVisible(wxSASH_LEFT, true);
		_pSashRight = pSash;
	} while (0);
	do {
		PatternEditor *pCtrl = new PatternEditor(
			this, pPageInfo->GetPatternInfoOwner().front()->Reference());
		_pPatternEditor = pCtrl;
		_pPatternEditor->AddListener(this);
	} while (0);
	do {
		PatternBrowser *pCtrl= new PatternBrowser(_pSashRight, pPageInfo->Reference());
		_pPatternBrowser = pCtrl;
	} while (0);
}

//-----------------------------------------------------------------------------
// Event Handler for PanelMain::Page
//-----------------------------------------------------------------------------
wxBEGIN_EVENT_TABLE(PanelMain::Page, wxPanel)
	EVT_SIZE(PanelMain::Page::OnSize)
	EVT_SASH_DRAGGED(ID_SASH_Left, PanelMain::Page::OnSashDrag_Left)
wxEND_EVENT_TABLE()

void PanelMain::Page::OnSize(wxSizeEvent &event)
{
	wxLayoutAlgorithm().LayoutWindow(this, _pPatternEditor);
	event.Skip();
}

void PanelMain::Page::OnSashDrag_Left(wxSashEvent &event)
{
	if (event.GetDragStatus() == wxSASH_STATUS_OUT_OF_RANGE) return;
	_pSashRight->SetDefaultSize(wxSize(event.GetDragRect().width, 1000));
	wxLayoutAlgorithm().LayoutWindow(this, _pPatternEditor);
    Refresh();
}

void PanelMain::Page::NotifyPatternModified()
{
	_pPatternBrowser->Refresh();
}

