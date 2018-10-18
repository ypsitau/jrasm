//=============================================================================
// PCGData.h
//=============================================================================
#ifndef __JRASMCORE_PCGDATA_H__
#define __JRASMCORE_PCGDATA_H__

#include "PCGChar.h"

class Expr;

//-----------------------------------------------------------------------------
// PCGData
//-----------------------------------------------------------------------------
class PCGData {
private:
	int _cntRef;
	String _symbol;
	size_t _wdChar;
	size_t _htChar;
	size_t _stepX;
	size_t _stepY;
	PCGCharOwner _pcgCharOwner;
public:
	DeclareReferenceAccessor(PCGData);
public:
	inline PCGData(const String &symbol,
				   size_t wdChar, size_t htChar, size_t stepX, size_t stepY) :
		_cntRef(1), _symbol(symbol), _wdChar(wdChar), _htChar(htChar),
		_stepX(stepX), _stepY(stepY) {}
private:
	inline ~PCGData() {}
public:
	inline const char *GetSymbol() const { return _symbol.c_str(); }
	inline size_t GetWdChar() const { return _wdChar; }
	inline size_t GetHtChar() const { return _htChar; }
	inline size_t GetStepX() const { return _stepX; }
	inline size_t GetStepY() const { return _stepY; }
	inline void AddPCGChar(PCGChar *pPCGChar) { _pcgCharOwner.push_back(pPCGChar); }
	Expr *ComposeExpr() const;
};

#endif
