//=============================================================================
// PatternEditor.cpp
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// PatternEditor
//-----------------------------------------------------------------------------
PatternEditor::PatternEditor(wxWindow *pParent) :
	wxPanel(pParent, wxID_ANY), _sizeDot(24), _nDotsX(16), _nDotsY(16)
{
	PrepareMatrix();
	UpdateMatrix();
}

void PatternEditor::PrepareMatrix()
{
	_dotTbl.reset(new UInt8 [_nDotsX * _nDotsY]);
	_pBmpMatrix.reset(new wxBitmap(_sizeDot * _nDotsX + 1, _sizeDot * _nDotsY + 1));
	::memset(_dotTbl.get(), 0x00, _nDotsX * _nDotsY);
}

void PatternEditor::UpdateMatrix()
{
	wxMemoryDC dc(*_pBmpMatrix);
	dc.SetBackground(*wxWHITE_BRUSH);
	dc.Clear();
	int xLeft = _sizeDot * _nDotsX;
	int yBottom = _sizeDot * _nDotsY;
	dc.SetPen(*wxBLACK_PEN);
	for (int iDotX = 0; iDotX <= _nDotsX; iDotX++) {
		int x = iDotX * _sizeDot;
		dc.DrawLine(x, 0, x, yBottom);
	}
	for (int iDotY = 0; iDotY <= _nDotsY; iDotY++) {
		int y = iDotY * _sizeDot;
		dc.DrawLine(0, y, xLeft, y);
	}
	dc.SetPen(*wxWHITE_PEN);
	dc.SetBrush(*wxBLACK_BRUSH);
	for (int iDotY = 0; iDotY < _nDotsY; iDotY++) {
		int y = iDotY * _sizeDot;
		for (int iDotX = 0; iDotX < _nDotsX; iDotX++) {
			int x = iDotX * _sizeDot;
			UInt8 dot = _dotTbl[iDotX + iDotY * _nDotsX];
			if (dot != 0) {
				dc.DrawRectangle(x, y, _sizeDot + 1, _sizeDot + 1);
			}
		}
	}
	
}

//-----------------------------------------------------------------------------
// Event Handler for PatternEditor
//-----------------------------------------------------------------------------
wxBEGIN_EVENT_TABLE(PatternEditor, wxPanel)
	EVT_ERASE_BACKGROUND(PatternEditor::OnEraseBackground)
	EVT_SIZE(PatternEditor::OnSize)
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

void PatternEditor::OnSize(wxSizeEvent &event)
{
	event.Skip();
}

void PatternEditor::OnPaint(wxPaintEvent &event)
{
	wxSize sizeClient = GetClientSize();
	wxPaintDC dc(this);
	dc.SetBackground(*wxWHITE_BRUSH);
	dc.Clear();
	wxSize sizeBmp = _pBmpMatrix->GetSize();
	_rcMatrix = wxRect(
		(sizeClient.GetWidth() - sizeBmp.GetWidth()) / 2,
		(sizeClient.GetHeight() - sizeBmp.GetHeight()) / 2,
		sizeBmp.GetWidth(), sizeBmp.GetHeight());
	dc.DrawBitmap(*_pBmpMatrix, _rcMatrix.x, _rcMatrix.y);
}

void PatternEditor::OnSetFocus(wxFocusEvent &event)
{
}

void PatternEditor::OnKillFocus(wxFocusEvent &event)
{
}

void PatternEditor::OnMotion(wxMouseEvent &event)
{
	wxPoint pt = event.GetPosition();
	if (event.Dragging() && _rcMatrix.Contains(event.GetPosition())) {
		int iDotX = (pt.x - _rcMatrix.x) / _sizeDot;
		int iDotY = (pt.y - _rcMatrix.y) / _sizeDot;
		UInt8 &dot = _dotTbl[iDotX + iDotY * _nDotsX];
		dot = 1;
		UpdateMatrix();
		Refresh();
	}
}

void PatternEditor::OnLeftDown(wxMouseEvent &event)
{
	wxPoint pt = event.GetPosition();
	if (_rcMatrix.Contains(event.GetPosition())) {
		int iDotX = (pt.x - _rcMatrix.x) / _sizeDot;
		int iDotY = (pt.y - _rcMatrix.y) / _sizeDot;
		UInt8 &dot = _dotTbl[iDotX + iDotY * _nDotsX];
		dot = 1;
		UpdateMatrix();
		Refresh();
	}
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
