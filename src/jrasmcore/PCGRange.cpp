//=============================================================================
// PCGRange.cpp
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// PCGRange
//-----------------------------------------------------------------------------
const char *PCGRange::GetPCGTypeName(bool upperCaseFlag) const
{
	if (_pcgType == PCGTYPE_CRAM) {
		return upperCaseFlag? "CRAM" : "cram";
	} else {
		return upperCaseFlag? "USER" : "user";
	}
}

String PCGRange::ComposeSource(bool upperCaseFlag) const
{
	return "";
}

//-----------------------------------------------------------------------------
// PCGRangeList
//-----------------------------------------------------------------------------
String PCGRangeList::ComposeSource(bool upperCaseFlag) const
{
	return "";
}

//-----------------------------------------------------------------------------
// PCGRangeOwner
//-----------------------------------------------------------------------------
PCGRangeOwner::~PCGRangeOwner()
{
	Clear();
}

void PCGRangeOwner::Clear()
{
	for (auto pPCGRange : *this) {
		delete pPCGRange;
	}
	clear();
}
