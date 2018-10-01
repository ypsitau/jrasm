//=============================================================================
// PCGData.cpp
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// PCGData
//-----------------------------------------------------------------------------
Expr *PCGData::ComposeExpr() const
{
	String asmCode;
	char str[256];
	do {
		::sprintf_s(str, "pcg.%s.put:\n", GetSymbol());
		asmCode += str;
		asmCode += "        .macro\n";
		size_t iCol = 0, iRow = 0;
		for (auto pPCGChar : _pcgCharOwner) {
			::sprintf_s(str, "        ldaa    0x%02x\n", pPCGChar->GetCharCode());
			asmCode += str;
			::sprintf_s(str, "        staa    [x+0x%02x]\n", static_cast<UInt8>(iCol + iRow * 0x20));
			asmCode += str;
			iCol++;
			if (iCol == _wdChar) {
				iCol = 0;
				iRow++;
			}
		}
		asmCode += "        .end\n";
	} while (0);
	do {
		::sprintf_s(str, "pcg.%s.erase:\n", GetSymbol());
		asmCode += str;
		asmCode += "        .macro\n";
		size_t iCol = 0, iRow = 0;
		::sprintf_s(str, "        ldaa    0x%02x\n", 0x00);
		asmCode += str;
		for (size_t i = 0; i < _pcgCharOwner.size(); i++) {
			::sprintf_s(str, "        staa    [x+0x%02x]\n", static_cast<UInt8>(iCol + iRow * 0x20));
			asmCode += str;
			iCol++;
			if (iCol == _wdChar) {
				iCol = 0;
				iRow++;
			}
		}
		asmCode += "        .end\n";
	} while (0);
	do {
		::sprintf_s(str, "pcg.%s.putattr:\n", GetSymbol());
		asmCode += str;
		::sprintf_s(str, "        .macro foreground,background\n");
		asmCode += str;
		size_t iCol = 0, iRow = 0;
		::sprintf_s(str, "        ldaa    foreground+background*8\n");
		asmCode += str;
		for (size_t i = 0; i < _pcgCharOwner.size(); i++) {
			::sprintf_s(str, "        staa    [x+0x%02x]\n", static_cast<UInt8>(iCol + iRow * 0x20));
			asmCode += str;
			iCol++;
			if (iCol == _wdChar) {
				iCol = 0;
				iRow++;
			}
		}
		asmCode += "        .end\n";
	} while (0);
	::printf("%s", asmCode.c_str());
	Parser parser("***PCGData.cpp***");
	if (!parser.ParseString(asmCode.c_str())) return nullptr;
	return parser.GetRoot()->Reference();
}
