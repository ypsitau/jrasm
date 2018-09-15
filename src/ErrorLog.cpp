//=============================================================================
// ErrorLog.cpp
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// ErrorLog
//-----------------------------------------------------------------------------
ErrorLog ErrorLog::_instance;

void ErrorLog::AddError(const Expr *pExpr, const char *format, ...)
{
	va_list ap;
	va_start(ap, format);
	_instance.AddErrorV(pExpr->GetFileNameSrc(), pExpr->GetLineNo(), format, ap);
}

void ErrorLog::AddError(const String &fileName, const char *format, ...)
{
	va_list ap;
	va_start(ap, format);
	_instance.AddErrorV(fileName, 0, format, ap);
}

void ErrorLog::AddError(const String &fileName, int lineNo, const char *format, ...)
{
	va_list ap;
	va_start(ap, format);
	_instance.AddErrorV(fileName, lineNo, format, ap);
}

void ErrorLog::AddErrorV(const String &fileName, int lineNo, const char *format, va_list ap)
{
	char message[512];
	::vsprintf_s(message, format, ap);
	_instance._entryOwner.push_back(new Entry(fileName, lineNo, message));
}

void ErrorLog::Print(FILE *fp)
{
	for (auto pEntry : _instance._entryOwner) {
		::fprintf(fp, "%s\n", pEntry->GetString().c_str());
	}
}

//-----------------------------------------------------------------------------
// ErrorLog::Entry
//-----------------------------------------------------------------------------
String ErrorLog::Entry::GetString() const
{
	String rtn;
	char buff[128];
	rtn += _fileName;
	if (_lineNo == 0) {
		rtn += ": ";
	} else {
		::sprintf_s(buff, ":%d: ", _lineNo);
		rtn += buff;
	}
	rtn += _message;
	return rtn;
}

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
