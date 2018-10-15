//=============================================================================
// Document.h
//=============================================================================
#ifndef __JRPCGEDIT_DOCUMENT_H__
#define __JRPCGEDIT_DOCUMENT_H__

#include "PageInfo.h"

//-----------------------------------------------------------------------------
// Document
//-----------------------------------------------------------------------------
class Document {
private:
	int _cntRef;
	std::unique_ptr<PageInfoOwner> _pPageInfoOwner;
public:
	DeclareReferenceAccessor(Document);
public:
	Document();
protected:
	inline ~Document() {};
public:
	inline PageInfoOwner &GetPageInfoOwner() { return *_pPageInfoOwner; }
	inline const PageInfoOwner &GetPageInfoOwner() const { return *_pPageInfoOwner; }
	inline void NewPageInfo() { _pPageInfoOwner->NewPageInfo(); }
	bool ReadFile(const char *pathName);
	bool WriteFile(const char *pathName);
};

#endif
