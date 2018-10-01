//=============================================================================
// PCGData.cpp
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// PCGData
//-----------------------------------------------------------------------------
Expr *PCGData::GenerateExpr(const char *pathNameSrc) const
{
	char asmCode[1024];
	::sprintf_s(asmCode, _asmCodeTmpl, GetSymbol());
	Parser parser(pathNameSrc);
	if (!parser.ParseString(asmCode)) return nullptr;
	return parser.GetRoot()->Reference();
}

const char *PCGData::_asmCodeTmpl = R"**(pcgdata.%s.put:
        .macro
        .end
)**";
