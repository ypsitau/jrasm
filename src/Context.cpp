//=============================================================================
// Context.h
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// Context
//-----------------------------------------------------------------------------
Context::Context(const String &fileNameSrc, Generator *pGenerator) :
	_fileNameSrc(fileNameSrc), _pGenerator(pGenerator)
{
}

void Context::SetError(int lineNo, const char *format, ...)
{
	char buff[256];
	va_list ap;
	_errMsg = _fileNameSrc;
	::sprintf_s(buff, ":%d ", lineNo);
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

