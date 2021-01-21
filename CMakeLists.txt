cmake_minimum_required(VERSION 3.15)

project(HW CXX)

add_subdirectory(01-1)
add_subdirectory(01-2)

##########
include(CTest)

add_test(NAME runs-dynamic
         COMMAND hello-bin-dynamic)

add_test(NAME runs-static
         COMMAND hello-bin-static)


add_test(NAME check_hello_exit_zero
         COMMAND Hello
         )
