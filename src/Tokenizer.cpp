//=============================================================================
// Tokenizer.cpp
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// Tokenizer
//-----------------------------------------------------------------------------
Tokenizer::Tokenizer(Listener *pListener, const String &pathNameSrc) :
	_stat(STAT_LineTop), _pListener(pListener),
	_pPathNameSrc(new StringShared(pathNameSrc)), _num(0), _nLines(0),
	_quotedType(QUOTEDTYPE_None)
{
}

bool Tokenizer::FeedChar(char ch)
{
	if (ch == '\r') return true;	// skip CR code
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
		} else if (ch == '"') {
			_quotedType = QUOTEDTYPE_String;
			_str.clear();
			_stat = STAT_Quoted;
		} else if (ch == '\'') {
			_quotedType = QUOTEDTYPE_Character;
			_str.clear();
			_str += ch;
			_stat = STAT_Quoted;
		} else if (ch == 'b') {
			_stat = STAT_BitPatternPre;
		} else if (IsSymbolFirst(ch)) {
			_str.clear();
			_str += ch;
			_stat = STAT_Symbol;
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
			_stat = STAT_Colon;
		} else if (ch == '=') {
			_stat = STAT_Equal;
		} else if (ch == '|') {
			_stat = STAT_VerticalBar;
		} else if (ch == '&') {
			_stat = STAT_Ampersand;
		} else if (ch == '<') {
			_stat = STAT_LessThan;
		} else if (ch == '>') {
			_stat = STAT_GreaterThan;
		} else if (ch == '!') {
			_stat = STAT_Bang;
		} else if (ch == '^') {
			rtn = FeedToken(TOKEN_Hat);
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
		} else if (ch == '%') {
			rtn = FeedToken(TOKEN_Percent);
		} else if (ch == '(') {
			rtn = FeedToken(TOKEN_ParenL);
		} else if (ch == ')') {
			rtn = FeedToken(TOKEN_ParenR);
		} else if (ch == '[') {
			rtn = FeedToken(TOKEN_BracketL);
		} else if (ch == ']') {
			rtn = FeedToken(TOKEN_BracketR);
		} else if (ch == '{') {
			rtn = FeedToken(TOKEN_BraceL);
		} else if (ch == '}') {
			rtn = FeedToken(TOKEN_BraceR);
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
	case STAT_Colon: {
		if (ch == ':') {
			rtn = FeedToken(TOKEN_ColonColon);
			_stat = STAT_Neutral;
		} else {
			rtn = FeedToken(TOKEN_Colon);
			_stat = STAT_Neutral;
			Pushback();
		}
		break;
	}
	case STAT_Equal: {
		if (ch == '=') {
			rtn = FeedToken(TOKEN_EqEq);
			_stat = STAT_Neutral;
		} else {
			rtn = FeedToken(TOKEN_Eq);
			_stat = STAT_Neutral;
			Pushback();
		}
		break;
	}
	case STAT_VerticalBar: {
		if (ch == '|') {
			rtn = FeedToken(TOKEN_VBarVBar);
			_stat = STAT_Neutral;
		} else {
			rtn = FeedToken(TOKEN_VBar);
			_stat = STAT_Neutral;
			Pushback();
		}
		break;
	}
	case STAT_Ampersand: {
		if (ch == '&') {
			rtn = FeedToken(TOKEN_AmpAmp);
			_stat = STAT_Neutral;
		} else {
			rtn = FeedToken(TOKEN_Amp);
			_stat = STAT_Neutral;
			Pushback();
		}
		break;
	}
	case STAT_LessThan: {
		if (ch == '=') {
			rtn = FeedToken(TOKEN_LtEq);
			_stat = STAT_Neutral;
		} else if (ch == '<') {
			rtn = FeedToken(TOKEN_LtLt);
			_stat = STAT_Neutral;
		} else {
			rtn = FeedToken(TOKEN_Lt);
			_stat = STAT_Neutral;
			Pushback();
		}
		break;
	}
	case STAT_GreaterThan: {
		if (ch == '=') {
			rtn = FeedToken(TOKEN_GtEq);
			_stat = STAT_Neutral;
		} else if (ch == '>') {
			rtn = FeedToken(TOKEN_GtGt);
			_stat = STAT_Neutral;
		} else {
			rtn = FeedToken(TOKEN_Gt);
			_stat = STAT_Neutral;
			Pushback();
		}
		break;
	}
	case STAT_Bang: {
		if (ch == '=') {
			rtn = FeedToken(TOKEN_BangEq);
			_stat = STAT_Neutral;
		} else {
			rtn = FeedToken(TOKEN_Bang);
			_stat = STAT_Neutral;
			Pushback();
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
	case STAT_BitPatternPre: {
		if (ch == '"') {
			_quotedType = QUOTEDTYPE_BitPattern;
			_str.clear();
			_stat = STAT_Quoted;
		} else {
			_str.clear();
			_str += 'b';
			_stat = STAT_Symbol;
			Pushback();
		}
		break;
	}
	case STAT_Quoted: {
		const char *literalName =
			(_quotedType == QUOTEDTYPE_String)? "string" :
			(_quotedType == QUOTEDTYPE_Character)? "character" :
			(_quotedType == QUOTEDTYPE_BitPattern)? "bit-pattern" : "none";
		char chBorder = _quotedType == (QUOTEDTYPE_Character)? '\'' : '"';
		if (IsEOF(ch) || IsEOL(ch)) {
			AddError("unclosed %s literal", literalName);
			rtn = false;
		} else if (ch == chBorder) {
			if (_quotedType == QUOTEDTYPE_String) {
				rtn = FeedToken(TOKEN_String, _str);
			} else if (_quotedType == QUOTEDTYPE_Character) {
				_str += ch;
				if (_str.size() > 3) {
					AddError("character literal must contain just one character");
					rtn = false;
				} else {
					rtn = FeedToken(TOKEN_Number, _str, _str[1]);
				}
			} else if (_quotedType == QUOTEDTYPE_BitPattern) {
				rtn = FeedToken(TOKEN_BitPattern, _str);
			}
			_stat = STAT_Neutral;
		} else if (ch == '\\') {
			_stat = STAT_QuotedEsc;
		} else {
			_str += ch;
		}
		break;
	}
	case STAT_QuotedEsc: {
		if (ch == '"') {
			_str += ch;
		} else if (ch == '\'') {
			_str += ch;
		} else if (ch == '\\') {
			_str += ch;
		} else if (ch == 'n') {
			_str += '\n';
		} else if (ch == 'r') {
			_str += '\r';
		} else if (ch == '0') {
			_str += '\0';
		} else {
			AddError("invalid escape character code: 0x%02x", static_cast<UInt8>(ch));
			rtn = false;
		}
		_stat = STAT_Quoted;
		break;
	}
	case STAT_DetectZero: {
		if (ch == 'b') {
			_str += ch;
			_stat = STAT_BinNumber;
		} else if (::isdigit(ch)) {
			_stat = STAT_OctNumber;
			Pushback();
		} else if (ch == 'x') {
			_str += ch;
			_stat = STAT_HexNumber;
		} else {
			rtn = FeedToken(TOKEN_Number, _str, _num);
			_stat = STAT_Neutral;
			Pushback();
		}
		break;
	}
	case STAT_BinNumber: {
		if ('0' <= ch && ch <= '1') {
			_str += ch;
			_num = (_num << 1) + (ch - '0');
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

bool Tokenizer::FeedToken(const TokenInfo &tokenInfo, const String &str, Number num)
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
	ErrorLog::AddErrorV(GetPathNameSrc(), _nLines + 1, format, ap);
}
