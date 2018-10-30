//=============================================================================
// InlineData.h
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// InlineData
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// InlineDataList
//-----------------------------------------------------------------------------
InlineData *InlineDataList::Lookup(const Binary &buff)
{
	for (auto pInlineData : *this) {
		if (pInlineData->GetBinary() == buff) return pInlineData;
	}
	return nullptr;
}

//-----------------------------------------------------------------------------
// InlineDataOwner
//-----------------------------------------------------------------------------
InlineDataOwner::~InlineDataOwner()
{
	Clear();
}

void InlineDataOwner::Clear()
{
	for (auto pInlineData : *this) {
		InlineData::Delete(pInlineData);
	}
	clear();
}