file(GLOB SRCS *.cpp)

add_executable(sample_test ${SRCS})

target_link_libraries(
	sample_test
	gtest
	gmock_main
)

install(TARGETS sample_test DESTINATION bin)
add_test(UnitTests sample_test)