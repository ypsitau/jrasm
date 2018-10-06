//=============================================================================
// PCGData.h
//=============================================================================
#ifndef __PCGDATA_H__
#define __PCGDATA_H__

#include "PCGChar.h"

class Expr;

//-----------------------------------------------------------------------------
// PCGData
//-----------------------------------------------------------------------------
class PCGData {
private:
	int _cntRef;
	String _symbol;
	PCGType _pcgType;
	size_t _wdChar;
	size_t _htChar;
	size_t _xStep;
	size_t _yStep;
	size_t _xOffset;
	size_t _yOffset;
	PCGCharOwner _pcgCharOwner;
public:
	DeclareReferenceAccessor(PCGData);
public:
	inline PCGData(const String &symbol, PCGType pcgType, size_t wdChar, size_t htChar,
				   size_t xStep, size_t yStep, size_t xOffset, size_t yOffset) :
		_cntRef(1), _symbol(symbol), _pcgType(pcgType), _wdChar(wdChar), _htChar(htChar),
		_xStep(xStep), _yStep(yStep), _xOffset(xOffset), _yOffset(yOffset) {}
private:
	inline ~PCGData() {}
public:
	inline const char *GetSymbol() const { return _symbol.c_str(); }
	inline PCGType GetPCGType() const { return _pcgType; }
	inline size_t GetWdChar() const { return _wdChar; }
	inline size_t GetHtChar() const { return _htChar; }
	inline size_t GetXStep() const { return _xStep; }
	inline size_t GetYStep() const { return _yStep; }
	inline size_t GetXOffset() const { return _xOffset; }
	inline size_t GetYOffset() const { return _yOffset; }
	inline void AddPCGChar(PCGChar *pPCGChar) { _pcgCharOwner.push_back(pPCGChar); }
	Expr *ComposeExpr() const;
};

#endif
