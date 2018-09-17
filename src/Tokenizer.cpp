//=============================================================================
// Tokenizer.cpp
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// Tokenizer
//-----------------------------------------------------------------------------
Tokenizer::Tokenizer(Listener *pListener, const String &fileNameSrc) :
	_stat(STAT_LineTop), _pListener(pListener),
	_pFileNameSrc(new StringShared(fileNameSrc)), _num(0), _nLines(0)
{
}

bool Tokenizer::FeedChar(char ch)
{
	bool rtn = true;
	BeginPushbackRegion();
	switch (_stat) {
	case STAT_LineTop: {
		if (IsEOF(ch) || IsEOL(ch)) {
			// nothing to do
		} else {
			_stat = STAT_Neutral;
			Pushback();
		}
		break;
	}
	case STAT_Neutral: {
		if (IsEOF(ch) || IsEOL(ch)) {
			rtn = FeedToken(TOKEN_EOL);
			_stat = STAT_LineTop;
		} else if (IsWhite(ch)) {
			_stat = STAT_White;
		} else if (IsSymbolFirst(ch)) {
			_str.clear();
			_str += ch;
			_stat = STAT_Symbol;
		} else if (ch == '"') {
			_str.clear();
			_stat = STAT_String;
		} else if (ch == '0') {
			_num = 0;
			_str.clear();
			_str += ch;
			_stat = STAT_DetectZero;
		} else if (IsDigit(ch)) {
			_num = 0;
			_str.clear();
			_stat = STAT_DecNumber;
			Pushback();
		} else if (ch == ';') {
			_stat = STAT_Comment;
		} else if (ch == ':') {
			rtn = FeedToken(TOKEN_Colon);
		} else if (ch == ',') {
			rtn = FeedToken(TOKEN_Comma);
		} else if (ch == '+') {
			rtn = FeedToken(TOKEN_Plus);
		} else if (ch == '-') {
			rtn = FeedToken(TOKEN_Minus);
		} else if (ch == '*') {
			rtn = FeedToken(TOKEN_Asterisk);
		} else if (ch == '/') {
			rtn = FeedToken(TOKEN_Slash);
		} else if (ch == '[') {
			rtn = FeedToken(TOKEN_BracketL);
		} else if (ch == ']') {
			rtn = FeedToken(TOKEN_BracketR);
		} else if (ch == '(') {
			rtn = FeedToken(TOKEN_ParenthesisL);
		} else if (ch == ')') {
			rtn = FeedToken(TOKEN_ParenthesisR);
		} else {
			if (::isprint(ch)) {
				AddError("invalid character: %c", ch);
			} else {
				AddError("invalid character: 0x%02x", ch);
			}
			rtn = false;
		}
		break;
	}
	case STAT_White: {
		if (IsWhite(ch)) {
			// nothing to do
		} else {
			rtn = FeedToken(TOKEN_White);
			_stat = STAT_Neutral;
			Pushback();
		}
		break;
	}
	case STAT_Comment: {
		if (IsEOF(ch) || IsEOL(ch)) {
			rtn = FeedToken(TOKEN_EOL);
			_stat = STAT_LineTop;
		} else {
			// nothing to do
		}
		break;
	}
	case STAT_Symbol: {
		if (IsSymbolFollow(ch)) {
			_str += ch;
		} else {
			rtn = FeedToken(TOKEN_Symbol, _str);
			_stat = STAT_Neutral;
			Pushback();
		}
		break;
	}
	case STAT_String: {
		if (IsEOF(ch) || IsEOL(ch)) {
			AddError("unclosed string literal");
			rtn = false;
		} else if (ch == '"') {
			rtn = FeedToken(TOKEN_String, _str);
			_stat = STAT_Neutral;
		} else if (ch == '\\') {
			_stat = STAT_StringEsc;
		} else {
			_str += ch;
		}
		break;
	}
	case STAT_StringEsc: {
		if (ch == '"') {
			_str += ch;
		} else if (ch == 'n') {
			_str += '\n';
		} else if (ch == 'r') {
			_str += '\r';
		} else {
			AddError("invalid escape character");
			rtn = false;
		}
		break;
	}
	case STAT_DetectZero: {
		if (ch == 'x') {
			_str += ch;
			_stat = STAT_HexNumber;
		} else if (::isdigit(ch)) {
			_stat = STAT_OctNumber;
			Pushback();
		} else {
			rtn = FeedToken(TOKEN_Number, _str, _num);
			_stat = STAT_Neutral;
			Pushback();
		}
		break;
	}
	case STAT_OctNumber: {
		if ('0' <= ch && ch <= '7') {
			_str += ch;
			_num = _num * 8 + (ch - '0');
		} else if ('8' <= ch && ch <= '9') {
			AddError("decimal number must not start with zero");
			rtn = false;
		} else {
			rtn = FeedToken(TOKEN_Number, _str, _num);
			_stat = STAT_Neutral;
			Pushback();
		}
		break;
	}
	case STAT_DecNumber: {
		if (IsDigit(ch)) {
			_str += ch;
			_num = _num * 10 + (ch - '0');
		} else {
			rtn = FeedToken(TOKEN_Number, _str, _num);
			_stat = STAT_Neutral;
			Pushback();
		}
		break;
	}
	case STAT_HexNumber: {
		if (IsDigit(ch)) {
			_str += ch;
			_num = (_num << 4) + (ch - '0');
		} else if ('a' <= ch && ch <= 'f') {
			_str += ch;
			_num = (_num << 4) + (ch - 'a' + 10);
		} else if ('A' <= ch && ch <= 'F') {
			_str += ch;
			_num = (_num << 4) + (ch - 'A' + 10);
		} else {
			rtn = FeedToken(TOKEN_Number, _str, _num);
			_stat = STAT_Neutral;
			Pushback();
		}
		break;
	}
	}
	EndPushbackRegion();
	if (IsEOL(ch)) _nLines++;
	return rtn;
}

bool Tokenizer::FeedToken(const TokenInfo &tokenInfo)
{
	return _pListener->FeedToken(new Token(tokenInfo, _nLines + 1));
}

bool Tokenizer::FeedToken(const TokenInfo &tokenInfo, const String &str)
{
	return _pListener->FeedToken(new Token(tokenInfo, _nLines + 1, str));
}

bool Tokenizer::FeedToken(const TokenInfo &tokenInfo, const String &str, UInt32 num)
{
	return _pListener->FeedToken(new Token(tokenInfo, _nLines + 1, str, num));
}

void Tokenizer::AddError(const char *format, ...) const
{
	va_list ap;
	va_start(ap, format);
	AddErrorV(format, ap);
}

void Tokenizer::AddErrorV(const char *format, va_list ap) const
{
	ErrorLog::AddErrorV(GetFileNameSrc(), _nLines + 1, format, ap);
}
