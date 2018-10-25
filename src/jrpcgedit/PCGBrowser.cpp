//=============================================================================
// PCGBrowser.cpp
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// PCGBrowser
//-----------------------------------------------------------------------------
int PCGBrowser::_dotSize = 4;
int PCGBrowser::_htBmpMin = 48;
int PCGBrowser::_mgnTop = 4;
int PCGBrowser::_mgnBottom = 4;
int PCGBrowser::_mgnLeft = 8;
int PCGBrowser::_mgnRight = 8;

PCGBrowser::PCGBrowser(wxWindow *pParent, PCGPageInfo *pPCGPageInfo) :
	wxPanel(pParent, wxID_ANY, wxDefaultPosition, wxDefaultSize,
			wxTAB_TRAVERSAL | wxBORDER_SUNKEN | wxWANTS_CHARS | wxVSCROLL),
	_pPCGPageInfo(pPCGPageInfo),
	_brushBg(wxColour("grey"), wxBRUSHSTYLE_SOLID),
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
	wxBufferedPaintDC dc(this);
	wxRect rcClient = GetClientRect();
	dc.SetBackground(_brushBg);
	dc.Clear();
	int y = 0;
	_rcLabel = _rcBtnUp = _rcBtnDown = _rcBtnDelete = wxRect();
	const PCGDataInfoOwner &pcgInfoOwner = _pPCGPageInfo->GetPCGDataInfoOwner();
	for (PCGDataInfoOwner::const_iterator ppPCGDataInfo = pcgInfoOwner.begin();
		 ppPCGDataInfo != pcgInfoOwner.end(); ppPCGDataInfo++) {
		PCGDataInfo *pPCGDataInfo = *ppPCGDataInfo;
		wxBitmap &bmp = pPCGDataInfo->MakeBitmapForBrowser(_dotSize);
		int xBmp = rcClient.GetRight() - (bmp.GetWidth() + _mgnRight);
		int yBmp = y + _mgnTop;
		int htBmp = bmp.GetHeight();
		if (htBmp < _htBmpMin) {
			yBmp += (_htBmpMin - htBmp) / 2;
			htBmp = _htBmpMin;
		}
		int htItem = _mgnTop + htBmp + _mgnBottom;
		pPCGDataInfo->SetRectItem(0, y, rcClient.GetWidth(), htItem);
		wxString strLabel(pPCGDataInfo->GetSymbol());
		wxSize sizeLabel = dc.GetTextExtent(strLabel);
		if (pPCGDataInfo->GetSelectedFlag()) {
			dc.SetBrush(_brushSelected);
			dc.SetPen(*wxTRANSPARENT_PEN);
			dc.DrawRectangle(0, y, rcClient.GetWidth(), htItem);
			if (HasFocus()) {
				if (pcgInfoOwner.size() > 1) {
					_rcBtnDelete = wxRect(
						xBmp - _bmpBtnDelete.GetWidth() - 4, y + (htItem - _bmpBtnDelete.GetHeight()) / 2,
						_bmpBtnDelete.GetWidth(), _bmpBtnDelete.GetHeight());
					dc.DrawBitmap(_bmpBtnDelete, _rcBtnDelete.GetTopLeft());
				}
				if (!pcgInfoOwner.IsFirst(ppPCGDataInfo)) {
					_rcBtnUp = wxRect(
						2, y + 2,
						_bmpBtnUp.GetWidth(), _bmpBtnDown.GetHeight());
					dc.DrawBitmap(_bmpBtnUp, _rcBtnUp.GetTopLeft());
				}
				if (!pcgInfoOwner.IsLast(ppPCGDataInfo)) {
					_rcBtnDown = wxRect(
						2, y + htItem - 2 - _bmpBtnDown.GetHeight(),
						_bmpBtnUp.GetWidth(), _bmpBtnDown.GetHeight());
					dc.DrawBitmap(_bmpBtnDown, _rcBtnDown.GetTopLeft());
				}
			}
		}
		int xLabel = _mgnLeft, yLabel = y + (htItem - sizeLabel.GetHeight()) / 2;
#if 0
		if (HasFocus()) {
			_rcLabel = wxRect(
				xLabel, yLabel, xBmp - _bmpBtnDelete.GetWidth() - 4, sizeLabel.GetHeight());
		}
#endif
		dc.DrawText(strLabel, xLabel, yLabel);
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
	if (_rcLabel.Contains(pt)) {
		SetCursor(wxCursor(wxCURSOR_IBEAM));
	} else if (_rcBtnUp.Contains(pt)) {
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
	const wxPoint &pt = event.GetPosition();
	PCGDataInfoOwner &pcgInfoOwner = _pPCGPageInfo->GetPCGDataInfoOwner();
	PCGDataInfo *pPCGDataInfoSelected = nullptr;
	for (auto pPCGDataInfo : pcgInfoOwner) {
		if (pPCGDataInfo->GetRectItem().Contains(pt)) {
			pPCGDataInfoSelected = pPCGDataInfo;
			break;
		}
	}
	bool refreshFlag = false;
	PCGDataInfo *pPCGDataInfoToNotify = nullptr;
	if (pPCGDataInfoSelected != nullptr) {
		for (auto pPCGDataInfo : pcgInfoOwner) {
			pPCGDataInfo->SetSelectedFlag(false);
		}
		pPCGDataInfoSelected->SetSelectedFlag(true);
		refreshFlag = true;
		pPCGDataInfoToNotify = pPCGDataInfoSelected;
	}
	if (_rcBtnUp.Contains(pt)) {
		refreshFlag = pcgInfoOwner.MoveSelectionUp();
	} else if (_rcBtnDown.Contains(pt)) {
		refreshFlag = pcgInfoOwner.MoveSelectionDown();
	} else if (_rcBtnDelete.Contains(pt)) {
		refreshFlag = pcgInfoOwner.DeleteSelection();
		pPCGDataInfoToNotify = *pcgInfoOwner.FindSelected();
	}
	if (refreshFlag) Refresh();
	if (pPCGDataInfoToNotify != nullptr) _listenerList.NotifyPCGSelected(pPCGDataInfoToNotify);
	SetFocus();
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
	const PCGDataInfoOwner &pcgInfoOwner = _pPCGPageInfo->GetPCGDataInfoOwner();
	PCGDataInfoOwner::const_iterator ppPCGDataInfo = pcgInfoOwner.begin();
	for ( ; ppPCGDataInfo != pcgInfoOwner.end(); ppPCGDataInfo++) {
		PCGDataInfo *pPCGDataInfo = *ppPCGDataInfo;
		if (pPCGDataInfo->GetSelectedFlag()) break;
	}
	PCGDataInfo *pPCGDataInfoTo = nullptr;
	if (pcgInfoOwner.empty()) {
		// nothing to do
	} else if (keyCode == WXK_UP) {
		if (ppPCGDataInfo == pcgInfoOwner.begin()) {
			// nothing to do
		} else if (ppPCGDataInfo == pcgInfoOwner.end()) {
			pPCGDataInfoTo = pcgInfoOwner.back();
		} else {
			(*ppPCGDataInfo)->SetSelectedFlag(false);
			pPCGDataInfoTo = *(ppPCGDataInfo - 1);
		}
	} else if (keyCode == WXK_DOWN) {
		if (ppPCGDataInfo == pcgInfoOwner.end()) {
			pPCGDataInfoTo = pcgInfoOwner.front();
		} else if (ppPCGDataInfo + 1 == pcgInfoOwner.end()) {
			// nothing to do
		} else {
			(*ppPCGDataInfo)->SetSelectedFlag(false);
			pPCGDataInfoTo = *(ppPCGDataInfo + 1);
		}
	}
	if (pPCGDataInfoTo != nullptr) {
		pPCGDataInfoTo->SetSelectedFlag(true);
		Refresh();
		_listenerList.NotifyPCGSelected(pPCGDataInfoTo);
	}
	
}

void PCGBrowser::OnKeyUp(wxKeyEvent &event)
{
}

//-----------------------------------------------------------------------------
// PCGBrowser::ListenerList
//-----------------------------------------------------------------------------
void PCGBrowser::ListenerList::NotifyPCGSelected(const PCGDataInfo *pPCGDataInfo)
{
	for (auto pListener : *this) pListener->NotifyPCGSelected(pPCGDataInfo);
}
