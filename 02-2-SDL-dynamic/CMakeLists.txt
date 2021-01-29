#set min version cmake
cmake_minimum_required(VERSION 3.15)

#set proj properties
project(sdl2-dynamic VERSION 1.0.1 LANGUAGES CXX)

#make configure version file
configure_file(Configure.h.in ${PROJECT_BINARY_DIR}/Configure.h)

#make exe file sdl2-dynamic
add_executable(sdl2-dynamic ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cxx)

#set compiler properties
target_compile_features(sdl2-dynamic PUBLIC cxx_std_17)

#find SDL
find_package(SDL2 REQUIRED)

#find specific lib
find_library(sdl2-dynamic-lib libSDL2-2.0.so.0)

message("path to sdl2-lib-dynamic: ${sdl2-dynamic-lib}")

#set pathes to headers
target_include_directories(${PROJECT_NAME} 
	PRIVATE ${SDL2_INCLUDE_DIRS} #SDL headers
	PRIVATE ${PROJECT_BINARY_DIR} #for Configure.h
)

#link with specific lib
target_link_libraries(${PROJECT_NAME} 
	${sdl2-dynamic-lib}
)




