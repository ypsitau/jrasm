//=============================================================================
// Token.cpp
//=============================================================================
#ifndef __TOKEN_H__
#define __TOKEN_H__

#include "Common.h"

class Token {
public:
	enum Type {
		TYPE_None,
		TYPE_EOL,
		TYPE_White,
		TYPE_Number,
		TYPE_Symbol,
		TYPE_String,
		TYPE_Colon,
		TYPE_Comma,
		TYPE_Plus,
		TYPE_Minus,
		TYPE_Asterisk,
		TYPE_Slash,
		TYPE_BracketL,
		TYPE_BracketR,
		TYPE_ParenthesisL,
		TYPE_ParenthesisR,
	};
	struct TypeInfo {
		Type type;
		const char *name;
	};
private:
	Type _type;
	String _str;
	UInt32 _num;
	bool _validStrFlag;
public:
	inline Token() : _type(TYPE_None), _num(0), _validStrFlag(false) {}
	inline bool IsType(Type type) const { return _type == type; }
	inline void Set(Type type) {
		_type = type;
		_validStrFlag = false;
	}
	inline void Set(Type type, const String &str) {
		_type = type, _str = str;
		_validStrFlag = true;
	}
	inline void Set(Type type, const String &str, UInt32 num) {
		_type = type, _str = str, _num = num;
		_validStrFlag = true;
	}
	inline UInt32 GetNumber() const { return _num; }
	inline const char *GetString() const { return _str.c_str(); }
	inline bool MatchCase(const char *str) const { return ::strcmp(_str.c_str(), str) == 0; }
	inline bool MatchICase(const char *str) const { return ::strcasecmp(_str.c_str(), str) == 0; }
	String ToString() const;
};

#endif
