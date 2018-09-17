//=============================================================================
// Parser.cpp
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// Parser
//-----------------------------------------------------------------------------
Parser::Parser(const String &fileNameSrc) :
	_tokenizer(this, fileNameSrc), _stat(STAT_LineTop), _pExprRoot(new Expr_Root())
{
}

bool Parser::FeedToken(AutoPtr<Token> pToken)
{
	//return ParseByPrec(pToken.release());
	//::printf("%s\n", pToken->ToString().c_str());
	switch (_stat) {
	case STAT_LineTop: {
		if (pToken->IsType(TOKEN_Symbol)) {
			Expr *pExpr = new Expr_LabelDef(pToken->GetString());
			SetExprSourceInfo(pExpr, pToken.get());
			_pExprRoot->GetChildren().push_back(pExpr);
			_stat = STAT_LabelDef;
		} else if (pToken->IsType(TOKEN_White)) {
			_stat = STAT_Instruction;
		}
		break;
	}
	case STAT_LabelDef: {
		if (pToken->IsType(TOKEN_Colon)) {
			_stat = STAT_Instruction;
		} else if (pToken->IsType(TOKEN_White)) {
			// nothing to do
		} else {
			_tokenizer.AddError("invalid format of label");
			return false;
		}
		break;
	}
	case STAT_Instruction: {
		if (pToken->IsType(TOKEN_White)) {
			// nothing to do
		} else if (pToken->IsType(TOKEN_Symbol)) {
			bool equFlag = false;
			const char *symbol = pToken->GetString();
			Expr *pExpr = nullptr;
			if (::strcasecmp(symbol, ".cseg") == 0) {
				pExpr = new Expr_Directive(Directive::CSEG);
			} else if (::strcasecmp(symbol, ".db") == 0) {
				pExpr = new Expr_Directive(Directive::DB);
			} else if (::strcasecmp(symbol, ".dseg") == 0) {
				pExpr = new Expr_Directive(Directive::DSEG);
			} else if (::strcasecmp(symbol, ".dw") == 0) {
				pExpr = new Expr_Directive(Directive::DW);
			} else if (::strcasecmp(symbol, ".endm") == 0) {
				pExpr = new Expr_Directive(Directive::ENDM);
			} else if (::strcasecmp(symbol, ".endp") == 0) {
				pExpr = new Expr_Directive(Directive::ENDP);
			} else if (::strcasecmp(symbol, ".equ") == 0) {
				pExpr = new Expr_Directive(Directive::EQU);
				equFlag = true;
			} else if (::strcasecmp(symbol, ".include") == 0) {
				pExpr = new Expr_Directive(Directive::INCLUDE);
			} else if (::strcasecmp(symbol, ".macro") == 0) {
				pExpr = new Expr_Directive(Directive::MACRO);
			} else if (::strcasecmp(symbol, ".mml") == 0) {
				pExpr = new Expr_Directive(Directive::MML);
			} else if (::strcasecmp(symbol, ".org") == 0) {
				pExpr = new Expr_Directive(Directive::ORG);
			} else if (::strcasecmp(symbol, ".pcg") == 0) {
				pExpr = new Expr_Directive(Directive::PCG);
			} else if (::strcasecmp(symbol, ".proc") == 0) {
				pExpr = new Expr_Directive(Directive::PROC);
			} else if (*symbol == '.') {
				_tokenizer.AddError("unknown directive: %s", symbol);
				return false;
			} else {
				pExpr = new Expr_Instruction(pToken->GetString());
			}
			SetExprSourceInfo(pExpr, pToken.get());
			if (equFlag) {
				// associate it to the last LabelDef
				ExprList &exprList = _pExprRoot->GetChildren();
				if (exprList.empty() || !exprList.back()->IsType(Expr::TYPE_LabelDef)) {
					_tokenizer.AddError("no label to associate .equ directive");
					return false;
				}
				Expr_LabelDef *pExprEx = dynamic_cast<Expr_LabelDef *>(exprList.back());
				if (pExprEx->IsAssigned()) {
					_tokenizer.AddError("no label to associate .equ directive");
					return false;
				}
				pExprEx->SetAssigned(pExpr);
			} else {
				_pExprRoot->GetChildren().push_back(pExpr);
			}
			_exprStack.push_back(pExpr);
			_stat = STAT_Operand;
		} else if (pToken->IsType(TOKEN_EOL)) {
			_stat = STAT_LineTop;
		} else {
			_tokenizer.AddError("instruction or directive is expected");
			return false;
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
			Expr *pExpr = new Expr_LabelRef(pToken->GetString());
			SetExprSourceInfo(pExpr, pToken.get());
			_exprStack.back()->AddChild(pExpr);
		} else if (pToken->IsType(TOKEN_Number)) {
			Expr *pExpr = new Expr_Number(pToken->GetNumber());
			SetExprSourceInfo(pExpr, pToken.get());
			_exprStack.back()->AddChild(pExpr);
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
			return false;
		}
		break;
	}
	}
	return true;
}

bool Parser::ParseByPrec(AutoPtr<Token> pToken)
{
	::printf("%d\n", __LINE__);
	if (pToken->IsType(TOKEN_White)) return true;
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
		::printf("%d\n", __LINE__);
		while (1) {
			::printf("check %d\n", __LINE__);
			ppTokenLeft = _tokenStack.SeekTerminal(ppTokenRight + 1);
			::printf("check %d\n", __LINE__);
			if (Token::LookupPrec(**ppTokenLeft, **ppTokenRight) == Token::PREC_LT) {
				ppTokenLeft--;
				break;
			}
			::printf("check %d\n", __LINE__);
			ppTokenRight = ppTokenLeft;
		}
		::printf("check %d\n", __LINE__);
		size_t cntToken = std::distance(_tokenStack.rbegin(), ppTokenLeft) + 1;
		::printf("cntToken = %d\n", cntToken);
		if (cntToken == 1) {
			AutoPtr<Token> pToken(_tokenStack.Pop());
			if (pToken->IsType(TOKEN_Symbol)) {
				_tokenStack.Push(new Token(new Expr_LabelRef(pToken->GetString())));
			} else if (pToken->IsType(TOKEN_Number)) {
				_tokenStack.Push(new Token(new Expr_Number(pToken->GetNumber())));
			} else if (pToken->IsType(TOKEN_String)) {
				_tokenStack.Push(new Token(new Expr_String(pToken->GetString())));
			} else {
				_tokenizer.AddError("invalid value type\n");
				return false;
			}
		} else if (cntToken == 2) {
			::printf("check\n");
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
			_tokenizer.AddError("invalid syntax");
			return false;
		}
	}
	::printf("%d\n", __LINE__);
	return true;
}
