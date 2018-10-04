//=============================================================================
// PCGChar.h
//=============================================================================
#ifndef __PCGCHAR_H__
#define __PCGCHAR_H__

#include "Common.h"

//-----------------------------------------------------------------------------
// PCGType
//-----------------------------------------------------------------------------
enum PCGType {
	PCGTYPE_None,
	PCGTYPE_User,
	PCGTYPE_CRAM,
};

//-----------------------------------------------------------------------------
// PCGChar
//-----------------------------------------------------------------------------
class PCGChar {
private:
	int _cntRef;
	PCGType _pcgType;
	int _charCode;
	Binary _buff;
public:
	DeclareReferenceAccessor(PCGChar);
public:
	inline PCGChar(PCGType pcgType, int charCode, const Binary &buff) :
		_cntRef(1), _pcgType(pcgType), _charCode(charCode), _buff(buff) {}
private:
	inline ~PCGChar() {}
public:
	inline PCGType GetPCGType() const { return _pcgType; }
	inline int GetCharCode() const { return _charCode; }
	inline bool HasSamePattern(const Binary &buff) { return _buff == buff; }
	inline const Binary &GetBuffer() const { return _buff; }
};

//-----------------------------------------------------------------------------
// PCGCharList
//-----------------------------------------------------------------------------
class PCGCharList : public std::vector<PCGChar *> {
public:
	const PCGChar *FindSamePattern(const Binary &buff) const;
};

//-----------------------------------------------------------------------------
// PCGCharOwner
//-----------------------------------------------------------------------------
class PCGCharOwner : public PCGCharList {
public:
	~PCGCharOwner();
	void Clear();
};

#endif
