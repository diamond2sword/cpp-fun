cmake_minimum_required (VERSION 3.25)
project(main LANGUAGES CXX VERSION 1.0.0)
find_package(ftxui 5 REQUIRED)
add_executable(main
	./hello-world.cpp
	./main.cpp
)
set_property(TARGET main PROPERTY CXX_STANDARD 26)
target_link_libraries(main
	PRIVATE ftxui::screen
	PRIVATE ftxui::dom
	PRIVATE ftxui::component
)
