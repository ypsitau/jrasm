add_executable(jrasm
	jrasm.cpp
)

include_directories("..")

target_link_libraries(jrasm jrasmcore)

install(TARGETS jrasm DESTINATION bin)
