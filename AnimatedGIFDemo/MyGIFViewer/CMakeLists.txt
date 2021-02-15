# CMakeList.txt : CMake project for MyGIFViewer, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("MyGIFViewer")

# set the C++ standard to C++ 17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(SDL2 CONFIG REQUIRED)
aux_source_directory(./getopt GETOPT_SRC)

# Add source to this project's executable.
add_executable (MyGIFViewer "main.cpp"
							"MyGIFViewer.cpp" "MyGIFViewer.h" 
							"AnimatedGIF.cpp" "AnimatedGIF.h"
							${GETOPT_SRC}
							"./giflib/dgif_lib.c"
							"./giflib/gifalloc.c"
							"./giflib/gif_err.c"
							"./giflib/openbsd-reallocarray.c"
							)

# libs
target_link_libraries(MyGIFViewer PRIVATE SDL2::SDL2 SDL2::SDL2main)
