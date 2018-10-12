//=============================================================================
// PatternInfo.cpp
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// PatternInfo
//-----------------------------------------------------------------------------
PatternInfo::PatternInfo(int nDotsX, int nDotsY) :
	_cntRef(1), _nDotsX(nDotsX), _nDotsY(nDotsY), _sizeDot(0),
	_dotTbl(new bool [nDotsX * nDotsY])
{
	ClearAll();
}

wxBitmap &PatternInfo::MakeBitmap(int sizeDot)
{
	if (_pBitmap.get() == nullptr || _sizeDot != sizeDot) {
		_pBitmap.reset(new wxBitmap(sizeDot * _nDotsX, sizeDot * _nDotsY));
		_sizeDot = sizeDot;
	}
	wxMemoryDC dc(*_pBitmap);
	dc.SetBackground(*wxWHITE_BRUSH);
	dc.Clear();
	dc.SetPen(wxNullPen);
	dc.SetBrush(*wxBLACK_BRUSH);
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
