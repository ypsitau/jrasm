//=============================================================================
// Token.cpp
//=============================================================================
#include "stdafx.h"
#include "Token.h"

//-----------------------------------------------------------------------------
// Token
//-----------------------------------------------------------------------------
String Token::ToString() const
{
	static const TypeInfo typeInfoTbl[] = {
		{ TYPE_EOL,				"EOL"		},
		{ TYPE_White,			"White"		},
		{ TYPE_Number,			"Number"	},
		{ TYPE_Symbol,			"Symbol"	},
		{ TYPE_String,			"String"	},
		{ TYPE_Colon,			":"			},
		{ TYPE_Comma,			","			},
		{ TYPE_Plus,			"+"			},
		{ TYPE_Minus,			"-"			},
		{ TYPE_Asterisk,		"*"			},
		{ TYPE_Slash,			"/"			},
		{ TYPE_BracketL,		"["			},
		{ TYPE_BracketR,		"]"			},
		{ TYPE_ParenthesisL,	"("			},
		{ TYPE_ParenthesisR,	")"			},
	};
	String str;
	for (int i = 0; i < ArraySizeOf(typeInfoTbl); i++) {
		if (_type == typeInfoTbl[i].type) {
			str = "[";
			str += typeInfoTbl[i].name;
			str += "]";
			if (_validStrFlag) {
				str += " ";
				str += _str;
			}
			break;
		}
	}
	return str;
}
