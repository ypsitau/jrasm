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

bool MacroDict::IsAssigned(const char *label) const
{
	return find(label) != end();
}

const Macro *MacroDict::Lookup(const char *label) const
{
	const_iterator iter = find(label);
	return (iter == end())? nullptr : iter->second;
}
