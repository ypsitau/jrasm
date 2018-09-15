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
		STAT_Label,
		STAT_Instruction,
		STAT_Operand,
	};
private:
	Tokenizer _tokenizer;
	Stat _stat;
	ExprOwner _exprOwner;
	ExprStack _exprStack;
	TokenStack _tokenStack;
public:
	Parser(const String &fileNameSrc);
	inline const ExprOwner &GetInstructions() const { return _exprOwner; }
	inline bool FeedChar(char ch) { return _tokenizer.FeedChar(ch); }
	bool ParseByPrec(AutoPtr<Token> pToken);
public:
	// implementation for Tokenizer::Listener
	virtual bool FeedToken(AutoPtr<Token> pToken);
};

#endif
