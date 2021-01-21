cmake_minimum_required(VERSION 3.15)
project(hello_bin CXX)


add_executable(hello-bin-static src/main.cpp)
target_compile_features(hello-bin-static PRIVATE cxx_std_17)
target_link_libraries(hello-bin-static LINK_PUBLIC hello-lib-static)

set(CMAKE_INSTALL_RPATH "${CMAKE_CURRENT_LIST_DIR}/../../install/ex02/lib")

add_executable(hello-bin-dynamic src/main.cpp)
target_compile_features(hello-bin-dynamic PRIVATE cxx_std_17)
target_link_libraries(hello-bin-dynamic LINK_PUBLIC hello-lib-dynamic)



install(TARGETS hello-bin-dynamic hello-bin-static
        RUNTIME DESTINATION ${CMAKE_CURRENT_LIST_DIR}/../../install/ex02/bin
        LIBRARY DESTINATION ${CMAKE_CURRENT_LIST_DIR}/../../install/ex02/lib
        ARCHIVE DESTINATION ${CMAKE_CURRENT_LIST_DIR}/../../install/ex02/lib)



