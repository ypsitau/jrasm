project(jrasm)

cmake_minimum_required(VERSION 2.8)

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(CMAKE_INSTALL_PREFIX /Applications/JRASM.app/Contents/Resources)
else()
    set(CMAKE_INSTALL_PREFIX /usr)
endif()

add_definitions("-Wall -std=c++11")

add_subdirectory(jrasmcore)
add_subdirectory(jrasm)
add_subdirectory(jrpcgedit)
