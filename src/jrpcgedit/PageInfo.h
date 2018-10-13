//=============================================================================
// PageInfo.h
//=============================================================================
#ifndef __JRPCGEDIT_PAGEINFO_H__
#define __JRPCGEDIT_PAGEINFO_H__

#include "PCGInfo.h"

//-----------------------------------------------------------------------------
// PageInfo
//-----------------------------------------------------------------------------
class PageInfo {
private:
	int _cntRef;
	PCGInfoOwner _pcgInfoOwner;
public:
	DeclareReferenceAccessor(PageInfo);
public:
	PageInfo();
protected:
	inline ~PageInfo() {};
public:
	void NewPCGInfo();
	const PCGInfoOwner &GetPCGInfoOwner() const { return _pcgInfoOwner; }
};

//-----------------------------------------------------------------------------
// PageInfoList
//-----------------------------------------------------------------------------
class PageInfoList : public std::vector<PageInfo *> {
};

//-----------------------------------------------------------------------------
// PageInfoOwner
//-----------------------------------------------------------------------------
class PageInfoOwner : public PageInfoList {
public:
	~PageInfoOwner();
	void Clear();
};

#endif

