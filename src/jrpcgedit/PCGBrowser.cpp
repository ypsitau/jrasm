//=============================================================================
// PCGBrowser.cpp
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// PCGBrowser
//-----------------------------------------------------------------------------
PCGBrowser::PCGBrowser(wxWindow *pParent, PageInfo *pPageInfo) :
	wxPanel(pParent, wxID_ANY, wxDefaultPosition, wxDefaultSize,
			wxTAB_TRAVERSAL | wxBORDER_SUNKEN), _pPageInfo(pPageInfo),
	_brushBg(wxColour("white"), wxBRUSHSTYLE_SOLID),
	_brushSelected(wxColour("light blue"), wxBRUSHSTYLE_SOLID),
	_bmpBtnUp(64, 12), _bmpBtnDown(64, 12), _bmpBtnDelete(16, 16)
{
	
}

//-----------------------------------------------------------------------------
// Event Handler for PCGBrowser
//-----------------------------------------------------------------------------
wxBEGIN_EVENT_TABLE(PCGBrowser, wxPanel)
	EVT_ERASE_BACKGROUND(PCGBrowser::OnEraseBackground)
	EVT_SIZE(PCGBrowser::OnSize)
	EVT_PAINT(PCGBrowser::OnPaint)
	EVT_SET_FOCUS(PCGBrowser::OnSetFocus)
	EVT_KILL_FOCUS(PCGBrowser::OnKillFocus)
	EVT_MOTION(PCGBrowser::OnMotion)
	EVT_LEFT_DOWN(PCGBrowser::OnLeftDown)
	EVT_LEFT_UP(PCGBrowser::OnLeftUp)
	EVT_LEFT_DCLICK(PCGBrowser::OnLeftDClick)
	EVT_RIGHT_DOWN(PCGBrowser::OnRightDown)
	EVT_RIGHT_UP(PCGBrowser::OnRightUp)
	EVT_LEAVE_WINDOW(PCGBrowser::OnLeaveWindow)
	EVT_KEY_DOWN(PCGBrowser::OnKeyDown)
	EVT_KEY_UP(PCGBrowser::OnKeyUp)
wxEND_EVENT_TABLE()

void PCGBrowser::OnEraseBackground(wxEraseEvent &event)
{
	// nothing to do
}

void PCGBrowser::OnSize(wxSizeEvent &event)
{
	event.Skip();
}

void PCGBrowser::OnPaint(wxPaintEvent &event)
{
	wxRect rcClient = GetClientRect();
	wxPaintDC dc(this);
	dc.SetBackground(_brushBg);
	dc.Clear();
	int y = 0;
	_rcBtnUp = _rcBtnDown = _rcBtnDelete = wxRect();
	for (auto pPCGInfo : _pPageInfo->GetPCGInfoOwner()) {
		wxBitmap &bmp = pPCGInfo->MakeBitmap(_sizeDot);
		int xBmp = rcClient.GetRight() - (bmp.GetWidth() + _mgnRight);
		int yBmp = y + _mgnTop;
		int htBmp = bmp.GetHeight();
		if (htBmp < _htBmpMin) {
			yBmp += (_htBmpMin - htBmp) / 2;
			htBmp = _htBmpMin;
		}
		int htItem = _mgnTop + htBmp + _mgnBottom;
		pPCGInfo->SetRectItem(0, y, rcClient.GetWidth(), htItem);
		wxString strLabel(pPCGInfo->GetSymbol());
		wxSize sizeLabel = dc.GetTextExtent(strLabel);
		if (pPCGInfo->GetSelectedFlag()) {
			dc.SetBrush(_brushSelected);
			dc.SetPen(*wxTRANSPARENT_PEN);
			dc.DrawRectangle(0, y, rcClient.GetWidth(), htItem);
			if (HasFocus()) {
				_rcBtnDelete = wxRect(
					xBmp - _bmpBtnDelete.GetWidth() - 4, y + (htItem - _bmpBtnDelete.GetHeight()) / 2,
					_bmpBtnDelete.GetWidth(), _bmpBtnDelete.GetHeight());
				dc.DrawBitmap(_bmpBtnDelete, _rcBtnDelete.GetTopLeft());
				if (!_pPageInfo->GetPCGInfoOwner().IsFirst(pPCGInfo)) {
					_rcBtnUp = wxRect(
						2, y + 2,
						_bmpBtnUp.GetWidth(), _bmpBtnDown.GetHeight());
					dc.DrawBitmap(_bmpBtnUp, _rcBtnUp.GetTopLeft());
				}
				if (!_pPageInfo->GetPCGInfoOwner().IsLast(pPCGInfo)) {
					_rcBtnDown = wxRect(
						2, y + htItem - 2 - _bmpBtnDown.GetHeight(),
						_bmpBtnUp.GetWidth(), _bmpBtnDown.GetHeight());
					dc.DrawBitmap(_bmpBtnDown, _rcBtnDown.GetTopLeft());
				}
			}
		}
		dc.DrawText(strLabel, _mgnLeft, y + (htItem - sizeLabel.GetHeight()) / 2);
		dc.DrawBitmap(bmp, xBmp, yBmp);
		y += htItem;
	}
}

void PCGBrowser::OnSetFocus(wxFocusEvent &event)
{
	Refresh();
}

void PCGBrowser::OnKillFocus(wxFocusEvent &event)
{
	Refresh();
}

void PCGBrowser::OnMotion(wxMouseEvent &event)
{
	const wxPoint &pt = event.GetPosition();
	if (_rcBtnUp.Contains(pt)) {
		SetCursor(wxCursor(wxCURSOR_HAND));
	} else if (_rcBtnDown.Contains(pt)) {
		SetCursor(wxCursor(wxCURSOR_HAND));
	} else if (_rcBtnDelete.Contains(pt)) {
		SetCursor(wxCursor(wxCURSOR_HAND));
	} else {
		SetCursor(wxCursor(wxCURSOR_ARROW));
	}
}

void PCGBrowser::OnLeftDown(wxMouseEvent &event)
{
	PCGInfo *pPCGInfoSelected = nullptr;
	for (auto pPCGInfo : _pPageInfo->GetPCGInfoOwner()) {
		if (pPCGInfo->GetRectItem().Contains(event.GetPosition())) {
			pPCGInfoSelected = pPCGInfo;
			break;
		}
	}
	if (pPCGInfoSelected != nullptr) {
		for (auto pPCGInfo : _pPageInfo->GetPCGInfoOwner()) {
			pPCGInfo->SetSelectedFlag(false);
		}
		pPCGInfoSelected->SetSelectedFlag(true);
		Refresh();
		_listenerList.NotifyPCGSelected(pPCGInfoSelected);
	}
}

void PCGBrowser::OnLeftUp(wxMouseEvent &event)
{
}

void PCGBrowser::OnLeftDClick(wxMouseEvent &event)
{
}

void PCGBrowser::OnRightDown(wxMouseEvent &event)
{
}

void PCGBrowser::OnRightUp(wxMouseEvent &event)
{
}

void PCGBrowser::OnLeaveWindow(wxMouseEvent &event)
{
}

void PCGBrowser::OnKeyDown(wxKeyEvent &event)
{
	int keyCode = event.GetKeyCode();
	const PCGInfoOwner &pcgInfoOwner = _pPageInfo->GetPCGInfoOwner();
	PCGInfoOwner::const_iterator ppPCGInfo = pcgInfoOwner.begin();
	for ( ; ppPCGInfo != pcgInfoOwner.end(); ppPCGInfo++) {
		PCGInfo *pPCGInfo = *ppPCGInfo;
		if (pPCGInfo->GetSelectedFlag()) break;
	}
	PCGInfo *pPCGInfoTo = nullptr;
	if (pcgInfoOwner.empty()) {
		// nothing to do
	} else if (keyCode == WXK_UP) {
		if (ppPCGInfo == pcgInfoOwner.begin()) {
			// nothing to do
		} else if (ppPCGInfo == pcgInfoOwner.end()) {
			pPCGInfoTo = pcgInfoOwner.back();
		} else {
			(*ppPCGInfo)->SetSelectedFlag(false);
			pPCGInfoTo = *(ppPCGInfo - 1);
		}
	} else if (keyCode == WXK_DOWN) {
		if (ppPCGInfo == pcgInfoOwner.end()) {
			pPCGInfoTo = pcgInfoOwner.front();
		} else if (ppPCGInfo + 1 == pcgInfoOwner.end()) {
			// nothing to do
		} else {
			(*ppPCGInfo)->SetSelectedFlag(false);
			pPCGInfoTo = *(ppPCGInfo + 1);
		}
	}
	if (pPCGInfoTo != nullptr) {
		pPCGInfoTo->SetSelectedFlag(true);
		Refresh();
		_listenerList.NotifyPCGSelected(pPCGInfoTo);
	}
	
}

void PCGBrowser::OnKeyUp(wxKeyEvent &event)
{
}

//-----------------------------------------------------------------------------
// PCGBrowser::ListenerList
//-----------------------------------------------------------------------------
void PCGBrowser::ListenerList::NotifyPCGSelected(const PCGInfo *pPCGInfo)
{
	for (auto pListener : *this) pListener->NotifyPCGSelected(pPCGInfo);
}
