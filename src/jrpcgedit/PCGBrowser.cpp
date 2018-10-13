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
	_brushSelected(wxColour("light blue"), wxBRUSHSTYLE_SOLID)
{
	for (auto pPCGInfo : pPageInfo->GetPCGInfoOwner()) {
		_itemOwner.push_back(new Item(pPCGInfo->Reference()));
	}
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
		wxBitmap &bmp = pItem->GetPCGInfo()->MakeBitmap(sizeDot);
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

void PCGBrowser::OnSetFocus(wxFocusEvent &event)
{
}

void PCGBrowser::OnKillFocus(wxFocusEvent &event)
{
}

void PCGBrowser::OnMotion(wxMouseEvent &event)
{
}

void PCGBrowser::OnLeftDown(wxMouseEvent &event)
{
	for (auto pItem : _itemOwner) {
		pItem->SetSelectedFlag(pItem->HitWhole(event.GetPosition()));
	}
	Refresh();
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
}

void PCGBrowser::OnKeyUp(wxKeyEvent &event)
{
}

//-----------------------------------------------------------------------------
// PCGBrowser::Item
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// PCGBrowser::ItemList
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// PCGBrowser::ItemOwner
//-----------------------------------------------------------------------------
PCGBrowser::ItemOwner::~ItemOwner()
{
	Clear();
}

void PCGBrowser::ItemOwner::Clear()
{
	for (auto pItem : *this) {
		delete pItem;
	}
	clear();
}

