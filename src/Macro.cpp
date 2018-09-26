//=============================================================================
// Expr.cpp
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// Macro
//-----------------------------------------------------------------------------
Macro::Macro(const String &symbol, StringList::const_iterator pParamName,
			 StringList::const_iterator pParamNameEnd, ExprOwner *pExprOwner) :
	_symbol(symbol), _pExprOwner(pExprOwner)
{
	_paramNames.insert(_paramNames.end(), pParamName, pParamNameEnd);
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

void MacroDict::Associate(const String &label, Macro *pMacro)
{
	insert(std::make_pair(label, pMacro));
}

bool MacroDict::IsDefined(const char *label) const
{
	return find(label) != end();
}

const Macro *MacroDict::Lookup(const char *label) const
{
	const_iterator iter = find(label);
	return (iter == end())? nullptr : iter->second;
}
