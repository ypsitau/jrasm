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
--print-list-l     -l       prints label list in lower case
--print-list-u     -L       prints label list in upper case
--print-memory-l   -m       prints memory image in lower case
--print-memory-u   -M       prints memory image in upper case
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
		{ "print-memory-l",		'm',	CommandLine::TYPE_Flag	},
		{ "print-memory-u",		'M',	CommandLine::TYPE_Flag	},
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
	ErrorLog::Clear();
	Operator::Initialize();
	Directive::Initialize();
	Generator::Initialize(new Generator_M6800());
	if (cmdLine.IsSet("help")) {
		fprintf(stderr, "%s%s%s", strBanner, strUsage, strOption);
		return 1;
	}
	if (argc < 2) {
		fprintf(stderr, "%s%s", strBanner, strUsage);
		return 1;
	}
	const char *pathNameSrc = argv[1];
	Context context(pathNameSrc);
	if (!context.ParseFile()) goto errorDone;
	if (!context.Prepare()) goto errorDone;
	upperCaseFlag = false;
	if (cmdLine.IsSet("print-disasm-l") || (upperCaseFlag = cmdLine.IsSet("print-disasm-u"))) {
		if (!context.DumpDisasm(stdout, upperCaseFlag,
							   Generator::GetInstance().GetBytesInstMax())) goto errorDone;
	} else {
		const char *fileNameSrc = ExtractFileName(pathNameSrc);
		String fileBaseNameSrc = ::RemoveExtName(fileNameSrc);
		size_t bytesGapToJoin = 128;
		UInt8 dataFiller = 0x00;
		context.SetFileNameJR(fileBaseNameSrc.c_str());
		std::unique_ptr<RegionOwner> pRegionOwner(context.Generate(bytesGapToJoin, dataFiller));
		if (pRegionOwner.get() == nullptr) goto errorDone;
		upperCaseFlag = false;
		if (cmdLine.IsSet("print-hexdump-l") || (upperCaseFlag = cmdLine.IsSet("print-hexdump-u"))) {
			String fileNameOut = cmdLine.GetString("output", "");
			FormatDump().Write(fileNameOut.c_str(), *pRegionOwner);
			::printf("%s was created\n", fileNameOut.c_str());
		} else {
			String fileNameOut = cmdLine.GetString("output", (fileBaseNameSrc + ".cjr").c_str());
			FormatCJR format(context.GetFileNameJR());
			if (!format.Write(fileNameOut.c_str(), *pRegionOwner)) goto errorDone;
			::printf("%s was created (filename.jr: %s)\n", fileNameOut.c_str(), context.GetFileNameJR());
		}
	}
	upperCaseFlag = false;
	if (cmdLine.IsSet("print-list-l") || (upperCaseFlag = cmdLine.IsSet("print-list-u"))) {
		const char *format = upperCaseFlag? "%04X  %s\n" : "%04x  %s\n";
		std::unique_ptr<Context::LabelInfoOwner> pLabelInfoOwner(context.MakeLabelInfoOwner());
		::printf("[Label List]\n");
		if (pLabelInfoOwner->empty()) {
			::printf("(no label)\n");
		} else {
			for (auto pLabelInfo : *pLabelInfoOwner) {
				::printf(format, pLabelInfo->GetNumber(), pLabelInfo->GetLabel());
			}
		}
	}
	upperCaseFlag = false;
	if (cmdLine.IsSet("print-memory-l") || (upperCaseFlag = cmdLine.IsSet("print-memory-u"))) {
		const char *formatRoot	= "%04x-%04x   %5dbytes\n";
		const char *formatChild	= " %04x-%04x  %5dbytes\n";
		if (upperCaseFlag) {
			formatRoot	= "%04X-%04X   %5dbytes\n";
			formatChild	= " %04X-%04X  %5dbytes\n";
		}
		size_t bytesGapToJoin = 128;
		UInt8 dataFiller = 0x00;
		std::unique_ptr<RegionOwner> pRegionOwner(context.Generate(bytesGapToJoin, dataFiller));
		::printf("[Memory Image]\n");
		for (auto pRegion : *pRegionOwner) {
			::printf(formatRoot, pRegion->GetAddrTop(), pRegion->GetAddrBtm() - 1, pRegion->GetBytes());
			for (auto pRegionIngredient : pRegion->GetRegionsIngredient()) {
				::printf(formatChild,
						 pRegionIngredient->GetAddrTop(), pRegionIngredient->GetAddrBtm() - 1,
						 pRegionIngredient->GetBytes());
			}
		}
	}
	return 0;
errorDone:
	ErrorLog::Print(stderr);
	return 1;
}
