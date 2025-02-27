cmake_minimum_required(VERSION 3.10) 
project(openGL_game_engine LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Add include directories
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/src/shaders)
include_directories(${CMAKE_SOURCE_DIR}/src/window_functions)
include_directories(${CMAKE_SOURCE_DIR}/src/textures)
include_directories(${CMAKE_SOURCE_DIR}/src/objects)

# Add src as a subdirectory
add_subdirectory(src)
