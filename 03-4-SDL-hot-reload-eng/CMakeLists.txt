cmake_minimum_required(VERSION 3.16)
project(03-4-SDL2-hot-reload)

add_executable(engine my_engine.cxx my_engine.hxx)
target_compile_features(engine PUBLIC cxx_std_17)

set_target_properties(engine PROPERTIES
	ENABLE_EXPORTS 1
)

find_package(SDL2 REQUIRED)

target_link_libraries(engine PRIVATE SDL2::SDL2main SDL2::SDL2)
#target_link_libraries(engine PRIVATE SDL2::SDL2)

add_library(game SHARED game.cxx my_engine.hxx)

target_compile_features(game PUBLIC cxx_std_17)
target_link_libraries(game PRIVATE engine)

