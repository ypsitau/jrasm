//=============================================================================
// PCGData.cpp
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// PCGData
//-----------------------------------------------------------------------------
Expr *PCGData::ComposeExpr() const
{
	Parser parser("***PCGData.cpp***");
	String asmCode;
	asmCode += ComposeAsm(false);
	asmCode += ComposeAsm(true);
	if (!parser.ParseString(asmCode.c_str())) return nullptr;
	return parser.GetExprRoot()->Reference();
}

String PCGData::ComposeAsm(bool putZeroFlag) const
{
	String asmCode;
	char str[4096];
	const char *suffix = putZeroFlag? "X" : "";
	const char *formatForwardX =
		"        STX     [forwardx%d+1]\n"
		"        INC     [forwardx%d+1]\n"
		"forwardx%d:\n"
		"        LDX     0x0000\n";
	do { // create macro: PCG.symbol.PUT
		::sprintf_s(str, "PCG.%s.PUT%s:\n", GetSymbol(), suffix);
		asmCode += str;
		asmCode += "        .MACRO offset=0\n";
		int charCodeToPutPrev = -1;
		int iCol = 0, iRow = 0;
		int iBoundary = 1;
		for (auto pPCGChar : _pcgCharOwner) {
			int offsetBase = iCol * _stepX + iRow * _stepY;
			if (offsetBase >= iBoundary * 0x100) {
				::sprintf_s(str, formatForwardX, iBoundary, iBoundary, iBoundary);
				asmCode += str;
				iBoundary++;
			}
			int charCodeToPut = pPCGChar->GetCharCodeToPut();
			if (!pPCGChar->IsZero() || putZeroFlag) {
				if (charCodeToPut == 0) {
					::sprintf_s(str, "        CLRA\n");
					asmCode += str;
				} else if (charCodeToPutPrev >= 0 && charCodeToPut == charCodeToPutPrev + 1) {
					::sprintf_s(str, "        INCA\n");
					asmCode += str;
				} else if (charCodeToPutPrev >= 0 && charCodeToPut == charCodeToPutPrev - 1) {
					::sprintf_s(str, "        DECA\n");
					asmCode += str;
				} else if (charCodeToPutPrev >= 0 && charCodeToPut == charCodeToPutPrev) {
					// nothing to do
				} else {
					::sprintf_s(str, "        LDAA    0x%02x\n", charCodeToPut);
					asmCode += str;
				}
				::sprintf_s(str, "        STAA    [x+0x%02x+offset]\n", static_cast<UInt8>(offsetBase));
				asmCode += str;
				charCodeToPutPrev = charCodeToPut;
			}
			iCol++;
			if (iCol == _wdChar) {
				iCol = 0;
				iRow++;
			}
		}
		asmCode += "        .END\n";
	} while (0);
	do { // create macro: PCG.symbol.ERASE
		::sprintf_s(str, "PCG.%s.ERASE%s:\n", GetSymbol(), suffix);
		asmCode += str;
		asmCode += "        .MACRO offset=0\n";
		int iCol = 0, iRow = 0;
		int iBoundary = 1;
		bool firstFlag = true;
		for (auto pPCGChar : _pcgCharOwner) {
			int offsetBase = iCol * _stepX + iRow * _stepY;
			if (offsetBase >= iBoundary * 0x100) {
				::sprintf_s(str, formatForwardX, iBoundary, iBoundary, iBoundary);
				asmCode += str;
				iBoundary++;
			}
			if (!pPCGChar->IsZero() || putZeroFlag) {
				if (firstFlag) {
					::sprintf_s(str, "        CLRA\n");
					asmCode += str;
					firstFlag = false;
				}
				::sprintf_s(str, "        STAA    [x+0x%02x+offset]\n", static_cast<UInt8>(offsetBase));
				asmCode += str;
			}
			iCol++;
			if (iCol == _wdChar) {
				iCol = 0;
				iRow++;
			}
		}
		asmCode += "        .END\n";
	} while (0);
	do { // create macro: PCG.symbol.FILL
		::sprintf_s(str, "PCG.%s.FILL%s:\n", GetSymbol(), suffix);
		asmCode += str;
		asmCode += "        .MACRO data,offset=0\n";
		int iCol = 0, iRow = 0;
		int iBoundary = 1;
		bool firstFlag = true;
		for (auto pPCGChar : _pcgCharOwner) {
			int offsetBase = iCol * _stepX + iRow * _stepY;
			if (offsetBase >= iBoundary * 0x100) {
				::sprintf_s(str, formatForwardX, iBoundary, iBoundary, iBoundary);
				asmCode += str;
				iBoundary++;
			}
			if (!pPCGChar->IsZero() || putZeroFlag) {
				if (firstFlag) {
					::sprintf_s(str, "        LDAA    data\n");
					asmCode += str;
					firstFlag = false;
				}
				::sprintf_s(str, "        STAA    [x+0x%02x+offset]\n", static_cast<UInt8>(offsetBase));
				asmCode += str;
			}
			iCol++;
			if (iCol == _wdChar) {
				iCol = 0;
				iRow++;
			}
		}
		asmCode += "        .END\n";
	} while (0);
	do { // create macro: PCG.symbol.PUTATTR
		::sprintf_s(str, "PCG.%s.PUTATTR%s:\n", GetSymbol(), suffix);
		asmCode += str;
		::sprintf_s(str, "        .MACRO fg=7,bg=0,offset=0\n");
		asmCode += str;
		int iCol = 0, iRow = 0;
		int iBoundary = 1;
		PCGType pcgTypePrev = PCGTYPE_None;
		for (auto pPCGChar : _pcgCharOwner) {
			int offsetBase = iCol * _stepX + iRow * _stepY;
			if (offsetBase >= iBoundary * 0x100) {
				::sprintf_s(str, formatForwardX, iBoundary, iBoundary, iBoundary);
				asmCode += str;
				iBoundary++;
			}
			if (!pPCGChar->IsZero() || putZeroFlag) {
				if (pcgTypePrev != pPCGChar->GetPCGType()) {
					if (pPCGChar->GetPCGType() == PCGTYPE_CRAM) {
						::sprintf_s(str, "        LDAA    fg+(bg<<3)\n");
					} else { // _pcgType == PCGTYPE_User
						::sprintf_s(str, "        LDAA    (1<<6)+fg+(bg<<3)\n");
					}
					asmCode += str;
					pcgTypePrev = pPCGChar->GetPCGType();
				}
				::sprintf_s(str, "        STAA    [x+0x%02x+offset]\n", static_cast<UInt8>(offsetBase));
				asmCode += str;
			}
			iCol++;
			if (iCol == _wdChar) {
				iCol = 0;
				iRow++;
			}
		}
		asmCode += "        .END\n";
	} while (0);
	do { // create macro: PCG.symbol.PUTATTRFG
		::sprintf_s(str, "PCG.%s.PUTATTRFG%s:\n", GetSymbol(), suffix);
		asmCode += str;
		::sprintf_s(str, "        .MACRO fg=7,offset=0\n");
		asmCode += str;
		int iCol = 0, iRow = 0;
		int iBoundary = 1;
		for (auto pPCGChar : _pcgCharOwner) {
			int offsetBase = iCol * _stepX + iRow * _stepY;
			if (offsetBase >= iBoundary * 0x100) {
				::sprintf_s(str, formatForwardX, iBoundary, iBoundary, iBoundary);
				asmCode += str;
				iBoundary++;
			}
			if (!pPCGChar->IsZero() || putZeroFlag) {
				::sprintf_s(str, "        LDAA    [x+0x%02x+offset]\n", static_cast<UInt8>(offsetBase));
				asmCode += str;
				::sprintf_s(str, "        ANDA    0x38\n");
				asmCode += str;
				if (pPCGChar->GetPCGType() == PCGTYPE_CRAM) {
					::sprintf_s(str, "        ORAA    fg\n");
				} else { // _pcgType == PCGTYPE_User
					::sprintf_s(str, "        ORAA    (1<<6)+fg\n");
				}
				asmCode += str;
				::sprintf_s(str, "        STAA    [x+0x%02x+offset]\n", static_cast<UInt8>(offsetBase));
				asmCode += str;
			}
			iCol++;
			if (iCol == _wdChar) {
				iCol = 0;
				iRow++;
			}
		}
		asmCode += "        .END\n";
	} while (0);
	do { // create macro: PCG.symbol.ERASEATTR
		::sprintf_s(str, "PCG.%s.ERASEATTR%s:\n", GetSymbol(), suffix);
		asmCode += str;
		::sprintf_s(str, "        .MACRO fg=7,bg=0,offset=0\n");
		asmCode += str;
		int iCol = 0, iRow = 0;
		int iBoundary = 1;
		bool firstFlag = true;
		for (auto pPCGChar : _pcgCharOwner) {
			int offsetBase = iCol * _stepX + iRow * _stepY;
			if (offsetBase >= iBoundary * 0x100) {
				::sprintf_s(str, formatForwardX, iBoundary, iBoundary, iBoundary);
				asmCode += str;
				iBoundary++;
			}
			if (!pPCGChar->IsZero() || putZeroFlag) {
				if (firstFlag) {
					::sprintf_s(str, "        LDAA    fg+(bg<<3)\n");
					asmCode += str;
					firstFlag = false;
				}
				::sprintf_s(str, "        STAA    [x+0x%02x+offset]\n", static_cast<UInt8>(offsetBase));
				asmCode += str;
			}
			iCol++;
			if (iCol == _wdChar) {
				iCol = 0;
				iRow++;
			}
		}
		asmCode += "        .END\n";
	} while (0);
	return asmCode;
}
