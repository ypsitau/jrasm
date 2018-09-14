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
#if 0
	for (;;) {
		TokenStack::reverse_iterator ppTokenTop = _tokenStack.SeekTerminal(_tokenStack.rbegin());
		::printf("%s  << %s\n", _tokenStack.ToString().c_str(), pToken->GetSymbol());
		Token::Precedence prec = Token::LookupPrec(**ppTokenTop, *pToken);
		if (prec == Token::PREC_LT || prec == Token::PREC_EQ) {
			_tokenStack.Push(pToken.release());
			break;
		} else if (prec == Token::PREC_GT) {
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
			bool rtn;
			if (cntToken == 1) {
				AutoPtr<Token> pToken(_tokenStack.Pop());
				if (pToken->IsType(TOKEN_Symbol)) {
					_tokenStack.Push(new Token(new Expr_Symbol(pToken->GetString())));
				} else if (pToken->IsType(TOKEN_Number)) {
					_tokenStack.Push(new Token(new Expr_Number(pToken->GetNumber())));
				} else if (pToken->IsType(TOKEN_String)) {
					_tokenStack.Push(new Token(new Expr_String(pToken->GetString())));
				} else {
					::printf("invalid value type\n");
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
				::printf("invalid number of token\n");
				rtn = false;
				break;
			}
		} else {
			::printf("precedence table error\n");
			break;
		}
	}
	return true;
#endif
#if 1
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
		if (pToken->IsType(TOKEN_White)) {
			// nothing to do
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
