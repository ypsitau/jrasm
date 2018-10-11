//=============================================================================
// PanelMain.h
//=============================================================================
#ifndef __PANELMAIN_H__
#define __PANELMAIN_H__

#include "ImageBrowser.h"
#include "PatternEditor.h"

//-----------------------------------------------------------------------------
// PanelMain
//-----------------------------------------------------------------------------
class PanelMain : public wxPanel {
public:
	class Page : public wxPanel {
	public:
		enum {
			ID_SASH_Left = 1000,
		};
	private:
		wxSashLayoutWindow *_pSashLeft;
		PatternEditor *_pPatternEditor;
		ImageBrowser *_pImageBrowser;
	public:
		Page(wxWindow *pParent);
	private:
		wxDECLARE_EVENT_TABLE();
		void OnSize(wxSizeEvent &event);
		void OnSashDrag_Left(wxSashEvent &event);
	};
public:
	PanelMain(wxWindow *pParent);
private:
    wxDECLARE_EVENT_TABLE();
};

#endif
