//=============================================================================
// Macro.h
//=============================================================================
#ifndef __MACRO_H__
#define __MACRO_H__

#include "Expr.h"

//-----------------------------------------------------------------------------
// Macro
//-----------------------------------------------------------------------------
class Macro {
private:
	int _cntRef;
	String _symbol;
	StringList _paramNames;
	AutoPtr<ExprOwner> _pExprOwner;
public:
	Macro(const String &symbol, StringList::const_iterator pParamName,
		  StringList::const_iterator pParamNameEnd, ExprOwner *pExprOwner);
public:
	DeclareReferenceAccessor(Macro);
public:
	inline Macro() : _cntRef(1) {}
private:
	inline ~Macro() {}
public:
	inline const char *GetSymbol() const { return _symbol.c_str(); }
	inline const ExprOwner &GetExprOwner() const { return *_pExprOwner; }
	inline const StringList &GetParamNames() const { return _paramNames; }
};

//-----------------------------------------------------------------------------
// MacroDict
//-----------------------------------------------------------------------------
class MacroDict : public std::map<String, Macro *, LessThan_StringICase> {
public:
	inline MacroDict() {}
	~MacroDict();
public:
	void Assign(const String &label, Macro *pMacro);
	bool IsDefined(const char *label) const;
	const Macro *Lookup(const char *label) const;
};

#endif

