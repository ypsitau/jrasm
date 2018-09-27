//=============================================================================
// DisasmDumper.cpp
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// DisasmDumper
//-----------------------------------------------------------------------------
DisasmDumper::DisasmDumper(FILE *fp, bool upperCaseFlag, size_t nColsPerLine) :
	_fp(fp), _upperCaseFlag(upperCaseFlag), _nColsPerLine(nColsPerLine)
{
	_paddingLeft = MakePadding(9 + 3 * _nColsPerLine + 1);
}

void DisasmDumper::DumpLabel(const char *strLabel)
{
	::fprintf(_fp, "%s\n", strLabel);
}

void DisasmDumper::DumpCode(const char *strCode, int indentLevelCode)
{
	String indentCode = MakePadding(indentLevelCode * 2);
	::fprintf(_fp, "%s%s%s\n", _paddingLeft.c_str(), indentCode.c_str(), strCode);
}

void DisasmDumper::DumpLabelAndCode(const char *strLabel, const char *strCode, int indentLevelCode)
{
	String indentCode = MakePadding(indentLevelCode * 2 + 1);
	::fprintf(_fp, "%s%s%s\n", JustifyLeft(strLabel, 9 + 3 * _nColsPerLine).c_str(),
			  indentCode.c_str(), strCode);
}

void DisasmDumper::DumpDataAndCode(UInt32 addr, const Binary &buff, const char *strCode, int indentLevelCode)
{
	const char *formatData = _upperCaseFlag? " %02X" : " %02x";
	const char *formatHead = _upperCaseFlag? "    %04X%s  %s%s\n" : "    %04x%s  %s%s\n";
	const char *formatFollow = _upperCaseFlag? "    %04X%s\n" : "    %04x%s\n";
	String indentCode = MakePadding(indentLevelCode * 2);
	String str;
	size_t iCol = 0;
	size_t iLine = 0;
	size_t nColsFold = (buff.size() <= _nColsPerLine)? _nColsPerLine : _nColsPerLine / 2 * 2;
	for (auto data : buff) {
		char buff[16];
		::sprintf_s(buff, formatData, static_cast<UInt8>(data));
		str += buff;
		iCol++;
		if (iCol == nColsFold) {
			if (iLine == 0) {
				::fprintf(_fp, formatHead, addr, JustifyLeft(str.c_str(), 3 * _nColsPerLine).c_str(),
						  indentCode.c_str(), strCode);
			} else {
				::fprintf(_fp, formatFollow, addr, JustifyLeft(str.c_str(), 3 * _nColsPerLine).c_str());
			}
			str.clear();
			addr += static_cast<UInt32>(iCol);
			iCol = 0;
			iLine++;
		}
	}
	if (iCol > 0) {
		if (iLine == 0) {
			::fprintf(_fp, formatHead, addr, JustifyLeft(str.c_str(), 3 * _nColsPerLine).c_str(),
					  indentCode.c_str(), strCode);
		} else {
			::fprintf(_fp, formatFollow, addr, JustifyLeft(str.c_str(), 3 * _nColsPerLine).c_str());
		}
	}
}

