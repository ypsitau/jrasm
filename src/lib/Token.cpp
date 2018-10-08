//=============================================================================
// Token.cpp
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// TokenInfo
//-----------------------------------------------------------------------------
const TokenInfo TOKEN_Expr =			{  0, "Expr",			"[Exp]"	};	// for parsing
const TokenInfo TOKEN_Empty =			{  1, "Empty",			"[Emp]"	};	// for parsing
const TokenInfo TOKEN_Eq =				{  2, "Eq",				"="		};
const TokenInfo TOKEN_VBarVBar =		{  3, "VBarVBar",		"||"	};
const TokenInfo TOKEN_AmpAmp =			{  4, "AmpAmp",			"&&"	};
const TokenInfo TOKEN_VBar =			{  5, "VBar",			"|"		};
const TokenInfo TOKEN_Hat =				{  6, "Hat",			"^"		};
const TokenInfo TOKEN_Amp =				{  7, "Amp",			"&"		};
const TokenInfo TOKEN_EqEq =			{  8, "EqEq",			"=="	};
const TokenInfo TOKEN_BangEq =			{  8, "BangEq",			"!="	};
const TokenInfo TOKEN_Lt =				{  9, "Lt",				"<"		};
const TokenInfo TOKEN_LtEq =			{  9, "LtEq",			"<="	};
const TokenInfo TOKEN_Gt =				{  9, "Gt",				">"		};
const TokenInfo TOKEN_GtEq =			{  9, "GtEq",			">="	};
const TokenInfo TOKEN_LtLt =			{ 10, "LtLt",			"<<"	};
const TokenInfo TOKEN_GtGt =			{ 10, "GtGt",			">>"	};
const TokenInfo TOKEN_Plus =			{ 11, "Plus",			"+"		};
const TokenInfo TOKEN_Minus =			{ 11, "Minus",			"-"		};
const TokenInfo TOKEN_Asterisk =		{ 12, "Asterisk",		"*"		};
const TokenInfo TOKEN_Slash =			{ 12, "Slash",			"/"		};
const TokenInfo TOKEN_Percent =			{ 12, "Percent",		"%"		};
const TokenInfo TOKEN_ParenL =			{ 13, "ParenL",			"("		};
const TokenInfo TOKEN_ParenR =			{ 14, "ParenR",			")"		};
const TokenInfo TOKEN_BracketL =		{ 13, "BracketL",		"["		};
const TokenInfo TOKEN_BracketR =		{ 14, "BracketR",		"]"		};
const TokenInfo TOKEN_BraceL =			{ 13, "BraceL",			"{"		};
const TokenInfo TOKEN_BraceR =			{ 14, "BraceR",			"}"		};
const TokenInfo TOKEN_Integer =			{ 15, "Integer",			"[Num]"	};
const TokenInfo TOKEN_Symbol =			{ 15, "Symbol",			"[Sym]"	};
const TokenInfo TOKEN_String =			{ 15, "String",			"[Str]"	};
const TokenInfo TOKEN_BitPattern =		{ 15, "BitPattern",		"[BPt]"	};
const TokenInfo TOKEN_Comma =			{ 16, "Comma",			","		};
const TokenInfo TOKEN_Bang =			{  0, "Bang",			"!"		};
const TokenInfo TOKEN_Colon =			{  0, "Colon",			":"		};
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

const Token::Precedence Token::_precMatrix[][17] = {
	/*          e   E   =  ||  &&   |   ^   & ==   <  <<    +   *   (   )   V   ,  */
	/*  e */ { xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, xx, },
	/*  E */ { xx, EQ, LT, LT, LT, LT, LT, LT, LT, LT, LT, LT, LT, LT, xx, LT, LT, },
	/*  = */ { xx, GT, GT, LT, LT, LT, LT, LT, LT, LT, LT, LT, LT, LT, GT, LT, GT, },
	/* || */ { xx, GT, GT, GT, LT, LT, LT, LT, LT, LT, LT, LT, LT, LT, GT, LT, GT, },
	/* && */ { xx, GT, GT, GT, GT, LT, LT, LT, LT, LT, LT, LT, LT, LT, GT, LT, GT, },
	/*  | */ { xx, GT, GT, GT, GT, GT, LT, LT, LT, LT, LT, LT, LT, LT, GT, LT, GT, },
	/*  ^ */ { xx, GT, GT, GT, GT, GT, GT, LT, LT, LT, LT, LT, LT, LT, GT, LT, GT, },
	/*  & */ { xx, GT, GT, GT, GT, GT, GT, GT, LT, LT, LT, LT, LT, LT, GT, LT, GT, },
	/* == */ { xx, GT, GT, GT, GT, GT, GT, GT, GT, LT, LT, LT, LT, LT, GT, LT, GT, },
	/*  < */ { xx, GT, GT, GT, GT, GT, GT, GT, GT, GT, LT, LT, LT, LT, GT, LT, GT, },
	/* << */ { xx, GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, LT, LT, LT, GT, LT, GT, },
	/*  + */ { xx, GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, LT, LT, GT, LT, GT, },
	/*  * */ { xx, GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, LT, GT, LT, GT, },
	/*  ( */ { xx, GT, LT, LT, LT, LT, LT, LT, LT, LT, LT, LT, LT, LT, EQ, LT, EQ, },
	/*  ) */ { xx, GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, xx, GT, xx, GT, },
	/*  V */ { xx, GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, xx, GT, xx, GT, },
	/*  , */ { xx, GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, xx, GT, GT, },
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
