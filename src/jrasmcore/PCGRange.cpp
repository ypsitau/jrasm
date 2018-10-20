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
	char str[128];
	int charCodeEndDef = (_pcgType == PCGTYPE_CRAM)? 256 : 96;
	if (_charCodeEnd == charCodeEndDef) {
		::sprintf_s(str, "%s:%d", GetPCGTypeName(upperCaseFlag), _charCodeStart);
	} else {
		::sprintf_s(str, "%s:%d:%d", GetPCGTypeName(upperCaseFlag), _charCodeStart, _charCodeEnd - 1);
	}
	return str;
}

//-----------------------------------------------------------------------------
// PCGRangeList
//-----------------------------------------------------------------------------
String PCGRangeList::ComposeSource(bool upperCaseFlag, const char *sepStr) const
{
	String rtn;
	for (auto pPCGRange : *this) {
		if (!rtn.empty()) rtn += sepStr;
		rtn += pPCGRange->ComposeSource(upperCaseFlag);
	}
	return rtn;
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
