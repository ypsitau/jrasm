//=============================================================================
// PCGInfo.cpp
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// PCGInfo
//-----------------------------------------------------------------------------
PCGInfo::PCGInfo(const String &symbol, int dotNumX, int dotNumY) :
	_cntRef(1), _symbol(symbol), _sizeDot(0),
	_dotNumX(dotNumX), _dotNumY(dotNumY), _dotPosX(0), _dotPosY(0), _selectedFlag(false),
	_dotTbl(new bool [dotNumX * dotNumY])
{
	ClearAll();
}

wxBitmap &PCGInfo::MakeBitmap(int sizeDot)
{
	if (_pBitmap.get() == nullptr || _sizeDot != sizeDot) {
		_pBitmap.reset(new wxBitmap(sizeDot * _dotNumX, sizeDot * _dotNumY));
		_sizeDot = sizeDot;
	}
	wxBrush brushBg(wxColour("black"), wxBRUSHSTYLE_SOLID);
	wxBrush brushFg(wxColour("white"), wxBRUSHSTYLE_SOLID);
	wxMemoryDC dc(*_pBitmap);
	dc.SetBackground(brushBg);
	dc.Clear();
	dc.SetPen(wxNullPen);
	dc.SetBrush(brushFg);
	for (int dotPosY = 0; dotPosY < _dotNumY; dotPosY++) {
		int y = dotPosY * _sizeDot;
		for (int dotPosX = 0; dotPosX < _dotNumX; dotPosX++) {
			int x = dotPosX * _sizeDot;
			if (GetDot(dotPosX, dotPosY)) {
				dc.DrawRectangle(x, y, _sizeDot, _sizeDot);
			}
		}
	}
	return *_pBitmap;
}

//-----------------------------------------------------------------------------
// PCGInfoList
//-----------------------------------------------------------------------------
const PCGInfo *PCGInfoList::FindSelected() const
{
	for (auto pPCGInfo : *this) {
		if (pPCGInfo->GetSelectedFlag()) return pPCGInfo;
	}
	return front();
}

//-----------------------------------------------------------------------------
// PCGInfoOwner
//-----------------------------------------------------------------------------
PCGInfoOwner::~PCGInfoOwner()
{
	Clear();
}

void PCGInfoOwner::Clear()
{
	for (auto pPCGInfo : *this) {
		PCGInfo::Delete(pPCGInfo);
	}
	clear();
}
