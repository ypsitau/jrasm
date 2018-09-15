//=============================================================================
// Context.h
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// Context
//-----------------------------------------------------------------------------
Context::Context(Generator *pGenerator) : _pGenerator(pGenerator), _addr(0x0000)
{
	_lookupTableStack.push_back(new LookupTable());
}

Context::LookupTable *Context::AddLookupTable()
{
	LookupTable *pLookupTable = new LookupTable(_lookupTableStack.back()->Reference());
	_lookupTableStack.push_back(pLookupTable);
	return pLookupTable;
}

void Context::RemoveLookupTable()
{
	LookupTable *pLookupTable = _lookupTableStack.back();
	_lookupTableStack.pop_back();
	LookupTable::Delete(pLookupTable);
}

void Context::PutByte(UInt8 data)
{
	_buff += data;
	ForwardAddress(1);
}

void Context::AddError(const char *format, ...)
{
	char buff[256];
	va_list ap;
	va_start(ap, format);
	::vsprintf_s(buff, format, ap);
	_errMsg = buff;
}

void Context::AddError(const Expr *pExpr, const char *format, ...)
{
	char buff[256];
	va_list ap;
	va_start(ap, format);
	_errMsg = pExpr->GetFileNameSrc();
	::sprintf_s(buff, ":%d ", pExpr->GetLineNo());
	_errMsg += buff;
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
	if (iter != end()) {
		*pFoundFlag = true;
		return iter->second;
	}
	if (!_pLookupTableParent.IsNull()) {
		return _pLookupTableParent->Lookup(label, pFoundFlag);
	}
	*pFoundFlag = false;
	return 0;
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
