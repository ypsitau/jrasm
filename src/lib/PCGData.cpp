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
	do { // create macro: PCG.symbol.PUT
		::sprintf_s(str, "PCG.%s.PUT:\n", GetSymbol());
		asmCode += str;
		asmCode += "        .MACRO offset=0\n";
		int charCodePrev = -1;
		size_t iCol = 0, iRow = 0;
		for (auto pPCGChar : _pcgCharOwner) {
			int charCode = pPCGChar->GetCharCode();
			if (_pcgType == PCGTYPE_User && charCode >= 0x20) {
				charCode = charCode - 0x20 + 0x80;
			}
			if (charCode == 0) {
				::sprintf_s(str, "        CLRA\n");
				asmCode += str;
			} else if (charCodePrev >= 0 && charCode == charCodePrev + 1) {
				::sprintf_s(str, "        INCA\n");
				asmCode += str;
			} else if (charCodePrev >= 0 && charCode == charCodePrev - 1) {
				::sprintf_s(str, "        DECA\n");
				asmCode += str;
			} else if (charCodePrev >= 0 && charCode == charCodePrev) {
				// nothing to do
			} else {
				::sprintf_s(str, "        LDAA    0x%02x\n", charCode);
				asmCode += str;
			}
			::sprintf_s(str, "        STAA    [x+0x%02x+offset]\n",
						static_cast<UInt8>(iCol * _xStep + iRow * _yStep));
			asmCode += str;
			iCol++;
			if (iCol == _wdChar) {
				iCol = 0;
				iRow++;
			}
			charCodePrev = charCode;
		}
		asmCode += "        .END\n";
	} while (0);
	do { // create macro: PCG.symbol.ERASE
		::sprintf_s(str, "PCG.%s.ERASE:\n", GetSymbol());
		asmCode += str;
		asmCode += "        .MACRO offset=0\n";
		::sprintf_s(str, "        CLRA\n");
		asmCode += str;
		size_t iCol = 0, iRow = 0;
		for (size_t i = 0; i < _pcgCharOwner.size(); i++) {
			::sprintf_s(str, "        STAA    [x+0x%02x+offset]\n",
						static_cast<UInt8>(iCol * _xStep + iRow * _yStep));
			asmCode += str;
			iCol++;
			if (iCol == _wdChar) {
				iCol = 0;
				iRow++;
			}
		}
		asmCode += "        .END\n";
	} while (0);
	do { // create macro: PCG.symbol.FILL
		::sprintf_s(str, "PCG.%s.FILL:\n", GetSymbol());
		asmCode += str;
		asmCode += "        .MACRO code,offset=0\n";
		::sprintf_s(str, "        LDAA    code\n");
		asmCode += str;
		size_t iCol = 0, iRow = 0;
		for (size_t i = 0; i < _pcgCharOwner.size(); i++) {
			::sprintf_s(str, "        STAA    [x+0x%02x+offset]\n",
						static_cast<UInt8>(iCol * _xStep + iRow * _yStep));
			asmCode += str;
			iCol++;
			if (iCol == _wdChar) {
				iCol = 0;
				iRow++;
			}
		}
		asmCode += "        .END\n";
	} while (0);
	do { // create macro: PCG.symbol.PUTATTR
		::sprintf_s(str, "PCG.%s.PUTATTR:\n", GetSymbol());
		asmCode += str;
		::sprintf_s(str, "        .MACRO foreground=7,background=0,offset=0\n");
		asmCode += str;
		if (_pcgType == PCGTYPE_CRAM) {
			::sprintf_s(str, "        LDAA    foreground+(background<<3)\n");
		} else { // _pcgType == PCGTYPE_User
			::sprintf_s(str, "        LDAA    (1<<6)+foreground+(background<<3)\n");
		}
		asmCode += str;
		size_t iCol = 0, iRow = 0;
		for (size_t i = 0; i < _pcgCharOwner.size(); i++) {
			::sprintf_s(str, "        STAA    [x+0x%02x+offset]\n",
						static_cast<UInt8>(iCol * _xStep + iRow * _yStep));
			asmCode += str;
			iCol++;
			if (iCol == _wdChar) {
				iCol = 0;
				iRow++;
			}
		}
		asmCode += "        .END\n";
	} while (0);
	do { // create macro: PCG.symbol.ERASEATTR
		::sprintf_s(str, "PCG.%s.ERASEATTR:\n", GetSymbol());
		asmCode += str;
		::sprintf_s(str, "        .MACRO foreground=7,background=0,offset=0\n");
		asmCode += str;
		::sprintf_s(str, "        LDAA    foreground+(background<<3)\n");
		asmCode += str;
		size_t iCol = 0, iRow = 0;
		for (size_t i = 0; i < _pcgCharOwner.size(); i++) {
			::sprintf_s(str, "        STAA    [x+0x%02x+offset]\n",
						static_cast<UInt8>(iCol * _xStep + iRow * _yStep));
			asmCode += str;
			iCol++;
			if (iCol == _wdChar) {
				iCol = 0;
				iRow++;
			}
		}
		asmCode += "        .END\n";
	} while (0);
	Parser parser("***PCGData.cpp***");
	if (!parser.ParseString(asmCode.c_str())) return nullptr;
	return parser.GetRoot()->Reference();
}
