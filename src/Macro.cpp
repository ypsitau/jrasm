//=============================================================================
// Expr.cpp
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// Macro
//-----------------------------------------------------------------------------

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
