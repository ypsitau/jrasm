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
	std::unique_ptr<PCGRangeOwner> _pPCGRangeOwner;
	PCGCharOwner _pcgCharOwner;
	static const char *_asmCodeTmpl;
	static const char *_asmCodeTmpl1;
	static const char *_asmCodeTmpl2;
public:
	DeclareReferenceAccessor(PCGPage);
public:
	inline PCGPage(const String &symbol, PCGRangeOwner *pPCGRangeOwner) :
		_cntRef(1), _symbol(symbol), _pPCGRangeOwner(pPCGRangeOwner) {}
private:
	inline ~PCGPage() {}
public:
	inline const char *GetSymbol() const { return _symbol.c_str(); }
	inline PCGType GetPCGType() const { return _pPCGRangeOwner->front()->GetPCGType(); }
	inline bool IsEmpty() const { return _pcgCharOwner.empty(); }
	inline int GetCharCodeCur() const {
		return _pPCGRangeOwner->front()->GetCharCodeStart() + static_cast<int>(_pcgCharOwner.size());
	}
	inline const PCGRangeOwner &GetPCGRangeOwner() const { return *_pPCGRangeOwner; }
	inline const PCGCharOwner &GetPCGCharOwner() const { return _pcgCharOwner; }
	PCGChar *CreatePCGChar(const Binary &buff);
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
