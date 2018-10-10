//=============================================================================
// FrameMain.h
//=============================================================================
#ifndef __FRAMEMAIN_H__
#define __FRAMEMAIN_H__

#include "Common.h"

//-----------------------------------------------------------------------------
// FrameMain
//-----------------------------------------------------------------------------
class FrameMain: public wxFrame
{
public:
	enum {
		ID_Hello = 1
	};
public:
	FrameMain(const wxString &title, const wxPoint &pos, const wxSize &size);
private:
    void OnHello(wxCommandEvent &event);
    void OnExit(wxCommandEvent &event);
    void OnAbout(wxCommandEvent &event);
    wxDECLARE_EVENT_TABLE();
};

#endif
