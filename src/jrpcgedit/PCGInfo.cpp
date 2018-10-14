//=============================================================================
// PCGInfo.cpp
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// PCGInfo
//-----------------------------------------------------------------------------
PCGInfo::PCGInfo(const String &symbol, int dotNumX, int dotNumY) :
	_cntRef(1), _symbol(symbol), _sizeDot(0), _dotPosX(0), _dotPosY(0), _selectedFlag(false),
	_pPattern(new Pattern(dotNumX, dotNumY))
{
}

void PCGInfo::ChangeDotNum(int dotNumX, int dotNumY)
{
	if (_pPatternOrg.IsNull() || dotNumX < _pPatternOrg->GetDotNumX() || dotNumY < _pPatternOrg->GetDotNumY()) {
		_pPatternOrg.reset(_pPattern.release());
	}
	_pPattern.reset(new Pattern(dotNumX, dotNumY));
	_pPattern->Paste(0, 0, _pPatternOrg.get());
	if (_dotPosX >= dotNumX) _dotPosX = dotNumX - 1;
	if (_dotPosY >= dotNumY) _dotPosY = dotNumY - 1;
	_pBitmap.reset(nullptr);
}

wxBitmap &PCGInfo::MakeBitmap(int sizeDot)
{
	if (_pBitmap.get() == nullptr || _sizeDot != sizeDot) {
		_pBitmap.reset(new wxBitmap(sizeDot * _pPattern->GetDotNumX(), sizeDot * _pPattern->GetDotNumY()));
		_sizeDot = sizeDot;
	}
	wxBrush brushBg(wxColour("black"), wxBRUSHSTYLE_SOLID);
	wxBrush brushFg(wxColour("white"), wxBRUSHSTYLE_SOLID);
	wxMemoryDC dc(*_pBitmap);
	dc.SetBackground(brushBg);
	dc.Clear();
	dc.SetPen(wxNullPen);
	dc.SetBrush(brushFg);
	for (int dotPosY = 0; dotPosY < _pPattern->GetDotNumY(); dotPosY++) {
		int y = dotPosY * _sizeDot;
		for (int dotPosX = 0; dotPosX < _pPattern->GetDotNumX(); dotPosX++) {
			int x = dotPosX * _sizeDot;
			if (GetDot(dotPosX, dotPosY)) {
				dc.DrawRectangle(x, y, _sizeDot, _sizeDot);
			}
		}
	}
	return *_pBitmap;
}

//-----------------------------------------------------------------------------
// PCGInfo::Pattern
//-----------------------------------------------------------------------------
PCGInfo::Pattern::Pattern(int dotNumX, int dotNumY) :
	_cntRef(1), _dotNumX(dotNumX), _dotNumY(dotNumY), _dotTbl(new bool [dotNumX * dotNumY])
{
	Clear();
}

void PCGInfo::Pattern::Paste(int dotPosDstX, int dotPosDstY, const Pattern *pPatternSrc)
{
	int dotPosSrcX = 0, dotPosSrcY = 0;
	for (int dotOffY = 0;
		 dotPosSrcY + dotOffY < pPatternSrc->GetDotNumY() && dotPosDstY + dotOffY < GetDotNumY();
		 dotOffY++) {
		for (int dotOffX = 0;
			 dotPosSrcX + dotOffX < pPatternSrc->GetDotNumX() && dotPosDstX + dotOffX < GetDotNumX();
			 dotOffX++) {
			PutDot(dotPosDstX + dotOffX, dotPosDstY + dotOffY,
				   pPatternSrc->GetDot(dotPosSrcX + dotOffX, dotPosSrcY + dotOffY));
		}
	}
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
