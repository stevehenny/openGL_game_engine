cmake_minimum_required(VERSION 3.10)
project(openGL_game_engine LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_BUILD_TYPE Release)

# Enable export of compile commands
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Optionally, specify OpenGL include directories if needed
# find_package(OpenGL REQUIRED)
# include_directories(${OPENGL_INCLUDE_DIRS})

# Add subdirectory for src (our main target and other libraries)
add_subdirectory(src)
