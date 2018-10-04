add_executable(jrasm
	jrasm.cpp
	stdafx.cpp
	CommandLine.cpp
	Common.cpp
	Context.cpp
	Directive.cpp
	DisasmDumper.cpp
	ErrorLog.cpp
	Expr.cpp
	FormatCJR.cpp
	FormatDump.cpp
	Generator.cpp
	Generator_M6800.cpp
	Macro.cpp
	MmlParser.cpp
	Operator.cpp
	Parser.cpp
	PCGChar.cpp
	PCGData.cpp
	PCGPage.cpp
	Region.cpp
	Segment.cpp
	Token.cpp
	Tokenizer.cpp
)

install(TARGETS jrasm DESTINATION bin)
