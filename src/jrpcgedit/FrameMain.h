//=============================================================================
// FrameMain.h
//=============================================================================
#ifndef __JRPCGEDIT_FRAMEMAIN_H__
#define __JRPCGEDIT_FRAMEMAIN_H__

#include "PanelMain.h"

//-----------------------------------------------------------------------------
// FrameMain
//-----------------------------------------------------------------------------
class FrameMain: public wxFrame {
public:
	enum {
		ID_Hello = 1
	};
private:
	PanelMain *_pPanelMain;
public:
	FrameMain(wxWindow *pParent, const wxString &title, const wxPoint &pos, const wxSize &size);
private:
    wxDECLARE_EVENT_TABLE();
    void OnHello(wxCommandEvent &event);
    void OnExit(wxCommandEvent &event);
    void OnAbout(wxCommandEvent &event);
};

#endif
