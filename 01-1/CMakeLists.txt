#set min version of cmake
cmake_minimum_required(VERSION 3.15)

#set name, version and language of project
project(Hello VERSION 1.0 LANGUAGES CXX)

#make configure version file
configure_file(Configure.h.in ${CMAKE_CURRENT_SOURCE_DIR}/Configure.h)

#get exe Hello
add_executable(Hello hello.cxx)

target_compile_features(Hello PUBLIC cxx_std_17)

#add path to Configure.h
target_include_directories(Hello INTERFACE
    "${PROJECT_BINARY_DIR}}")

#make install
install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION ${CMAKE_CURRENT_LIST_DIR}/install/bin/
        LIBRARY DESTINATION ${CMAKE_CURRENT_LIST_DIR}/install/bin/
        ARCHIVE DESTINATION ${CMAKE_CURRENT_LIST_DIR}/install/bin/)

#testing
include(CTest)
#enable_testing()

add_test(NAME check_hello_exit_zero
         COMMAND Hello
         )

