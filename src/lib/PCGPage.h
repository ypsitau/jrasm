//=============================================================================
// PCGPage.h
//=============================================================================
#ifndef __JRASMCORE_PCGPAGE_H__
#define __JRASMCORE_PCGPAGE_H__

#include "PCGChar.h"

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
	PCGCharOwner _pcgCharOwner;
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
	inline bool IsEmpty() const { return _pcgCharOwner.empty(); }
	inline int GetCharCodeStart() const { return _charCodeStart; }
	inline int GetCharCodeEnd() const { return _charCodeStart + static_cast<int>(_pcgCharOwner.size()) - 1; }
	inline int GetCharCodeCur() const { return _charCodeStart + static_cast<int>(_pcgCharOwner.size()); }
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
