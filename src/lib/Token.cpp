//=============================================================================
// Token.cpp
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// TokenInfo
//-----------------------------------------------------------------------------
const TokenInfo TOKEN_Expr =			{  0, "Expr",			"[Exp]"	};	// for parsing
const TokenInfo TOKEN_Empty =			{  1, "Empty",			"[Emp]"	};	// for parsing
const TokenInfo TOKEN_Colon =			{  2, "Pair",			":"		};
const TokenInfo TOKEN_Eq =				{  3, "Eq",				"="		};
const TokenInfo TOKEN_VBarVBar =		{  4, "VBarVBar",		"||"	};
const TokenInfo TOKEN_AmpAmp =			{  5, "AmpAmp",			"&&"	};
const TokenInfo TOKEN_VBar =			{  6, "VBar",			"|"		};
const TokenInfo TOKEN_Hat =				{  7, "Hat",			"^"		};
const TokenInfo TOKEN_Amp =				{  8, "Amp",			"&"		};
const TokenInfo TOKEN_EqEq =			{  9, "EqEq",			"=="	};
const TokenInfo TOKEN_BangEq =			{  9, "BangEq",			"!="	};
const TokenInfo TOKEN_Lt =				{ 10, "Lt",				"<"		};
const TokenInfo TOKEN_LtEq =			{ 10, "LtEq",			"<="	};
const TokenInfo TOKEN_Gt =				{ 10, "Gt",				">"		};
const TokenInfo TOKEN_GtEq =			{ 10, "GtEq",			">="	};
const TokenInfo TOKEN_LtLt =			{ 11, "LtLt",			"<<"	};
const TokenInfo TOKEN_GtGt =			{ 11, "GtGt",			">>"	};
const TokenInfo TOKEN_Plus =			{ 12, "Plus",			"+"		};
const TokenInfo TOKEN_Minus =			{ 12, "Minus",			"-"		};
const TokenInfo TOKEN_LtPlus =			{ 12, "LtPlus",			"<+>"	};
const TokenInfo TOKEN_Asterisk =		{ 13, "Asterisk",		"*"		};
const TokenInfo TOKEN_Slash =			{ 13, "Slash",			"/"		};
const TokenInfo TOKEN_Percent =			{ 13, "Percent",		"%"		};
const TokenInfo TOKEN_ParenL =			{ 14, "ParenL",			"("		};
const TokenInfo TOKEN_ParenR =			{ 15, "ParenR",			")"		};
const TokenInfo TOKEN_BracketL =		{ 14, "BracketL",		"["		};
const TokenInfo TOKEN_BracketR =		{ 15, "BracketR",		"]"		};
const TokenInfo TOKEN_BraceL =			{ 14, "BraceL",			"{"		};
const TokenInfo TOKEN_BraceR =			{ 15, "BraceR",			"}"		};
const TokenInfo TOKEN_Integer =			{ 16, "Integer",		"[Int]"	};
const TokenInfo TOKEN_Symbol =			{ 16, "Symbol",			"[Sym]"	};
const TokenInfo TOKEN_String =			{ 16, "String",			"[Str]"	};
const TokenInfo TOKEN_BitPattern =		{ 16, "BitPattern",		"[BPt]"	};
const TokenInfo TOKEN_Comma =			{ 17, "Comma",			","		};
const TokenInfo TOKEN_Bang =			{  0, "Bang",			"!"		};
const TokenInfo TOKEN_ColonColon =		{  0, "ColonColon",		"::"	};
const TokenInfo TOKEN_EOL =				{  0, "EOL",			"[EOL]"	};
const TokenInfo TOKEN_White =			{  0, "White",			"[Whi]"	};

//-----------------------------------------------------------------------------
// Token
//-----------------------------------------------------------------------------
static const Token::Precedence
	LT = Token::PREC_LT,
	EQ = Token::PREC_EQ,
	GT = Token::PREC_GT,
	xx = Token::PREC_Error;

const Token::Precedence Token::_precMatrix[][18] = {
	/*          e   E   :   =  ||  &&   |   ^   & ==   <  <<    +   *   (   )   V   ,  */
	/*  e */ { xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, },
	/*  E */ { xx, EQ, LT, LT, LT, LT, LT, LT, LT, LT, LT, LT, LT, LT, LT, xx, LT, LT, },
	/*  : */ { xx, GT, GT, LT, LT, LT, LT, LT, LT, LT, LT, LT, LT, LT, LT, GT, LT, GT, },
	/*  = */ { xx, GT, GT, GT, LT, LT, LT, LT, LT, LT, LT, LT, LT, LT, LT, GT, LT, GT, },
	/* || */ { xx, GT, GT, GT, GT, LT, LT, LT, LT, LT, LT, LT, LT, LT, LT, GT, LT, GT, },
	/* && */ { xx, GT, GT, GT, GT, GT, LT, LT, LT, LT, LT, LT, LT, LT, LT, GT, LT, GT, },
	/*  | */ { xx, GT, GT, GT, GT, GT, GT, LT, LT, LT, LT, LT, LT, LT, LT, GT, LT, GT, },
	/*  ^ */ { xx, GT, GT, GT, GT, GT, GT, GT, LT, LT, LT, LT, LT, LT, LT, GT, LT, GT, },
	/*  & */ { xx, GT, GT, GT, GT, GT, GT, GT, GT, LT, LT, LT, LT, LT, LT, GT, LT, GT, },
	/* == */ { xx, GT, GT, GT, GT, GT, GT, GT, GT, GT, LT, LT, LT, LT, LT, GT, LT, GT, },
	/*  < */ { xx, GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, LT, LT, LT, LT, GT, LT, GT, },
	/* << */ { xx, GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, LT, LT, LT, GT, LT, GT, },
	/*  + */ { xx, GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, LT, LT, GT, LT, GT, },
	/*  * */ { xx, GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, LT, GT, LT, GT, },
	/*  ( */ { xx, GT, LT, LT, LT, LT, LT, LT, LT, LT, LT, LT, LT, LT, LT, EQ, LT, EQ, },
	/*  ) */ { xx, GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, xx, GT, xx, GT, },
	/*  V */ { xx, GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, xx, GT, xx, GT, },
	/*  , */ { xx, GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, xx, GT, GT, },
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
