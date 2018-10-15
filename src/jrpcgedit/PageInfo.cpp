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
}

void PageInfo::AddPCGInfo(PCGInfo *pPCGInfo)
{
	_pcgInfoOwner.push_back(pPCGInfo);
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

void PageInfoOwner::NewPageInfo()
{
	char symbol[256];
	::sprintf_s(symbol, "page%d", static_cast<int>(size()) + 1);
	AutoPtr<PageInfo> pPageInfo(new PageInfo(symbol, PCGTYPE_User, 0x20));
	pPageInfo->NewPCGInfo();
	push_back(pPageInfo.release());
}
