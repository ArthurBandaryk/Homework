cmake_minimum_required(VERSION 3.16)
project(04-triangle-interpolated CXX)

add_executable(triangle-interpolated-render
        src/canvas_basic.cxx
        src/line_render.cxx
        src/triangle_basic_render.cxx
        src/triangle_indexed_render.cxx
        src/triangle_interpolated_render.cxx
        src/my_gfx_program.cxx
        src/triangle_main.cxx
)
target_compile_features(triangle-interpolated-render PUBLIC cxx_std_17)

target_include_directories(triangle-interpolated-render
       PRIVATE include
)
