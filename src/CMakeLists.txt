include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/src/shaders)
include_directories(${CMAKE_SOURCE_DIR}/src/window_functions)
include_directories(${CMAKE_SOURCE_DIR}/src/textures)
include_directories(${CMAKE_SOURCE_DIR}/src/objects)

add_subdirectory(shaders)
add_subdirectory(window_functions)
add_subdirectory(textures)
add_subdirectory(objects)

add_executable(openGL_game_engine
    triangle.cpp
    glad.c
)

add_executable(transformations
  transformations.cpp
  glad.c
)

add_executable(triangle_test
  triangle_test.cpp
  glad.c
)

add_executable(camera_translations
  camera_translations.cpp 
  glad.c
)


# Set OpenGL preference to GLVND
cmake_policy(SET CMP0072 NEW)
set(OpenGL_GL_PREFERENCE GLVND)


# Link libraries
target_link_libraries(openGL_game_engine glm::glm OpenGL::GL glfw dl m shaders window_functions textures objects)



target_link_libraries(transformations glm::glm OpenGL::GL glfw dl m shaders window_functions textures objects)
target_link_libraries(camera_translations glm::glm OpenGL::GL glfw dl m shaders window_functions textures objects)
target_link_libraries(triangle_test glm::glm OpenGL::GL glfw dl m shaders window_functions textures objects)
