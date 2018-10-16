//=============================================================================
// ErrorDialog.cpp
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// ErrorDialog
//-----------------------------------------------------------------------------
ErrorDialog::ErrorDialog(wxWindow *pParent) :
	wxDialog(pParent, wxID_ANY, wxT("Error"), wxDefaultPosition, wxSize(400, 300))
{
	wxBoxSizer *pVBox = new wxBoxSizer(wxVERTICAL);
	SetSizer(pVBox);
	do {
		wxTextCtrl *pCtrl = new wxTextCtrl(this, wxID_ANY, ErrorLog::MakeResultText().c_str());
		pVBox->Add(pCtrl, wxSizerFlags(1).Expand());
	} while (0);
	pVBox->Add(CreateSeparatedButtonSizer(wxOK), wxSizerFlags().Align(wxALIGN_CENTRE));
}
