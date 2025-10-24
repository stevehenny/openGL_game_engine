cmake_minimum_required(VERSION 3.10)
project(openGL_game_engine LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)

include_directories(${CMAKE_SOURCE_DIR}/include)
cmake_policy(SET CMP0136 NEW)

cmake_policy(SET CMP0072 NEW)  
set(OpenGL_GL_PREFERENCE GLVND)  
find_package(glm REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

add_subdirectory(src)

