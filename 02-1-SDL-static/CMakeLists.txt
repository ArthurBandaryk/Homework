#set min version cmake
cmake_minimum_required(VERSION 3.15)

#set proj properties
project(SDL2-static VERSION 1.0.0 LANGUAGES CXX)

#make configure version file
configure_file(Configure.h.in ${PROJECT_BINARY_DIR}/Configure.h)

#make exe file sdl2-static
add_executable(SDL2-static ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cxx)

#set compiler properties
target_compile_features(SDL2-static PUBLIC cxx_std_17)

#add pathes to include files
target_include_directories(SDL2-static 
#PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
PUBLIC ${PROJECT_BINARY_DIR})

#link with specific libs
target_link_libraries(
	SDL2-static
 	${CMAKE_CURRENT_SOURCE_DIR}/libs/libSDL2d.a
 	${CMAKE_CURRENT_SOURCE_DIR}/libs/libSDL2maind.a
 	-lm
 	-ldl
 	-lpthread
)

