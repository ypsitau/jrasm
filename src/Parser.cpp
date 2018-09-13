//=============================================================================
// Parser.cpp
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// Parser
//-----------------------------------------------------------------------------
Parser::Parser(const String &fileNameSrc) : _tokenizer(this, fileNameSrc), _stat(STAT_LineTop)
{
}

bool Parser::FeedToken(const Token &token)
{
	bool rtn = true;
	//::printf("%s\n", token.ToString().c_str());
	switch (_stat) {
	case STAT_LineTop: {
		if (token.IsType(Token::TYPE_Symbol)) {
			_stat = STAT_Label;
		} else if (token.IsType(Token::TYPE_White)) {
			_stat = STAT_Instruction;
		}
		break;
	}
	case STAT_Label: {
		if (token.IsType(Token::TYPE_Colon)) {
			
		} else if (token.IsType(Token::TYPE_White)) {
			// nothing to do
		} else {
			_tokenizer.SetErrMsg("invalid format of label");
			rtn = false;
		}
		break;
	}
	case STAT_Instruction: {
		if (token.IsType(Token::TYPE_Symbol)) {
			Element *pElem = new Element_Inst(token.GetString());
			_elemOwner.push_back(pElem);
			_elemStack.push_back(pElem);
			_stat = STAT_Operand;
		} else {
			_tokenizer.SetErrMsg("instruction or pseudo command is expected");
			rtn = false;
		}
		break;
	}
	case STAT_Operand: {
		if (token.IsType(Token::TYPE_White)) {
			// nothing to do
		} else if (token.IsType(Token::TYPE_EOL)) {
			_elemStack.pop_back();
			_stat = STAT_LineTop;
		} else if (token.IsType(Token::TYPE_Comma)) {
			
		} else if (token.IsType(Token::TYPE_Symbol)) {
			_elemStack.back()->AddChild(new Element_Symbol(token.GetString()));
		} else if (token.IsType(Token::TYPE_Number)) {
			_elemStack.back()->AddChild(new Element_Number(token.GetNumber()));
		} else if (token.IsType(Token::TYPE_Plus)) {
			new Element_BinOp_Add(nullptr, nullptr);
		} else if (token.IsType(Token::TYPE_Minus)) {
			new Element_BinOp_Sub(nullptr, nullptr);
		} else if (token.IsType(Token::TYPE_Asterisk)) {
			new Element_BinOp_Mul(nullptr, nullptr);
		} else if (token.IsType(Token::TYPE_Slash)) {
			new Element_BinOp_Div(nullptr, nullptr);
		} else if (token.IsType(Token::TYPE_BracketL)) {
			Element *pElem = new Element_Bracket();
			_elemStack.back()->AddChild(pElem);
			_elemStack.push_back(pElem);
		} else if (token.IsType(Token::TYPE_BracketR)) {
			if (!_elemStack.back()->IsType(Element::TYPE_Bracket)) {
				_tokenizer.SetErrMsg("no opening bracket matched");
			}
			_elemStack.pop_back();
		} else if (token.IsType(Token::TYPE_ParenthesisL)) {
			Element *pElem = new Element_Parenthesis();
			_elemStack.back()->AddChild(pElem);
			_elemStack.push_back(pElem);
		} else if (token.IsType(Token::TYPE_ParenthesisR)) {
			if (!_elemStack.back()->IsType(Element::TYPE_Parenthesis)) {
				_tokenizer.SetErrMsg("no opening parenthesis matched");
			}
			_elemStack.pop_back();
		} else {
			_tokenizer.SetErrMsg("invalid format of operands");
			rtn = false;
		}
		break;
	}
	}
	return rtn;
}
