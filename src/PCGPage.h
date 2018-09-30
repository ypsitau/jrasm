//=============================================================================
// PCGPage.h
//=============================================================================
#ifndef __PCGPAGE_H__
#define __PCGPAGE_H__

#include "PCGData.h"

//-----------------------------------------------------------------------------
// PCGPage
//-----------------------------------------------------------------------------
class PCGPage {
public:
private:
	PCGType _pcgType;
	int _charCodeStart;
	int _charCodeCur;
	PCGDataOwner _pcgDataOwner;
public:
	inline PCGPage(PCGType pcgType, int charCodeStart) :
		_pcgType(pcgType), _charCodeStart(charCodeStart), _charCodeCur(charCodeStart) {}
	inline PCGType GetPCGType() const { return _pcgType; }
	inline int GetCharCodeStart() const { return _charCodeStart; }
	const PCGData *NewPCGData(const Binary &binary);
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
