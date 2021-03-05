cmake_minimum_required(VERSION 3.16)
project(math-gpu)

add_library(engine-dyn SHARED
	src/my_engine.cxx
	include/my_engine.hxx
	include/picopng.hxx
        src/gl_check.cxx
        include/gl_check.hxx
        src/gfx_program.cxx
        include/gfx_program.hxx
        src/glad.c
        include/glad/glad.h
        include/KHR/khrplatform.h
	)
target_compile_features(engine-dyn PUBLIC cxx_std_17)

find_package(SDL2 REQUIRED)
target_include_directories(engine-dyn PRIVATE include)
target_link_libraries(engine-dyn SDL2::SDL2 SDL2::SDL2main)

add_executable(game src/game.cxx)
target_compile_features(game PUBLIC cxx_std_17)
target_include_directories(game PRIVATE include)
target_link_libraries(game PRIVATE engine-dyn)

