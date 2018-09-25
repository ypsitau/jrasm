//=============================================================================
// Common.cpp
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// String
//-----------------------------------------------------------------------------
const StringList StringList::Empty;

//-----------------------------------------------------------------------------
// Utilities
//-----------------------------------------------------------------------------
String MakePadding(size_t width, char padding)
{
	String rtn;
	while (width-- > 0) rtn += padding;
	return rtn;
}

String JustifyLeft(const char *str, size_t width, char padding)
{
	size_t widthBody = ::strlen(str);
	if (width <= widthBody) return String(str);
	String rtn;
	size_t widthRight = width - widthBody;
	rtn += str;
	while (widthRight-- > 0) rtn += padding;
	return rtn;
}

String JustifyRight(const char *str, size_t width, char padding)
{
	size_t widthBody = ::strlen(str);
	if (width <= widthBody) return String(str);
	String rtn;
	size_t widthLeft = width - widthBody;
	while (widthLeft-- > 0) rtn += padding;
	rtn += str;
	return rtn;
}

String ToLower(const char *str)
{
	String rtn;
	for (const char *p = str; *p != '\0'; ) {
		char ch = *p;
		if ('A' <= ch && ch <= 'Z') {
			rtn += ch - 'A' + 'a';
			p++;
		} else {
			rtn += ch;
			p++;
		}
	}
	return rtn;
}

String ToUpper(const char *str)
{
	String rtn;
	for (const char *p = str; *p != '\0'; ) {
		char ch = *p;
		if ('a' <= ch && ch <= 'z') {
			rtn += ch - 'a' + 'A';
			p++;
		} else {
			rtn += ch;
			p++;
		}
	}
	return rtn;
}

String MakeQuotedString(const String &str, char chBorder)
{
	String strDst;
	for (auto ch : str) {
		if (ch == '\0') {
			strDst += "\\0";
		} else if (ch == '\a') {
			strDst += "\\a";
		} else if (ch == '\b') {
			strDst += "\\b";
		} else if (ch == '\f') {
			strDst += "\\f";
		} else if (ch == '\n') {
			strDst += "\\n";
		} else if (ch == '\r') {
			strDst += "\\r";
		} else if (ch == '\t') {
			strDst += "\\t";
		} else if (ch == '\v') {
			strDst += "\\v";
		} else if (ch == chBorder) {
			strDst += '\\';
			strDst += chBorder;
		} else if (ch == '\\') {
			strDst += "\\\\";
		} else if (ch < 0x20 || ch >= 0x7f) {
			char tmp[16];
			::sprintf(tmp, "\\x%02x", static_cast<UInt8>(ch));
			strDst += tmp;
		} else {
			strDst += ch;
		}
	}
	return strDst;
}

const char *ExtractFileName(const char *pathName)
{
	const char *p = pathName + ::strlen(pathName);
	for ( ; p > pathName; p--) {
		if (IsFileSeparator(*(p - 1))) break;
	}
	return p;
}

const char *SeekExtName(const char *pathName)
{
	const char *pBtm = pathName + ::strlen(pathName);
	for (const char *p = pBtm; p >= pathName; p--) {
		if (*p == '.') {
			return (p == pathName || IsFileSeparator(*(p - 1)))? pBtm : p;
		}
		if (IsFileSeparator(*p)) break;
	}
	return pBtm;
}

String RemoveExtName(const char *pathName)
{
	return String(pathName, SeekExtName(pathName));
}
