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
		wxNotebook *pNotebook = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNB_TOP);
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
	const PCGInfo *pPCGInfoSelected = *_pPageInfo->GetPCGInfoOwner().FindSelected();
	do {
		wxSashLayoutWindow *pSash = new wxSashLayoutWindow(
			this, ID_SASH_Vert, wxDefaultPosition, wxDefaultSize,
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
			PCGBrowser *pCtrl= new PCGBrowser(pPanel, _pPageInfo->Reference());
			pVBox->Add(pCtrl, wxSizerFlags(1).Expand());
			_pPCGBrowser = pCtrl;
			_pPCGBrowser->AddListener(this);
		} while (0);
		do {
			wxBoxSizer *pHBox = new wxBoxSizer(wxHORIZONTAL);
			pVBox->Add(pHBox, wxSizerFlags().Expand());
			do {
				wxButton *pCtrl = new wxButton(pPanel, ID_BTN_NewPCG, wxT("New PCG"));
				pHBox->Add(pCtrl, wxSizerFlags(1).Expand());
			} while (0);
		} while (0);
	} while (0);
	do { // panel on right (main)
		wxPanel *pPanel = new wxPanel(this);
		wxBoxSizer *pVBox = new wxBoxSizer(wxVERTICAL);
		pPanel->SetSizer(pVBox);
		do {
			wxBoxSizer *pHBox = new wxBoxSizer(wxHORIZONTAL);
			pVBox->Add(pHBox, wxSizerFlags(1).Expand());
			_pMainWindow = pPanel;
			do {
				PCGEditor *pCtrl = new PCGEditor(pPanel, pPCGInfoSelected->Reference());
				pHBox->Add(pCtrl, wxSizerFlags(1).Expand());
				_pPCGEditor = pCtrl;
				_pPCGEditor->AddListener(this);
			} while (0);
			do {
				wxBoxSizer *pVBox = new wxBoxSizer(wxVERTICAL);
				pHBox->Add(pVBox, wxSizerFlags().Expand());
			} while (0);
		} while (0);
		do {
			wxBoxSizer *pHBox = new wxBoxSizer(wxHORIZONTAL);
			pVBox->Add(pHBox, wxSizerFlags().Expand().Border(wxTOP, 2));
			do {
				wxSlider *pCtrl = new wxSlider(pPanel, ID_SLIDER_DotSize, 4, 4, 24,
											   wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL);
				pHBox->Add(pCtrl, wxSizerFlags(1).Expand());
				_pSlider_DotSize = pCtrl;
			} while (0);
			do {
				wxStaticText *pCtrl = new wxStaticText(pPanel, wxID_ANY, wxT("Size"));
				pHBox->Add(pCtrl, wxSizerFlags().Align(wxALIGN_CENTRE_VERTICAL).Border(wxLEFT, 8));
			} while (0);
			do {
				wxSpinCtrl *pCtrl = new wxSpinCtrl(
					pPanel, ID_SPIN_Width, wxEmptyString, wxDefaultPosition, wxSize(50, -1),
					wxSP_ARROW_KEYS | wxALIGN_CENTRE_HORIZONTAL, 1, 16, 1);
				pHBox->Add(pCtrl, wxSizerFlags().Align(wxALIGN_CENTRE_VERTICAL).Border(wxLEFT, 4));
				_pSpin_Width = pCtrl;
			} while (0);
			do {
				wxStaticText *pCtrl = new wxStaticText(pPanel, wxID_ANY, wxT("x"));
				pHBox->Add(pCtrl, wxSizerFlags().Align(wxALIGN_CENTRE_VERTICAL).Border(wxLEFT, 2));
			} while (0);
			do {
				wxSpinCtrl *pCtrl = new wxSpinCtrl(
					pPanel, ID_SPIN_Height, wxEmptyString, wxDefaultPosition, wxSize(50, -1),
					wxSP_ARROW_KEYS | wxALIGN_CENTRE_HORIZONTAL, 1, 16, 1);
				pHBox->Add(pCtrl, wxSizerFlags().Align(wxALIGN_CENTRE_VERTICAL).Border(wxLEFT, 4));
				_pSpin_Height = pCtrl;
			} while (0);
#if 0
			pHBox->AddSpacer(4);
			for (int i = 0; i < 8; i++) {
				wxBitmap bmp(16, 16);
				wxButton *pCtrl = new wxButton(pPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(32, 32));
				pCtrl->SetBitmap(bmp);
				pHBox->Add(pCtrl, wxSizerFlags().Expand());
			}
#endif
		} while (0);
	} while (0);
	_pSlider_DotSize->SetValue(pPCGInfoSelected->GetDotSizeEditor());
	_pSpin_Width->SetValue(pPCGInfoSelected->GetDotNumX() / 8);
	_pSpin_Height->SetValue(pPCGInfoSelected->GetDotNumY() / 8);
}

//-----------------------------------------------------------------------------
// Event Handler for PanelMain::Page
//-----------------------------------------------------------------------------
wxBEGIN_EVENT_TABLE(PanelMain::Page, wxPanel)
	EVT_SIZE(PanelMain::Page::OnSize)
	EVT_SASH_DRAGGED(ID_SASH_Vert, PanelMain::Page::OnSashDrag_Vert)
	EVT_COMMAND_SCROLL(ID_SLIDER_DotSize, PanelMain::Page::OnSlider_DotSize)
	EVT_BUTTON(ID_BTN_NewPCG, PanelMain::Page::OnButton_NewPCG)
	EVT_SPINCTRL(ID_SPIN_Width, PanelMain::Page::OnSpin_WidthHeight)
	EVT_SPINCTRL(ID_SPIN_Height, PanelMain::Page::OnSpin_WidthHeight)
wxEND_EVENT_TABLE()

void PanelMain::Page::OnSize(wxSizeEvent &event)
{
	wxLayoutAlgorithm().LayoutWindow(this, _pMainWindow);
	event.Skip();
}

void PanelMain::Page::OnSashDrag_Vert(wxSashEvent &event)
{
	if (event.GetDragStatus() == wxSASH_STATUS_OUT_OF_RANGE) return;
	_pSashVert->SetDefaultSize(wxSize(event.GetDragRect().width, 1000));
	wxLayoutAlgorithm().LayoutWindow(this, _pMainWindow);
    Refresh();
}

void PanelMain::Page::OnSlider_DotSize(wxScrollEvent &event)
{
	_pPCGEditor->SetDotSize(event.GetPosition());
}

void PanelMain::Page::OnButton_NewPCG(wxCommandEvent &event)
{
	_pPageInfo->NewPCGInfo();
	_pPCGBrowser->Refresh();
}

void PanelMain::Page::OnSpin_WidthHeight(wxSpinEvent &event)
{
	AutoPtr<PCGInfo> pPCGInfoSelected((*_pPageInfo->GetPCGInfoOwner().FindSelected())->Reference());
	pPCGInfoSelected->ChangeDotNum(_pSpin_Width->GetValue() * 8, _pSpin_Height->GetValue() * 8);
	_pPCGEditor->PrepareMatrix(true);
	_pPCGBrowser->Refresh();
}

void PanelMain::Page::NotifyPCGSelected(const PCGInfo *pPCGInfo)
{
	_pPCGEditor->SetPCGInfo(pPCGInfo->Reference());
	_pSlider_DotSize->SetValue(pPCGInfo->GetDotSizeEditor());
	_pSpin_Width->SetValue(pPCGInfo->GetDotNumX() / 8);
	_pSpin_Height->SetValue(pPCGInfo->GetDotNumY() / 8);
}

void PanelMain::Page::NotifyPCGModified()
{
	_pPCGBrowser->Refresh();
}
