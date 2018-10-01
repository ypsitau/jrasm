//=============================================================================
// PCGPattern.h
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// PCGPattern
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// PCGPatternList
//-----------------------------------------------------------------------------
const PCGPattern *PCGPatternList::FindSamePattern(const Binary &buff) const
{
	for (auto pPCGPattern : *this) {
		if (pPCGPattern->HasSamePattern(buff)) return pPCGPattern;
	}
	return nullptr;
}

//-----------------------------------------------------------------------------
// PCGPatternOwner
//-----------------------------------------------------------------------------
PCGPatternOwner::~PCGPatternOwner()
{
	Clear();
}

void PCGPatternOwner::Clear()
{
	for (auto pPCGPattern : *this) {
		PCGPattern::Delete(pPCGPattern);
	}
	clear();
}
