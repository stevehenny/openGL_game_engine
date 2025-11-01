# -----------------------------------------------------------
# Include project directories
# -----------------------------------------------------------
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/src/shaders)
include_directories(${CMAKE_SOURCE_DIR}/src/window_functions)
include_directories(${CMAKE_SOURCE_DIR}/src/textures)
include_directories(${CMAKE_SOURCE_DIR}/src/objects)

# -----------------------------------------------------------
# Subdirectories for static libraries (shaders, textures, etc.)
# -----------------------------------------------------------
add_subdirectory(shaders)
add_subdirectory(window_functions)
add_subdirectory(textures)
add_subdirectory(objects)

# -----------------------------------------------------------
# Executable targets
# -----------------------------------------------------------



add_executable(camera_translations
    camera_translations.cpp
    glad.c
)

add_executable(light_source 
    light_source.cpp
    glad.c
)


add_executable(sphere_practice
    sphere.cpp
    glad.c
)

add_executable(sphere_texture
    sphere_w_tex.cpp
    glad.c
)

add_executable(mesh_gravity
    mesh_gravity.cpp 
    glad.c
)

# -----------------------------------------------------------
# OpenGL preference
# -----------------------------------------------------------
cmake_policy(SET CMP0072 NEW)
set(OpenGL_GL_PREFERENCE GLVND)

# -----------------------------------------------------------
# Link libraries
# (Order matters! glfw after your own libs)
# -----------------------------------------------------------
set(COMMON_LIBS
    glm::glm
    OpenGL::GL
    shaders
    window_functions
    textures
    objects
    glfw
)

target_link_libraries(camera_translations PRIVATE ${COMMON_LIBS})
target_link_libraries(light_source PRIVATE ${COMMON_LIBS})
target_link_libraries(sphere_practice PRIVATE ${COMMON_LIBS})
target_link_libraries(sphere_texture PRIVATE ${COMMON_LIBS})
target_link_libraries(mesh_gravity PRIVATE ${COMMON_LIBS})

# -----------------------------------------------------------
# Platform-specific linking
# -----------------------------------------------------------
if(WIN32)
    # Link against required Win32 system libs
    target_link_libraries(camera_translations PRIVATE opengl32 user32 gdi32 shell32)
    target_link_libraries(light_source PRIVATE opengl32 user32 gdi32 shell32)
    target_link_libraries(sphere_practice PRIVATE opengl32 user32 gdi32 shell32)
    target_link_libraries(sphere_texture PRIVATE opengl32 user32 gdi32 shell32)
    target_link_libraries(mesh_gravity PRIVATE opengl32 user32 gdi32 shell32)

else()
    # Link against dl on Linux / WSL
    target_link_libraries(camera_translations PRIVATE dl m)
    target_link_libraries(light_source PRIVATE dl m)
    target_link_libraries(sphere_practice PRIVATE dl m)
    target_link_libraries(sphere_texture PRIVATE dl m)
    target_link_libraries(mesh_gravity PRIVATE dl m)

endif()

