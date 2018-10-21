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
private:
	PanelMain *_pPanelMain;
	AutoPtr<Document> _pDocument;
public:
	FrameMain(wxWindow *pParent, const wxPoint &pos, const wxSize &size);
	void UpdateTitle();
	void OpenFile();
	void SaveFile();
	void SaveFileAs();
private:
    wxDECLARE_EVENT_TABLE();
	void OnOpen(wxCommandEvent &event);
	void OnSave(wxCommandEvent &event);
	void OnSaveAs(wxCommandEvent &event);
	void OnExit(wxCommandEvent &event);
	void OnAbout(wxCommandEvent &event);
};

#endif
