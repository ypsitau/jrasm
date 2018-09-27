//=============================================================================
// DisasmDumper.cpp
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// DisasmDumper
//-----------------------------------------------------------------------------
DisasmDumper::DisasmDumper(FILE *fp, bool upperCaseFlag, size_t nColsPerLineMax) :
	_fp(fp), _upperCaseFlag(upperCaseFlag), _nColsPerLineMax(nColsPerLineMax)
{
	_paddingLeft = MakePadding(9 + 3 * _nColsPerLineMax + 1);
}

void DisasmDumper::DumpLabel(const char *strLabel) const
{
	::fprintf(_fp, "%s\n", strLabel);
}

void DisasmDumper::DumpCode(const char *strCode) const
{
	::fprintf(_fp, "%s%s\n", _paddingLeft.c_str(), strCode);
}

void DisasmDumper::DumpLabelAndCode(const char *strLabel, const char *strCode) const
{
	::fprintf(_fp, "%s %s\n", JustifyLeft(strLabel, 9 + 3 * _nColsPerLineMax).c_str(), strCode);
}

void DisasmDumper::DumpDataAndCode(UInt32 addr, const Binary &buff, size_t nColsPerLine, const char *strCode) const
{
	const char *formatData = _upperCaseFlag? " %02X" : " %02x";
	const char *formatHead = _upperCaseFlag? "    %04X%s  %s\n" : "    %04x%s  %s\n";
	String str;
	size_t iCol = 0;
	size_t iLine = 0;
	for (auto data : buff) {
		char buff[16];
		::sprintf_s(buff, formatData, static_cast<UInt8>(data));
		str += buff;
		iCol++;
		if (iCol == nColsPerLine) {
			::fprintf(_fp, formatHead, addr, JustifyLeft(str.c_str(), 3 * _nColsPerLineMax).c_str(),
					  (iLine == 0)? strCode : "");
			str.clear();
			addr += static_cast<UInt32>(iCol);
			iCol = 0;
			iLine++;
		}
	}
	if (iCol > 0) {
		::fprintf(_fp, formatHead, addr, JustifyLeft(str.c_str(), 3 * _nColsPerLineMax).c_str(),
				  (iLine == 0)? strCode : "");
	}
}

