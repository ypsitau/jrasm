//=============================================================================
// PCGEditor.cpp
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// PCGEditor
//-----------------------------------------------------------------------------
PCGEditor::PCGEditor(wxWindow *pParent, PCGInfo *pPCGInfo) :
	wxPanel(pParent, wxID_ANY, wxDefaultPosition, wxDefaultSize,
			wxTAB_TRAVERSAL | wxBORDER_SUNKEN),
	_sizeDot(20), _iDotXCur(0), _iDotYCur(0), _pPCGInfo(pPCGInfo),
	_penBorder(wxColour("light grey"), 1, wxPENSTYLE_SOLID),
	_penGrid(wxColour("light grey"), 1, wxPENSTYLE_DOT),
	_penGridHL(wxColour("grey"), 1, wxPENSTYLE_SOLID),
	_brushBg(wxColour("light blue"), wxBRUSHSTYLE_SOLID),
	_brushMatrix(wxColour("black"), wxBRUSHSTYLE_SOLID)
{
	PrepareMatrix();
	UpdateMatrix(false);
}

void PCGEditor::SetSizeDot(int sizeDot)
{
	_sizeDot = sizeDot;
	PrepareMatrix();
}

void PCGEditor::PrepareMatrix()
{
	_pBmpMatrix.reset
		(new wxBitmap(
			_mgnLeft + _sizeDot * _pPCGInfo->GetNDotsX() + 1 + _mgnRight,
			_mgnTop + _sizeDot * _pPCGInfo->GetNDotsY() + 1 + _mgnBottom));
}

void PCGEditor::UpdateMatrix(bool refreshFlag)
{
	wxMemoryDC dc(*_pBmpMatrix);
	wxBrush brushDot(wxColour("white"), wxBRUSHSTYLE_SOLID);
	dc.SetBackground(_brushBg);
	dc.Clear();
	int nDotsX = _pPCGInfo->GetNDotsX();
	int nDotsY = _pPCGInfo->GetNDotsY();
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
			if (_pPCGInfo->GetDot(iDotX, iDotY)) {
				dc.SetPen(*wxTRANSPARENT_PEN);
				dc.DrawRectangle(x + 1, y + 1, _sizeDot - 1, _sizeDot - 1);
			}
		}
	}
	if (refreshFlag) Refresh();
}

void PCGEditor::PutDot(int iDotX, int iDotY, bool data)
{
	_pPCGInfo->PutDot(_iDotXCur, _iDotYCur, data);
	_listenerList.NotifyPCGModified();
	UpdateMatrix(true);
}

wxRect PCGEditor::DotXYToCursorRect(int iDotX, int iDotY)
{
	return wxRect(
		_rcMatrix.x + DotXToMatrixCoord(iDotX), _rcMatrix.y + DotYToMatrixCoord(iDotY),
		_sizeDot + 1, _sizeDot + 1);
}

void PCGEditor::PointToDotXY(const wxPoint &pt, int *piDotX, int *piDotY) const
{
	int iDotX = (pt.x - _rcMatrix.x - _mgnLeft) / _sizeDot;
	int iDotY = (pt.y - _rcMatrix.y - _mgnTop) / _sizeDot;
	if (iDotX < 0) iDotX = 0;
	if (iDotX > _pPCGInfo->GetDotXMax()) iDotX = _pPCGInfo->GetDotXMax();
	if (iDotY < 0) iDotY = 0;
	if (iDotY > _pPCGInfo->GetDotYMax()) iDotX = _pPCGInfo->GetDotYMax();
	*piDotX = iDotX, *piDotY = iDotY;
}

//-----------------------------------------------------------------------------
// Event Handler for PCGEditor
//-----------------------------------------------------------------------------
wxBEGIN_EVENT_TABLE(PCGEditor, wxPanel)
	EVT_ERASE_BACKGROUND(PCGEditor::OnEraseBackground)
	EVT_SIZE(PCGEditor::OnSize)
	EVT_PAINT(PCGEditor::OnPaint)
	EVT_SET_FOCUS(PCGEditor::OnSetFocus)
	EVT_KILL_FOCUS(PCGEditor::OnKillFocus)
	EVT_MOTION(PCGEditor::OnMotion)
	EVT_LEFT_DOWN(PCGEditor::OnLeftDown)
	EVT_LEFT_UP(PCGEditor::OnLeftUp)
	EVT_LEFT_DCLICK(PCGEditor::OnLeftDClick)
	EVT_RIGHT_DOWN(PCGEditor::OnRightDown)
	EVT_RIGHT_UP(PCGEditor::OnRightUp)
	EVT_LEAVE_WINDOW(PCGEditor::OnLeaveWindow)
	EVT_KEY_DOWN(PCGEditor::OnKeyDown)
	EVT_KEY_UP(PCGEditor::OnKeyUp)
wxEND_EVENT_TABLE()

void PCGEditor::OnEraseBackground(wxEraseEvent &event)
{
	// nothing to do
}

void PCGEditor::OnSize(wxSizeEvent &event)
{
	event.Skip();
}

void PCGEditor::OnPaint(wxPaintEvent &event)
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

void PCGEditor::OnSetFocus(wxFocusEvent &event)
{
	Refresh();
}

void PCGEditor::OnKillFocus(wxFocusEvent &event)
{
	Refresh();
}

void PCGEditor::OnMotion(wxMouseEvent &event)
{
	wxPoint pt = event.GetPosition();
	if (event.Dragging() && _rcMatrix.Contains(event.GetPosition())) {
		PointToDotXY(pt, &_iDotXCur, &_iDotYCur);
		PutDot(_iDotXCur, _iDotYCur, true);
	}
}

void PCGEditor::OnLeftDown(wxMouseEvent &event)
{
	wxPoint pt = event.GetPosition();
	if (_rcMatrix.Contains(event.GetPosition())) {
		PointToDotXY(pt, &_iDotXCur, &_iDotYCur);
		PutDot(_iDotXCur, _iDotYCur, true);
	}
}

void PCGEditor::OnLeftUp(wxMouseEvent &event)
{
}

void PCGEditor::OnLeftDClick(wxMouseEvent &event)
{
}

void PCGEditor::OnRightDown(wxMouseEvent &event)
{
}

void PCGEditor::OnRightUp(wxMouseEvent &event)
{
}

void PCGEditor::OnLeaveWindow(wxMouseEvent &event)
{
}

void PCGEditor::OnKeyDown(wxKeyEvent &event)
{
	int keyCode = event.GetKeyCode();
	if (keyCode == WXK_LEFT) {
		if (_iDotXCur > 0) {
			_iDotXCur--;
		}
		Refresh();
	} else if (keyCode == WXK_RIGHT) {
		if (_iDotXCur < _pPCGInfo->GetNDotsX() - 1) {
			_iDotXCur++;
		}
		Refresh();
	} else if (keyCode == WXK_UP) {
		if (_iDotYCur > 0) {
			_iDotYCur--;
		}
		Refresh();
	} else if (keyCode == WXK_DOWN) {
		if (_iDotYCur < _pPCGInfo->GetNDotsY() - 1) {
			_iDotYCur++;
		}
		Refresh();
	} else if (keyCode == WXK_SPACE || keyCode == 'Z') {
		PutDot(_iDotXCur, _iDotYCur, true);
		
	} else if (keyCode == WXK_DELETE || keyCode == WXK_BACK || keyCode == 'X') {
		PutDot(_iDotXCur, _iDotYCur, false);
	}
}

void PCGEditor::OnKeyUp(wxKeyEvent &event)
{
}

//-----------------------------------------------------------------------------
// PCGEditor::ListenerList
//-----------------------------------------------------------------------------
void PCGEditor::ListenerList::NotifyPCGModified()
{
	for (auto pListener : *this) pListener->NotifyPCGModified();
}
