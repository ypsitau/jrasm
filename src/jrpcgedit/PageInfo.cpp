//=============================================================================
// PageInfo.cpp
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// PageInfo
//-----------------------------------------------------------------------------
PageInfo::PageInfo() : _cntRef(1)
{
	_patternInfoOwner.push_back(new PatternInfo("pattern1", 16, 16));
	_patternInfoOwner.push_back(new PatternInfo("pattern2", 16, 16));
	_patternInfoOwner.push_back(new PatternInfo("pattern3", 16, 16));
}

void PageInfo::NewPatternInfo()
{
	_patternInfoOwner.push_back(new PatternInfo("patternX", 16, 16));
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
