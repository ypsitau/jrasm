//=============================================================================
// PCGData.h
//=============================================================================
#ifndef __PCGDATA_H__
#define __PCGDATA_H__

#include "PCGPattern.h"

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
	//PCGPatternOwner _pcgPatternOwner;
	static const char *_asmCodeTmpl;
public:
	DeclareReferenceAccessor(PCGData);
public:
	inline PCGData(const String &symbol, size_t wdChar, size_t htChar) : _cntRef(1) {}
private:
	inline ~PCGData() {}
public:
	inline const char *GetSymbol() const { return _symbol.c_str(); }
	inline size_t GetWdChar() const { return _wdChar; }
	inline size_t GetHtChar() const { return _htChar; }
	Expr *GenerateExpr(const char *pathNameSrc) const;
};

#endif
