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
--output=file     -o file  specifies filename to output
--print-disasm-l  -d       prints disassembler dump in lower case
--print-disasm-u  -D       prints disassembler dump in upper case
--print-list      -l       prints label list
--print-memory    -m       prints memory image
--verbose         -v       verbose mode
--help            -h       prints this help
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
		{ "print-list",			'l',	CommandLine::TYPE_Flag	},
		{ "print-memory",		'm',	CommandLine::TYPE_Flag	},
		{ "verbose",			'v',	CommandLine::TYPE_Flag	},
		{ "help",				'h',	CommandLine::TYPE_Flag	},
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
	const char *pathNameSrc = argv[1];
	Parser parser(pathNameSrc);
	Context context;
	if (!parser.ParseFile()) goto errorDone;
	if (!parser.GetRoot()->Prepare(context)) goto errorDone;
	if (cmdLine.IsSet("print-disasm-l")) {
		if (!parser.DumpDisasm(context, stdout, false, 3)) goto errorDone;
	} else if (cmdLine.IsSet("print-disasm-u")) {
		if (!parser.DumpDisasm(context, stdout, true, 3)) goto errorDone;
	} else {
		const char *fileNameSrc = ExtractFileName(pathNameSrc);
		size_t bytesGapToJoin = 128;
		UInt8 dataFiller = 0x00;
		std::unique_ptr<RegionOwner> pRegionOwner(parser.Generate(context, bytesGapToJoin, dataFiller));
		if (pRegionOwner.get() == nullptr) goto errorDone;
		String fileNameCJR = ::RemoveExtName(fileNameSrc) + ".cjr";
		FormatCJR format("JRASM");
		if (!format.Write(fileNameCJR.c_str(), *pRegionOwner)) goto errorDone;
		::printf("%s was generated\n", fileNameCJR.c_str());
	}
	if (cmdLine.IsSet("print-list")) {
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
	if (cmdLine.IsSet("print-memory")) {
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
