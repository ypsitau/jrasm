//=============================================================================
// ImageBrowser.cpp
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// ImageBrowser
//-----------------------------------------------------------------------------
ImageBrowser::ImageBrowser(wxWindow *pParent) : wxPanel(pParent, wxID_ANY)
{
}

//-----------------------------------------------------------------------------
// Event Handler for ImageBrowser
//-----------------------------------------------------------------------------
wxBEGIN_EVENT_TABLE(ImageBrowser, wxPanel)
wxEND_EVENT_TABLE()