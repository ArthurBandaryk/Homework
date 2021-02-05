cmake_minimum_required(VERSION 3.16)
project(03-4-sdl-dynamic-engine)

add_library(engine-dyn SHARED my_engine.cxx my_engine.hxx)
target_compile_features(engine-dyn PUBLIC cxx_std_17)

find_package(SDL2 REQUIRED)
#target_include_directories(engine-stat PUBLIC ${SDL2_INCLUDE_DIRS})
#target_link_libraries(engine-stat PRIVATE ${SDL2_LIBRARIES})
target_link_libraries(engine-dyn SDL2::SDL2)


add_executable(game game.cxx)
target_compile_features(game PUBLIC cxx_std_17)

target_link_libraries(game PRIVATE engine-dyn)

