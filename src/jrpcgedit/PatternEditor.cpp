//=============================================================================
// PatternEditor.cpp
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// PatternEditor
//-----------------------------------------------------------------------------
PatternEditor::PatternEditor(wxWindow *pParent) : wxPanel(pParent, wxID_ANY)
{
}

//-----------------------------------------------------------------------------
// Event Handler
//-----------------------------------------------------------------------------
wxBEGIN_EVENT_TABLE(PatternEditor, wxPanel)
	EVT_ERASE_BACKGROUND(PatternEditor::OnEraseBackground)
	EVT_PAINT(PatternEditor::OnPaint)
	EVT_SET_FOCUS(PatternEditor::OnSetFocus)
	EVT_KILL_FOCUS(PatternEditor::OnKillFocus)
	EVT_MOTION(PatternEditor::OnMotion)
	EVT_LEFT_DOWN(PatternEditor::OnLeftDown)
	EVT_LEFT_UP(PatternEditor::OnLeftUp)
	EVT_LEFT_DCLICK(PatternEditor::OnLeftDClick)
	EVT_RIGHT_DOWN(PatternEditor::OnRightDown)
	EVT_RIGHT_UP(PatternEditor::OnRightUp)
	EVT_LEAVE_WINDOW(PatternEditor::OnLeaveWindow)
	EVT_KEY_DOWN(PatternEditor::OnKeyDown)
	EVT_KEY_UP(PatternEditor::OnKeyUp)
wxEND_EVENT_TABLE()

void PatternEditor::OnEraseBackground(wxEraseEvent &event)
{
	// nothing to do
}

void PatternEditor::OnPaint(wxPaintEvent &event)
{
	wxPaintDC dc(this);
	dc.SetBackground(*wxWHITE_BRUSH);
	dc.Clear();
}

void PatternEditor::OnSetFocus(wxFocusEvent &event)
{
}

void PatternEditor::OnKillFocus(wxFocusEvent &event)
{
}

void PatternEditor::OnMotion(wxMouseEvent &event)
{
}

void PatternEditor::OnLeftDown(wxMouseEvent &event)
{
}

void PatternEditor::OnLeftUp(wxMouseEvent &event)
{
}

void PatternEditor::OnLeftDClick(wxMouseEvent &event)
{
}

void PatternEditor::OnRightDown(wxMouseEvent &event)
{
}

void PatternEditor::OnRightUp(wxMouseEvent &event)
{
}

void PatternEditor::OnLeaveWindow(wxMouseEvent &event)
{
}

void PatternEditor::OnKeyDown(wxKeyEvent &event)
{
}

void PatternEditor::OnKeyUp(wxKeyEvent &event)
{
}
