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
	_sizeDot(20), _dotPosXCur(0), _dotPosYCur(0), _pPCGInfo(pPCGInfo),
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
	UpdateMatrix(true);
}

void PCGEditor::SetPCGInfo(PCGInfo *pPCGInfo)
{
	_pPCGInfo.reset(pPCGInfo);
	PrepareMatrix();
	UpdateMatrix(true);
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
	int xLeft = DotPosXToMatrixCoord(0);
	int xRight = DotPosXToMatrixCoord(nDotsX);
	int yTop = DotPosYToMatrixCoord(0);
	int yBottom = DotPosYToMatrixCoord(nDotsY);
	dc.SetPen(_penBorder);
	dc.SetBrush(_brushMatrix);
	dc.DrawRectangle(xLeft, yTop, xRight - xLeft + 1, yBottom - yTop + 1);
	for (int dotPosX = 1; dotPosX < nDotsX; dotPosX++) {
		int x = DotPosXToMatrixCoord(dotPosX);
		if (dotPosX % 8 == 0) {
			dc.SetPen(_penGridHL);
			dc.DrawLine(x, yTop - _mgnTop, x, yBottom + _mgnBottom);
		} else {
			dc.SetPen(_penGrid);
			dc.DrawLine(x, yTop, x, yBottom);
		}
	}
	for (int dotPosY = 1; dotPosY < nDotsY; dotPosY++) {
		int y = DotPosYToMatrixCoord(dotPosY);
		if (dotPosY % 8 == 0) {
			dc.SetPen(_penGridHL);
			dc.DrawLine(xLeft - _mgnLeft, y, xRight + _mgnRight, y);
		} else {
			dc.SetPen(_penGrid);
			dc.DrawLine(xLeft, y, xRight, y);
		}
	}
	dc.SetBrush(brushDot);
	for (int dotPosY = 0; dotPosY < nDotsY; dotPosY++) {
		int y = DotPosYToMatrixCoord(dotPosY);
		for (int dotPosX = 0; dotPosX < nDotsX; dotPosX++) {
			int x = DotPosXToMatrixCoord(dotPosX);
			if (_pPCGInfo->GetDot(dotPosX, dotPosY)) {
				dc.SetPen(*wxTRANSPARENT_PEN);
				dc.DrawRectangle(x + 1, y + 1, _sizeDot - 1, _sizeDot - 1);
			}
		}
	}
	if (refreshFlag) Refresh();
}

void PCGEditor::PutDot(int dotPosX, int dotPosY, bool data)
{
	_pPCGInfo->PutDot(_dotPosXCur, _dotPosYCur, data);
	_listenerList.NotifyPCGModified();
	UpdateMatrix(true);
}

wxRect PCGEditor::DotPosToCursorRect(int dotPosX, int dotPosY)
{
	return wxRect(
		_rcMatrix.x + DotPosXToMatrixCoord(dotPosX), _rcMatrix.y + DotPosYToMatrixCoord(dotPosY),
		_sizeDot + 1, _sizeDot + 1);
}

void PCGEditor::PointToDotPos(const wxPoint &pt, int *pDotPosX, int *pDotPosY) const
{
	int dotPosX = (pt.x - _rcMatrix.x - _mgnLeft) / _sizeDot;
	int dotPosY = (pt.y - _rcMatrix.y - _mgnTop) / _sizeDot;
	if (dotPosX < 0) dotPosX = 0;
	if (dotPosX > _pPCGInfo->GetDotPosXMax()) dotPosX = _pPCGInfo->GetDotPosXMax();
	if (dotPosY < 0) dotPosY = 0;
	if (dotPosY > _pPCGInfo->GetDotPosYMax()) dotPosX = _pPCGInfo->GetDotPosYMax();
	*pDotPosX = dotPosX, *pDotPosY = dotPosY;
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
	dc.DrawRectangle(DotPosToCursorRect(_dotPosXCur, _dotPosYCur));
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
		PointToDotPos(pt, &_dotPosXCur, &_dotPosYCur);
		PutDot(_dotPosXCur, _dotPosYCur, true);
	}
}

void PCGEditor::OnLeftDown(wxMouseEvent &event)
{
	wxPoint pt = event.GetPosition();
	if (_rcMatrix.Contains(event.GetPosition())) {
		PointToDotPos(pt, &_dotPosXCur, &_dotPosYCur);
		PutDot(_dotPosXCur, _dotPosYCur, true);
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
		if (_dotPosXCur > 0) {
			_dotPosXCur--;
		}
		Refresh();
	} else if (keyCode == WXK_RIGHT) {
		if (_dotPosXCur < _pPCGInfo->GetNDotsX() - 1) {
			_dotPosXCur++;
		}
		Refresh();
	} else if (keyCode == WXK_UP) {
		if (_dotPosYCur > 0) {
			_dotPosYCur--;
		}
		Refresh();
	} else if (keyCode == WXK_DOWN) {
		if (_dotPosYCur < _pPCGInfo->GetNDotsY() - 1) {
			_dotPosYCur++;
		}
		Refresh();
	} else if (keyCode == WXK_SPACE || keyCode == 'Z') {
		PutDot(_dotPosXCur, _dotPosYCur, true);
		
	} else if (keyCode == WXK_DELETE || keyCode == WXK_BACK || keyCode == 'X') {
		PutDot(_dotPosXCur, _dotPosYCur, false);
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
