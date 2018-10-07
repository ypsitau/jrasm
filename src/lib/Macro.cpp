//=============================================================================
// Expr.cpp
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// Macro
//-----------------------------------------------------------------------------
Macro::Macro(const String &symbol, ExprOwner *pExprOwner) :
	_cntRef(1), _symbol(symbol), _pExprOwner(pExprOwner)
{
}

ExprOwner *Macro::Expand(Context &context, const Expr_Instruction *pExpr) const
{
	const ExprList &exprOperands = pExpr->GetExprOperands();
	AutoPtr<ExprDict> pExprDict(new ExprDict());
	const Macro::ParamOwner &paramOwner = GetParamOwner();
	Macro::ParamOwner::const_iterator ppParam = paramOwner.begin();
	ExprList::const_iterator ppExpr = exprOperands.begin();

	//::printf("%zu %zu\n", paramOwner.size(), exprOperands.size());

	for ( ; ppParam != paramOwner.end() && ppExpr != exprOperands.end(); ppParam++, ppExpr++) {
		const Macro::Param *pParam = *ppParam;
		const Expr *pExpr = *ppExpr;
		if (pExpr->IsType(Expr::TYPE_Null)) {
			const Expr *pExprDefault = pParam->GetExprDefault();
			if (pExprDefault == nullptr) {
				ErrorLog::AddError(pExpr, "no default value for parameter %s", pParam->GetSymbol());
				return nullptr;
			} else {
				pExprDict->Assign(pParam->GetSymbol(), pExprDefault->Reference(), false);
			}
		} else {
			pExprDict->Assign(pParam->GetSymbol(), pExpr->Reference(), false);
		}
	}
	for ( ; ppParam != paramOwner.end(); ppParam++) {
		const Macro::Param *pParam = *ppParam;
		const Expr *pExprDefault = pParam->GetExprDefault();
		if (pExprDefault == nullptr) {
			ErrorLog::AddError(pExpr, "too few parameters for %s", GetSymbol());
			return nullptr;
		} else {
			pExprDict->Assign(pParam->GetSymbol(), pExprDefault->Reference(), false);
		}
	}
	if (ppExpr != exprOperands.end()) {
		ErrorLog::AddError(pExpr, "too many parameters for %s", GetSymbol());
		return nullptr;
	}
	return GetExprOwner().Substitute(*pExprDict);
}

//-----------------------------------------------------------------------------
// Macro::Param
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Macro::ParamList
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Macro::ParamOwner
//-----------------------------------------------------------------------------
Macro::ParamOwner::~ParamOwner()
{
	Clear();
}

void Macro::ParamOwner::Clear()
{
	for (auto pParam : *this) {
		delete pParam;
	}
	clear();
}

//-----------------------------------------------------------------------------
// MacroDict
//-----------------------------------------------------------------------------
MacroDict::~MacroDict()
{
	for (auto iter : *this) {
		Macro::Delete(iter.second);
	}
}

void MacroDict::Assign(Macro *pMacro)
{
	insert(std::make_pair(pMacro->GetSymbol(), pMacro));
}

bool MacroDict::IsAssigned(const char *symbol) const
{
	return find(symbol) != end();
}

const Macro *MacroDict::Lookup(const char *symbol) const
{
	const_iterator iter = find(symbol);
	return (iter == end())? nullptr : iter->second;
}
