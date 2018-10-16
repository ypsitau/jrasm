//=============================================================================
// PCGRange.h
//=============================================================================
#ifndef __JRASMCORE_PCGRANGE_H__
#define __JRASMCORE_PCGRANGE_H__

#include "PCGChar.h"

//-----------------------------------------------------------------------------
// PCGRange
//-----------------------------------------------------------------------------
class PCGRange {
private:
	PCGType _pcgType;
	int _charCodeStart;
	int _charCodeEnd;
public:
	inline PCGRange(PCGType pcgType, int charCodeStart, int charCodeEnd) :
		_pcgType(pcgType), _charCodeStart(charCodeStart), _charCodeEnd(charCodeEnd) {}
	inline PCGType GetPCGType() const { return _pcgType; }
	inline int GetCharCodeStart() const { return _charCodeStart; }
	inline int GetCharCodeEnd() const { return _charCodeEnd; }
};

//-----------------------------------------------------------------------------
// PCGRangeList
//-----------------------------------------------------------------------------
class PCGRangeList : public std::vector<PCGRange *> {
};

//-----------------------------------------------------------------------------
// PCGRangeOwner
//-----------------------------------------------------------------------------
class PCGRangeOwner : public PCGRangeList {
public:
	~PCGRangeOwner();
	void Clear();
};

#endif
