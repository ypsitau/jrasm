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
	AutoPtr<Token> _pTokenPrev;
public:
	Parser(const String &fileNameSrc);
	inline const char *GetFileNameSrc() const { return _tokenizer.GetFileNameSrc(); }
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
	bool ParseFile();
	bool Prepare(Context &context);
	RegionOwner *Generate(Context &context, size_t bytesGapToJoin, UInt8 dataFiller);
	bool DumpDisasm(Context &context, FILE *fp, bool upperCaseFlag, size_t nColsPerLine) const;
	void AddError(const char *format, ...) const;
	void AddErrorV(const char *format, va_list ap) const;
public:
	// implementation for Tokenizer::Listener
	virtual bool FeedToken(AutoPtr<Token> pToken);
private:
	bool ParseByPrec(AutoPtr<Token> pToken);
};

#endif
