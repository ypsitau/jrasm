//=============================================================================
// PCGBrowser.h
//=============================================================================
#ifndef __JRPCGEDIT_PCGBROWSER_H__
#define __JRPCGEDIT_PCGBROWSER_H__

#include "PCGPageInfo.h"

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
private:
	AutoPtr<PCGPageInfo> _pPCGPageInfo;
	wxBrush _brushBg;
	wxBrush _brushSelected;
	wxBitmap _bmpBtnUp;
	wxBitmap _bmpBtnDown;
	wxBitmap _bmpBtnDelete;
	wxRect _rcLabel;
	wxRect _rcBtnUp;
	wxRect _rcBtnDown;
	wxRect _rcBtnDelete;
	ListenerList _listenerList;
private:
	static int _dotSize;
	static int _htBmpMin;
	static int _mgnTop;
	static int _mgnBottom;
	static int _mgnLeft;
	static int _mgnRight;
public:
	PCGBrowser(wxWindow *pParent, PCGPageInfo *pPCGPageInfo);
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
