//=============================================================================
// PatternEditor.h
//=============================================================================
#ifndef __PATTERNEDITOR_H__
#define __PATTERNEDITOR_H__

//-----------------------------------------------------------------------------
// PatternEditor
//-----------------------------------------------------------------------------
class PatternEditor : public wxPanel {
private:
	int _sizeDot;
	int _nDotsX;
	int _nDotsY;
	wxRect _rcMatrix;
	std::unique_ptr<UInt8[]> _dotTbl;
	std::unique_ptr<wxBitmap> _pBmpMatrix;
public:
	PatternEditor(wxWindow *pParent);
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
