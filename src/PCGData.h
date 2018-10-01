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
	size_t _wdChar;
	size_t _htChar;
	PCGCharOwner _pcgCharOwner;
public:
	DeclareReferenceAccessor(PCGData);
public:
	inline PCGData(const String &symbol, size_t wdChar, size_t htChar) :
		_cntRef(1), _symbol(symbol), _wdChar(wdChar), _htChar(htChar) {}
private:
	inline ~PCGData() {}
public:
	inline const char *GetSymbol() const { return _symbol.c_str(); }
	inline size_t GetWdChar() const { return _wdChar; }
	inline size_t GetHtChar() const { return _htChar; }
	inline void AddPCGChar(PCGChar *pPCGChar) { _pcgCharOwner.push_back(pPCGChar); }
	Expr *ComposeExpr() const;
};

#endif
