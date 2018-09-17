//=============================================================================
// Parser.h
//=============================================================================
#ifndef __PARSER_H__
#define __PARSER_H__

#include "Tokenizer.h"

//-----------------------------------------------------------------------------
// Parser
//-----------------------------------------------------------------------------
class Parser : public Tokenizer::Listener {
private:
	enum Stat {
		STAT_LineTop,
		STAT_LabelDef,
		STAT_Directive,
		STAT_Operand,
	};
private:
	Tokenizer _tokenizer;
	Stat _stat;
	ExprStack _exprStack;
	TokenStack _tokenStack;
public:
	Parser(const String &fileNameSrc);
	inline Expr_Root *GetRoot() {
		return dynamic_cast<Expr_Root *>(_exprStack.front());
	}
	inline const Expr_Root *GetRoot() const {
		return dynamic_cast<const Expr_Root *>(_exprStack.front());
	}
	inline bool FeedChar(char ch) { return _tokenizer.FeedChar(ch); }
	inline void SetExprSourceInfo(Expr *pExpr, const Token *pToken) const {
		pExpr->SetSourceInfo(_tokenizer.GetFileNameSrcShared()->Reference(), pToken->GetLineNo());
	}
	bool ParseByPrec(AutoPtr<Token> pToken);
public:
	// implementation for Tokenizer::Listener
	virtual bool FeedToken(AutoPtr<Token> pToken);
};

#endif
