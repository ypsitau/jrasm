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
	class Page : public wxPanel, public PCGEditor::Listener {
	public:
		enum {
			ID_SASH_Vert = 1000,
			ID_SLIDER,
			ID_BTN_NewPCG,
		};
	private:
		wxSashLayoutWindow *_pSashVert;
		PCGEditor *_pPCGEditor;
		PCGBrowser *_pPCGBrowser;
		wxWindow *_pMainWindow;
		AutoPtr<PageInfo> _pPageInfo;
	public:
		Page(wxWindow *pParent, PageInfo *pPageInfo);
	private:
		wxDECLARE_EVENT_TABLE();
		void OnSize(wxSizeEvent &event);
		void OnSashDrag_Vert(wxSashEvent &event);
		void OnCommandScroll_SLIDER(wxScrollEvent &event);
		void OnButton_NewPCG(wxCommandEvent &event);
	public:
		// virtual function of PCGEditor::Listener
		virtual void NotifyPCGModified();
	};
private:
	AutoPtr<Document> _pDocument;
public:
	PanelMain(wxWindow *pParent);
private:
    wxDECLARE_EVENT_TABLE();
};

#endif
