//=============================================================================
// Token.cpp
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// TokenInfo
//-----------------------------------------------------------------------------
const TokenInfo TOKEN_Empty =			{  1, "Empty",			"[Emp]"	};	// for parsing
const TokenInfo TOKEN_Expr =			{  0, "Expr",			"[Exp]"	};	// for parsing
const TokenInfo TOKEN_Plus =			{  2, "Plus",			"+"		};
const TokenInfo TOKEN_Minus =			{  2, "Minus",			"-"		};
const TokenInfo TOKEN_Asterisk =		{  3, "Asterisk",		"*"		};
const TokenInfo TOKEN_Slash =			{  3, "Slash",			"/"		};
const TokenInfo TOKEN_Number =			{  4, "Number",			"[Num]"	};
const TokenInfo TOKEN_Symbol =			{  4, "Symbol",			"[Sym]"	};
const TokenInfo TOKEN_String =			{  4, "String",			"[Str]"	};
const TokenInfo TOKEN_BitPattern =		{  4, "BitPattern",		"[BPt]"	};
const TokenInfo TOKEN_Comma =			{  0, "Comma",			","		};
const TokenInfo TOKEN_Colon =			{  0, "Colon",			":"		};
const TokenInfo TOKEN_ColonColon =		{  0, "ColonColon",		"::"	};
const TokenInfo TOKEN_BracketL =		{  0, "BracketL",		"["		};
const TokenInfo TOKEN_BracketR =		{  0, "BracketR",		"]"		};
const TokenInfo TOKEN_BraceL =			{  0, "BraceL",			"{"		};
const TokenInfo TOKEN_BraceR =			{  0, "BraceR",			"}"		};
const TokenInfo TOKEN_EOL =				{  0, "EOL",			"[EOL]"	};
const TokenInfo TOKEN_White =			{  0, "White",			"[Whi]"	};

const TokenInfo TOKEN_Eq =				{  0, "Eq",				"="		};
const TokenInfo TOKEN_Lt =				{  0, "Lt",				"<"		};
const TokenInfo TOKEN_Gt =				{  0, "Gt",				">"		};
const TokenInfo TOKEN_Amp =				{  0, "Amp",			"&"		};
const TokenInfo TOKEN_VBar =			{  0, "VBar",			"|"		};
const TokenInfo TOKEN_Hat =				{  0, "Hat",			"^"		};
const TokenInfo TOKEN_Percent =			{  0, "Percent",		"%"		};
const TokenInfo TOKEN_EqEq =			{  0, "EqEq",			"=="	};
const TokenInfo TOKEN_LtLt =			{  0, "LtLt",			"<<"	};
const TokenInfo TOKEN_GtGt =			{  0, "GtGt",			">>"	};
const TokenInfo TOKEN_AmpAmp =			{  0, "AmpAmp",			"&&"	};
const TokenInfo TOKEN_VBarVBar =		{  0, "VBarVBar",		"||"	};
const TokenInfo TOKEN_ParenL =			{  0, "ParenL",			"("		};
const TokenInfo TOKEN_ParenR =			{  0, "ParenR",			")"		};

//-----------------------------------------------------------------------------
// Token
//-----------------------------------------------------------------------------
static const Token::Precedence
	LT = Token::PREC_LT,
	EQ = Token::PREC_EQ,
	GT = Token::PREC_GT,
	xx = Token::PREC_Error;

const Token::Precedence Token::_precMatrix[][16] = {
	/*         e   E   +   *   V  */
	/* e */ { xx, xx, xx, xx, xx, },
	/* E */ { xx, EQ, LT, LT, LT, },
	/* + */ { xx, GT, GT, LT, LT, },
	/* * */ { xx, GT, GT, GT, LT, },
	/* V */ { xx, GT, GT, GT, xx, },
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
	push_back(new Token(TOKEN_Empty));
}

TokenStack::~TokenStack()
{
	Clear();
}

void TokenStack::Clear()
{
	for (auto pToken : *this) {
		Token::Delete(pToken);
	}
	clear();
}

void TokenStack::Reset()
{
	Clear();
	push_back(new Token(TOKEN_Empty));
}

TokenStack::reverse_iterator TokenStack::SeekTerminal(reverse_iterator ppToken)
{
	for ( ; (*ppToken)->IsType(TOKEN_Expr); ppToken++) ;
	return ppToken;
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
