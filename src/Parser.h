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
		STAT_Instruction,
		STAT_Operand,
	};
private:
	Tokenizer _tokenizer;
	Stat _stat;
	AutoPtr<Expr_Root> _pExprRoot;
	ExprStack _exprStack;
	TokenStack _tokenStack;
public:
	Parser(const String &fileNameSrc);
	inline Expr_Root *GetRoot() { return _pExprRoot.get(); }
	inline const Expr_Root *GetRoot() const { return _pExprRoot.get(); }
	inline bool FeedChar(char ch) { return _tokenizer.FeedChar(ch); }
	bool ParseByPrec(AutoPtr<Token> pToken);
public:
	// implementation for Tokenizer::Listener
	virtual bool FeedToken(AutoPtr<Token> pToken);
private:
	inline void SetExprSourceInfo(Expr *pExpr, const Token *pToken) const {
		pExpr->SetSourceInfo(_tokenizer.GetFileNameSrcShared()->Reference(), pToken->GetLineNo());
	}
};

#endif
