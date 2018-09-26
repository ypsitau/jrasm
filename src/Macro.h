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
public:
	DeclareReferenceAccessor(Macro);
public:
	inline Macro() : _cntRef(1) {}
private:
	inline ~Macro() {}
};

//-----------------------------------------------------------------------------
// MacroDict
//-----------------------------------------------------------------------------
class MacroDict : public std::map<String, Macro *, LessThan_StringICase> {
public:
	inline MacroDict() {}
	~MacroDict();
public:
	void Associate(const String &label, Macro *pMacro);
	bool IsDefined(const char *label) const;
	const Macro *Lookup(const char *label) const;
};

#endif

