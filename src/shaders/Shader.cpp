#include "Shader.h"
#include "ShaderProgram.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

Shader::Shader(const ShaderProgram &vertex, const ShaderProgram &fragment) {
  ID = glCreateProgram();
  glAttachShader(ID, vertex.getBindValue());
  glAttachShader(ID, fragment.getBindValue());

  glLinkProgram(ID);

  int success;
  char infoLog[512];
  glGetProgramiv(ID, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(ID, 512, nullptr, infoLog);
    std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
              << infoLog << std::endl;
    throw std::runtime_error("Shader program linking failed");
  }

  // glDetachShader(ID, vertex.getBindValue());
  // glDetachShader(ID, fragment.getBindValue());
}

Shader::~Shader() {
  if (glIsProgram(ID)) {
    glDeleteProgram(ID);
  }
}

void Shader::useShader() const { glUseProgram(ID); }

// ---- Uniform setters by SPIR-V location ----
void Shader::setBool(unsigned int location, bool value) const {
  glProgramUniform1i(ID, location, (int)value);
}

void Shader::setInt(unsigned int location, int value) const {
  glProgramUniform1i(ID, location, value);
}

void Shader::setFloat(unsigned int location, float value) const {
  glProgramUniform1f(ID, location, value);
}

void Shader::setVec2(unsigned int location, const glm::vec2 &value) const {
  glProgramUniform2fv(ID, location, 1, glm::value_ptr(value));
}

void Shader::setVec2(unsigned int location, float x, float y) const {
  glProgramUniform2f(ID, location, x, y);
}

void Shader::setVec3(unsigned int location, const glm::vec3 &value) const {
  glProgramUniform3fv(ID, location, 1, glm::value_ptr(value));
}

void Shader::setVec3(unsigned int location, float x, float y, float z) const {
  glProgramUniform3f(ID, location, x, y, z);
}

void Shader::setVec4(unsigned int location, const glm::vec4 &value) const {
  glProgramUniform4fv(ID, location, 1, glm::value_ptr(value));
}

void Shader::setVec4(unsigned int location, float x, float y, float z,
                     float w) const {
  glProgramUniform4f(ID, location, x, y, z, w);
}

void Shader::setMat2(unsigned int location, const glm::mat2 &mat) const {
  glProgramUniformMatrix2fv(ID, location, 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::setMat3(unsigned int location, const glm::mat3 &mat) const {
  glProgramUniformMatrix3fv(ID, location, 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::setMat4(unsigned int location, const glm::mat4 &mat) const {
  glProgramUniformMatrix4fv(ID, location, 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::setUniformTextures(unsigned int location,
                                vector<GLuint> &textures) const {
  glUniform4uiv(location, textures.size(), textures.data());
}
