//=============================================================================
// jrasm.cpp
//=============================================================================
#include "stdafx.h"

bool Parse(const char *fileName)
{
	FILE *fp;
	if (::fopen_s(&fp, fileName, "rt") != 0) {
		::fprintf(stderr, "failed to open file\n");
		::exit(1);
	}
	Parser parser(fileName);
	for (;;) {
		int chRaw = ::fgetc(fp);
		char ch = (chRaw < 0)? '\0' : static_cast<unsigned char>(chRaw);
		if (!parser.FeedChar(ch)) {
			::fprintf(stderr, "%s\n", parser.GetErrMsg());
		}
		if (ch == '\0') break;
	}
	::fclose(fp);
	const ElementList &elemList = parser.GetInstructions();
	//elemList.Print();
	for (auto pElem : elemList) {
		if (!pElem->IsType(Element::TYPE_Inst)) continue;
		const Element_Inst *pElemEx = dynamic_cast<const Element_Inst *>(pElem);
		const InstInfo *pInstInfo = InstInfo::Lookup(pElemEx->GetSymbol());
		if (pInstInfo == nullptr) {
			::printf("unknown instruction: %s\n", pElemEx->GetSymbol());
		} else {
			::printf("%s .. ", pElemEx->ToString().c_str());
		}
		Context context;
		pInstInfo->ApplyRule(context, pElemEx->GetOperands());
		for (auto data : context.GetBuffer()) {
			::printf(" %02x", static_cast<UInt8>(data));
		}
		:: printf("\n");
	}
	return true;
}

int main(int argc, char *argv[])
{

#if 1
	if (argc < 2) {
		::fprintf(stderr, "usage: jasm file\n");
		::exit(1);
	}
	::Parse(argv[1]);
#else
	FILE *fp = ::fopen("test.cjr", "wb");
	CJRFormat::Write(fp, "hello", 0x2000,
					 (const UInt8 *)"\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f", 16);
	::fclose(fp);
#endif
	return 0;
}
