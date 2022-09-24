#===============================================================================
# CMakeLists for jrasm project
#===============================================================================
cmake_minimum_required(VERSION 3.16) # 3.16 or newer supports handling of precompiled header
project(JRASM
	VERSION			1.0.2
	DESCRIPTION 	"Assembler for JR-200"
	HOMEPAGE_URL	https://ypsitau.github.io/jrasm/
	LANGUAGES		CXX)

set(JRASM_COPYRIGHT "Copyright (C) 2018-2021 ypsitau")

if (${CMAKE_SYSTEM_NAME} MATCHES Windows)
	set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR})
elseif (${CMAKE_SYSTEM_NAME} MATCHES Linux)
	set(CMAKE_INSTALL_PREFIX /usr)
elseif (${CMAKE_SYSTEM_NAME} MATCHES Darwin)
	set(CMAKE_INSTALL_PREFIX /Applications/GURAX.app/Contents/Resources)
else ()
	set(CMAKE_INSTALL_PREFIX /usr)
endif ()

add_subdirectory(src)

include(CPack)
