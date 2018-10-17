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
	int _charCodeCur;
public:
	inline PCGRange(PCGType pcgType, int charCodeStart, int charCodeEnd) :
	_pcgType(pcgType), _charCodeStart(charCodeStart), _charCodeEnd(charCodeEnd), _charCodeCur(charCodeStart) {}
	inline PCGType GetPCGType() const { return _pcgType; }
	inline int GetCharCodeStart() const { return _charCodeStart; }
	inline int GetCharCodeEnd() const { return _charCodeEnd; }
	inline int GetCharCodeCur() const { return _charCodeCur; }
	inline void ForwardCharCodeCur() { _charCodeCur++; }
	const char *GetPCGTypeName(bool upperCaseFlag) const;
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
