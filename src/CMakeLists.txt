cmake_minimum_required(VERSION 2.8)

add_definitions("-Wall -std=c++11")

add_executable(jrasm
	jrasm.cpp
	stdafx.cpp
	CJRFormat.cpp
	Common.cpp
	Context.cpp	
	Error.cpp
	Expr.cpp
	Generator.cpp
	Generator_M6800.cpp
	MmlParser.cpp
	Operator.cpp
	Parser.cpp
	Token.cpp
	Tokenizer.cpp
)
