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
	enum {
		_mgnLeft = 8,
		_mgnRight = 8,
		_mgnTop = 8,
		_mgnBottom = 8,
	};
private:
	int _sizeDot;
	int _iDotXCur, _iDotYCur;
	wxRect _rcMatrix;
	AutoPtr<PatternInfo> _pPatternInfo;
	std::unique_ptr<wxBitmap> _pBmpMatrix;
	ListenerList _listenerList;
	wxPen _penBorder;
	wxPen _penGrid;
	wxPen _penGridHL;
	wxBrush _brushBg;
	wxBrush _brushMatrix;
public:
	PatternEditor(wxWindow *pParent, PatternInfo *pPatternInfo);
	inline void AddListener(Listener *pListener) { _listenerList.push_back(pListener); }
	inline int DotXToMatrixCoord(int iDotX) const { return _mgnLeft + iDotX * _sizeDot; }
	inline int DotYToMatrixCoord(int iDotY) const { return _mgnTop + iDotY * _sizeDot; }
	inline int GetSizeDot() const { return _sizeDot; }
	void SetSizeDot(int sizeDot);
	void PrepareMatrix();
	void UpdateMatrix(bool refreshFlag);
	void PutDot(int iDotX, int iDotY, bool data);
	wxRect DotXYToCursorRect(int iDotX, int iDotY);
	void PointToDotXY(const wxPoint &pt, int *piDotX, int *piDotY) const;
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
