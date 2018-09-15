//=============================================================================
// Context.h
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// Context
//-----------------------------------------------------------------------------
Context::Context(Generator *pGenerator) : _pGenerator(pGenerator)
{
}

void Context::SetError(const Expr *pExpr, const char *format, ...)
{
	char buff[256];
	va_list ap;
	_errMsg = pExpr->GetFileNameSrc();
	::sprintf_s(buff, ":%d ", pExpr->GetLineNo());
	_errMsg += buff;
	va_start(ap, format);
	::vsprintf_s(buff, format, ap);
	_errMsg += buff;
}

void Context::Dump()
{
	size_t col = 0;
	for (auto data : _buff) {
		if (col > 0) ::printf(" ");
		::printf("%02x", static_cast<UInt8>(data));
		col++;
		if (col == 32) {
			::printf("\n");
			col = 0;
		}
	}
	if (col > 0) ::printf("\n");
}

//-----------------------------------------------------------------------------
// Context::LookupTable
//-----------------------------------------------------------------------------
void Context::LookupTable::Set(const String &label, UInt32 value)
{
	insert(std::make_pair(label, value));
}

UInt32 Context::LookupTable::Lookup(const char *label, bool *pFoundFlag) const
{
	const_iterator iter = find(label);
	return (*pFoundFlag = (iter != end()))? iter->second : 0;
}

//-----------------------------------------------------------------------------
// Context::LookupTableOwner
//-----------------------------------------------------------------------------
Context::LookupTableOwner::~LookupTableOwner()
{
	Clear();
}

void Context::LookupTableOwner::Clear()
{
	for (auto pLookupTable : *this) {
		LookupTable::Delete(pLookupTable);
	}
	clear();
}
