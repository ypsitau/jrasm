//=============================================================================
// PCGEditor.cpp
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// PCGEditor
//-----------------------------------------------------------------------------
int PCGEditor::_mgnLeft = 8;
int PCGEditor::_mgnRight = 8;
int PCGEditor::_mgnTop = 8;
int PCGEditor::_mgnBottom = 8;

PCGEditor::PCGEditor(wxWindow *pParent, PCGDataInfo *pPCGDataInfo) :
	wxPanel(pParent, wxID_ANY, wxDefaultPosition, wxDefaultSize,
			wxTAB_TRAVERSAL | wxBORDER_SUNKEN),
	_pPCGDataInfo(pPCGDataInfo),
	_penBorder(wxColour("light grey"), 1, wxPENSTYLE_SOLID),
	_penGrid(wxColour("light grey"), 1, wxPENSTYLE_DOT),
	_penGridHL(wxColour("grey"), 1, wxPENSTYLE_SOLID),
	_brushBg(wxColour("light blue"), wxBRUSHSTYLE_SOLID),
	_brushMatrix(wxColour("black"), wxBRUSHSTYLE_SOLID)
{
	PrepareMatrix(false);
}

void PCGEditor::SetDotSize(int dotSize)
{
	_pPCGDataInfo->SetDotSizeEditor(dotSize);
	PrepareMatrix(true);
}

void PCGEditor::SetPCGDataInfo(PCGDataInfo *pPCGDataInfo)
{
	_pPCGDataInfo.reset(pPCGDataInfo);
	PrepareMatrix(true);
}

void PCGEditor::PrepareMatrix(bool refreshFlag)
{
	int dotSize = _pPCGDataInfo->GetDotSizeEditor();
	_pBmpMatrix.reset
		(new wxBitmap(
			_mgnLeft + dotSize * _pPCGDataInfo->GetDotNumX() + 1 + _mgnRight,
			_mgnTop + dotSize * _pPCGDataInfo->GetDotNumY() + 1 + _mgnBottom));
	UpdateMatrix(refreshFlag);
}

void PCGEditor::UpdateMatrix(bool refreshFlag)
{
	wxMemoryDC dc(*_pBmpMatrix);
	wxBrush brushDot(wxColour("white"), wxBRUSHSTYLE_SOLID);
	dc.SetBackground(_brushBg);
	dc.Clear();
	int dotNumX = _pPCGDataInfo->GetDotNumX();
	int dotNumY = _pPCGDataInfo->GetDotNumY();
	int xLeft = DotPosXToMatrixCoord(0);
	int xRight = DotPosXToMatrixCoord(dotNumX);
	int yTop = DotPosYToMatrixCoord(0);
	int yBottom = DotPosYToMatrixCoord(dotNumY);
	dc.SetPen(_penBorder);
	dc.SetBrush(_brushMatrix);
	dc.DrawRectangle(xLeft, yTop, xRight - xLeft + 1, yBottom - yTop + 1);
	for (int dotPosX = 1; dotPosX < dotNumX; dotPosX++) {
		int x = DotPosXToMatrixCoord(dotPosX);
		if (dotPosX % 8 == 0) {
			dc.SetPen(_penGridHL);
			dc.DrawLine(x, yTop - _mgnTop, x, yBottom + _mgnBottom);
		} else {
			dc.SetPen(_penGrid);
			dc.DrawLine(x, yTop, x, yBottom);
		}
	}
	for (int dotPosY = 1; dotPosY < dotNumY; dotPosY++) {
		int y = DotPosYToMatrixCoord(dotPosY);
		if (dotPosY % 8 == 0) {
			dc.SetPen(_penGridHL);
			dc.DrawLine(xLeft - _mgnLeft, y, xRight + _mgnRight, y);
		} else {
			dc.SetPen(_penGrid);
			dc.DrawLine(xLeft, y, xRight, y);
		}
	}
	int dotSize = _pPCGDataInfo->GetDotSizeEditor();
	dc.SetBrush(brushDot);
	for (int dotPosY = 0; dotPosY < dotNumY; dotPosY++) {
		int y = DotPosYToMatrixCoord(dotPosY);
		for (int dotPosX = 0; dotPosX < dotNumX; dotPosX++) {
			int x = DotPosXToMatrixCoord(dotPosX);
			if (_pPCGDataInfo->GetDot(dotPosX, dotPosY)) {
				dc.SetPen(*wxTRANSPARENT_PEN);
				dc.DrawRectangle(x + 1, y + 1, dotSize - 1, dotSize - 1);
			}
		}
	}
	if (refreshFlag) Refresh();
}

void PCGEditor::PutDot(int dotPosX, int dotPosY, bool flag)
{
	_pPCGDataInfo->PutDot(_pPCGDataInfo->GetDotPosX(), _pPCGDataInfo->GetDotPosY(), flag);
	_listenerList.NotifyPCGModified();
	UpdateMatrix(true);
}

wxRect PCGEditor::DotPosToCursorRect(int dotPosX, int dotPosY)
{
	int dotSize = _pPCGDataInfo->GetDotSizeEditor();
	return wxRect(
		_rcMatrix.x + DotPosXToMatrixCoord(dotPosX), _rcMatrix.y + DotPosYToMatrixCoord(dotPosY),
		dotSize + 1, dotSize + 1);
}

void PCGEditor::PointToDotPos(const wxPoint &pt, int *pDotPosX, int *pDotPosY) const
{
	int dotSize = _pPCGDataInfo->GetDotSizeEditor();
	int dotPosX = (pt.x - _rcMatrix.x - _mgnLeft) / dotSize;
	int dotPosY = (pt.y - _rcMatrix.y - _mgnTop) / dotSize;
	if (dotPosX < 0) dotPosX = 0;
	if (dotPosX > _pPCGDataInfo->GetDotPosXMax()) dotPosX = _pPCGDataInfo->GetDotPosXMax();
	if (dotPosY < 0) dotPosY = 0;
	if (dotPosY > _pPCGDataInfo->GetDotPosYMax()) dotPosX = _pPCGDataInfo->GetDotPosYMax();
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
	dc.DrawRectangle(DotPosToCursorRect(_pPCGDataInfo->GetDotPosX(), _pPCGDataInfo->GetDotPosY()));
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
		int dotPosX, dotPosY;
		PointToDotPos(pt, &dotPosX, &dotPosY);
		_pPCGDataInfo->SetDotPos(dotPosX, dotPosY);
		PutDot(dotPosX, dotPosY, true);
	}
}

void PCGEditor::OnLeftDown(wxMouseEvent &event)
{
	wxPoint pt = event.GetPosition();
	if (_rcMatrix.Contains(event.GetPosition())) {
		int dotPosX, dotPosY;
		PointToDotPos(pt, &dotPosX, &dotPosY);
		_pPCGDataInfo->SetDotPos(dotPosX, dotPosY);
		PutDot(dotPosX, dotPosY, true);
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
	int dotPosX = _pPCGDataInfo->GetDotPosX();
	int dotPosY = _pPCGDataInfo->GetDotPosY();
	if (keyCode == WXK_LEFT) {
		if (dotPosX > 0) _pPCGDataInfo->SetDotPosX(dotPosX - 1);
		Refresh();
	} else if (keyCode == WXK_RIGHT) {
		if (dotPosX < _pPCGDataInfo->GetDotNumX() - 1) _pPCGDataInfo->SetDotPosX(dotPosX + 1);
		Refresh();
	} else if (keyCode == WXK_UP) {
		if (dotPosY > 0) _pPCGDataInfo->SetDotPosY(dotPosY - 1);
		Refresh();
	} else if (keyCode == WXK_DOWN) {
		if (dotPosY < _pPCGDataInfo->GetDotNumY() - 1) _pPCGDataInfo->SetDotPosY(dotPosY + 1);
		Refresh();
	} else if (keyCode == WXK_SPACE || keyCode == 'Z') {
		PutDot(dotPosX, dotPosY, true);
	} else if (keyCode == WXK_DELETE || keyCode == WXK_BACK || keyCode == 'X') {
		PutDot(dotPosX, dotPosY, false);
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
