#set min version cmake
cmake_minimum_required(VERSION 3.15)

#set proj properties
project(sdl2-static VERSION 1.0.0 LANGUAGES CXX)

#make configure version file
configure_file(Configure.h.in ${PROJECT_BINARY_DIR}/Configure.h)

#make exe file sdl2-static
add_executable(sdl2-static ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cxx)

#set compiler properties
target_compile_features(sdl2-static PUBLIC cxx_std_17)

#find SDL
find_package(SDL2 REQUIRED)

#find specific lib
find_library(sdl2-static-lib1 libSDL2.a)
find_library(sdl2-static-lib2 libSDL2main.a)

message("path to static lib: ${sdl2-static-lib}")

#add pathes to include files
target_include_directories(sdl2-static 
	PRIVATE ${SDL2_INCLUDE_DIRS} #SDL headers
	PUBLIC ${PROJECT_BINARY_DIR} #Configure.h
)

#link with specific libs
target_link_libraries(
	sdl2-static
	${sdl2-static-lib1}
	${sdl2-static-lib2}
	-Wl,--no-undefined 
 	-lm
 	-ldl
 	-lpthread
	-lrt
)

