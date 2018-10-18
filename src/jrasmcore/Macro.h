//=============================================================================
// Macro.h
//=============================================================================
#ifndef __JRASMCORE_MACRO_H__
#define __JRASMCORE_MACRO_H__

#include "Expr.h"

//-----------------------------------------------------------------------------
// Macro
//-----------------------------------------------------------------------------
class Macro {
public:
	class Param {
	private:
		String _symbol;
		AutoPtr<Expr> _pExprDefault;
	public:
		inline Param(const String &symbol) : _symbol(symbol) {}
		inline Param(const String &symbol, Expr *pExprDefault) :
			_symbol(symbol), _pExprDefault(pExprDefault) {}
		inline const char *GetSymbol() const { return _symbol.c_str(); }
		inline const Expr *GetExprDefault() const { return _pExprDefault.get(); }
	};
	class ParamList : public std::vector<Param *> {
	};
	class ParamOwner : public ParamList {
	public:
		~ParamOwner();
		void Clear();
	};
private:
	int _cntRef;
	String _symbol;
	ParamOwner _paramOwner;
	AutoPtr<ExprOwner> _pExprOwner;
public:
	DeclareReferenceAccessor(Macro);
public:
	Macro(const String &symbol, ExprOwner *pExprOwner);
private:
	inline ~Macro() {}
public:
	inline const char *GetSymbol() const { return _symbol.c_str(); }
	inline const ExprOwner &GetExprOwner() const { return *_pExprOwner; }
	inline ParamOwner &GetParamOwner() { return _paramOwner; }
	inline const ParamOwner &GetParamOwner() const { return _paramOwner; }
	ExprOwner *Expand(Context &context, const Expr_Instruction *pExpr) const;
};

//-----------------------------------------------------------------------------
// MacroDict
//-----------------------------------------------------------------------------
class MacroDict : public std::map<String, Macro *, LessThan_StringICase> {
public:
	inline MacroDict() {}
	~MacroDict();
public:
	void Assign(Macro *pMacro);
	bool IsAssigned(const char *symbol) const;
	const Macro *Lookup(const char *symbol) const;
};

#endif

