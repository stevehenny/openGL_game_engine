add_library(shaders
  Shader.cpp ShaderProgram.cpp 
)
target_link_libraries(shaders PRIVATE glm::glm)

