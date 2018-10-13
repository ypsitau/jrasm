//=============================================================================
// PatternEditor.cpp
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// PatternEditor
//-----------------------------------------------------------------------------
PatternEditor::PatternEditor(wxWindow *pParent, PatternInfo *pPatternInfo) :
	wxPanel(pParent, wxID_ANY, wxDefaultPosition, wxDefaultSize,
			wxTAB_TRAVERSAL | wxBORDER_SUNKEN),
	_sizeDot(20), _iDotXCur(0), _iDotYCur(0), _pPatternInfo(pPatternInfo),
	_penBorder(wxColour("light grey"), 1, wxPENSTYLE_SOLID),
	_penGrid(wxColour("light grey"), 1, wxPENSTYLE_DOT),
	_penGridHL(wxColour("grey"), 1, wxPENSTYLE_SOLID),
	_brushBg(wxColour("light blue"), wxBRUSHSTYLE_SOLID),
	_brushMatrix(wxColour("black"), wxBRUSHSTYLE_SOLID)
{
	PrepareMatrix();
	UpdateMatrix(false);
}

void PatternEditor::SetSizeDot(int sizeDot)
{
	_sizeDot = sizeDot;
	PrepareMatrix();
}

void PatternEditor::PrepareMatrix()
{
	_pBmpMatrix.reset
		(new wxBitmap(
			_mgnLeft + _sizeDot * _pPatternInfo->GetNDotsX() + 1 + _mgnRight,
			_mgnTop + _sizeDot * _pPatternInfo->GetNDotsY() + 1 + _mgnBottom));
}

void PatternEditor::UpdateMatrix(bool refreshFlag)
{
	wxMemoryDC dc(*_pBmpMatrix);
	wxBrush brushDot(wxColour("white"), wxBRUSHSTYLE_SOLID);
	dc.SetBackground(_brushBg);
	dc.Clear();
	int nDotsX = _pPatternInfo->GetNDotsX();
	int nDotsY = _pPatternInfo->GetNDotsY();
	int xLeft = DotXToMatrixCoord(0);
	int xRight = DotXToMatrixCoord(nDotsX);
	int yTop = DotYToMatrixCoord(0);
	int yBottom = DotYToMatrixCoord(nDotsY);
	dc.SetPen(_penBorder);
	dc.SetBrush(_brushMatrix);
	dc.DrawRectangle(xLeft, yTop, xRight - xLeft + 1, yBottom - yTop + 1);
	for (int iDotX = 1; iDotX < nDotsX; iDotX++) {
		int x = DotXToMatrixCoord(iDotX);
		if (iDotX % 8 == 0) {
			dc.SetPen(_penGridHL);
			dc.DrawLine(x, yTop - _mgnTop, x, yBottom + _mgnBottom);
		} else {
			dc.SetPen(_penGrid);
			dc.DrawLine(x, yTop, x, yBottom);
		}
	}
	for (int iDotY = 1; iDotY < nDotsY; iDotY++) {
		int y = DotYToMatrixCoord(iDotY);
		if (iDotY % 8 == 0) {
			dc.SetPen(_penGridHL);
			dc.DrawLine(xLeft - _mgnLeft, y, xRight + _mgnRight, y);
		} else {
			dc.SetPen(_penGrid);
			dc.DrawLine(xLeft, y, xRight, y);
		}
	}
	dc.SetBrush(brushDot);
	for (int iDotY = 0; iDotY < nDotsY; iDotY++) {
		int y = DotYToMatrixCoord(iDotY);
		for (int iDotX = 0; iDotX < nDotsX; iDotX++) {
			int x = DotXToMatrixCoord(iDotX);
			if (_pPatternInfo->GetDot(iDotX, iDotY)) {
				dc.SetPen(*wxTRANSPARENT_PEN);
				dc.DrawRectangle(x + 1, y + 1, _sizeDot - 1, _sizeDot - 1);
			}
		}
	}
	if (refreshFlag) Refresh();
}

void PatternEditor::PutDot(int iDotX, int iDotY, bool data)
{
	_pPatternInfo->PutDot(_iDotXCur, _iDotYCur, data);
	_listenerList.NotifyPatternModified();
	UpdateMatrix(true);
}

wxRect PatternEditor::DotXYToCursorRect(int iDotX, int iDotY)
{
	return wxRect(
		_rcMatrix.x + DotXToMatrixCoord(iDotX), _rcMatrix.y + DotYToMatrixCoord(iDotY),
		_sizeDot + 1, _sizeDot + 1);
}

void PatternEditor::PointToDotXY(const wxPoint &pt, int *piDotX, int *piDotY) const
{
	int iDotX = (pt.x - _rcMatrix.x - _mgnLeft) / _sizeDot;
	int iDotY = (pt.y - _rcMatrix.y - _mgnTop) / _sizeDot;
	if (iDotX < 0) iDotX = 0;
	if (iDotX > _pPatternInfo->GetDotXMax()) iDotX = _pPatternInfo->GetDotXMax();
	if (iDotY < 0) iDotY = 0;
	if (iDotY > _pPatternInfo->GetDotYMax()) iDotX = _pPatternInfo->GetDotYMax();
	*piDotX = iDotX, *piDotY = iDotY;
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
	dc.SetBackground(_brushBg);
	dc.Clear();
	wxSize sizeBmp = _pBmpMatrix->GetSize();
	_rcMatrix = wxRect(
		(sizeClient.GetWidth() - sizeBmp.GetWidth()) / 2,
		(sizeClient.GetHeight() - sizeBmp.GetHeight()) / 2,
		sizeBmp.GetWidth(), sizeBmp.GetHeight());
	dc.DrawBitmap(*_pBmpMatrix, _rcMatrix.x, _rcMatrix.y);
	wxPen penCursor(wxColour(HasFocus()? "red" : "pink"), 2, wxPENSTYLE_SOLID);
	dc.SetPen(penCursor);
	dc.SetBrush(*wxTRANSPARENT_BRUSH);
	dc.DrawRectangle(DotXYToCursorRect(_iDotXCur, _iDotYCur));
}

void PatternEditor::OnSetFocus(wxFocusEvent &event)
{
	Refresh();
}

void PatternEditor::OnKillFocus(wxFocusEvent &event)
{
	Refresh();
}

void PatternEditor::OnMotion(wxMouseEvent &event)
{
	wxPoint pt = event.GetPosition();
	if (event.Dragging() && _rcMatrix.Contains(event.GetPosition())) {
		PointToDotXY(pt, &_iDotXCur, &_iDotYCur);
		PutDot(_iDotXCur, _iDotYCur, true);
	}
}

void PatternEditor::OnLeftDown(wxMouseEvent &event)
{
	wxPoint pt = event.GetPosition();
	if (_rcMatrix.Contains(event.GetPosition())) {
		PointToDotXY(pt, &_iDotXCur, &_iDotYCur);
		PutDot(_iDotXCur, _iDotYCur, true);
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
	int keyCode = event.GetKeyCode();
	if (keyCode == WXK_LEFT) {
		if (_iDotXCur > 0) {
			_iDotXCur--;
		}
		Refresh();
	} else if (keyCode == WXK_RIGHT) {
		if (_iDotXCur < _pPatternInfo->GetNDotsX() - 1) {
			_iDotXCur++;
		}
		Refresh();
	} else if (keyCode == WXK_UP) {
		if (_iDotYCur > 0) {
			_iDotYCur--;
		}
		Refresh();
	} else if (keyCode == WXK_DOWN) {
		if (_iDotYCur < _pPatternInfo->GetNDotsY() - 1) {
			_iDotYCur++;
		}
		Refresh();
	} else if (keyCode == WXK_SPACE || keyCode == 'Z') {
		PutDot(_iDotXCur, _iDotYCur, true);
		
	} else if (keyCode == WXK_DELETE || keyCode == WXK_BACK || keyCode == 'X') {
		PutDot(_iDotXCur, _iDotYCur, false);
	}
}

void PatternEditor::OnKeyUp(wxKeyEvent &event)
{
}

//-----------------------------------------------------------------------------
// PatternEditor::ListenerList
//-----------------------------------------------------------------------------
void PatternEditor::ListenerList::NotifyPatternModified()
{
	for (auto pListener : *this) pListener->NotifyPatternModified();
}
