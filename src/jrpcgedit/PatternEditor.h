//=============================================================================
// PatternEditor.h
//=============================================================================
#ifndef __JRPCGEDIT_PATTERNEDITOR_H__
#define __JRPCGEDIT_PATTERNEDITOR_H__

#include "PatternInfo.h"

//-----------------------------------------------------------------------------
// PatternEditor
//-----------------------------------------------------------------------------
class PatternEditor : public wxPanel {
public:
	class Listener {
	public:
		virtual void NotifyPatternModified() = 0;
	};
	class ListenerList : public std::vector<Listener *> {
	public:
		void NotifyPatternModified();
	};
private:
	int _sizeDot;
	wxRect _rcMatrix;
	AutoPtr<PatternInfo> _pPatternInfo;
	std::unique_ptr<wxBitmap> _pBmpMatrix;
	ListenerList _listenerList;
public:
	PatternEditor(wxWindow *pParent, PatternInfo *pPatternInfo);
	inline void AddListener(Listener *pListener) { _listenerList.push_back(pListener); }
	void PrepareMatrix();
	void UpdateMatrix();
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
