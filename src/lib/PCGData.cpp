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
	char str[4096];
	const char *formatForwardX =
		"        STX     [forwardx%d+1]\n"
		"        INC     [forwardx%d+1]\n"
		"forwardx%d:\n"
		"        LDX     0x0000\n";
	do { // create macro: PCG.symbol.PUT
		::sprintf_s(str, "PCG.%s.PUT:\n", GetSymbol());
		asmCode += str;
		asmCode += "        .MACRO offset=0\n";
		int charCodePrev = -1;
		int iCol = 0, iRow = 0;
		int iBoundary = 1;
		for (auto pPCGChar : _pcgCharOwner) {
			int offsetBase = iCol * _stepX + iRow * _stepY;
			if (offsetBase >= iBoundary * 0x100) {
				::sprintf_s(str, formatForwardX, iBoundary, iBoundary, iBoundary);
				asmCode += str;
				iBoundary++;
			}
			int charCode = pPCGChar->GetCharCode();
			if (_pcgType == PCGTYPE_User) {
				charCode -= 32;
				if (charCode >= 32) {
					charCode = charCode - 32 + 0x80;
				}
			}
			//if (_pcgType == PCGTYPE_User && charCode >= 0x20) {
			//	charCode = charCode - 0x20 + 0x80;
			//}
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
			::sprintf_s(str, "        STAA    [x+0x%02x+offset]\n", static_cast<UInt8>(offsetBase));
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
		int iCol = 0, iRow = 0;
		int iBoundary = 1;
		for (size_t i = 0; i < _pcgCharOwner.size(); i++) {
			int offsetBase = iCol * _stepX + iRow * _stepY;
			if (offsetBase >= iBoundary * 0x100) {
				::sprintf_s(str, formatForwardX, iBoundary, iBoundary, iBoundary);
				asmCode += str;
				iBoundary++;
			}
			::sprintf_s(str, "        STAA    [x+0x%02x+offset]\n", static_cast<UInt8>(offsetBase));
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
		asmCode += "        .MACRO data,offset=0\n";
		::sprintf_s(str, "        LDAA    data\n");
		asmCode += str;
		int iCol = 0, iRow = 0;
		int iBoundary = 1;
		for (size_t i = 0; i < _pcgCharOwner.size(); i++) {
			int offsetBase = iCol * _stepX + iRow * _stepY;
			if (offsetBase >= iBoundary * 0x100) {
				::sprintf_s(str, formatForwardX, iBoundary, iBoundary, iBoundary);
				asmCode += str;
				iBoundary++;
			}
			::sprintf_s(str, "        STAA    [x+0x%02x+offset]\n", static_cast<UInt8>(offsetBase));
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
		::sprintf_s(str, "        .MACRO fg=7,bg=0,offset=0\n");
		asmCode += str;
		if (_pcgType == PCGTYPE_CRAM) {
			::sprintf_s(str, "        LDAA    fg+(bg<<3)\n");
		} else { // _pcgType == PCGTYPE_User
			::sprintf_s(str, "        LDAA    (1<<6)+fg+(bg<<3)\n");
		}
		asmCode += str;
		int iCol = 0, iRow = 0;
		int iBoundary = 1;
		for (size_t i = 0; i < _pcgCharOwner.size(); i++) {
			int offsetBase = iCol * _stepX + iRow * _stepY;
			if (offsetBase >= iBoundary * 0x100) {
				::sprintf_s(str, formatForwardX, iBoundary, iBoundary, iBoundary);
				asmCode += str;
				iBoundary++;
			}
			::sprintf_s(str, "        STAA    [x+0x%02x+offset]\n", static_cast<UInt8>(offsetBase));
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
		::sprintf_s(str, "        .MACRO fg=7,bg=0,offset=0\n");
		asmCode += str;
		::sprintf_s(str, "        LDAA    fg+(bg<<3)\n");
		asmCode += str;
		int iCol = 0, iRow = 0;
		int iBoundary = 1;
		for (size_t i = 0; i < _pcgCharOwner.size(); i++) {
			int offsetBase = iCol * _stepX + iRow * _stepY;
			if (offsetBase >= iBoundary * 0x100) {
				::sprintf_s(str, formatForwardX, iBoundary, iBoundary, iBoundary);
				asmCode += str;
				iBoundary++;
			}
			::sprintf_s(str, "        STAA    [x+0x%02x+offset]\n", static_cast<UInt8>(offsetBase));
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
	return parser.GetExprRoot()->Reference();
}
