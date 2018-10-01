//=============================================================================
// PCGData.cpp
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// PCGData
//-----------------------------------------------------------------------------
Expr *PCGData::ComposeExpr() const
{
	char asmCode[1024];
	::sprintf_s(asmCode, _asmCodeTmpl, GetSymbol());
	Parser parser("***PCGData.cpp***");
	if (!parser.ParseString(asmCode)) return nullptr;
	return parser.GetRoot()->Reference();
}

const char *PCGData::_asmCodeTmpl = R"**(pcgdata.%s.put:
        .macro
        .end
)**";
