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
	PageInfoOwner _pageInfoOwner;
public:
	DeclareReferenceAccessor(Document);
public:
	Document();
protected:
	inline ~Document() {};
public:
	const PageInfoOwner &GetPageInfoOwner() const { return _pageInfoOwner; }
};

#endif
