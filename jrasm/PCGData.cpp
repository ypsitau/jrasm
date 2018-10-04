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
	do { // create pcg.SYMBOL.put macro
		::sprintf_s(str, "pcg.%s.put:\n", GetSymbol());
		asmCode += str;
		asmCode += "        .macro\n";
		int charCodePrev = -1;
		size_t iCol = 0, iRow = 0;
		for (auto pPCGChar : _pcgCharOwner) {
			if (pPCGChar->GetCharCode() == 0) {
				::sprintf_s(str, "        clra\n");
				asmCode += str;
			} else if (charCodePrev >= 0 && pPCGChar->GetCharCode() - charCodePrev == 1) {
				::sprintf_s(str, "        inca\n");
				asmCode += str;
			} else if (charCodePrev >= 0 && pPCGChar->GetCharCode() - charCodePrev == -1) {
				::sprintf_s(str, "        deca\n");
				asmCode += str;
			} else if (charCodePrev >= 0 && pPCGChar->GetCharCode() == charCodePrev) {
				// nothing to do
			} else {
				::sprintf_s(str, "        ldaa    0x%02x\n", pPCGChar->GetCharCode());
				asmCode += str;
			}
			::sprintf_s(str, "        staa    [x+0x%02x]\n", static_cast<UInt8>(iCol + iRow * 0x20));
			asmCode += str;
			iCol++;
			if (iCol == _wdChar) {
				iCol = 0;
				iRow++;
			}
			charCodePrev = pPCGChar->GetCharCode();
		}
		asmCode += "        .end\n";
	} while (0);
	do { // create pcg.SYMBOL.erase macro
		::sprintf_s(str, "pcg.%s.erase:\n", GetSymbol());
		asmCode += str;
		asmCode += "        .macro\n";
		::sprintf_s(str, "        clra\n");
		asmCode += str;
		size_t iCol = 0, iRow = 0;
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
	do { // create pcg.SYMBOL.putattr macro
		::sprintf_s(str, "pcg.%s.putattr:\n", GetSymbol());
		asmCode += str;
		::sprintf_s(str, "        .macro foreground,background\n");
		asmCode += str;
		if (_pcgType == PCGTYPE_CRAM) {
			::sprintf_s(str, "        ldaa    foreground+background*8\n");
		} else { // _pcgType == PCGTYPE_User
			::sprintf_s(str, "        ldaa    0x40+foreground+background*8\n");
		}
		asmCode += str;
		size_t iCol = 0, iRow = 0;
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
	do { // create pcg.SYMBOL.eraseattr macro
		::sprintf_s(str, "pcg.%s.eraseattr:\n", GetSymbol());
		asmCode += str;
		::sprintf_s(str, "        .macro foreground,background\n");
		asmCode += str;
		::sprintf_s(str, "        ldaa    foreground+background*8\n");
		asmCode += str;
		size_t iCol = 0, iRow = 0;
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
	//::printf("%s", asmCode.c_str());
	Parser parser("***PCGData.cpp***");
	if (!parser.ParseString(asmCode.c_str())) return nullptr;
	return parser.GetRoot()->Reference();
}
