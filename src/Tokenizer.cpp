//=============================================================================
// Tokenizer.cpp
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// Tokenizer
//-----------------------------------------------------------------------------
Tokenizer::Tokenizer(Listener *pListener, const String &fileNameSrc) :
	_stat(STAT_LineTop), _pListener(pListener), _fileNameSrc(fileNameSrc), _num(0), _nLines(0)
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
			rtn = CompleteToken(Token::TYPE_EOL);
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
				SetErrMsg("invalid character: %c", ch);
			} else {
				SetErrMsg("invalid character: 0x%02x", ch);
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
			rtn = CompleteToken(Token::TYPE_EOL);
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
	case STAT_String: {
		if (IsEOF(ch) || IsEOL(ch)) {
			SetErrMsg("unclosed string literal");
			rtn = false;
		} else if (ch == '"') {
			rtn = CompleteToken(Token::TYPE_String, _str);
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
			SetErrMsg("invalid escape character");
			rtn = false;
		}
		break;
	}
	case STAT_DecNumber: {
		if (IsDigit(ch)) {
			_str += ch;
			_num = _num * 10 + (ch - '0');
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
			_num = (_num << 4) + (ch - '0');
		} else if ('a' <= ch && ch <= 'f') {
			_str += ch;
			_num = (_num << 4) + (ch - 'a' + 10);
		} else if ('A' <= ch && ch <= 'F') {
			_str += ch;
			_num = (_num << 4) + (ch - 'A' + 10);
		} else {
			rtn = CompleteToken(Token::TYPE_Number, _str, _num);
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
	//::printf("CompleteToken(%d)\n", type);
	_token.Set(type, str, num);
	return _pListener->FeedToken(_token);
}

void Tokenizer::SetErrMsg(const char *format, ...)
{
	char buff[256];
	va_list ap;
	_errMsg = _fileNameSrc;
	::sprintf_s(buff, ":%d ", _nLines + 1);
	_errMsg += buff;
	va_start(ap, format);
	::vsprintf_s(buff, format, ap);
	_errMsg += buff;
}
