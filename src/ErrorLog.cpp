//=============================================================================
// ErrorLog.cpp
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// ErrorLog
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// ErrorLog::EntryOwner
//-----------------------------------------------------------------------------
ErrorLog::EntryOwner::~EntryOwner()
{
	Clear();
}

void ErrorLog::EntryOwner::Clear()
{
	for (auto pEntry : *this) {
		delete pEntry;
	}
}