add_library(shaders
  Shader.cpp ShaderProgram.cpp ShaderTest.cpp
)
target_link_libraries(shaders PRIVATE glm::glm)

