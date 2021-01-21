#set min version of cmake
cmake_minimum_required(VERSION 3.15)

#set name
project(Ex01-2)

#add directories
add_subdirectory(hello_lib)
add_subdirectory(hello_bin)

#testing
include(CTest)

add_test(NAME run-dynamic
         COMMAND hello-bin-dynamic)

add_test(NAME run-static
         COMMAND hello-bin-static)
