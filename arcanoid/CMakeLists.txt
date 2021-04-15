cmake_minimum_required(VERSION 3.16)
project(arcanoid)

file(GLOB_RECURSE SRC1
                  src/gfx_program.cxx
                  src/gl_check.cxx
                  src/glad.c
                  src/my_engine.cxx
                  src/my_math.cxx
                  src/texture.cxx

                  include/gfx_program.hxx
                  include/gl_check.hxx
                  include/my_engine.hxx
                  include/my_math.hxx
                  include/stb_image.h
                  include/texture.hxx

                  include/glad/*.h
                  include/KHR/*.h)

add_library(engine-dyn SHARED ${SRC1})

target_compile_features(engine-dyn PUBLIC cxx_std_17)

find_package(SDL2 REQUIRED)
target_include_directories(engine-dyn PRIVATE include)
target_link_libraries(engine-dyn SDL2::SDL2 SDL2::SDL2main)

file(GLOB_RECURSE SRC2
                  src/main.cxx
                  src/game.cxx
                  include/game.hxx
                  src/game_components.cxx
                  include/game_components.hxx
                 )

add_executable(game ${SRC2})
target_compile_features(game PUBLIC cxx_std_17)
target_include_directories(game PRIVATE include)
target_link_libraries(game PRIVATE engine-dyn)

