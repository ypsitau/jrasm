//=============================================================================
// PCGPage.cpp
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// PCGPage
//-----------------------------------------------------------------------------
const PCGPattern *PCGPage::AddPCGPattern(const Binary &buff)
{
	AutoPtr<PCGPattern> pPCGPattern(new PCGPattern(_pcgType, _charCodeCur, buff));
	_charCodeCur++;
	_pcgPatternOwner.push_back(pPCGPattern->Reference());
	return pPCGPattern.release();
}

//-----------------------------------------------------------------------------
// PCGPageList
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// PCGPageOwner
//-----------------------------------------------------------------------------
PCGPageOwner::~PCGPageOwner()
{
	Clear();
}

void PCGPageOwner::Clear()
{
	for (auto pPCGPage : *this) {
		PCGPage::Delete(pPCGPage);
	}
	clear();
}
