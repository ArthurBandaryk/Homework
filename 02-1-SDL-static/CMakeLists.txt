#set min version cmake
cmake_minimum_required(VERSION 3.19)

#set proj properties
project(sdl2-static VERSION 1.0.0 LANGUAGES CXX)

#make configure version file
configure_file(Configure.h.in ${PROJECT_BINARY_DIR}/Configure.h)

#make exe file sdl2-static
add_executable(sdl2-static ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cxx)

#set compiler properties
target_compile_features(sdl2-static PUBLIC cxx_std_17)

#find SDL

find_package(SDL2 CONFIG REQUIRED)

target_link_libraries(sdl2-static PRIVATE usr/lib/x86_64-linux-gnu/libSDL2.a)

#add pathes to include files
target_include_directories(sdl2-static
	PRIVATE ${SDL2_INCLUDE_DIRS}
	PRIVATE ${PROJECT_BINARY_DIR} #Configure.h
)

