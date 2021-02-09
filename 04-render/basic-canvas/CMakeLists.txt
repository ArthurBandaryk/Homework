cmake_minimum_required(VERSION 3.16)
project(04-canvas-basic CXX)

add_executable(canvas-basic canvas_basic.cxx
                            canvas_basic.hxx
                            canvas_basic_main.cxx
)
target_compile_features(canvas-basic PUBLIC cxx_std_17)
