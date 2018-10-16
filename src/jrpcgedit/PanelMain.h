//=============================================================================
// PanelMain.h
//=============================================================================
#ifndef __JRPCGEDIT_PANELMAIN_H__
#define __JRPCGEDIT_PANELMAIN_H__

#include "PCGBrowser.h"
#include "PCGEditor.h"

//-----------------------------------------------------------------------------
// PanelMain
//-----------------------------------------------------------------------------
class PanelMain : public wxPanel {
public:
	class Page : public wxPanel, public PCGBrowser::Listener, public PCGEditor::Listener {
	public:
		enum {
			ID_SASH_Vert = 1000,
			ID_SLIDER_DotSize,
			ID_BTN_NewPCG,
			ID_SPIN_Width,
			ID_SPIN_Height,
		};
	private:
		wxSashLayoutWindow *_pSashVert;
		PCGEditor *_pPCGEditor;
		PCGBrowser *_pPCGBrowser;
		wxWindow *_pMainWindow;
		wxSlider *_pSlider_DotSize;
		wxSpinCtrl *_pSpin_Width;
		wxSpinCtrl *_pSpin_Height;
		AutoPtr<PCGPageInfo> _pPCGPageInfo;
	public:
		Page(wxWindow *pParent, PCGPageInfo *pPCGPageInfo);
	private:
		wxDECLARE_EVENT_TABLE();
		void OnSize(wxSizeEvent &event);
		void OnSashDrag_Vert(wxSashEvent &event);
		void OnSlider_DotSize(wxScrollEvent &event);
		void OnButton_NewPCG(wxCommandEvent &event);
		void OnSpin_WidthHeight(wxSpinEvent &event);
	public:
		// virtual function of PCGBrowser::Listener
		virtual void NotifyPCGSelected(const PCGInfo *pPCGInfo);
		// virtual function of PCGEditor::Listener
		virtual void NotifyPCGModified();
	};
private:
	AutoPtr<Document> _pDocument;
public:
	PanelMain(wxWindow *pParent, Document *pDocument);
private:
    wxDECLARE_EVENT_TABLE();
};

#endif
