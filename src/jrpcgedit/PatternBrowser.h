//=============================================================================
// PatternBrowser.h
//=============================================================================
#ifndef __JRPCGEDIT_PATTERNBROWSER_H__
#define __JRPCGEDIT_PATTERNBROWSER_H__

#include "PageInfo.h"

//-----------------------------------------------------------------------------
// PatternBrowser
//-----------------------------------------------------------------------------
class PatternBrowser : public wxPanel {
public:
	class Item {
	private:
		AutoPtr<PatternInfo> _pPatternInfo;
		bool _selectedFlag;
		wxRect _rcWhole;
	public:
		inline Item(PatternInfo *pPatternInfo) : _pPatternInfo(pPatternInfo), _selectedFlag(false) {}
		inline void SetSelectedFlag(bool selectedFlag) { _selectedFlag = selectedFlag; }
		inline bool GetSelectedFlag() { return _selectedFlag; }
		PatternInfo *GetPatternInfo() { return _pPatternInfo.get(); }
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
public:
	PatternBrowser(wxWindow *pParent, PageInfo *pPageInfo);
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
