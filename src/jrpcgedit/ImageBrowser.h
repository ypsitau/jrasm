//=============================================================================
// ImageBrowser.h
//=============================================================================
#ifndef __IMAGEBROWSER_H__
#define __IMAGEBROWSER_H__

//-----------------------------------------------------------------------------
// ImageBrowser
//-----------------------------------------------------------------------------
class ImageBrowser : public wxListCtrl {
public:
	ImageBrowser(wxWindow *pParent);
private:
    wxDECLARE_EVENT_TABLE();
};

#endif
