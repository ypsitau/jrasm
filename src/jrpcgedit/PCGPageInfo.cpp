//=============================================================================
// PCGPageInfo.cpp
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// PCGPageInfo
//-----------------------------------------------------------------------------
PCGPageInfo::PCGPageInfo(const String &symbol, PCGType pcgType, int charCodeStart) :
	_cntRef(1), _symbol(symbol), _pcgType(pcgType), _charCodeStart(charCodeStart)
{
}

void PCGPageInfo::AddPCGInfo(PCGInfo *pPCGInfo)
{
	_pcgInfoOwner.push_back(pPCGInfo);
}

//-----------------------------------------------------------------------------
// PCGPageInfoList
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// PCGPageInfoOwner
//-----------------------------------------------------------------------------
PCGPageInfoOwner::~PCGPageInfoOwner()
{
	Clear();
}

void PCGPageInfoOwner::Clear()
{
	for (auto pPCGPageInfo : *this) {
		PCGPageInfo::Delete(pPCGPageInfo);
	}
	clear();
}

void PCGPageInfoOwner::NewPCGPageInfo()
{
	char symbol[256];
	::sprintf_s(symbol, "page%d", static_cast<int>(size()) + 1);
	AutoPtr<PCGPageInfo> pPCGPageInfo(new PCGPageInfo(symbol, PCGTYPE_User, 0x20));
	pPCGPageInfo->NewPCGInfo();
	push_back(pPCGPageInfo.release());
}
