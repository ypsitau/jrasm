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
	Binary _pattern;
public:
	DeclareReferenceAccessor(PCGPattern);
public:
	inline PCGPattern(PCGType pcgType, int charCode, const Binary &pattern) :
		_cntRef(1), _pcgType(pcgType), _charCode(charCode), _pattern(pattern) {}
private:
	inline ~PCGPattern() {}
public:
	inline PCGType GetPCGType() const { return _pcgType; }
	inline int GetCharCode() const { return _charCode; }
	inline bool HasSamePattern(const Binary &pattern) { return _pattern == pattern; }
	inline const Binary &GetPattern() const { return _pattern; }
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
