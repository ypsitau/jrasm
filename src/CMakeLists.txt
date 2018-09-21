cmake_minimum_required(VERSION 2.8)

add_definitions("-Wall -std=c++11")

add_executable(jrasm
	jrasm.cpp
	stdafx.cpp
	CommandLine.cpp
	Common.cpp
	Context.cpp
	Directive.cpp
	ErrorLog.cpp
	Expr.cpp
	FormatCJR.cpp
	FormatDump.cpp
	Generator.cpp
	Generator_M6800.cpp
	MmlParser.cpp
	Operator.cpp
	Parser.cpp
	Region.cpp
	Segment.cpp
	Token.cpp
	Tokenizer.cpp
)

install(TARGETS jrasm DESTINATION bin)
