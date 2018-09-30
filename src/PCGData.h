//=============================================================================
// PCGData.h
//=============================================================================
#ifndef __PCGDATA_H__
#define __PCGDATA_H__

#include "PCGPattern.h"

//-----------------------------------------------------------------------------
// PCGData
//-----------------------------------------------------------------------------
class PCGData {
private:
	int _cntRef;
	//String _symbol;
	//size_t _wdChar;
	//size_t _htChar;
	//PCGPatternOwner _pcgPatternOwner;
public:
	DeclareReferenceAccessor(PCGData);
public:
	inline PCGData() : _cntRef(1) {}
private:
	inline ~PCGData() {}
};

#endif
