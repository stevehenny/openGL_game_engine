add_library(shaders
    Shader.cpp
)
target_include_directories(shaders PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(shaders PRIVATE glm::glm)

