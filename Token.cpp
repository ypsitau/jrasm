//=============================================================================
// Token.cpp
//=============================================================================
#include "Token.h"

String Token::ToString() const
{
	static const TypeInfo typeInfoTbl[] = {
		{ TYPE_NewLine,			"NL"	},
		{ TYPE_White,			"Wh"	},
		{ TYPE_Number,			"Num"	},
		{ TYPE_Symbol,			"Sym"	},
		{ TYPE_Colon,			":"		},
		{ TYPE_Comma,			","		},
		{ TYPE_Plus,			"+"		},
		{ TYPE_Minus,			"-"		},
		{ TYPE_Asterisk,		"*"		},
		{ TYPE_Slash,			"/"		},
		{ TYPE_BracketL,		"["		},
		{ TYPE_BracketR,		"]"		},
		{ TYPE_ParenthesisL,	"("		},
		{ TYPE_ParenthesisR,	")"		},
	};
	String str;
	for (int i = 0; i < ArraySizeOf(typeInfoTbl); i++) {
		if (_type == typeInfoTbl[i].type) {
			str = typeInfoTbl[i].name;
			if (_validStrFlag) {
				str += " ";
				str += _str;
			}
			break;
		}
	}
	return str;
}
