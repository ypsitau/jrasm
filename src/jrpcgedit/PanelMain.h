//=============================================================================
// PanelMain.h
//=============================================================================
#ifndef __JRPCGEDIT_PANELMAIN_H__
#define __JRPCGEDIT_PANELMAIN_H__

#include "PatternBrowser.h"
#include "PatternEditor.h"

//-----------------------------------------------------------------------------
// PanelMain
//-----------------------------------------------------------------------------
class PanelMain : public wxPanel {
public:
	class Page : public wxPanel, public PatternEditor::Listener {
	public:
		enum {
			ID_SASH_Left = 1000,
		};
	private:
		wxSashLayoutWindow *_pSashVert;
		PatternEditor *_pPatternEditor;
		PatternBrowser *_pPatternBrowser;
		wxWindow *_pMainWindow;
		AutoPtr<PageInfo> _pPageInfo;
	public:
		Page(wxWindow *pParent, PageInfo *pPageInfo);
	private:
		wxDECLARE_EVENT_TABLE();
		void OnSize(wxSizeEvent &event);
		void OnSashDrag_Left(wxSashEvent &event);
	public:
		// virtual function of PatternEditor::Listener
		virtual void NotifyPatternModified();
	};
private:
	AutoPtr<Document> _pDocument;
public:
	PanelMain(wxWindow *pParent);
private:
    wxDECLARE_EVENT_TABLE();
};

#endif
