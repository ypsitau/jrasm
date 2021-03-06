//=============================================================================
// Parser.cpp
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// Parser
//-----------------------------------------------------------------------------
Parser::Parser(const String &pathNameSrc) :
	_tokenizer(this, pathNameSrc), _stat(STAT_LineTop), _pExprStack(new ExprStack())
{
	_pExprStack->push_back(new Expr_Group());
}

bool Parser::ParseFile()
{
	FILE *fp = nullptr;
	if (::strcmp(GetPathNameSrc(), "-") == 0) {
		fp = stdin;
	} else if (::fopen_s(&fp, GetPathNameSrc(), "rt") != 0) {
		ErrorLog::AddError("failed to open file: %s\n", GetPathNameSrc());
		return false;
	}
	for (;;) {
		int chRaw = ::fgetc(fp);
		char ch = (chRaw < 0)? '\0' : static_cast<unsigned char>(chRaw);
		if (!FeedChar(ch)) break;
		if (ch == '\0') break;
	}
	if (fp != stdin) ::fclose(fp);
	return !ErrorLog::HasError();
}

bool Parser::ParseString(const char *str)
{
	const char *p = str;
	for (;;) {
		char ch = *p++;
		if (!FeedChar(ch)) break;
		if (ch == '\0') break;
	}
	return !ErrorLog::HasError();
}

bool Parser::FeedToken(AutoPtr<Token> pToken)
{
	//::printf("%s\n", pToken->ToString().c_str());
	switch (_stat) {
	case STAT_LineTop: {
		if (pToken->IsType(TOKEN_Symbol)) {
			_stat = STAT_Label;
		} else if (pToken->IsType(TOKEN_White)) {
			_stat = STAT_Directive;
		}
		break;
	}
	case STAT_Label: {
		bool forceGlobalFlag = false;
		if (pToken->IsType(TOKEN_Colon) || (forceGlobalFlag = pToken->IsType(TOKEN_ColonColon))) {
			AutoPtr<Expr> pExpr(new Expr_Label(_pTokenPrev->GetString(), forceGlobalFlag));
			SetExprSourceInfo(pExpr.get(), _pTokenPrev.get());
			_pExprStack->back()->GetExprChildren().push_back(pExpr.release());
			_stat = STAT_Directive;
		} else if (pToken->IsType(TOKEN_White)) {
			// nothing to do
		} else {
			AddError("invalid format of symbol");
			return false;
		}
		break;
	}
	case STAT_Directive: {
		if (pToken->IsType(TOKEN_White)) {
			// nothing to do
		} else if (pToken->IsType(TOKEN_Symbol)) {
			const char *symbol = pToken->GetString();
			const DirectiveFactory *pDirectiveFactory = Directive::LookupFactory(symbol);
			if (pDirectiveFactory == nullptr) {
				if (*symbol == '.') {
					AddError("unknown directive: %s", symbol);
					return false;
				}
				AutoPtr<Expr> pExpr(new Expr_Instruction(pToken->GetStringSTL()));
				SetExprSourceInfo(pExpr.get(), pToken.get());
				_pExprStack->back()->GetExprChildren().push_back(pExpr->Reference());
				_pExprStack->push_back(pExpr.release());
			} else {
				AutoPtr<Directive> pDirective(pDirectiveFactory->Create());
				if (!pDirective->OnPhaseParse(this, *_pExprStack, pToken.get())) {
					return false;
				}
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
			//::printf("%s\n", _tokenStack.ToString().c_str());
			if (_tokenStack.size() == 3 && _tokenStack[1]->IsType(TOKEN_Expr)) {
				_pExprStack->back()->GetExprOperands().push_back(_tokenStack[1]->GetExpr()->Reference());
			} else if (_tokenStack.size() == 2) {
				// nothing to do as the stack has no tokens
			} else {
				AddError("syntax error1\n");
				return false;
			}
			_tokenStack.Reset();
			if (pToken->IsType(TOKEN_EOL)) {
				_pExprStack->pop_back();
				_stat = STAT_LineTop;
			} else {
				AddError("invalid format of operands");
				return false;
			}
		}
		break;
	}
	}
	_pTokenPrev.reset(pToken->Reference());
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
				if (!ReduceOne(pToken->IsType(TOKEN_Empty))) return false;
			} else if (cntToken == 2) {
				if (!ReduceTwo()) return false;
			} else if (cntToken == 3) {
				if (!ReduceThree()) return false;
			} else {
				AddError("syntax error");
				return false;
			}
		} else {
			AddError("syntax error: %s", _tokenStack.ToString().c_str());
			return false;
		}
	}
	return true;
}

bool Parser::ReduceOne(bool lastFlag)
{
	AutoPtr<Token> pToken(_tokenStack.Pop());
	AutoPtr<Expr> pExpr;
	if (pToken->IsType(TOKEN_Integer)) {
		// [Exp] -> [Num]
		pExpr.reset(new Expr_Integer(pToken->GetStringSTL(), pToken->GetInteger()));
	} else if (pToken->IsType(TOKEN_Symbol)) {
		// [Exp] -> [Sym]
		pExpr.reset(new Expr_Symbol(pToken->GetStringSTL()));
	} else if (pToken->IsType(TOKEN_String)) {
		// [Exp] -> [Str]
		pExpr.reset(new Expr_String(pToken->GetStringSTL()));
	} else if (pToken->IsType(TOKEN_BitPattern)) {
		// [Exp] -> [BPt]
		size_t len = pToken->GetStringSTL().size();
		if (len % 8 != 0) {
			AddError("the bit pattern length is %zu, not a multiple of eight", len);
			return false;
		}
		pExpr.reset(new Expr_BitPattern(pToken->GetStringSTL(), BitPatternToBinary(pToken->GetString())));
	} else if (pToken->IsType(TOKEN_MML)) {
		// [Exp] -> [MML]
		AutoPtr<BinaryShared> pBuffShared(new BinaryShared());
		if (!_mmlParser.Parse(pToken->GetString(), pBuffShared->Reference())) {
			AddError("%s", _mmlParser.GetError());
			return false;
		}
		pExpr.reset(new Expr_MML(pToken->GetStringSTL(), pBuffShared.release()));
	} else if (pToken->IsType(TOKEN_Comma)) {
		// (null) -> ,
		_pExprStack->back()->GetExprOperands().push_back(new Expr_Null());
		if (lastFlag) {
			_pExprStack->back()->GetExprOperands().push_back(new Expr_Null());
		}
	} else {
		AddError("unacceptable syntax: %s", pToken->ToString().c_str());
		return false;
	}
	if (!pExpr.IsNull()) {
		SetExprSourceInfo(pExpr.get(), pToken.get());
		_tokenStack.Push(new Token(pExpr.release()));
	}
	return true;
}

bool Parser::ReduceTwo()
{
	AutoPtr<Token> pToken2(_tokenStack.Pop());
	AutoPtr<Token> pToken1(_tokenStack.Pop());
	if (pToken1->IsType(TOKEN_Expr) && pToken2->IsType(TOKEN_Comma)) {
		// (null) -> [Exp] ,
		_pExprStack->back()->GetExprOperands().push_back(pToken1->GetExpr()->Reference());
	} else if (pToken2->IsType(TOKEN_Expr)) {
		// [Exp] -> OP [Exp]
		AutoPtr<Expr> pExprOperand(pToken2->GetExpr()->Reference());
		AutoPtr<Expr> pExpr;
		if (pToken1->IsType(TOKEN_Plus)) {
			pExpr.reset(new Expr_UnaryOp(Operator::Add, pExprOperand.release()));
		} else if (pToken1->IsType(TOKEN_Minus)) {
			if (pExprOperand->IsTypeInteger()) {
				Expr_Integer *pExprEx = dynamic_cast<Expr_Integer *>(pExprOperand.get());
				String str = "-";
				str += pExprEx->ComposeSource(false);
				pExpr.reset(new Expr_Integer(str, -pExprEx->GetInteger()));
			} else {
				pExpr.reset(new Expr_UnaryOp(Operator::Sub, pExprOperand.release()));
			}
#if 0
		} else if (pToken1->IsType(TOKEN_Hash)) {
			pExpr.reset(new Expr_UnaryOp(Operator::ToString, pExprOperand.release()));
#endif
		} else {
			AddError("unacceptable unary operator: %s", pToken1->GetSymbol());
			return false;
		}
		SetExprSourceInfo(pExpr.get(), pToken1.get());
		_tokenStack.Push(new Token(pExpr.release()));
	} else {
		AddError("unacceptable syntax: %s %s",
				 pToken1->ToString().c_str(), pToken2->ToString().c_str());
		return false;
	}
	return true;
}

bool Parser::ReduceThree()
{
	AutoPtr<Token> pToken3(_tokenStack.Pop());
	AutoPtr<Token> pToken2(_tokenStack.Pop());
	AutoPtr<Token> pToken1(_tokenStack.Pop());
	if (pToken1->IsType(TOKEN_Expr) && pToken3->IsType(TOKEN_Expr)) {
		// [Exp] -> [Exp] OP [Exp]
		AutoPtr<Expr> pExprL(pToken1->GetExpr()->Reference());
		AutoPtr<Expr> pExprR(pToken3->GetExpr()->Reference());
		AutoPtr<Expr> pExpr;
		if (pToken2->IsType(TOKEN_Eq)) {
			pExpr.reset(new Expr_Assign(pExprL.release(), pExprR.release()));
		} else if (pToken2->IsType(TOKEN_Plus)) {
			pExpr.reset(new Expr_BinaryOp(Operator::Add, pExprL.release(), pExprR.release()));
		} else if (pToken2->IsType(TOKEN_Minus)) {
			pExpr.reset(new Expr_BinaryOp(Operator::Sub, pExprL.release(), pExprR.release()));
		} else if (pToken2->IsType(TOKEN_LtPlus)) {
			pExpr.reset(new Expr_BinaryOp(Operator::AddInj, pExprL.release(), pExprR.release()));
		} else if (pToken2->IsType(TOKEN_Asterisk)) {
			pExpr.reset(new Expr_BinaryOp(Operator::Mul, pExprL.release(), pExprR.release()));
		} else if (pToken2->IsType(TOKEN_Slash)) {
			pExpr.reset(new Expr_BinaryOp(Operator::Div, pExprL.release(), pExprR.release()));
		} else if (pToken2->IsType(TOKEN_Percent)) {
			pExpr.reset(new Expr_BinaryOp(Operator::Mod, pExprL.release(), pExprR.release()));
		} else if (pToken2->IsType(TOKEN_VBarVBar)) {
			pExpr.reset(new Expr_BinaryOp(Operator::LogicOr, pExprL.release(), pExprR.release()));
		} else if (pToken2->IsType(TOKEN_AmpAmp)) {
			pExpr.reset(new Expr_BinaryOp(Operator::LogicAnd, pExprL.release(), pExprR.release()));
		} else if (pToken2->IsType(TOKEN_VBar)) {
			pExpr.reset(new Expr_BinaryOp(Operator::Or, pExprL.release(), pExprR.release()));
		} else if (pToken2->IsType(TOKEN_Hat)) {
			pExpr.reset(new Expr_BinaryOp(Operator::Xor, pExprL.release(), pExprR.release()));
		} else if (pToken2->IsType(TOKEN_Amp)) {
			pExpr.reset(new Expr_BinaryOp(Operator::And, pExprL.release(), pExprR.release()));
		} else if (pToken2->IsType(TOKEN_EqEq)) {
			pExpr.reset(new Expr_BinaryOp(Operator::Eq, pExprL.release(), pExprR.release()));
		} else if (pToken2->IsType(TOKEN_BangEq)) {
			pExpr.reset(new Expr_BinaryOp(Operator::NotEq, pExprL.release(), pExprR.release()));
		} else if (pToken2->IsType(TOKEN_Lt)) {
			pExpr.reset(new Expr_BinaryOp(Operator::Lt, pExprL.release(), pExprR.release()));
		} else if (pToken2->IsType(TOKEN_LtEq)) {
			pExpr.reset(new Expr_BinaryOp(Operator::Le, pExprL.release(), pExprR.release()));
		} else if (pToken2->IsType(TOKEN_Gt)) {
			pExpr.reset(new Expr_BinaryOp(Operator::Gt, pExprL.release(), pExprR.release()));
		} else if (pToken2->IsType(TOKEN_GtEq)) {
			pExpr.reset(new Expr_BinaryOp(Operator::Ge, pExprL.release(), pExprR.release()));
		} else if (pToken2->IsType(TOKEN_LtLt)) {
			pExpr.reset(new Expr_BinaryOp(Operator::ShiftL, pExprL.release(), pExprR.release()));
		} else if (pToken2->IsType(TOKEN_GtGt)) {
			pExpr.reset(new Expr_BinaryOp(Operator::ShiftR, pExprL.release(), pExprR.release()));
		} else if (pToken2->IsType(TOKEN_Colon)) {
			pExpr.reset(new Expr_BinaryOp(Operator::FieldSep, pExprL.release(), pExprR.release()));
		} else {
			AddError("unacceptable binary operator: %s", pToken2->GetSymbol());
			return false;
		}
		SetExprSourceInfo(pExpr.get(), pToken1.get());
		_tokenStack.Push(new Token(pExpr.release()));
	} else if (pToken1->IsType(TOKEN_ParenL) && pToken3->IsType(TOKEN_ParenR)) {
		// [Exp] -> ( [Exp] )
		_tokenStack.Push(pToken2.release());
	} else if (pToken1->IsType(TOKEN_BracketL) && pToken2->IsType(TOKEN_Expr) && pToken3->IsType(TOKEN_BracketR)) {
		// [Exp] -> [ [Exp] ]
		AutoPtr<Expr_Bracket> pExpr(dynamic_cast<Expr_Bracket *>(pToken1->GetExpr()->Reference()));
		pExpr->GetExprOperands().push_back(pToken2->GetExpr()->Reference());
		_tokenStack.Push(new Token(pExpr.release()));
	} else if (pToken1->IsType(TOKEN_BracketL) && pToken2->IsType(TOKEN_Expr) && pToken3->IsType(TOKEN_Comma)) {
		// [ -> [ [Exp] ,
		AutoPtr<Expr_Bracket> pExpr(dynamic_cast<Expr_Bracket *>(pToken1->GetExpr()->Reference()));
		pExpr->GetExprOperands().push_back(pToken2->GetExpr()->Reference());
		_tokenStack.Push(pToken1.release());
	} else if (pToken1->IsType(TOKEN_BraceL) && pToken2->IsType(TOKEN_Expr) && pToken3->IsType(TOKEN_BraceR)) {
		// [Exp] -> { [Exp] }
		AutoPtr<Expr_Brace> pExpr(dynamic_cast<Expr_Brace *>(pToken1->GetExpr()->Reference()));
		pExpr->GetExprOperands().push_back(pToken2->GetExpr()->Reference());
		_tokenStack.Push(new Token(pExpr.release()));
	} else if (pToken1->IsType(TOKEN_BraceL) && pToken2->IsType(TOKEN_Expr) && pToken3->IsType(TOKEN_Comma)) {
		// { -> { [Exp] ,
		AutoPtr<Expr_Brace> pExpr(dynamic_cast<Expr_Brace *>(pToken1->GetExpr()->Reference()));
		pExpr->GetExprOperands().push_back(pToken2->GetExpr()->Reference());
		_tokenStack.Push(pToken1.release());
	} else {
		AddError("unacceptable syntax: %s %s %s",
				 pToken1->ToString().c_str(), pToken2->ToString().c_str(), pToken3->ToString().c_str());
		return false;
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
