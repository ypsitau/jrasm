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
			Expr *pExpr = new Expr_Inst(token.GetString());
			_exprOwner.push_back(pExpr);
			_exprStack.push_back(pExpr);
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
			_exprStack.pop_back();
			_stat = STAT_LineTop;
		} else if (token.IsType(Token::TYPE_Comma)) {
			
		} else if (token.IsType(Token::TYPE_Symbol)) {
			_exprStack.back()->AddChild(new Expr_Symbol(token.GetString()));
		} else if (token.IsType(Token::TYPE_Number)) {
			_exprStack.back()->AddChild(new Expr_Number(token.GetNumber()));
		} else if (token.IsType(Token::TYPE_Plus)) {
			new Expr_BinOp_Add(nullptr, nullptr);
		} else if (token.IsType(Token::TYPE_Minus)) {
			new Expr_BinOp_Sub(nullptr, nullptr);
		} else if (token.IsType(Token::TYPE_Asterisk)) {
			new Expr_BinOp_Mul(nullptr, nullptr);
		} else if (token.IsType(Token::TYPE_Slash)) {
			new Expr_BinOp_Div(nullptr, nullptr);
		} else if (token.IsType(Token::TYPE_BracketL)) {
			Expr *pExpr = new Expr_Bracket();
			_exprStack.back()->AddChild(pExpr);
			_exprStack.push_back(pExpr);
		} else if (token.IsType(Token::TYPE_BracketR)) {
			if (!_exprStack.back()->IsType(Expr::TYPE_Bracket)) {
				_tokenizer.SetErrMsg("no opening bracket matched");
			}
			_exprStack.pop_back();
		} else if (token.IsType(Token::TYPE_ParenthesisL)) {
			Expr *pExpr = new Expr_Parenthesis();
			_exprStack.back()->AddChild(pExpr);
			_exprStack.push_back(pExpr);
		} else if (token.IsType(Token::TYPE_ParenthesisR)) {
			if (!_exprStack.back()->IsType(Expr::TYPE_Parenthesis)) {
				_tokenizer.SetErrMsg("no opening parenthesis matched");
			}
			_exprStack.pop_back();
		} else {
			_tokenizer.SetErrMsg("invalid format of operands");
			rtn = false;
		}
		break;
	}
	}
	return rtn;
}
