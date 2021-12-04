#===============================================================================
# CMakeLists for jrasm project
#===============================================================================
set(JRASM_SRCDIR_INC ${PROJECT_SOURCE_DIR}/../inc)
set(JRASM_RELDIR_INC share/jrasm/inc)

add_executable(jrasm jrasm.cpp)

include_directories("..")

target_link_libraries(jrasm jrasmcore)

install(TARGETS jrasm RUNTIME DESTINATION bin)
