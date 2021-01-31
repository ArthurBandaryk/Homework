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

list(APPEND CMAKE_PREFIX_PATH "usr/local/lib/cmake/SDL2/" "/usr/lib/x86_64-linux-gnu" "usr/local/lib")
#set(CMAKE_PREFIX_PATH usr/local/lib /usr/lib/x86_64-linux-gnu)
#find SDL
find_package(SDL2 CONFIG REQUIRED)

#find specific lib
find_library(sdl2-static-lib1 libSDL2.a)
#find_library(sdl2-static-lib2 libSDL2main.a)

target_link_libraries(sdl2-static PRIVATE SDL2::SDL2-static)
message("path to static lib1: ${sdl2-static-lib1}")
#message("path to static lib2: ${sdl2-static-lib2}")

#add pathes to include files
target_include_directories(sdl2-static 
	#PRIVATE ${SDL2_INCLUDE_DIRS} #SDL headers
	PRIVATE ${PROJECT_BINARY_DIR} #Configure.h
)

#link with specific libs
#target_link_libraries(
	sdl2-static
	${sdl2-static-lib1}
#	${sdl2-static-lib2}
	#-Wl,--no-undefined 
 #	-lm
 #	-ldl
 #	-lpthread
#	-lrt
	#-lX11 
	#-lXext 
	#-lXcursor
	#-lXinerama
	#-lXi
	#-lXrandr
	#-lXss
	#-lXxf86vm
	#-lwayland-egl
	#-lwayland-client
	#-lwayland-cursor
	#-lxkbcommon
	#-lasound
	#-lpulse-simple
	#-lpulse
	#-lsndio
)

