//=============================================================================
// Token.cpp
//=============================================================================
#include "stdafx.h"

const TokenInfo TOKEN_Begin =			{  1, "Begin",			"[Bgn]"	};
const TokenInfo TOKEN_EOL =				{  1, "EOL",			"[EOL]"	};
const TokenInfo TOKEN_Expr =			{  1, "Expr",			"[Exp]"	};
const TokenInfo TOKEN_White =			{  1, "White",			"[Whi]"	};
const TokenInfo TOKEN_Number =			{  1, "Number",			"[Num]"	};
const TokenInfo TOKEN_Symbol =			{  1, "Symbol",			"[Sym]"	};
const TokenInfo TOKEN_String =			{  1, "String",			"[Str]"	};
const TokenInfo TOKEN_Colon =			{  1, "Colon",			":"		};
const TokenInfo TOKEN_Comma =			{  1, "Comma",			","		};
const TokenInfo TOKEN_Plus =			{  1, "Plus",			"+"		};
const TokenInfo TOKEN_Minus =			{  1, "Minus",			"-"		};
const TokenInfo TOKEN_Asterisk =		{  1, "Asterisk",		"*"		};
const TokenInfo TOKEN_Slash =			{  1, "Slash",			"/"		};
const TokenInfo TOKEN_BracketL =		{  1, "BracketL",		"["		};
const TokenInfo TOKEN_BracketR =		{  1, "BracketR",		"]"		};
const TokenInfo TOKEN_ParenthesisL =	{  1, "ParenthesisL",	"("		};
const TokenInfo TOKEN_ParenthesisR =	{  1, "ParenthesisR",	")"		};

//-----------------------------------------------------------------------------
// Token
//-----------------------------------------------------------------------------
String Token::ToString() const
{
	String str;
	str = "[";
	str += GetSymbol();
	str += "]";
	if (_validStrFlag) {
		str += " ";
		str += _str;
	}
	return str;
}

//-----------------------------------------------------------------------------
// TokenStack
//-----------------------------------------------------------------------------
TokenStack::~TokenStack()
{
}

TokenStack::reverse_iterator TokenStack::SeekTerminal(reverse_iterator ppToken)
{
	for ( ; (*ppToken)->IsType(TOKEN_Expr); ppToken++) ;
	return ppToken;
}

void TokenStack::Clear()
{
	for (auto pToken : *this) {
		Token::Delete(pToken);
	}
	clear();
}

String TokenStack::ToString() const
{
	String rtn;
	for (const_reverse_iterator ppToken = rbegin(); ppToken != rend(); ppToken++) {
		const Token *pToken = *ppToken;
		if (ppToken != rbegin()) rtn.append(" ");
		rtn.append(pToken->GetSymbol());
	}
	return rtn;
}
