cmake_minimum_required(VERSION 3.16)
project(04-line-render CXX)

add_executable(line-render canvas_basic.cxx
                            canvas_basic.hxx
                            line_render.cxx
                            line_render.hxx
                            line_render_main.cxx
)
target_compile_features(line-render PUBLIC cxx_std_17)
