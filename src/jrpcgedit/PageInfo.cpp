//=============================================================================
// PageInfo.cpp
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// PageInfo
//-----------------------------------------------------------------------------
PageInfo::PageInfo(const String &symbol, PCGType pcgType, int charCodeStart) :
	_cntRef(1), _symbol(symbol), _pcgType(pcgType), _charCodeStart(charCodeStart)
{
	_pcgInfoOwner.push_back(new PCGInfo("pcg1", 16, 16));
#if 0
	_pcgInfoOwner.push_back(new PCGInfo("pcg2", 8, 8));
	_pcgInfoOwner.push_back(new PCGInfo("pcg3", 16, 8));
	_pcgInfoOwner.push_back(new PCGInfo("pcg4", 24, 24));
	_pcgInfoOwner.push_back(new PCGInfo("pcg5", 32, 32));
#endif
	_pcgInfoOwner.front()->SetSelectedFlag(true);
}

void PageInfo::NewPCGInfo()
{
	char symbol[256];
	::sprintf_s(symbol, "pcg%d", static_cast<int>(_pcgInfoOwner.size()) + 1);
	_pcgInfoOwner.push_back(new PCGInfo(symbol, 16, 16));
}

//-----------------------------------------------------------------------------
// PageInfoList
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// PageInfoOwner
//-----------------------------------------------------------------------------
PageInfoOwner::~PageInfoOwner()
{
	Clear();
}

void PageInfoOwner::Clear()
{
	for (auto pPageInfo : *this) {
		PageInfo::Delete(pPageInfo);
	}
	clear();
}
