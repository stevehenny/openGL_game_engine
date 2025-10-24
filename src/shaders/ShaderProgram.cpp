#include "ShaderProgram.h"
#include <fstream>
#include <glad/glad.h>
#include <iostream>
#include <sstream>
#include <stdexcept>

ShaderProgram::ShaderProgram(const std::string &ProgramPath, uint8_t type) {
  std::ifstream shaderFile(ProgramPath);
  if (!shaderFile.is_open()) {
    throw std::runtime_error("Failed to open shader file: " + ProgramPath);
  }

  std::stringstream shaderStream;
  shaderStream << shaderFile.rdbuf();
  std::string shaderCode = shaderStream.str();
  shaderFile.close();

  if (shaderCode.empty()) {
    throw std::runtime_error("Shader code is empty: " + ProgramPath);
  }

  const char *code = shaderCode.c_str();

  if (type == ShaderTypes::VERTEX)
    bind = glCreateShader(GL_VERTEX_SHADER);
  else if (type == ShaderTypes::FRAGMENT)
    bind = glCreateShader(GL_FRAGMENT_SHADER);
  else
    throw std::runtime_error("Invalid shader type");

  glShaderSource(bind, 1, &code, nullptr);
  glCompileShader(bind);

  int success;
  char infoLog[512];
  glGetShaderiv(bind, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(bind, 512, nullptr, infoLog);
    std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
    throw std::runtime_error("Shader compilation failed: " + ProgramPath);
  }
}

ShaderProgram::~ShaderProgram() {
  if (glIsShader(bind)) {
    glDeleteShader(bind);
  }
}

unsigned int ShaderProgram::getBindValue() const { return bind; }
