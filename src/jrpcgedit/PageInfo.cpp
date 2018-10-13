//=============================================================================
// PageInfo.cpp
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// PageInfo
//-----------------------------------------------------------------------------
PageInfo::PageInfo() : _cntRef(1)
{
	_pcgInfoOwner.push_back(new PCGInfo("pcg1", 16, 16));
	_pcgInfoOwner.push_back(new PCGInfo("pcg2", 8, 8));
	_pcgInfoOwner.push_back(new PCGInfo("pcg3", 16, 8));
	_pcgInfoOwner.push_back(new PCGInfo("pcg4", 24, 24));
	_pcgInfoOwner.push_back(new PCGInfo("pcg5", 32, 32));
}

void PageInfo::NewPCGInfo()
{
	_pcgInfoOwner.push_back(new PCGInfo("pcgX", 16, 16));
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
