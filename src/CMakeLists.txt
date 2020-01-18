set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(SFML REQUIRED COMPONENTS graphics window)

set(
	2048_srcs
	main.cpp
	game.h
	game.cpp
	scene.h
	scene.cpp
	mainscene.h
	mainscene.cpp
	board.h
	board.cpp
	resourcemanager.h
	resourcemanager.cpp
)
add_executable(sfml-2048 ${2048_srcs})
target_link_libraries(sfml-2048 PRIVATE sfml-graphics sfml-window)
set_target_properties(sfml-2048 PROPERTIES LINKER_LANGUAGE CXX)