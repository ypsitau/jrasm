set(JRASM_SRCDIR_INC ${PROJECT_SOURCE_DIR}/../inc)
set(JRASM_RELDIR_INC share/jrasm/inc)

add_executable(jrasm
	jrasm.cpp
)

include_directories("..")

target_link_libraries(jrasm jrasmcore)

install(TARGETS jrasm DESTINATION bin)

install(DIRECTORY ${JRASM_SRCDIR_INC}/
  DESTINATION ${JRASM_RELDIR_INC}
  USE_SOURCE_PERMISSIONS
  FILES_MATCHING PATTERN "*" PATTERN "#*" EXCLUDE)
