//=============================================================================
// PCGPattern.h
//=============================================================================
#ifndef __PCGPATTERN_H__
#define __PCGPATTERN_H__

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
// PCGPattern
//-----------------------------------------------------------------------------
class PCGPattern {
private:
	int _cntRef;
	PCGType _pcgType;
	int _charCode;
	Binary _buff;
public:
	DeclareReferenceAccessor(PCGPattern);
public:
	inline PCGPattern(PCGType pcgType, int charCode, const Binary &buff) :
		_cntRef(1), _pcgType(pcgType), _charCode(charCode), _buff(buff) {}
private:
	inline ~PCGPattern() {}
public:
	inline PCGType GetPCGType() const { return _pcgType; }
	inline int GetCharCode() const { return _charCode; }
	inline bool HasSamePattern(const Binary &buff) { return _buff == buff; }
	inline const Binary &GetBuffer() const { return _buff; }
};

//-----------------------------------------------------------------------------
// PCGPatternList
//-----------------------------------------------------------------------------
class PCGPatternList : public std::vector<PCGPattern *> {
public:
	const PCGPattern *FindSamePattern(const Binary &pattern) const;
};

//-----------------------------------------------------------------------------
// PCGPatternOwner
//-----------------------------------------------------------------------------
class PCGPatternOwner : public PCGPatternList {
public:
	~PCGPatternOwner();
	void Clear();
};

#endif
