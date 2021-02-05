#set min version cmake
cmake_minimum_required(VERSION 3.15)

#set proj properties
project(03-1-SDL2-engine)

#i wanna debug
option(CMAKE_BUILD_TYPE "build type: Debug" Debug)

#make exe file
add_executable(03-1-SDL2-engine
    my_engine.cxx
    game.cxx
    my_engine.hxx
)

#set compiler properties
target_compile_features(03-1-SDL2-engine PUBLIC cxx_std_17)

#find SDL
find_package(SDL2 REQUIRED)

#link with specific lib
target_link_libraries(03-1-SDL2-engine SDL2::SDL2)
#target_include_directories(04-sdl2-static-engine PUBLIC ${SDL2_INCLUDE_DIRS})
#target_link_libraries(04-sdl2-static-engine PUBLIC ${SDL2_LIBRARIES})
