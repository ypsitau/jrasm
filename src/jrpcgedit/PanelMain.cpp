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
	do {
		wxSashLayoutWindow *pSash = new wxSashLayoutWindow(
			this, ID_SASH_Left, wxDefaultPosition, wxDefaultSize,
			wxNO_BORDER | wxSW_3D | wxCLIP_CHILDREN);
		pSash->SetDefaultSize(wxSize(300, 1000));
		pSash->SetOrientation(wxLAYOUT_VERTICAL);
		pSash->SetAlignment(wxLAYOUT_LEFT);
		pSash->SetSashVisible(wxSASH_RIGHT, true);
		_pSashVert = pSash;
	} while (0);
	do { // panel on left
		wxPanel *pPanel = new wxPanel(_pSashVert);
		wxBoxSizer *pVBox = new wxBoxSizer(wxVERTICAL);
		pPanel->SetSizer(pVBox);
		do {
			PatternBrowser *pCtrl= new PatternBrowser(pPanel, pPageInfo->Reference());
			pVBox->Add(pCtrl, wxSizerFlags(1).Expand());
			_pPatternBrowser = pCtrl;
		} while (0);
	} while (0);
	do { // panel on right (main)
		wxPanel *pPanel = new wxPanel(this);
		wxBoxSizer *pHBox = new wxBoxSizer(wxHORIZONTAL);
		pPanel->SetSizer(pHBox);
		_pMainWindow = pPanel;
		do {
			PatternEditor *pCtrl = new PatternEditor(
				pPanel, pPageInfo->GetPatternInfoOwner().front()->Reference());
			pHBox->Add(pCtrl, wxSizerFlags(1).Expand());
			_pPatternEditor = pCtrl;
			_pPatternEditor->AddListener(this);
		} while (0);
		do {
			wxSlider *pCtrl = new wxSlider(pPanel, wxID_ANY, 0, 0, 100,
										   wxDefaultPosition, wxDefaultSize, wxSL_VERTICAL);
			pHBox->Add(pCtrl, wxSizerFlags().Expand());
		} while (0);
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
	wxLayoutAlgorithm().LayoutWindow(this, _pMainWindow);
	event.Skip();
}

void PanelMain::Page::OnSashDrag_Left(wxSashEvent &event)
{
	if (event.GetDragStatus() == wxSASH_STATUS_OUT_OF_RANGE) return;
	_pSashVert->SetDefaultSize(wxSize(event.GetDragRect().width, 1000));
	wxLayoutAlgorithm().LayoutWindow(this, _pMainWindow);
    Refresh();
}

void PanelMain::Page::NotifyPatternModified()
{
	_pPatternBrowser->Refresh();
}

