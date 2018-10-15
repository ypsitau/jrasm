//=============================================================================
// PCGInfo.cpp
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// PCGInfo
//-----------------------------------------------------------------------------
PCGInfo::PCGInfo(const String &symbol, Pattern *pPattern) :
	_cntRef(1), _symbol(symbol), _dotSizeBrowser(0), _dotSizeEditor(20),
	_dotPosX(0), _dotPosY(0), _selectedFlag(false), _pPattern(pPattern)
{
}

PCGInfo::PCGInfo(const String &symbol, int dotNumX, int dotNumY) :
	_cntRef(1), _symbol(symbol), _dotSizeBrowser(0), _dotSizeEditor(20),
	_dotPosX(0), _dotPosY(0), _selectedFlag(false), _pPattern(new Pattern(dotNumX, dotNumY))
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

wxBitmap &PCGInfo::MakeBitmapForBrowser(int dotSizeBrowser)
{
	if (_pBitmapForBrowser.get() == nullptr || _dotSizeBrowser != dotSizeBrowser) {
		_dotSizeBrowser = dotSizeBrowser;
		_pBitmapForBrowser.reset(
			new wxBitmap(
				_dotSizeBrowser * _pPattern->GetDotNumX(), _dotSizeBrowser * _pPattern->GetDotNumY()));
	}
	wxBrush brushBg(wxColour("black"), wxBRUSHSTYLE_SOLID);
	wxBrush brushFg(wxColour("white"), wxBRUSHSTYLE_SOLID);
	wxMemoryDC dc(*_pBitmapForBrowser);
	dc.SetBackground(brushBg);
	dc.Clear();
	dc.SetPen(wxNullPen);
	dc.SetBrush(brushFg);
	for (int dotPosY = 0; dotPosY < _pPattern->GetDotNumY(); dotPosY++) {
		int y = dotPosY * _dotSizeBrowser;
		for (int dotPosX = 0; dotPosX < _pPattern->GetDotNumX(); dotPosX++) {
			int x = dotPosX * _dotSizeBrowser;
			if (GetDot(dotPosX, dotPosY)) {
				dc.DrawRectangle(x, y, _dotSizeBrowser, _dotSizeBrowser);
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

PCGInfo::Pattern *PCGInfo::Pattern::CreateFromBuff(int wdChar, int htChar, Binary &buff)
{
	AutoPtr<Pattern> pPattern(new Pattern(wdChar * 8, htChar * 8));
	bool *dotTbl = pPattern->GetDotTbl();
	for (auto data : buff) {
		for (int nBits = 0; nBits < 8; nBits++, data <<= 1) {
			*dotTbl++ = (data & 0x80)? true : false;
		}
	}
	return pPattern.release();
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

void PCGInfoOwner::NewPCGInfo()
{
	char symbol[256];
	::sprintf_s(symbol, "pcg%d", static_cast<int>(size()) + 1);
	push_back(new PCGInfo(symbol, 16, 16));
}
