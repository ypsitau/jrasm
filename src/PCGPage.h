//=============================================================================
// PCGPage.h
//=============================================================================
#ifndef __PCGPAGE_H__
#define __PCGPAGE_H__

#include "PCGPattern.h"

class Expr;

//-----------------------------------------------------------------------------
// PCGPage
//-----------------------------------------------------------------------------
class PCGPage {
private:
	int _cntRef;
	String _symbol;
	PCGType _pcgType;
	int _charCodeStart;
	PCGPatternOwner _pcgPatternOwner;
	static const char *_asmCodeTmpl1;
	static const char *_asmCodeTmpl2;
public:
	DeclareReferenceAccessor(PCGPage);
public:
	inline PCGPage(const String &symbol, PCGType pcgType, int charCodeStart) :
		_cntRef(1), _symbol(symbol), _pcgType(pcgType), _charCodeStart(charCodeStart) {}
private:
	inline ~PCGPage() {}
public:
	inline const char *GetSymbol() const { return _symbol.c_str(); }
	inline PCGType GetPCGType() const { return _pcgType; }
	inline int GetCharCodeStart() const { return _charCodeStart; }
	inline int GetCharCodeCur() const { return _charCodeStart + static_cast<int>(_pcgPatternOwner.size()); }
	inline const PCGPatternOwner &GetPCGPatternOwner() const { return _pcgPatternOwner; }
	PCGPattern *CreatePCGPattern(const Binary &buff);
	Expr *ComposeExpr() const;
};

//-----------------------------------------------------------------------------
// PCGPageList
//-----------------------------------------------------------------------------
class PCGPageList : public std::vector<PCGPage *> {
};

//-----------------------------------------------------------------------------
// PCGPageOwner
//-----------------------------------------------------------------------------
class PCGPageOwner : public PCGPageList {
public:
	~PCGPageOwner();
	void Clear();
};

#endif
