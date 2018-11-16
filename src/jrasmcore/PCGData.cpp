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
	asmCode += ComposeSource(false);
	asmCode += ComposeSource(true);
	Parser parser("***PCGData.cpp***");
	return parser.ParseString(asmCode.c_str())? parser.GetExprRoot()->Reference() : nullptr;
	//if (!parser.ParseString(asmCode.c_str())) return nullptr;
	//return parser.GetExprRoot()->Reference();
}

String PCGData::ComposeSource(bool putZeroFlag) const
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
			int offsetBase = iCol * _pcgStride.strideX + iRow * _pcgStride.strideY;
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
			int offsetBase = iCol * _pcgStride.strideX + iRow * _pcgStride.strideY;
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
			int offsetBase = iCol * _pcgStride.strideX + iRow * _pcgStride.strideY;
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
#if 0
	do { // create macro: PCG.symbol.SAVE
		::sprintf_s(str, "PCG.%s.SAVE%s:\n", GetSymbol(), suffix);
		asmCode += str;
		asmCode += "        .MACRO dst,offset=0\n";
		asmCode += "        LDX     dst\n";
		asmCode += "        LDX     [x]\n";
		asmCode += str;
		int iCol = 0, iRow = 0, iChar = 0;
		int iBoundary = 1;
		for (auto pPCGChar : _pcgCharOwner) {
			int offsetBase = iCol * _pcgStride.strideX + iRow * _pcgStride.strideY;
			if (offsetBase >= iBoundary * 0x100) {
				::sprintf_s(str, formatForwardX, iBoundary, iBoundary, iBoundary);
				asmCode += str;
				iBoundary++;
			}
			if (!pPCGChar->IsZero() || putZeroFlag) {
				::sprintf_s(str, "        LDAA    [x+0x%02x+offset]\n", static_cast<UInt8>(offsetBase));
				asmCode += str;
				::sprintf_s(str, "        .SAVE   X\n");
				asmCode += str;
				::sprintf_s(str, "        LDX     dst\n");
				asmCode += str;
				::sprintf_s(str, "        LDX     [X]\n");
				asmCode += str;
				::sprintf_s(str, "        STAA    [x+0x%02x]\n", static_cast<UInt8>(iChar));
				asmCode += str;
				::sprintf_s(str, "        .END\n");
				asmCode += str;
				iChar++;
			}
			iCol++;
			if (iCol == _wdChar) {
				iCol = 0;
				iRow++;
			}
		}
		asmCode += "        .END\n";
	} while (0);
	do { // create macro: PCG.symbol.RESTORE
		::sprintf_s(str, "PCG.%s.RESTORE%s:\n", GetSymbol(), suffix);
		asmCode += str;
		asmCode += "        .MACRO src,offset=0\n";
		int iCol = 0, iRow = 0, iChar = 0;
		int iBoundary = 1;
		for (auto pPCGChar : _pcgCharOwner) {
			int offsetBase = iCol * _pcgStride.strideX + iRow * _pcgStride.strideY;
			if (offsetBase >= iBoundary * 0x100) {
				::sprintf_s(str, formatForwardX, iBoundary, iBoundary, iBoundary);
				asmCode += str;
				iBoundary++;
			}
			if (!pPCGChar->IsZero() || putZeroFlag) {
				::sprintf_s(str, "        .SAVE   X\n");
				asmCode += str;
				::sprintf_s(str, "        LDX     src\n");
				asmCode += str;
				::sprintf_s(str, "        LDX     [X]\n");
				asmCode += str;
				::sprintf_s(str, "        LDAA    [x+0x%02x]\n", static_cast<UInt8>(iChar));
				asmCode += str;
				::sprintf_s(str, "        .END\n");
				asmCode += str;
				::sprintf_s(str, "        STAA    [x+0x%02x+offset]\n", static_cast<UInt8>(offsetBase));
				asmCode += str;
				iChar++;
			}
			iCol++;
			if (iCol == _wdChar) {
				iCol = 0;
				iRow++;
			}
		}
		asmCode += "        .END\n";
	} while (0);
#endif
	do { // create macro: PCG.symbol.PUTATTR
		::sprintf_s(str, "PCG.%s.PUTATTR%s:\n", GetSymbol(), suffix);
		asmCode += str;
		::sprintf_s(str, "        .MACRO offset=0\n");
		asmCode += str;
		int iCol = 0, iRow = 0;
		int iBoundary = 1;
		int colorFg = 7, colorBg = 0;
		int colorFgPrev = -1, colorBgPrev = -1;
		int charCount = 0;
		PCGColorOwner::iterator ppPCGColor = _pPCGColorOwner->begin();
		PCGType pcgTypePrev = PCGTYPE_None;
		for (auto pPCGChar : _pcgCharOwner) {
			int offsetBase = iCol * _pcgStride.strideX + iRow * _pcgStride.strideY;
			if (offsetBase >= iBoundary * 0x100) {
				::sprintf_s(str, formatForwardX, iBoundary, iBoundary, iBoundary);
				asmCode += str;
				iBoundary++;
			}
			if (charCount == 0) {
				if (ppPCGColor != _pPCGColorOwner->end()) {
					PCGColor *pPCGColor = *ppPCGColor;
					colorFg = pPCGColor->GetColorFg();
					colorBg = pPCGColor->GetColorBg();
					charCount = pPCGColor->GetCharCount();
					ppPCGColor++;
				}
			}
			if (charCount > 0) charCount--;
			if (!pPCGChar->IsZero() || putZeroFlag) {
				if (pcgTypePrev != pPCGChar->GetPCGType() ||
					colorFgPrev != colorFg || colorBgPrev != colorBg) {
					if (pPCGChar->GetPCGType() == PCGTYPE_CRAM) {
						::sprintf_s(str, "        LDAA    %d+(%d<<3)\n", colorFg, colorBg);
					} else { // _pcgType == PCGTYPE_User
						::sprintf_s(str, "        LDAA    (1<<6)+%d+(%d<<3)\n", colorFg, colorBg);
					}
					asmCode += str;
					pcgTypePrev = pPCGChar->GetPCGType();
					colorFgPrev = colorFg, colorBgPrev = colorBg;
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
		::sprintf_s(str, "        .MACRO offset=0\n");
		asmCode += str;
		int iCol = 0, iRow = 0;
		int iBoundary = 1;
		int colorFg = 7, colorBg = 0;
		int charCount = 0;
		PCGColorOwner::iterator ppPCGColor = _pPCGColorOwner->begin();
		for (auto pPCGChar : _pcgCharOwner) {
			int offsetBase = iCol * _pcgStride.strideX + iRow * _pcgStride.strideY;
			if (offsetBase >= iBoundary * 0x100) {
				::sprintf_s(str, formatForwardX, iBoundary, iBoundary, iBoundary);
				asmCode += str;
				iBoundary++;
			}
			if (charCount == 0) {
				if (ppPCGColor != _pPCGColorOwner->end()) {
					PCGColor *pPCGColor = *ppPCGColor;
					colorFg = pPCGColor->GetColorFg();
					colorBg = pPCGColor->GetColorBg();
					charCount = pPCGColor->GetCharCount();
					ppPCGColor++;
				}
			}
			if (charCount > 0) charCount--;
			if (!pPCGChar->IsZero() || putZeroFlag) {
				::sprintf_s(str, "        LDAA    [x+0x%02x+offset]\n", static_cast<UInt8>(offsetBase));
				asmCode += str;
				::sprintf_s(str, "        ANDA    0x38\n");
				asmCode += str;
				if (pPCGChar->GetPCGType() == PCGTYPE_CRAM) {
					::sprintf_s(str, "        ORAA    %d\n", colorFg);
				} else { // _pcgType == PCGTYPE_User
					::sprintf_s(str, "        ORAA    (1<<6)+%d\n", colorFg);
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
	do { // create macro: PCG.symbol.SETATTR
		::sprintf_s(str, "PCG.%s.SETATTR%s:\n", GetSymbol(), suffix);
		asmCode += str;
		::sprintf_s(str, "        .MACRO fg=7,bg=0,offset=0\n");
		asmCode += str;
		int iCol = 0, iRow = 0;
		int iBoundary = 1;
		PCGType pcgTypePrev = PCGTYPE_None;
		for (auto pPCGChar : _pcgCharOwner) {
			int offsetBase = iCol * _pcgStride.strideX + iRow * _pcgStride.strideY;
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
	do { // create macro: PCG.symbol.ERASEATTR
		::sprintf_s(str, "PCG.%s.ERASEATTR%s:\n", GetSymbol(), suffix);
		asmCode += str;
		::sprintf_s(str, "        .MACRO fg=7,bg=0,offset=0\n");
		asmCode += str;
		int iCol = 0, iRow = 0;
		int iBoundary = 1;
		bool firstFlag = true;
		for (auto pPCGChar : _pcgCharOwner) {
			int offsetBase = iCol * _pcgStride.strideX + iRow * _pcgStride.strideY;
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

//-----------------------------------------------------------------------------
// PCGDataList
//-----------------------------------------------------------------------------
PCGData *PCGDataList::FindBySymbol(const char *symbol) const
{
	for (auto pPCGData : *this) {
		if (::strcasecmp(pPCGData->GetSymbol(), symbol) == 0) return pPCGData;
	}
	return nullptr;
}

//-----------------------------------------------------------------------------
// PCGDataOwner
//-----------------------------------------------------------------------------
PCGDataOwner::~PCGDataOwner()
{
	Clear();
}

void PCGDataOwner::Clear()
{
	for (auto pPCGData : *this) {
		PCGData::Delete(pPCGData);
	}
	clear();
}
