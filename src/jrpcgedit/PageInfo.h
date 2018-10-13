//=============================================================================
// PageInfo.h
//=============================================================================
#ifndef __JRPCGEDIT_PAGEINFO_H__
#define __JRPCGEDIT_PAGEINFO_H__

#include "PatternInfo.h"

//-----------------------------------------------------------------------------
// PageInfo
//-----------------------------------------------------------------------------
class PageInfo {
private:
	int _cntRef;
	PatternInfoOwner _patternInfoOwner;
public:
	DeclareReferenceAccessor(PageInfo);
public:
	PageInfo();
protected:
	inline ~PageInfo() {};
public:
	void NewPatternInfo();
	const PatternInfoOwner &GetPatternInfoOwner() const { return _patternInfoOwner; }
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

