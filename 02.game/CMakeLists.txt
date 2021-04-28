cmake_minimum_required(VERSION 3.0.0)

project(snake VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

add_executable(snake)

target_sources(snake PRIVATE
	src/main.cpp
	src/view.cpp
	src/cli_view.cpp
	src/console.cpp
	src/game.cpp
	)

target_include_directories(snake PRIVATE include)