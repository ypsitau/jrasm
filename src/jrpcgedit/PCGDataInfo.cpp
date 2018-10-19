//=============================================================================
// PCGDataInfo.cpp
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// PCGDataInfo
//-----------------------------------------------------------------------------
PCGDataInfo::PCGDataInfo(const String &symbol, Pattern *pPattern, int stepX, int stepY,
						 PCGColorOwner *pPCGColorOwner, bool upperCaseFlag) :
	_cntRef(1), _symbol(symbol), _stepX(stepX), _stepY(stepY), _pPCGColorOwner(pPCGColorOwner),
	_upperCaseFlag(upperCaseFlag), _dotSizeBrowser(0), _dotSizeEditor(20),
	_dotPosX(0), _dotPosY(0), _selectedFlag(false), _pPattern(pPattern)
{
}

PCGDataInfo::PCGDataInfo(const String &symbol, int dotNumX, int dotNumY, int stepX, int stepY,
						 PCGColorOwner *pPCGColorOwner, bool upperCaseFlag) :
	_cntRef(1), _symbol(symbol), _stepX(stepX), _stepY(stepY), _pPCGColorOwner(pPCGColorOwner),
	_upperCaseFlag(upperCaseFlag), _dotSizeBrowser(0), _dotSizeEditor(20),
	_dotPosX(0), _dotPosY(0), _selectedFlag(false), _pPattern(new Pattern(dotNumX, dotNumY))
{
}

void PCGDataInfo::ChangeDotNum(int dotNumX, int dotNumY)
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

wxBitmap &PCGDataInfo::MakeBitmapForBrowser(int dotSizeBrowser)
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

bool PCGDataInfo::WriteFile(FILE *fp)
{
	const char *strPCG = ".pcg";
	const char *strDB = ".db";
	const char *strEND = ".end";
	if (_upperCaseFlag) {
		strPCG = ".PCG";
		strDB = ".DB";
		strEND = ".END";
	}
	::fprintf(fp, "\t%s\t%s,%d,%d,%d,%d\n",
			  strPCG, GetSymbol(), GetDotNumX() / 8, GetDotNumY() / 8, GetStepX(), GetStepY());
	for (int dotPosY = 0; dotPosY < GetDotNumY(); dotPosY++) {
		String str = "b\"";
		for (int dotPosX = 0; dotPosX < GetDotNumX(); dotPosX++) {
			str += GetDot(dotPosX, dotPosY)? "#" : ".";
		}
		str += "\"";
		::fprintf(fp, "\t%s\t%s\n", strDB, str.c_str());
	}
	::fprintf(fp, "\t%s\n", strEND);
	return true;
}

//-----------------------------------------------------------------------------
// PCGDataInfo::Pattern
//-----------------------------------------------------------------------------
PCGDataInfo::Pattern::Pattern(int dotNumX, int dotNumY) :
	_cntRef(1), _dotNumX(dotNumX), _dotNumY(dotNumY), _dotTbl(new bool [dotNumX * dotNumY])
{
	Clear();
}

PCGDataInfo::Pattern *PCGDataInfo::Pattern::CreateFromBuff(int wdChar, int htChar, Binary &buff)
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

void PCGDataInfo::Pattern::Paste(int dotPosDstX, int dotPosDstY, const Pattern *pPatternSrc)
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
// PCGDataInfoList
//-----------------------------------------------------------------------------
PCGDataInfoList::iterator PCGDataInfoList::FindSelected()
{
	for (iterator ppPCGDataInfo = begin(); ppPCGDataInfo != end(); ppPCGDataInfo++) {
		if ((*ppPCGDataInfo)->GetSelectedFlag()) return ppPCGDataInfo;
	}
	return begin();
}

PCGDataInfoList::const_iterator PCGDataInfoList::FindSelected() const
{
	for (const_iterator ppPCGDataInfo = begin(); ppPCGDataInfo != end(); ppPCGDataInfo++) {
		if ((*ppPCGDataInfo)->GetSelectedFlag()) return ppPCGDataInfo;
	}
	return begin();
}

bool PCGDataInfoList::MoveSelectionUp()
{
	iterator ppPCGDataInfo = FindSelected();
	if (ppPCGDataInfo == begin()) return false;
	ppPCGDataInfo = insert(ppPCGDataInfo - 1, *ppPCGDataInfo);
	erase(ppPCGDataInfo + 2);
	return true;
}

bool PCGDataInfoList::MoveSelectionDown()
{
	iterator ppPCGDataInfo = FindSelected();
	if (ppPCGDataInfo + 1 == end()) return false;
	PCGDataInfo *pPCGDataInfo = *ppPCGDataInfo;
	ppPCGDataInfo = erase(ppPCGDataInfo);
	insert(ppPCGDataInfo + 1, pPCGDataInfo);
	return true;
}

//-----------------------------------------------------------------------------
// PCGDataInfoOwner
//-----------------------------------------------------------------------------
PCGDataInfoOwner::~PCGDataInfoOwner()
{
	Clear();
}

void PCGDataInfoOwner::Clear()
{
	for (auto pPCGDataInfo : *this) {
		PCGDataInfo::Delete(pPCGDataInfo);
	}
	clear();
}

bool PCGDataInfoOwner::DeleteSelection()
{
	if (size() <= 1) return false;
	iterator ppPCGDataInfo = FindSelected();
	PCGDataInfo::Delete(*ppPCGDataInfo);
	ppPCGDataInfo = erase(ppPCGDataInfo);
	if (ppPCGDataInfo == end()) {
		back()->SetSelectedFlag(true);
	} else {
		(*ppPCGDataInfo)->SetSelectedFlag(true);
	}
	return true;
}

void PCGDataInfoOwner::NewPCGDataInfo(bool selectedFlag)
{
	bool upperCaseFlag = true;
	char symbol[256];
	::sprintf_s(symbol, "pcg%d", static_cast<int>(size()) + 1);
	if (selectedFlag) {
		for (auto pPCGDataInfo : *this) pPCGDataInfo->SetSelectedFlag(false);
	}
	AutoPtr<PCGDataInfo> pPCGDataInfo(new PCGDataInfo(symbol, 16, 16, 1, 32,
													  new PCGColorOwner(), upperCaseFlag));
	pPCGDataInfo->SetSelectedFlag(selectedFlag);
	push_back(pPCGDataInfo.release());
}
