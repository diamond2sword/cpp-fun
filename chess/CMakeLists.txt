cmake_minimum_required (VERSION 3.25)
project(main LANGUAGES CXX VERSION 1.0.0)

add_executable(main
	main.cpp
)
set_property(TARGET main PROPERTY CXX_STANDARD 26)
