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
	static const char *_asmCodeTmpl;
public:
	DeclareReferenceAccessor(PCGPage);
public:
	inline PCGPage(const String &symbol, PCGType pcgType, int charCodeStart) :
		_cntRef(1), _symbol(symbol), _pcgType(pcgType), _charCodeStart(charCodeStart) {}
private:
	inline ~PCGPage() {}
public:
	inline PCGType GetPCGType() const { return _pcgType; }
	inline int GetCharCodeStart() const { return _charCodeStart; }
	inline int GetCharCodeCur() const { return _charCodeStart + static_cast<int>(_pcgPatternOwner.size()); }
	inline const PCGPatternOwner &GetPCGPatternOwner() const { return _pcgPatternOwner; }
	const PCGPattern *AddPCGPattern(const Binary &buff);
	Expr *GenerateExpr(const char *pathNameSrc) const;
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
