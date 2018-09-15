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
	bool rtn = true;
	//::printf("%s\n", pToken->ToString().c_str());
	switch (_stat) {
	case STAT_LineTop: {
		if (pToken->IsType(TOKEN_Symbol)) {
			Expr *pExpr = new Expr_Label(pToken->GetString());
			_exprOwner.push_back(pExpr);
			_stat = STAT_Label;
		} else if (pToken->IsType(TOKEN_White)) {
			_stat = STAT_Instruction;
		}
		break;
	}
	case STAT_Label: {
		if (pToken->IsType(TOKEN_Colon)) {
			_stat = STAT_Instruction;
		} else if (pToken->IsType(TOKEN_White)) {
			// nothing to do
		} else {
			_tokenizer.AddError("invalid format of label");
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
		} else if (pToken->IsType(TOKEN_EOL)) {
			_stat = STAT_LineTop;
		} else {
			_tokenizer.AddError("instruction or pseudo command is expected");
			rtn = false;
		}
		break;
	}
	case STAT_Operand: {
		if (pToken->IsType(TOKEN_White)) {
			// nothing to do
		//} else if (pToken->HasPrecedence()) {
		//	rtn = ParseByPrec(pToken.release());
		} else if (pToken->IsType(TOKEN_EOL)) {
			_exprStack.pop_back();
			_stat = STAT_LineTop;
#if 1
		} else if (pToken->IsType(TOKEN_Comma)) {
			
		} else if (pToken->IsType(TOKEN_Symbol)) {
			_exprStack.back()->AddChild(new Expr_Symbol(pToken->GetString()));
		} else if (pToken->IsType(TOKEN_Number)) {
			_exprStack.back()->AddChild(new Expr_Number(pToken->GetNumber()));
		} else if (pToken->IsType(TOKEN_Plus)) {

		} else if (pToken->IsType(TOKEN_Minus)) {

		} else if (pToken->IsType(TOKEN_Asterisk)) {

		} else if (pToken->IsType(TOKEN_Slash)) {

#endif
		} else if (pToken->IsType(TOKEN_BracketL)) {
			Expr *pExpr = new Expr_Bracket();
			_exprStack.back()->AddChild(pExpr);
			_exprStack.push_back(pExpr);
		} else if (pToken->IsType(TOKEN_BracketR)) {
			if (!_exprStack.back()->IsType(Expr::TYPE_Bracket)) {
				_tokenizer.AddError("no opening bracket matched");
			}
			_exprStack.pop_back();
		} else if (pToken->IsType(TOKEN_ParenthesisL)) {
			Expr *pExpr = new Expr_Parenthesis();
			_exprStack.back()->AddChild(pExpr);
			_exprStack.push_back(pExpr);
		} else if (pToken->IsType(TOKEN_ParenthesisR)) {
			if (!_exprStack.back()->IsType(Expr::TYPE_Parenthesis)) {
				_tokenizer.AddError("no opening parenthesis matched");
			}
			_exprStack.pop_back();
		} else {
			_tokenizer.AddError("invalid format of operands");
			rtn = false;
		}
		break;
	}
	}
	return rtn;
}

bool Parser::ParseByPrec(AutoPtr<Token> pToken)
{
	for (;;) {
		TokenStack::reverse_iterator ppTokenTop = _tokenStack.SeekTerminal(_tokenStack.rbegin());
		::printf("%s  << %s\n", _tokenStack.ToString().c_str(), pToken->GetSymbol());
		Token::Precedence prec = Token::LookupPrec(**ppTokenTop, *pToken);
		if (prec == Token::PREC_LT || prec == Token::PREC_EQ) {
			_tokenStack.Push(pToken.release());
			break;
		} else if (prec != Token::PREC_GT) {
			_tokenizer.AddError("syntax error");
			return false;
		}
		TokenStack::reverse_iterator ppTokenLeft;
		TokenStack::reverse_iterator ppTokenRight = ppTokenTop;
		while (1) {
			ppTokenLeft = _tokenStack.SeekTerminal(ppTokenRight + 1);
			if (Token::LookupPrec(**ppTokenLeft, **ppTokenRight) == Token::PREC_LT) {
				ppTokenLeft--;
				break;
			}
			ppTokenRight = ppTokenLeft;
		}
		size_t cntToken = std::distance(_tokenStack.rbegin(), ppTokenLeft) + 1;
		if (cntToken == 1) {
			AutoPtr<Token> pToken(_tokenStack.Pop());
			if (pToken->IsType(TOKEN_Symbol)) {
				_tokenStack.Push(new Token(new Expr_Symbol(pToken->GetString())));
			} else if (pToken->IsType(TOKEN_Number)) {
				_tokenStack.Push(new Token(new Expr_Number(pToken->GetNumber())));
			} else if (pToken->IsType(TOKEN_String)) {
				_tokenStack.Push(new Token(new Expr_String(pToken->GetString())));
			} else {
				_tokenizer.AddError("invalid value type\n");
				return false;
			}
		} else if (cntToken == 3) {
			AutoPtr<Token> pToken3(_tokenStack.Pop());
			AutoPtr<Token> pToken2(_tokenStack.Pop());
			AutoPtr<Token> pToken1(_tokenStack.Pop());
			if (pToken1->IsType(TOKEN_Expr) && pToken3->IsType(TOKEN_Expr)) {
				const Expr *pExprL = pToken1->GetExpr();
				const Expr *pExprR = pToken1->GetExpr();
				if (pToken2->IsType(TOKEN_Plus)) {
					_tokenStack.Push(
						new Token(new Expr_BinOp(Operator::Add, pExprL->Reference(), pExprR->Reference())));
				} else if (pToken2->IsType(TOKEN_Minus)) {
					_tokenStack.Push(
						new Token(new Expr_BinOp(Operator::Sub, pExprL->Reference(), pExprR->Reference())));
				} else if (pToken2->IsType(TOKEN_Asterisk)) {
					_tokenStack.Push(
						new Token(new Expr_BinOp(Operator::Mul, pExprL->Reference(), pExprR->Reference())));
				} else if (pToken2->IsType(TOKEN_Slash)) {
					_tokenStack.Push(
						new Token(new Expr_BinOp(Operator::Div, pExprL->Reference(), pExprR->Reference())));
				}
			}
		} else {
			_tokenizer.AddError("invalid number of token\n");
			return false;
		}
	}
	return true;
}