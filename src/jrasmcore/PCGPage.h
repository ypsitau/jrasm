//=============================================================================
// PCGPage.h
//=============================================================================
#ifndef __JRASMCORE_PCGPAGE_H__
#define __JRASMCORE_PCGPAGE_H__

#include "PCGChar.h"
#include "PCGRange.h"

class Expr;

//-----------------------------------------------------------------------------
// PCGPage
//-----------------------------------------------------------------------------
class PCGPage {
private:
	int _cntRef;
	String _symbol;
	size_t _iPCGRangeCur;
	std::unique_ptr<PCGRangeOwner> _pPCGRangeOwner;
	PCGCharOwner _pcgCharOwner;
	static const char *_asmCodeTmpl;
public:
	DeclareReferenceAccessor(PCGPage);
public:
	inline PCGPage(const String &symbol, PCGRangeOwner *pPCGRangeOwner) :
		_cntRef(1), _symbol(symbol), _iPCGRangeCur(0), _pPCGRangeOwner(pPCGRangeOwner) {}
private:
	inline ~PCGPage() {}
public:
	inline const char *GetSymbol() const { return _symbol.c_str(); }
	inline bool IsEmpty() const { return _pcgCharOwner.empty(); }
	inline const PCGRangeOwner &GetPCGRangeOwner() const { return *_pPCGRangeOwner; }
	inline const PCGCharOwner &GetPCGCharOwner() const { return _pcgCharOwner; }
	bool GenerateCharCode(int *pCharCode, PCGType *pPCGType);
	PCGChar *CreatePCGChar(const Binary &buff, int);
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
