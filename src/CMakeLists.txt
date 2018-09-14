cmake_minimum_required(VERSION 2.8)

add_definitions("-Wall -std=c++11") # <= new

add_executable(jrasm
	jrasm.cpp
	stdafx.cpp
	CJRFormat.cpp
	Common.cpp
	Context.cpp	
	Expr.cpp
	Generator.cpp
	Generator_M6800.cpp
	MmlParser.cpp
	Parser.cpp
	Token.cpp
	Tokenizer.cpp
)
