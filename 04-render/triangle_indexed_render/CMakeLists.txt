cmake_minimum_required(VERSION 3.16)
project(04-triangle-indexed CXX)

add_executable(triangle-indexed-render  canvas_basic.cxx
                           	canvas_basic.hxx
                           	line_render.cxx
                           	line_render.hxx
                           	triangle_basic_render.cxx
                           	triangle_basic_render.hxx
                           	triangle_indexed_render.cxx
                           	triangle_indexed_render.hxx
                           	triangle_main.cxx
)
target_compile_features(triangle-indexed-render PUBLIC cxx_std_17)
