cmake_minimum_required(VERSION 3.10)
project(openGL_game_engine LANGUAGES C CXX)

# Include directories
include_directories(${OPENGL_INCLUDE_DIRS} include shaders src)
add_subdirectory(src)

