//=============================================================================
// PCGPage.h
//=============================================================================
#ifndef __PCGPAGE_H__
#define __PCGPAGE_H__

#include "PCGPattern.h"

//-----------------------------------------------------------------------------
// PCGPage
//-----------------------------------------------------------------------------
class PCGPage {
private:
	int _cntRef;
	PCGType _pcgType;
	int _charCodeStart;
	int _charCodeCur;
	PCGPatternOwner _pcgPatternOwner;
public:
	DeclareReferenceAccessor(PCGPage);
public:
	inline PCGPage(PCGType pcgType, int charCodeStart) :
		_cntRef(1), _pcgType(pcgType), _charCodeStart(charCodeStart), _charCodeCur(charCodeStart) {}
private:
	inline ~PCGPage() {}
public:
	inline PCGType GetPCGType() const { return _pcgType; }
	inline int GetCharCodeStart() const { return _charCodeStart; }
	inline const PCGPatternOwner &GetPCGPatternOwner() const { return _pcgPatternOwner; }
	const PCGPattern *AddPCGPattern(const Binary &buff);
};

//-----------------------------------------------------------------------------
// PCGPageList
//-----------------------------------------------------------------------------
class PCGPageList : public std::vector<PCGPage *> {
};

//-----------------------------------------------------------------------------
// PCGPageOwner
//-----------------------------------------------------------------------------
class PCGPageOwner : public PCGPageList {
public:
	~PCGPageOwner();
	void Clear();
};

#endif
