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

void DisasmDumper::DumpLabel(const char *strLabel, int indentLevelCode)
{
	if (indentLevelCode == 0) {
		::fprintf(_fp, "%s\n", strLabel);
	} else {
		String indentCode = MakePadding(indentLevelCode, "| ");
		::fprintf(_fp, "%s%s\n", JustifyLeft(strLabel, 9 + 3 * _nColsPerLine + 1).c_str(),
				  indentCode.c_str());
	}
}

void DisasmDumper::DumpCode(const char *strCode, int indentLevelCode)
{
	String indentCode = MakePadding(indentLevelCode, "| ");
	::fprintf(_fp, "%s%s%s\n", _paddingLeft.c_str(), indentCode.c_str(), strCode);
}

void DisasmDumper::DumpLabelAndCode(const char *strLabel, const char *strCode, int indentLevelCode)
{
	String indentCode = MakePadding(indentLevelCode, "| ");
	::fprintf(_fp, "%s%s%s\n", JustifyLeft(strLabel, 9 + 3 * _nColsPerLine + 1).c_str(),
			  indentCode.c_str(), strCode);
}

void DisasmDumper::DumpDataAndCode(Number addr, const Binary &buff, const char *strCode, int indentLevelCode)
{
	const char *formatData = _upperCaseFlag? " %02X" : " %02x";
	const char *formatHead = _upperCaseFlag? "    %04X%s  %s%s\n" : "    %04x%s  %s%s\n";
	const char *formatFollow = _upperCaseFlag? "    %04X%s\n" : "    %04x%s\n";
	String indentCode = MakePadding(indentLevelCode, "| ");
	String strRow;
	size_t iCol = 0;
	size_t iLine = 0;
	size_t nColsFold = (buff.size() <= _nColsPerLine)? _nColsPerLine : _nColsPerLine / 2 * 2;
	for (auto data : buff) {
		char strData[16];
		::sprintf_s(strData, formatData, static_cast<UInt8>(data));
		strRow += strData;
		iCol++;
		if (iCol == nColsFold) {
			strRow = JustifyLeft(strRow.c_str(), 3 * _nColsPerLine);
			if (iLine == 0) {
				::fprintf(_fp, formatHead, addr, strRow.c_str(), indentCode.c_str(), strCode);
			} else if (!indentCode.empty()) {
				::fprintf(_fp, formatHead, addr, strRow.c_str(), indentCode.c_str(), "");
			} else {
				::fprintf(_fp, formatFollow, addr, strRow.c_str());
			}
			strRow.clear();
			addr += static_cast<Number>(iCol);
			iCol = 0;
			iLine++;
		}
	}
	if (iCol > 0) {
		strRow = JustifyLeft(strRow.c_str(), 3 * _nColsPerLine);
		if (iLine == 0) {
			::fprintf(_fp, formatHead, addr, strRow.c_str(), indentCode.c_str(), strCode);
		} else if (!indentCode.empty()) {
			::fprintf(_fp, formatHead, addr, strRow.c_str(), indentCode.c_str(), "");
		} else {
			::fprintf(_fp, formatFollow, addr, strRow.c_str());
		}
	}
}

