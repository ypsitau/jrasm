//=============================================================================
// Token.cpp
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// TokenInfo
//-----------------------------------------------------------------------------
const TokenInfo TOKEN_Begin =			{  1, "Begin",			"[Bgn]"	};
const TokenInfo TOKEN_Plus =			{  2, "Plus",			"+"		};
const TokenInfo TOKEN_Minus =			{  2, "Minus",			"-"		};
const TokenInfo TOKEN_Asterisk =		{  3, "Asterisk",		"*"		};
const TokenInfo TOKEN_Slash =			{  3, "Slash",			"/"		};
const TokenInfo TOKEN_Number =			{  4, "Number",			"[Num]"	};
const TokenInfo TOKEN_Symbol =			{  4, "Symbol",			"[Sym]"	};
const TokenInfo TOKEN_String =			{  4, "String",			"[Str]"	};
const TokenInfo TOKEN_Comma =			{  5, "Comma",			","		};
const TokenInfo TOKEN_BracketL =		{  6, "BracketL",		"["		};
const TokenInfo TOKEN_BracketR =		{  7, "BracketR",		"]"		};
const TokenInfo TOKEN_ParenthesisL =	{  8, "ParenthesisL",	"("		};
const TokenInfo TOKEN_ParenthesisR =	{  9, "ParenthesisR",	")"		};
const TokenInfo TOKEN_EOL =				{ 10, "EOL",			"[EOL]"	};
const TokenInfo TOKEN_Expr =			{  0, "Expr",			"[Exp]"	};
const TokenInfo TOKEN_White =			{  0, "White",			"[Whi]"	};
const TokenInfo TOKEN_Colon =			{  0, "Colon",			":"		};

//-----------------------------------------------------------------------------
// Token
//-----------------------------------------------------------------------------
static const Token::Precedence
	LT = Token::PREC_LT,
	EQ = Token::PREC_EQ,
	GT = Token::PREC_GT,
	xx = Token::PREC_Error;

const Token::Precedence Token::_precMatrix[][16] = {
	/*         e   B   +   *   V   ,   [   ]   (   )   E */
	/* e */ { xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx },
	/* B */ { xx, xx, LT, LT, LT, EQ, LT, xx, LT, xx, EQ },
	/* + */ { xx, xx, GT, LT, LT, GT, LT, GT, LT, GT, GT },
	/* * */ { xx, xx, GT, GT, LT, GT, LT, GT, LT, GT, GT },
	/* V */ { xx, xx, GT, GT, GT, GT, LT, GT, LT, GT, GT },
	/* , */ { xx, xx, GT, GT, GT, GT, LT, GT, LT, GT, GT },
	/* [ */ { xx, xx, GT, GT, GT, GT, LT, EQ, LT, xx, GT },
	/* ] */ { xx, xx, GT, GT, GT, GT, LT, GT, LT, EQ, GT },
	/* ( */ { xx, xx, GT, GT, GT, GT, LT, GT, LT, GT, GT },
	/* ) */ { xx, xx, GT, GT, GT, GT, LT, xx, LT, GT, GT },
};

String Token::ToString() const
{
	String str;
	str += GetSymbol();
	if (_validStrFlag) {
		str += " ";
		str += _str;
	}
	return str;
}

//-----------------------------------------------------------------------------
// TokenStack
//-----------------------------------------------------------------------------
TokenStack::TokenStack()
{
	push_back(new Token(TOKEN_Begin, 0));
}

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
	for (auto pToken : *this) {
		if (!rtn.empty()) rtn.append(" ");
		rtn.append(pToken->GetSymbol());
	}
	return rtn;
}
