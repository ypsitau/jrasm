//=============================================================================
// Parser.cpp
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// Parser
//-----------------------------------------------------------------------------
Parser::Parser(const String &fileNameSrc) :
	_tokenizer(this, fileNameSrc), _stat(STAT_LineTop)
{
	_exprStack.push_back(new Expr_Root());
}

bool Parser::FeedToken(AutoPtr<Token> pToken)
{
	//::printf("%s\n", pToken->ToString().c_str());
	switch (_stat) {
	case STAT_LineTop: {
		if (pToken->IsType(TOKEN_Symbol)) {
			Expr *pExpr = new Expr_LabelDef(pToken->GetString());
			SetExprSourceInfo(pExpr, pToken.get());
			_exprStack.back()->GetChildren().push_back(pExpr);
			_stat = STAT_LabelDef;
		} else if (pToken->IsType(TOKEN_White)) {
			_stat = STAT_Directive;
		}
		break;
	}
	case STAT_LabelDef: {
		if (pToken->IsType(TOKEN_Colon)) {
			_stat = STAT_Directive;
		} else if (pToken->IsType(TOKEN_White)) {
			// nothing to do
		} else {
			AddError("invalid format of label");
			return false;
		}
		break;
	}
	case STAT_Directive: {
		if (pToken->IsType(TOKEN_White)) {
			// nothing to do
		} else if (pToken->IsType(TOKEN_Symbol)) {
			const char *symbol = pToken->GetString();
			const Directive *pDirective = Directive::FindBuiltIn(symbol);
			if (pDirective == nullptr) {
				if (*symbol == '.') {
					AddError("unknown directive: %s", symbol);
					return false;
				}
				Expr *pExpr = new Expr_Instruction(pToken->GetString());
				SetExprSourceInfo(pExpr, pToken.get());
				_exprStack.back()->GetChildren().push_back(pExpr);
				_exprStack.push_back(pExpr);
			} else if (!pDirective->HandleToken(this, _exprStack, pToken.get())) {
				return false;
			}
			_tokenStack.Reset();
			_stat = STAT_Operand;
		} else if (pToken->IsType(TOKEN_EOL)) {
			_stat = STAT_LineTop;
		} else {
			AddError("instruction or directive is expected");
			return false;
		}
		break;
	}
	case STAT_Operand: {
		if (pToken->IsType(TOKEN_White)) {
			// nothing to do
		} else if (pToken->HasPrecedence()) {
			if (!ParseByPrec(pToken.release())) return false;
		} else {
			if (!ParseByPrec(new Token(TOKEN_Empty))) return false;
			//::printf("check\n");
			//::printf("%s\n", _tokenStack.ToString().c_str());
			if (_tokenStack.size() == 3 && _tokenStack[1]->IsType(TOKEN_Expr)) {
				_exprStack.back()->AddChild(_tokenStack[1]->GetExpr()->Reference());
			} else if (_tokenStack.size() == 2) {
				// nothing to do as the stack has no tokens
			} else {
				AddError("syntax error\n");
				return false;
			}
			_tokenStack.Reset();
			if (pToken->IsType(TOKEN_EOL)) {
				_exprStack.pop_back();
				_stat = STAT_LineTop;
			} else if (pToken->IsType(TOKEN_Comma)) {
				
			} else if (pToken->IsType(TOKEN_BracketL)) {
				Expr *pExpr = new Expr_Bracket();
				_exprStack.back()->AddChild(pExpr);
				_exprStack.push_back(pExpr);
			} else if (pToken->IsType(TOKEN_BracketR)) {
				if (!_exprStack.back()->IsType(Expr::TYPE_Bracket)) {
					AddError("no opening bracket matched");
				}
				_exprStack.pop_back();
			} else if (pToken->IsType(TOKEN_ParenthesisL)) {
				Expr *pExpr = new Expr_Parenthesis();
				_exprStack.back()->AddChild(pExpr);
				_exprStack.push_back(pExpr);
			} else if (pToken->IsType(TOKEN_ParenthesisR)) {
				if (!_exprStack.back()->IsType(Expr::TYPE_Parenthesis)) {
					AddError("no opening parenthesis matched");
				}
				_exprStack.pop_back();
			} else {
				AddError("invalid format of operands");
				return false;
			}
		}
		break;
	}
	}
	return true;
}

bool Parser::ParseByPrec(AutoPtr<Token> pToken)
{
	for (;;) {
		TokenStack::reverse_iterator ppTokenTop = _tokenStack.SeekTerminal(_tokenStack.rbegin());
		//::printf("%s  << %s\n", _tokenStack.ToString().c_str(), pToken->GetSymbol());
		Token::Precedence prec = Token::LookupPrec(**ppTokenTop, *pToken);
		if (prec == Token::PREC_LT || prec == Token::PREC_EQ) {
			_tokenStack.push_back(pToken.release());
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
			if (cntToken == 1) {
				AutoPtr<Token> pToken(_tokenStack.Pop());
				AutoPtr<Expr> pExpr;
				if (pToken->IsType(TOKEN_Symbol)) {
					pExpr.reset(new Expr_LabelRef(pToken->GetString()));
				} else if (pToken->IsType(TOKEN_Number)) {
					pExpr.reset(new Expr_Number(pToken->GetNumber()));
				} else if (pToken->IsType(TOKEN_String)) {
					pExpr.reset(new Expr_String(pToken->GetString()));
				} else {
					AddError("invalid value type");
					return false;
				}
				_tokenStack.Push(new Token(pExpr.release()));
			} else if (cntToken == 3) {
				AutoPtr<Token> pToken3(_tokenStack.Pop());
				AutoPtr<Token> pToken2(_tokenStack.Pop());
				AutoPtr<Token> pToken1(_tokenStack.Pop());
				if (!pToken1->IsType(TOKEN_Expr) || !pToken3->IsType(TOKEN_Expr)) {
					AddError("syntax error");
					return false;
				}
				AutoPtr<Expr> pExprL(pToken1->GetExpr()->Reference());
				AutoPtr<Expr> pExprR(pToken3->GetExpr()->Reference());
				AutoPtr<Expr> pExpr;
				if (pToken2->IsType(TOKEN_Plus)) {
					pExpr.reset(new Expr_BinOp(Operator::Add, pExprL.release(), pExprR.release()));
				} else if (pToken2->IsType(TOKEN_Minus)) {
					pExpr.reset(new Expr_BinOp(Operator::Sub, pExprL.release(), pExprR.release()));
				} else if (pToken2->IsType(TOKEN_Asterisk)) {
					pExpr.reset(new Expr_BinOp(Operator::Mul, pExprL.release(), pExprR.release()));
				} else if (pToken2->IsType(TOKEN_Slash)) {
					pExpr.reset(new Expr_BinOp(Operator::Div, pExprL.release(), pExprR.release()));
				} else {
					AddError("syntax error");
					return false;
				}
				_tokenStack.Push(new Token(pExpr.release()));
			} else {
				AddError("syntax error");
				return false;
			}
		} else {
			AddError("syntax error");
			return false;
		}
	}
	return true;
}

void Parser::AddError(const char *format, ...) const
{
	va_list ap;
	va_start(ap, format);
	AddErrorV(format, ap);
}

void Parser::AddErrorV(const char *format, va_list ap) const
{
	_tokenizer.AddErrorV(format, ap);
}
