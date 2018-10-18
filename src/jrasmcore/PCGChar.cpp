//=============================================================================
// PCGChar.h
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// PCGChar
//-----------------------------------------------------------------------------
int PCGChar::GetCharCodeToPut() const
{
	return (_pcgType == PCGTYPE_CRAM)? _charCode :
		(_charCode < 64)? _charCode - 32 : _charCode - 64 + 0x80;
}

//-----------------------------------------------------------------------------
// PCGCharList
//-----------------------------------------------------------------------------
const PCGChar *PCGCharList::FindSamePattern(const Binary &buff) const
{
	for (auto pPCGChar : *this) {
		if (pPCGChar->HasSamePattern(buff)) return pPCGChar;
	}
	return nullptr;
}

//-----------------------------------------------------------------------------
// PCGCharOwner
//-----------------------------------------------------------------------------
PCGCharOwner::~PCGCharOwner()
{
	Clear();
}

void PCGCharOwner::Clear()
{
	for (auto pPCGChar : *this) {
		PCGChar::Delete(pPCGChar);
	}
	clear();
}
