//=============================================================================
// PatternBrowser.cpp
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// PatternBrowser
//-----------------------------------------------------------------------------
PatternBrowser::PatternBrowser(wxWindow *pParent, PageInfo *pPageInfo) :
	wxPanel(pParent, wxID_ANY, wxDefaultPosition, wxDefaultSize,
			wxTAB_TRAVERSAL | wxBORDER_SUNKEN), _pPageInfo(pPageInfo),
	_brushBg(wxColour("white"), wxBRUSHSTYLE_SOLID),
	_brushSelected(wxColour("light blue"), wxBRUSHSTYLE_SOLID)
{
	for (auto pPatternInfo : pPageInfo->GetPatternInfoOwner()) {
		_itemOwner.push_back(new Item(pPatternInfo->Reference()));
	}
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
	wxRect rcClient = GetClientRect();
	const int wdItemMin = 48;
	const int htItemMin = 48;
	const int mgnTop = 4, mgnBottom = 4;
	const int mgnLeft = 8;
	wxPaintDC dc(this);
	dc.SetBackground(_brushBg);
	dc.Clear();
	int sizeDot = 4;
	int x = mgnLeft, y = 0;
	dc.SetPen(*wxTRANSPARENT_PEN);
	dc.SetBrush(_brushSelected);
	bool firstFlag = true;
	for (auto pItem : _itemOwner) {
		wxBitmap &bmp = pItem->GetPatternInfo()->MakeBitmap(sizeDot);
		int htItem = mgnTop + bmp.GetHeight() + mgnBottom;
		int yBmp = y + mgnTop;
		if (htItem < htItemMin) {
			yBmp = y + (htItemMin - htItem) / 2;
			htItem = htItemMin;
		}
		pItem->SetRectWhole(0, y, rcClient.GetWidth(), htItem);
		if (pItem->GetSelectedFlag()) {
			dc.DrawRectangle(0, y, rcClient.GetWidth(), htItem);
			firstFlag = false;
		}
		dc.DrawBitmap(bmp, x, yBmp);
		//dc.DrawLine(0, y + htItem, rcClient.GetRight(), y + htItem);
		y += htItem;
	}
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
	for (auto pItem : _itemOwner) {
		pItem->SetSelectedFlag(pItem->HitWhole(event.GetPosition()));
	}
	Refresh();
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

//-----------------------------------------------------------------------------
// PatternBrowser::Item
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// PatternBrowser::ItemList
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// PatternBrowser::ItemOwner
//-----------------------------------------------------------------------------
PatternBrowser::ItemOwner::~ItemOwner()
{
	Clear();
}

void PatternBrowser::ItemOwner::Clear()
{
	for (auto pItem : *this) {
		delete pItem;
	}
	clear();
}

