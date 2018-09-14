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

bool Parser::FeedToken(AutoPtr<Token> pToken)
{
	::printf("%s\n", pToken->ToString().c_str());
	return true;
#if 0
	bool rtn = true;
	//::printf("%s\n", pToken->ToString().c_str());
	switch (_stat) {
	case STAT_LineTop: {
		if (pToken->IsType(TOKEN_Symbol)) {
			_stat = STAT_Label;
		} else if (pToken->IsType(TOKEN_White)) {
			_stat = STAT_Instruction;
		}
		break;
	}
	case STAT_Label: {
		if (pToken->IsType(TOKEN_Colon)) {
			
		} else if (pToken->IsType(TOKEN_White)) {
			// nothing to do
		} else {
			_tokenizer.SetErrMsg("invalid format of label");
			rtn = false;
		}
		break;
	}
	case STAT_Instruction: {
		if (pToken->IsType(TOKEN_Symbol)) {
			Expr *pExpr = new Expr_Inst(pToken->GetString());
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
		if (pToken->HasPrecedence()) {
			
		} else if (pToken->IsType(TOKEN_White)) {
			// nothing to do
		} else if (pToken->IsType(TOKEN_EOL)) {
			_exprStack.pop_back();
			_stat = STAT_LineTop;
#if 0
		} else if (pToken->IsType(TOKEN_Comma)) {
			
		} else if (pToken->IsType(TOKEN_Symbol)) {
			_exprStack.back()->AddChild(new Expr_Symbol(pToken->GetString()));
		} else if (pToken->IsType(TOKEN_Number)) {
			_exprStack.back()->AddChild(new Expr_Number(pToken->GetNumber()));
		} else if (pToken->IsType(TOKEN_Plus)) {
			new Expr_BinOp_Add(nullptr, nullptr);
		} else if (pToken->IsType(TOKEN_Minus)) {
			new Expr_BinOp_Sub(nullptr, nullptr);
		} else if (pToken->IsType(TOKEN_Asterisk)) {
			new Expr_BinOp_Mul(nullptr, nullptr);
		} else if (pToken->IsType(TOKEN_Slash)) {
			new Expr_BinOp_Div(nullptr, nullptr);
#endif
		} else if (pToken->IsType(TOKEN_BracketL)) {
			Expr *pExpr = new Expr_Bracket();
			_exprStack.back()->AddChild(pExpr);
			_exprStack.push_back(pExpr);
		} else if (pToken->IsType(TOKEN_BracketR)) {
			if (!_exprStack.back()->IsType(Expr::TYPE_Bracket)) {
				_tokenizer.SetErrMsg("no opening bracket matched");
			}
			_exprStack.pop_back();
		} else if (pToken->IsType(TOKEN_ParenthesisL)) {
			Expr *pExpr = new Expr_Parenthesis();
			_exprStack.back()->AddChild(pExpr);
			_exprStack.push_back(pExpr);
		} else if (pToken->IsType(TOKEN_ParenthesisR)) {
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
#endif
}
