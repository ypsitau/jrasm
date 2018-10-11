//=============================================================================
// ImageBrowser.cpp
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// ImageBrowser
//-----------------------------------------------------------------------------
ImageBrowser::ImageBrowser(wxWindow *pParent) : wxListCtrl(
	pParent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxLC_VIRTUAL)
{
}
