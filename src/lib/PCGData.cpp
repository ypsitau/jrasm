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
	do { // create macro: pcg.SYMBOL.put
		::sprintf_s(str, "pcg.%s.put:\n", GetSymbol());
		asmCode += str;
		asmCode += "        .macro offset=0\n";
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
			::sprintf_s(str, "        staa    [x+0x%02x+offset]\n",
						static_cast<UInt8>(iCol * _xStep + iRow * _yStep));
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
	do { // create macro: pcg.SYMBOL.erase
		::sprintf_s(str, "pcg.%s.erase:\n", GetSymbol());
		asmCode += str;
		asmCode += "        .macro offset=0\n";
		::sprintf_s(str, "        clra\n");
		asmCode += str;
		size_t iCol = 0, iRow = 0;
		for (size_t i = 0; i < _pcgCharOwner.size(); i++) {
			::sprintf_s(str, "        staa    [x+0x%02x+offset]\n",
						static_cast<UInt8>(iCol * _xStep + iRow * _yStep));
			asmCode += str;
			iCol++;
			if (iCol == _wdChar) {
				iCol = 0;
				iRow++;
			}
		}
		asmCode += "        .end\n";
	} while (0);
	do { // create macro: pcg.SYMBOL.fill
		::sprintf_s(str, "pcg.%s.fill:\n", GetSymbol());
		asmCode += str;
		asmCode += "        .macro code,offset=0\n";
		::sprintf_s(str, "        ldaa    code\n");
		asmCode += str;
		size_t iCol = 0, iRow = 0;
		for (size_t i = 0; i < _pcgCharOwner.size(); i++) {
			::sprintf_s(str, "        staa    [x+0x%02x+offset]\n",
						static_cast<UInt8>(iCol * _xStep + iRow * _yStep));
			asmCode += str;
			iCol++;
			if (iCol == _wdChar) {
				iCol = 0;
				iRow++;
			}
		}
		asmCode += "        .end\n";
	} while (0);
	do { // create macro: pcg.SYMBOL.putattr
		::sprintf_s(str, "pcg.%s.putattr:\n", GetSymbol());
		asmCode += str;
		::sprintf_s(str, "        .macro foreground,background,offset=0\n");
		asmCode += str;
		if (_pcgType == PCGTYPE_CRAM) {
			::sprintf_s(str, "        ldaa    foreground+(background<<3)\n");
		} else { // _pcgType == PCGTYPE_User
			::sprintf_s(str, "        ldaa    (1<<6)+foreground+(background<<3)\n");
		}
		asmCode += str;
		size_t iCol = 0, iRow = 0;
		for (size_t i = 0; i < _pcgCharOwner.size(); i++) {
			::sprintf_s(str, "        staa    [x+0x%02x+offset]\n",
						static_cast<UInt8>(iCol * _xStep + iRow * _yStep));
			asmCode += str;
			iCol++;
			if (iCol == _wdChar) {
				iCol = 0;
				iRow++;
			}
		}
		asmCode += "        .end\n";
	} while (0);
	do { // create macro: pcg.SYMBOL.eraseattr
		::sprintf_s(str, "pcg.%s.eraseattr:\n", GetSymbol());
		asmCode += str;
		::sprintf_s(str, "        .macro foreground,background,offset\n");
		asmCode += str;
		::sprintf_s(str, "        ldaa    foreground+(background<<3)\n");
		asmCode += str;
		size_t iCol = 0, iRow = 0;
		for (size_t i = 0; i < _pcgCharOwner.size(); i++) {
			::sprintf_s(str, "        staa    [x+0x%02x+offset]\n",
						static_cast<UInt8>(iCol * _xStep + iRow * _yStep));
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
