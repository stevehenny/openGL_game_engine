# Specify include directories for your source files. 
# Use target_include_directories for openGL_game_engine target.
add_executable(openGL_game_engine
    triangle.cpp
    glad.c
)

# Set OpenGL preference to GLVND
cmake_policy(SET CMP0072 NEW)

# Find OpenGL package
find_package(OpenGL REQUIRED)

# Add subdirectories that build libraries for your engine.
add_subdirectory(shaders)
add_subdirectory(window_functions)
add_subdirectory(textures)
add_subdirectory(objects)

# Link libraries and OpenGL/glfw/dl if applicable.
# Replace "shaders", "window_functions", "textures", and "objects" with the actual target names defined in those directories.
target_link_libraries(openGL_game_engine PRIVATE
    OpenGL::GL
    glfw
    dl
    shaders
    window_functions
    textures
    objects
)

# Add include directories for openGL_game_engine target.
# We assume headers for these modules are located at these paths relative to CMAKE_SOURCE_DIR.
target_include_directories(openGL_game_engine PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/shaders
    ${CMAKE_SOURCE_DIR}/window_functions
    ${CMAKE_SOURCE_DIR}/textures
    ${CMAKE_SOURCE_DIR}/objects
)
