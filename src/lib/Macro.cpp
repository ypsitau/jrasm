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
