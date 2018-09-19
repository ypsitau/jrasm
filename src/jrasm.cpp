//=============================================================================
// jrasm.cpp
//=============================================================================
#include "stdafx.h"

#define JRASM_VERSION 			"0.0.1"
#define JRASM_COPYRIGHT_YEAR	"2018"

const char *strBanner = "JR-200 Assembler " JRASM_VERSION " Copyright (C) " \
	JRASM_COPYRIGHT_YEAR " ypsitau\n";

const char *strUsage = "usage: jrasm [option] source\n";

const char *strOption = R"**(available options:
--output=file   -o file  specifies filename to output
--print_disasm  -d       prints dump in disassembler format
--print_list    -l       prints label list
--print_memory  -m       prints memory image
--verbose       -v       verbose mode
--help          -h       prints this help
)**";

//-----------------------------------------------------------------------------
// main
//-----------------------------------------------------------------------------
int main(int argc, const char *argv[])
{
	static const CommandLine::Info infoTbl[] = {
		{ "output",			'o',	CommandLine::TYPE_Value	},
		{ "print_disasm",	'd',	CommandLine::TYPE_Flag	},
		{ "print_list",		'l',	CommandLine::TYPE_Flag	},
		{ "print_memory",	'm',	CommandLine::TYPE_Flag	},
		{ "verbose",		'v',	CommandLine::TYPE_Flag	},
		{ "help",			'h',	CommandLine::TYPE_Flag	},
	};
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
	const char *fileNameSrc = argv[1];
	Parser parser(fileNameSrc);
	Context context;
	if (!parser.ParseFile()) goto errorDone;
	if (!parser.GetRoot()->Prepare(context)) goto errorDone;
	if (cmdLine.IsSet("disasm")) {
		if (!parser.GetRoot()->DumpDisasm(context, stdout, true, 3)) goto errorDone;
	}
	if (cmdLine.IsSet("print_list")) {
		const char *format = "%04X  %s\n";
		std::unique_ptr<Context::LabelInfoOwner> pLabelInfoOwner(context.MakeLabelInfoOwner());
		::printf("[Label List]\n");
		if (pLabelInfoOwner->empty()) {
			::printf("(no label)\n");
		} else {
			for (auto pLabelInfo : *pLabelInfoOwner) {
				::printf(format, pLabelInfo->GetAddr(), pLabelInfo->GetLabel());
			}
		}
	}
	if (cmdLine.IsSet("print_memory")) {
		size_t bytesGapToJoin = 128;
		UInt8 dataFiller = 0x00;
		std::unique_ptr<RegionOwner> pRegionOwner(parser.Generate(context, bytesGapToJoin, dataFiller));
		::printf("[Memory Image]\n");
		for (auto pRegion : *pRegionOwner) {
			::printf("%04X-%04X   %5dbytes\n",
					 pRegion->GetAddrTop(), pRegion->GetAddrBtm() - 1, pRegion->GetBytes());
			for (auto pRegionIngredient : pRegion->GetRegionsIngredient()) {
				::printf(" %04X-%04X  %5dbytes\n",
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
