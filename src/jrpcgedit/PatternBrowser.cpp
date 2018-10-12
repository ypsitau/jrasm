//=============================================================================
// PatternBrowser.cpp
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// PatternBrowser
//-----------------------------------------------------------------------------
PatternBrowser::PatternBrowser(wxWindow *pParent) :
	wxPanel(pParent, wxID_ANY, wxDefaultPosition, wxDefaultSize,
			wxTAB_TRAVERSAL | wxBORDER_SUNKEN)
{
}

//-----------------------------------------------------------------------------
// Event Handler for PatternBrowser
//-----------------------------------------------------------------------------
wxBEGIN_EVENT_TABLE(PatternBrowser, wxPanel)
	EVT_ERASE_BACKGROUND(PatternBrowser::OnEraseBackground)
	EVT_SIZE(PatternBrowser::OnSize)
	EVT_PAINT(PatternBrowser::OnPaint)
	EVT_SET_FOCUS(PatternBrowser::OnSetFocus)
	EVT_KILL_FOCUS(PatternBrowser::OnKillFocus)
	EVT_MOTION(PatternBrowser::OnMotion)
	EVT_LEFT_DOWN(PatternBrowser::OnLeftDown)
	EVT_LEFT_UP(PatternBrowser::OnLeftUp)
	EVT_LEFT_DCLICK(PatternBrowser::OnLeftDClick)
	EVT_RIGHT_DOWN(PatternBrowser::OnRightDown)
	EVT_RIGHT_UP(PatternBrowser::OnRightUp)
	EVT_LEAVE_WINDOW(PatternBrowser::OnLeaveWindow)
	EVT_KEY_DOWN(PatternBrowser::OnKeyDown)
	EVT_KEY_UP(PatternBrowser::OnKeyUp)
wxEND_EVENT_TABLE()

void PatternBrowser::OnEraseBackground(wxEraseEvent &event)
{
	// nothing to do
}

void PatternBrowser::OnSize(wxSizeEvent &event)
{
	event.Skip();
}

void PatternBrowser::OnPaint(wxPaintEvent &event)
{
	wxPaintDC dc(this);
	dc.SetBackground(*wxWHITE_BRUSH);
	dc.Clear();
/*
	wxSize sizeClient = GetClientSize();
	wxSize sizeBmp = _pBmpMatrix->GetSize();
	_rcMatrix = wxRect(
		(sizeClient.GetWidth() - sizeBmp.GetWidth()) / 2,
		(sizeClient.GetHeight() - sizeBmp.GetHeight()) / 2,
		sizeBmp.GetWidth(), sizeBmp.GetHeight());
	dc.DrawBitmap(*_pBmpMatrix, _rcMatrix.x, _rcMatrix.y);
*/
}

void PatternBrowser::OnSetFocus(wxFocusEvent &event)
{
}

void PatternBrowser::OnKillFocus(wxFocusEvent &event)
{
}

void PatternBrowser::OnMotion(wxMouseEvent &event)
{
}

void PatternBrowser::OnLeftDown(wxMouseEvent &event)
{
}

void PatternBrowser::OnLeftUp(wxMouseEvent &event)
{
}

void PatternBrowser::OnLeftDClick(wxMouseEvent &event)
{
}

void PatternBrowser::OnRightDown(wxMouseEvent &event)
{
}

void PatternBrowser::OnRightUp(wxMouseEvent &event)
{
}

void PatternBrowser::OnLeaveWindow(wxMouseEvent &event)
{
}

void PatternBrowser::OnKeyDown(wxKeyEvent &event)
{
}

void PatternBrowser::OnKeyUp(wxKeyEvent &event)
{
}
