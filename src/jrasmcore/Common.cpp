//=============================================================================
// Common.cpp
//=============================================================================
#include "stdafx.h"

#if defined(JRASM_ON_MSWIN)
const char FileSeparator = '\\';
#else
const char FileSeparator = '/';
#endif

//-----------------------------------------------------------------------------
// String
//-----------------------------------------------------------------------------
const StringList StringList::Empty;

//-----------------------------------------------------------------------------
// Utilities
//-----------------------------------------------------------------------------
Binary BitPatternToBinary(const char *str)
{
	Binary buff;
	UInt8 data = 0x00;
	size_t nCols = 0;
	for (const char *p = str; *p != '\0'; p++) {
		char ch = *p;
		data <<= 1;
		if (!(ch == ' ' || ch == '.' || ch == ',' || ch == '_' || ch == '-')) data |= 1;
		nCols++;
		if (nCols == 8) {
			buff += data;
			nCols = 0;
		}
	}
	return buff;
}

String MakePadding(size_t width, const char *padding)
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
			::sprintf_s(tmp, "\\x%02x", static_cast<UInt8>(ch));
			strDst += tmp;
		} else {
			strDst += ch;
		}
	}
	return strDst;
}

String CorrectFileSeparator(const char *pathName)
{
	String rtn;
	for (const char *p = pathName; *p != '\0'; p++) {
		char ch = *p;
		rtn += IsFileSeparator(ch)? FileSeparator : ch;
	}
	return rtn;
}

String JoinPathName(const char *pathName1, const char *pathName2)
{
	const char fileSepChar = '/';
	if (*pathName1 == '\0') return pathName2;
	if (*pathName2 == '\0') return pathName1;
	String rtn;
	rtn += pathName1;
	if (!IsFileSeparator(*(pathName1 + ::strlen(pathName1) - 1))) rtn += fileSepChar;
	rtn += pathName2;
	return rtn;
}

void SplitFileName(const char *pathName, String *pDirName, String *pFileName)
{
	const char *p = pathName + ::strlen(pathName);
	for ( ; p > pathName; p--) {
		if (IsFileSeparator(*(p - 1))) break;
	}
	if (pDirName != nullptr) *pDirName = String(pathName, p);
	if (pFileName != nullptr) *pFileName = String(p);
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
