cmake_minimum_required(VERSION 3.16)
project(opengl-renderdoc)

add_library(engine-static STATIC 
	src/my_engine.cxx
	include/my_engine.hxx
        src/glad.c
        include/glad/glad.h
        include/KHR/khrplatform.h
	)
target_compile_features(engine-static PUBLIC cxx_std_17)

find_package(SDL2 REQUIRED)
target_include_directories(engine-static PRIVATE include)
target_link_libraries(engine-static SDL2::SDL2 SDL2::SDL2main)

add_executable(game src/game.cxx)
target_compile_features(game PUBLIC cxx_std_17)
target_include_directories(game PRIVATE include)
target_link_libraries(game PRIVATE engine-static)

