cmake_minimum_required(VERSION 3.16)
project(shader-sdl CXX)


find_package(SDL2 REQUIRED)

add_executable(shader
        src/canvas_basic.cxx
        src/line_render.cxx
        src/triangle_basic_render.cxx
        src/triangle_indexed_render.cxx
        src/triangle_interpolated_render.cxx
        src/my_gfx_program.cxx
        src/main.cxx
)
target_compile_features(shader PUBLIC cxx_std_17)

target_link_libraries(shader PRIVATE SDL2::SDL2 SDL2::SDL2main)

target_include_directories(shader
       PRIVATE include
       PRIVATE ${SDL2_INCLUDE_DIRS}
)
