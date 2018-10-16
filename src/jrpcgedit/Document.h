//=============================================================================
// Document.h
//=============================================================================
#ifndef __JRPCGEDIT_DOCUMENT_H__
#define __JRPCGEDIT_DOCUMENT_H__

#include "PCGPageInfo.h"

//-----------------------------------------------------------------------------
// Document
//-----------------------------------------------------------------------------
class Document {
private:
	int _cntRef;
	std::unique_ptr<PCGPageInfoOwner> _pPCGPageInfoOwner;
public:
	DeclareReferenceAccessor(Document);
public:
	Document();
protected:
	inline ~Document() {};
public:
	inline PCGPageInfoOwner &GetPCGPageInfoOwner() { return *_pPCGPageInfoOwner; }
	inline const PCGPageInfoOwner &GetPCGPageInfoOwner() const { return *_pPCGPageInfoOwner; }
	inline void NewPCGPageInfo() { _pPCGPageInfoOwner->NewPCGPageInfo(); }
	bool ReadFile(const char *pathName);
	bool WriteFile(const char *pathName);
};

#endif
