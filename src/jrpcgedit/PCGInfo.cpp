//=============================================================================
// PCGInfo.cpp
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// PCGInfo
//-----------------------------------------------------------------------------
PCGInfo::PCGInfo(const String &symbol, int dotNumX, int dotNumY) :
	_cntRef(1), _symbol(symbol), _sizeDotBrowser(0), _sizeDotEditor(0),
	_dotPosX(0), _dotPosY(0), _selectedFlag(false),
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
	_pBitmapForBrowser.reset(nullptr);
}

wxBitmap &PCGInfo::MakeBitmapForBrowser(int sizeDotBrowser)
{
	if (_pBitmapForBrowser.get() == nullptr || _sizeDotBrowser != sizeDotBrowser) {
		_sizeDotBrowser = sizeDotBrowser;
		_pBitmapForBrowser.reset(
			new wxBitmap(
				_sizeDotBrowser * _pPattern->GetDotNumX(), _sizeDotBrowser * _pPattern->GetDotNumY()));
	}
	wxBrush brushBg(wxColour("black"), wxBRUSHSTYLE_SOLID);
	wxBrush brushFg(wxColour("white"), wxBRUSHSTYLE_SOLID);
	wxMemoryDC dc(*_pBitmapForBrowser);
	dc.SetBackground(brushBg);
	dc.Clear();
	dc.SetPen(wxNullPen);
	dc.SetBrush(brushFg);
	for (int dotPosY = 0; dotPosY < _pPattern->GetDotNumY(); dotPosY++) {
		int y = dotPosY * _sizeDotBrowser;
		for (int dotPosX = 0; dotPosX < _pPattern->GetDotNumX(); dotPosX++) {
			int x = dotPosX * _sizeDotBrowser;
			if (GetDot(dotPosX, dotPosY)) {
				dc.DrawRectangle(x, y, _sizeDotBrowser, _sizeDotBrowser);
			}
		}
	}
	return *_pBitmapForBrowser;
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
PCGInfoList::iterator PCGInfoList::FindSelected()
{
	for (iterator ppPCGInfo = begin(); ppPCGInfo != end(); ppPCGInfo++) {
		if ((*ppPCGInfo)->GetSelectedFlag()) return ppPCGInfo;
	}
	return begin();
}

PCGInfoList::const_iterator PCGInfoList::FindSelected() const
{
	for (const_iterator ppPCGInfo = begin(); ppPCGInfo != end(); ppPCGInfo++) {
		if ((*ppPCGInfo)->GetSelectedFlag()) return ppPCGInfo;
	}
	return begin();
}

bool PCGInfoList::MoveSelectionUp()
{
	iterator ppPCGInfo = FindSelected();
	if (ppPCGInfo == begin()) return false;
	ppPCGInfo = insert(ppPCGInfo - 1, *ppPCGInfo);
	erase(ppPCGInfo + 2);
	return true;
}

bool PCGInfoList::MoveSelectionDown()
{
	iterator ppPCGInfo = FindSelected();
	if (ppPCGInfo + 1 == end()) return false;
	PCGInfo *pPCGInfo = *ppPCGInfo;
	ppPCGInfo = erase(ppPCGInfo);
	insert(ppPCGInfo + 1, pPCGInfo);
	return true;
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

bool PCGInfoOwner::DeleteSelection()
{
	if (size() <= 1) return false;
	iterator ppPCGInfo = FindSelected();
	PCGInfo::Delete(*ppPCGInfo);
	ppPCGInfo = erase(ppPCGInfo);
	if (ppPCGInfo == end()) {
		back()->SetSelectedFlag(true);
	} else {
		(*ppPCGInfo)->SetSelectedFlag(true);
	}
	return true;
}
