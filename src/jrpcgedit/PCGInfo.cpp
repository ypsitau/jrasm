//=============================================================================
// PCGInfo.cpp
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// PCGInfo
//-----------------------------------------------------------------------------
PCGInfo::PCGInfo(const String &symbol, int nDotsX, int nDotsY) :
	_cntRef(1), _symbol(symbol), _sizeDot(0),
	_nDotsX(nDotsX), _nDotsY(nDotsY), _dotPosX(0), _dotPosY(0),
	_dotTbl(new bool [nDotsX * nDotsY])
{
	ClearAll();
}

wxBitmap &PCGInfo::MakeBitmap(int sizeDot)
{
	if (_pBitmap.get() == nullptr || _sizeDot != sizeDot) {
		_pBitmap.reset(new wxBitmap(sizeDot * _nDotsX, sizeDot * _nDotsY));
		_sizeDot = sizeDot;
	}
	wxBrush brushBg(wxColour("black"), wxBRUSHSTYLE_SOLID);
	wxBrush brushFg(wxColour("white"), wxBRUSHSTYLE_SOLID);
	wxMemoryDC dc(*_pBitmap);
	dc.SetBackground(brushBg);
	dc.Clear();
	dc.SetPen(wxNullPen);
	dc.SetBrush(brushFg);
	for (int dotPosY = 0; dotPosY < _nDotsY; dotPosY++) {
		int y = dotPosY * _sizeDot;
		for (int dotPosX = 0; dotPosX < _nDotsX; dotPosX++) {
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
