//=============================================================================
// PatternEditor.h
//=============================================================================
#ifndef __PATTERNEDITOR_H__
#define __PATTERNEDITOR_H__

//-----------------------------------------------------------------------------
// PatternEditor
//-----------------------------------------------------------------------------
class PatternEditor : public wxPanel {
public:
	PatternEditor(wxWindow *pParent);
private:
    wxDECLARE_EVENT_TABLE();
	void OnEraseBackground(wxEraseEvent &event);
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
