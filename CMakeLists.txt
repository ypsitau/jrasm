#===============================================================================
# CMakeLists for jrasm project
#===============================================================================
cmake_minimum_required(VERSION 3.16) # 3.16 or newer supports handling of precompiled header
project(JRASM
	VERSION			0.0.1
	DESCRIPTION 	"Assembler for JR-200"
	HOMEPAGE_URL	https://ypsitau.github.io/jrasm/
	LANGUAGES		CXX)

set(JRASM_COPYRIGHT "Copyright (C) 2011-2021 ypsitau")

add_subdirectory(src)
