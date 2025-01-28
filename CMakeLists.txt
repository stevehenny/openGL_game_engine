cmake_minimum_required(VERSION 3.10)
project(opengl_practice)


# Include directories
include_directories(include)

# Add the source files
set(SOURCES
    src/glad.c 
    window_callbacks.c
    triangle.c
    )

# Define the executable
add_executable(${PROJECT_NAME} ${SOURCES})

#Link libraries
target_link_libraries(${PROJECT_NAME} GL glfw dl)
