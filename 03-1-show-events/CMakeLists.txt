#set min version cmake
cmake_minimum_required(VERSION 3.19)

#set proj properties
project(sdl2-show-events LANGUAGES CXX)

#i wanna debug
option(CMAKE_BUILD_TYPE "build type: Debug" Debug)

#make exe file
add_executable(sdl2-show-events ${CMAKE_CURRENT_SOURCE_DIR}/show_events.cxx)

#set compiler properties
target_compile_features(sdl2-show-events PUBLIC cxx_std_17)

#find SDL
find_package(SDL2 REQUIRED)

#link with specific lib
#target_link_libraries(sdl2-show-events SDL2::SDL2)
target_include_directories(sdl2-show-events PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(sdl2-show-events PRIVATE ${SDL2_LIBRARIES})
