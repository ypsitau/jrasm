//=============================================================================
// jrasm.cpp
//=============================================================================
#include "stdafx.h"

const char *strBanner = "JR-200 Assembler " JRASM_VERSION " Copyright (C) " \
	JRASM_COPYRIGHT_YEAR " ypsitau\n";

const char *strUsage = "usage: jrasm [option] source\n";

const char *strOption = R"**(available options:
--output=file      -o file  specifies filename to output
--print-disasm-l   -d       prints disassembler dump in lower case
--print-disasm-u   -D       prints disassembler dump in upper case
--print-hexdump-l  -x       prints hex dump in lower case
--print-hexdump-u  -X       prints hex dump in upper case
--print-list-l     -l       prints symbol list in lower case
--print-list-u     -L       prints symbol list in upper case
--print-resource-l -r       prints resource information in lower case
--print-resource-u -R       prints resource information in upper case
--verbose          -v       verbose mode
--help             -h       prints this help
)**";

//-----------------------------------------------------------------------------
// main
//-----------------------------------------------------------------------------
int main(int argc, const char *argv[])
{
	static const CommandLine::Info infoTbl[] = {
		{ "output",				'o',	CommandLine::TYPE_Value	},
		{ "print-disasm-l",		'd',	CommandLine::TYPE_Flag	},
		{ "print-disasm-u",		'D',	CommandLine::TYPE_Flag	},
		{ "print-hexdump-l",	'x',	CommandLine::TYPE_Flag	},
		{ "print-hexdump-u",	'X',	CommandLine::TYPE_Flag	},
		{ "print-list-l",		'l',	CommandLine::TYPE_Flag	},
		{ "print-list-u",		'L',	CommandLine::TYPE_Flag	},
		{ "print-resource-l",	'r',	CommandLine::TYPE_Flag	},
		{ "print-resource-u",	'R',	CommandLine::TYPE_Flag	},
		{ "verbose",			'v',	CommandLine::TYPE_Flag	},
		{ "help",				'h',	CommandLine::TYPE_Flag	},
	};
	bool upperCaseFlag = false;
	String strErr;
	CommandLine cmdLine;
	if (!cmdLine.AddInfo(infoTbl, ArraySizeOf(infoTbl)).Parse(argc, argv, strErr)) {
		::fprintf(stderr, "%s\n", strErr.c_str());
		return 1;
	}
	if (cmdLine.IsSet("help")) {
		fprintf(stderr, "%s%s%s", strBanner, strUsage, strOption);
		return 1;
	}
	if (argc < 2) {
		fprintf(stderr, "%s%s", strBanner, strUsage);
		return 1;
	}
	Context::Initialize(new Generator_M6800());
	const char *pathNameSrc = argv[1];
	Context context(pathNameSrc);
	if (!context.ParseFile() || !context.Prepare()) goto errorDone;
	upperCaseFlag = false;
	if (cmdLine.IsSet("print-disasm-l") || (upperCaseFlag = cmdLine.IsSet("print-disasm-u"))) {
		if (!context.DumpDisasm(stdout, upperCaseFlag,
							   Generator::GetInstance().GetBytesInstMax())) goto errorDone;
	} else {
		size_t bytesGapToJoin = 32768; // always join the regions
		UInt8 dataFiller = 0x00;
		std::unique_ptr<RegionOwner> pRegionOwner(context.Generate(bytesGapToJoin, dataFiller));
		if (pRegionOwner.get() == nullptr) goto errorDone;
		upperCaseFlag = false;
		if (cmdLine.IsSet("print-hexdump-l") || (upperCaseFlag = cmdLine.IsSet("print-hexdump-u"))) {
			String fileNameOut = cmdLine.GetString("output", "");
			FormatDump().Write(fileNameOut.c_str(), *pRegionOwner);
			if (!fileNameOut.empty()) ::printf("%s was created\n", fileNameOut.c_str());
		} else {
			String fileNameOut = cmdLine.GetString("output", context.MakeFileNameOut(".cjr").c_str());
			FormatCJR format(context.GetFileNameJR());
			if (!format.Write(fileNameOut.c_str(), *pRegionOwner)) goto errorDone;
			::printf("%s was created (filename.jr: %s)\n", fileNameOut.c_str(), context.GetFileNameJR());
		}
	}
	upperCaseFlag = false;
	if (cmdLine.IsSet("print-list-l") || (upperCaseFlag = cmdLine.IsSet("print-list-u"))) {
		context.PrintSymbolList(stdout, upperCaseFlag);
	}
	upperCaseFlag = false;
	if (cmdLine.IsSet("print-resource-l") || (upperCaseFlag = cmdLine.IsSet("print-resource-u"))) {
		if (!context.PrintMemoryUsage(stdout, upperCaseFlag)) goto errorDone;
		context.PrintPCGUsage(stdout, upperCaseFlag);
	}
	return 0;
errorDone:
	ErrorLog::Print(stderr);
	return 1;
}
