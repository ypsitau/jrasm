//=============================================================================
// Parser.h
//=============================================================================
#ifndef __PARSER_H__
#define __PARSER_H__

#include "Common.h"

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
	ElementOwner _elemOwner;
	ElementStack _elemStack;
public:
	Parser(const String &fileNameSrc);
	inline const ElementOwner &GetInstructions() const { return _elemOwner; }
	inline bool FeedChar(char ch) { return _tokenizer.FeedChar(ch); }
	inline const char *GetErrMsg() const { return _tokenizer.GetErrMsg(); }
	virtual bool FeedToken(const Token &token);
};

#endif
