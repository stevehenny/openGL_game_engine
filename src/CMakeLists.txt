include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/shaders)
include_directories(${CMAKE_SOURCE_DIR}/window_functions)

add_subdirectory(shaders)
add_subdirectory(window_functions)

add_executable(openGL_game_engine
    triangle.cpp
    glad.c
)

# Set OpenGL preference to GLVND
cmake_policy(SET CMP0072 NEW)
set(OpenGL_GL_PREFERENCE GLVND)

# Find OpenGL package
find_package(OpenGL REQUIRED)

# Link libraries
target_link_libraries(openGL_game_engine OpenGL::GL glfw dl m shaders window_functions)

