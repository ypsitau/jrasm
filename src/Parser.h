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
	AutoPtr<ExprStack> _pExprStack;
	TokenStack _tokenStack;
	AutoPtr<Token> _pTokenPrev;
public:
	Parser(const String &fileNameSrc);
	inline const char *GetPathNameSrc() const { return _tokenizer.GetPathNameSrc(); }
	inline const Expr *GetRoot() const { return _pExprStack->front(); }
	inline bool FeedChar(char ch) { return _tokenizer.FeedChar(ch); }
	inline void SetExprSourceInfo(Expr *pExpr, const Token *pToken) const {
		pExpr->SetSourceInfo(_tokenizer.GetPathNameSrcShared()->Reference(), pToken->GetLineNo());
	}
	void AddError(const char *format, ...) const;
	void AddErrorV(const char *format, va_list ap) const;
public:
	// implementation for Tokenizer::Listener
	virtual bool FeedToken(AutoPtr<Token> pToken);
private:
	bool ParseByPrec(AutoPtr<Token> pToken);
};

#endif
