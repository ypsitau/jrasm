#===============================================================================
# CMakeLists for jrasm project
#===============================================================================
set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/../..)

add_library(jrasmcore STATIC
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
	InlineData.cpp
	Macro.cpp
	MMLParser.cpp
	Operator.cpp
	Parser.cpp
	PCGChar.cpp
	PCGColor.cpp
	PCGData.cpp
	PCGPage.cpp
	PCGRange.cpp
	Region.cpp
	Segment.cpp
	Token.cpp
	Tokenizer.cpp
)

install(TARGETS jrasmcore
	RUNTIME DESTINATION bin
	ARCHIVE DESTINATION lib
)
