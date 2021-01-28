#set min version cmake
cmake_minimum_required(VERSION 3.15)

#set proj properties
project(sdl2-dynamic VERSION 1.0.1 LANGUAGES CXX)

#make configure version file
configure_file(Configure.h.in ${PROJECT_BINARY_DIR}/Configure.h)

#make exe file sdl2-static
add_executable(sdl2-dynamic ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cxx)

#set compiler properties
target_compile_features(sdl2-dynamic PUBLIC cxx_std_17)

#add pathes to include files
target_include_directories(sdl2-dynamic 
PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
PUBLIC ${PROJECT_BINARY_DIR})

#link with specific libs
target_link_libraries(
	sdl2-dynamic
 	${CMAKE_CURRENT_SOURCE_DIR}/libs/libSDL2d.so
	${CMAKE_CURRENT_SOURCE_DIR}/libs/libSDL2-2.0d.so.0
	${CMAKE_CURRENT_SOURCE_DIR}/libs/libSDL2-2.0d.so
	${CMAKE_CURRENT_SOURCE_DIR}/libs/libSDL2-2.0d.so.0.14.0
)

