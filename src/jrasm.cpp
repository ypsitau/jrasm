//=============================================================================
// jrasm.cpp
//=============================================================================
#include "stdafx.h"

bool Parse(const char *fileName)
{
	ErrorLog::Clear();
	FILE *fp;
	if (::fopen_s(&fp, fileName, "rt") != 0) {
		::fprintf(stderr, "failed to open file\n");
		::exit(1);
	}
	Parser parser(fileName);
	for (;;) {
		int chRaw = ::fgetc(fp);
		char ch = (chRaw < 0)? '\0' : static_cast<unsigned char>(chRaw);
		if (!parser.FeedChar(ch)) break;
		if (ch == '\0') break;
	}
	::fclose(fp);
	if (ErrorLog::HasError()) {
		ErrorLog::Print(stderr);
		return false;
	}
	//exprList.Print();
	Context context(new Generator_M6800());
	parser.GetRoot()->PrepareLookupTable(context);
#if 0
	for (auto pExpr : parser.GetRoot()->GetChildren()) {
		context.ClearBuffer();
		pExpr->Generate(context);
		if (pExpr->IsType(Expr::TYPE_LabelDef)) {
			// nothing to do
		} else {
			::printf("%-32s", pExpr->ToString().c_str());
			if (context.GetBuffer().empty()) {
				::printf("(none)\n");
			} else {
				context.Dump();
			}
		}
	}
#else
	Context::LookupTable *pLookupTable = context.GetLookupTableRoot();
	for (auto iter : *pLookupTable) {
		::printf("%04x  %s\n", iter.second, iter.first.c_str());
	}
#endif
	return true;
}

int main(int argc, char *argv[])
{
	Operator::Initialize();
	Directive::Initialize();
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
