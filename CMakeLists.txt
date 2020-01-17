cmake_minimum_required (VERSION 3.2)

project (sfml-2048)

option(ENABLE_TESTS "Enable tests" TRUE)

include_directories(".")
add_subdirectory(src)

if(ENABLE_TESTS)
	enable_testing()
	add_subdirectory(test)
endif()