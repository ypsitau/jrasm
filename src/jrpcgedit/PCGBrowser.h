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
	class Item {
	private:
		AutoPtr<PCGInfo> _pPCGInfo;
		bool _selectedFlag;
		wxRect _rcWhole;
	public:
		inline Item(PCGInfo *pPCGInfo) : _pPCGInfo(pPCGInfo), _selectedFlag(false) {}
		inline void SetSelectedFlag(bool selectedFlag) { _selectedFlag = selectedFlag; }
		inline bool GetSelectedFlag() { return _selectedFlag; }
		PCGInfo *GetPCGInfo() { return _pPCGInfo.get(); }
		inline bool HitWhole(const wxPoint &pt) const { return _rcWhole.Contains(pt); }
		inline void SetRectWhole(int x, int y, int width, int height) {
			_rcWhole = wxRect(x, y, width, height);
		}
	};
	class ItemList : public std::vector<Item *> {
	};
	class ItemOwner : public ItemList {
	public:
		~ItemOwner();
		void Clear();
	};
private:
	AutoPtr<PageInfo> _pPageInfo;
	wxBrush _brushBg;
	wxBrush _brushSelected;
	ItemOwner _itemOwner;
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
