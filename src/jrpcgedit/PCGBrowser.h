//=============================================================================
// PCGBrowser.h
//=============================================================================
#ifndef __JRPCGEDIT_PCGBROWSER_H__
#define __JRPCGEDIT_PCGBROWSER_H__

#include "PageInfo.h"

//-----------------------------------------------------------------------------
// PCGBrowser
//-----------------------------------------------------------------------------
class PCGBrowser : public wxPanel {
public:
	class Listener {
	public:
		virtual void NotifyPCGSelected(const PCGInfo *pPCGInfo) = 0;
	};
	class ListenerList : public std::vector<Listener *> {
	public:
		void NotifyPCGSelected(const PCGInfo *pPCGInfo);
	};
public:
	enum {
		_sizeDot = 4,
		_htBmpMin = 48,
		_mgnTop = 4,
		_mgnBottom = 4,
		_mgnLeft = 4,
		_mgnRight = 4,
	};	
private:
	AutoPtr<PageInfo> _pPageInfo;
	wxBrush _brushBg;
	wxBrush _brushSelected;
	wxBitmap _bmpBtnUp;
	wxBitmap _bmpBtnDown;
	wxBitmap _bmpBtnDelete;
	wxRect _rcBtnUp;
	wxRect _rcBtnDown;
	wxRect _rcBtnDelete;
	ListenerList _listenerList;
public:
	PCGBrowser(wxWindow *pParent, PageInfo *pPageInfo);
	inline void AddListener(Listener *pListener) { _listenerList.push_back(pListener); }
private:
    wxDECLARE_EVENT_TABLE();
	void OnEraseBackground(wxEraseEvent &event);
	void OnSize(wxSizeEvent &event);
	void OnPaint(wxPaintEvent &event);
	void OnSetFocus(wxFocusEvent &event);
	void OnKillFocus(wxFocusEvent &event);
	void OnMotion(wxMouseEvent &event);
	void OnLeftDown(wxMouseEvent &event);
	void OnLeftUp(wxMouseEvent &event);
	void OnLeftDClick(wxMouseEvent &event);
	void OnRightDown(wxMouseEvent &event);
	void OnRightUp(wxMouseEvent &event);
	void OnLeaveWindow(wxMouseEvent &event);
	void OnKeyDown(wxKeyEvent &event);
	void OnKeyUp(wxKeyEvent &event);
};

#endif
