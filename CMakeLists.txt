cmake_minimum_required(VERSION 3.10)
project(openGL_game_engine LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_BUILD_TYPE Release)
# Include directories
include_directories(${OPENGL_INCLUDE_DIRS} include shaders src)
add_subdirectory(src)

