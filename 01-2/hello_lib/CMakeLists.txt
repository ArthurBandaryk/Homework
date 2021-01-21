cmake_minimum_required(VERSION 3.15)
project(hello_lib CXX)


add_library(hello-lib-static STATIC include/greeting.hpp src/greeting.cpp)
target_include_directories(hello-lib-static PUBLIC 
                          ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(hello-lib-static PUBLIC cxx_std_17)

add_library(hello-lib-dynamic SHARED include/greeting.hpp src/greeting.cpp)
target_include_directories(hello-lib-dynamic PUBLIC 
                          ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(hello-lib-dynamic PUBLIC cxx_std_17)

install(TARGETS hello-lib-dynamic hello-lib-static
        RUNTIME DESTINATION ${CMAKE_CURRENT_LIST_DIR}/../../install/ex02/bin
        LIBRARY DESTINATION ${CMAKE_CURRENT_LIST_DIR}/../../install/ex02/lib
        ARCHIVE DESTINATION ${CMAKE_CURRENT_LIST_DIR}/../../install/ex02/lib)

