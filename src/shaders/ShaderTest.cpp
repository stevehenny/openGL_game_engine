

#include "ShaderTest.h"
#include "glad/glad.h"
#include <fstream>
#include <iostream>
#include <sstream>

ShaderTest::ShaderTest(ShaderProgram &vertex, ShaderProgram &fragment) {
  ID = glCreateProgram();
  glAttachShader(ID, vertex.getBindValue());
  glAttachShader(ID, vertex.getBindValue());

  int success;
  char infoLog[512];
  glGetProgramiv(ID, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(ID, 512, NULL, infoLog);
    std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
              << infoLog << std::endl;
    exit(EXIT_FAILURE);
  }
}
void ShaderTest::useShader() { glUseProgram(ID); }

void ShaderTest::setBool(const std::string &name, bool value) const {
  glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void ShaderTest::setInt(const std::string &name, int value) const {
  glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void ShaderTest::setFloat(const std::string &name, float value) const {
  glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void ShaderTest::setVec2(const std::string &name,
                         const glm::vec2 &value) const {
  glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
void ShaderTest::setVec2(const std::string &name, float x, float y) const {
  glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
}
void ShaderTest::setVec3(const std::string &name,
                         const glm::vec3 &value) const {
  glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
void ShaderTest::setVec3(const std::string &name, float x, float y,
                         float z) const {
  glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}
void ShaderTest::setVec4(const std::string &name,
                         const glm::vec4 &value) const {
  glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
void ShaderTest::setVec4(const std::string &name, float x, float y, float z,
                         float w) const {
  glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
}
void ShaderTest::setMat2(const std::string &name, const glm::mat2 &mat) const {
  glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE,
                     &mat[0][0]);
}
void ShaderTest::setMat3(const std::string &name, const glm::mat3 &mat) const {
  glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE,
                     &mat[0][0]);
}
void ShaderTest::setMat4(const std::string &name, const glm::mat4 &mat) const {
  glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE,
                     &mat[0][0]);
}
