//=============================================================================
// Parser.h
//=============================================================================
#ifndef __JRASMCORE_PARSER_H__
#define __JRASMCORE_PARSER_H__

#include "Tokenizer.h"

//-----------------------------------------------------------------------------
// Parser
//-----------------------------------------------------------------------------
class Parser : public Tokenizer::Listener {
private:
	enum Stat {
		STAT_LineTop,
		STAT_Label,
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
	Parser(const String &pathNameSrc);
	inline const char *GetPathNameSrc() const { return _tokenizer.GetPathNameSrc(); }
	inline const Expr *GetExprRoot() const { return _pExprStack->front(); }
	inline bool FeedChar(char ch) { return _tokenizer.FeedChar(ch); }
	inline void SetExprSourceInfo(Expr *pExpr, const Token *pToken) const {
		pExpr->SetSourceInfo(_tokenizer.GetPathNameSrcShared()->Reference(), pToken->GetLineNo());
	}
	bool ParseFile();
	bool ParseString(const char *str);
	void AddError(const char *format, ...) const;
	void AddErrorV(const char *format, va_list ap) const;
public:
	// implementation for Tokenizer::Listener
	virtual bool FeedToken(AutoPtr<Token> pToken);
private:
	bool ParseByPrec(AutoPtr<Token> pToken);
	bool ReduceOne(bool lastFlag);
	bool ReduceTwo();
	bool ReduceThree();
};

#endif
