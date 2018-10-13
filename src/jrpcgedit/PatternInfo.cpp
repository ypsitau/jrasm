//=============================================================================
// PatternInfo.cpp
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// PatternInfo
//-----------------------------------------------------------------------------
PatternInfo::PatternInfo(const String &symbol, int nDotsX, int nDotsY) :
	_cntRef(1), _symbol(symbol), _nDotsX(nDotsX), _nDotsY(nDotsY),
	_sizeDot(0), _dotTbl(new bool [nDotsX * nDotsY])
{
	ClearAll();
}

wxBitmap &PatternInfo::MakeBitmap(int sizeDot)
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
	for (int iDotY = 0; iDotY < _nDotsY; iDotY++) {
		int y = iDotY * _sizeDot;
		for (int iDotX = 0; iDotX < _nDotsX; iDotX++) {
			int x = iDotX * _sizeDot;
			if (GetDot(iDotX, iDotY)) {
				dc.DrawRectangle(x, y, _sizeDot, _sizeDot);
			}
		}
	}
	return *_pBitmap;
}

//-----------------------------------------------------------------------------
// PatternInfoList
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// PatternInfoOwner
//-----------------------------------------------------------------------------
PatternInfoOwner::~PatternInfoOwner()
{
	Clear();
}

void PatternInfoOwner::Clear()
{
	for (auto pPatternInfo : *this) {
		PatternInfo::Delete(pPatternInfo);
	}
	clear();
}
