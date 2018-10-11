//=============================================================================
// PanelMain.h
//=============================================================================
#ifndef __PANELMAIN_H__
#define __PANELMAIN_H__

//-----------------------------------------------------------------------------
// PanelMain
//-----------------------------------------------------------------------------
class PanelMain : public wxPanel {
public:
	class Page : public wxPanel {
	public:
		Page(wxWindow *pParent);
	private:
		wxDECLARE_EVENT_TABLE();
	};
public:
	PanelMain(wxWindow *pParent);
private:
    wxDECLARE_EVENT_TABLE();
};

#endif
