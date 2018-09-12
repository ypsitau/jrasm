//=============================================================================
// Tokenizer.cpp
//=============================================================================
#include "Tokenizer.h"

Tokenizer::Tokenizer(Listener *pListener) : _stat(STAT_LineTop), _pListener(pListener), _num(0)
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
			rtn = CompleteToken(Token::TYPE_NewLine);
			_stat = STAT_LineTop;
		} else if (IsWhite(ch)) {
			_stat = STAT_White;
		} else if (IsSymbolFirst(ch)) {
			_str.clear();
			_str += ch;
			_stat = STAT_Symbol;
		} else if (IsDigit(ch)) {
			_num = 0;
			_str.clear();
			_stat = STAT_DecNumber;
			Pushback();
		} else if (ch == '$') {
			_num = 0;
			_str.clear();
			_str += '$';
			_stat = STAT_HexNumber;
		} else if (ch == ';') {
			_stat = STAT_Comment;
		} else if (ch == ':') {
			rtn = CompleteToken(Token::TYPE_Colon);
		} else if (ch == ',') {
			rtn = CompleteToken(Token::TYPE_Comma);
		} else if (ch == '+') {
			rtn = CompleteToken(Token::TYPE_Plus);
		} else if (ch == '-') {
			rtn = CompleteToken(Token::TYPE_Minus);
		} else if (ch == '*') {
			rtn = CompleteToken(Token::TYPE_Asterisk);
		} else if (ch == '/') {
			rtn = CompleteToken(Token::TYPE_Slash);
		} else if (ch == '[') {
			rtn = CompleteToken(Token::TYPE_BracketL);
		} else if (ch == ']') {
			rtn = CompleteToken(Token::TYPE_BracketR);
		} else if (ch == '(') {
			rtn = CompleteToken(Token::TYPE_ParenthesisL);
		} else if (ch == ')') {
			rtn = CompleteToken(Token::TYPE_ParenthesisR);
		} else {
			if (::isprint(ch)) {
				FormatErrMsg("invalid character: %c", ch);
			} else {
				FormatErrMsg("invalid character: 0x%02x", ch);
			}
			return false;
		}
		break;
	}
	case STAT_White: {
		if (IsWhite(ch)) {
			// nothing to do
		} else {
			rtn = CompleteToken(Token::TYPE_White);
			_stat = STAT_Neutral;
			Pushback();
		}
		break;
	}
	case STAT_Comment: {
		if (IsEOF(ch) || IsEOL(ch)) {
			rtn = CompleteToken(Token::TYPE_NewLine);
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
			rtn = CompleteToken(Token::TYPE_Symbol, _str);
			_stat = STAT_Neutral;
			Pushback();
		}
		break;
	}
	case STAT_DecNumber: {
		if (IsDigit(ch)) {
			_str += ch;
			_num += ch - '0';
		} else {
			rtn = CompleteToken(Token::TYPE_Number, _str, _num);
			_stat = STAT_Neutral;
			Pushback();
		}
		break;
	}
	case STAT_HexNumber: {
		if (IsDigit(ch)) {
			_str += ch;
			_num += ch - '0';
		} else if ('a' <= ch && ch <= 'f') {
			_str += ch;
			_num += ch - 'a' + 10;
		} else if ('A' <= ch && ch <= 'F') {
			_str += ch;
			_num += ch - 'A' + 10;
		} else {
			rtn = CompleteToken(Token::TYPE_Number, _str, _num);
			_stat = STAT_Neutral;
			Pushback();
		}
		break;
	}
	}
	EndPushbackRegion();
	return true;
}

bool Tokenizer::CompleteToken(Token::Type type)
{
	_token.Set(type);
	return _pListener->FeedToken(_token);
}

bool Tokenizer::CompleteToken(Token::Type type, const String &str)
{
	_token.Set(type, str);
	return _pListener->FeedToken(_token);
}

bool Tokenizer::CompleteToken(Token::Type type, const String &str, UInt32 num)
{
	_token.Set(type, str, num);
	return _pListener->FeedToken(_token);
}

void Tokenizer::FormatErrMsg(const char *format, ...)
{
	char buff[256];
	va_list ap;
	va_start(ap, format);
	::vsprintf(buff, format, ap);
	_errMsg = buff;
}
