#include "ShaderProgram.h"
#include <fstream>
#include <glad/glad.h>
#include <iostream>
#include <stdexcept>
#include <vector>

ShaderProgram::ShaderProgram(const std::string &programPath, uint8_t type) {
  // Open binary SPIR-V file
  std::ifstream file(programPath, std::ios::binary | std::ios::ate);
  if (!file.is_open()) {
    throw std::runtime_error("Failed to open SPIR-V file: " + programPath);
  }

  std::streamsize size = file.tellg();
  file.seekg(0, std::ios::beg);

  std::vector<uint32_t> buffer(size / sizeof(uint32_t));
  if (!file.read(reinterpret_cast<char *>(buffer.data()), size)) {
    throw std::runtime_error("Failed to read SPIR-V file: " + programPath);
  }

  // Create shader object
  if (type == ShaderTypes::VERTEX)
    bind = glCreateShader(GL_VERTEX_SHADER);
  else if (type == ShaderTypes::FRAGMENT)
    bind = glCreateShader(GL_FRAGMENT_SHADER);
  else if (type == ShaderTypes::COMPUTE)
    bind = glCreateShader(GL_COMPUTE_SHADER);
  else
    throw std::runtime_error("Invalid shader type");

  // Load SPIR-V binary
  glShaderBinary(1, &bind, GL_SHADER_BINARY_FORMAT_SPIR_V, buffer.data(), size);

  // Specialize shader (entry point "main")
  glSpecializeShader(bind, "main", 0, nullptr, nullptr);

  // Check compilation
  GLint success;
  glGetShaderiv(bind, GL_COMPILE_STATUS, &success);
  if (!success) {
    char infoLog[512];
    glGetShaderInfoLog(bind, 512, nullptr, infoLog);
    std::cerr << "ERROR::SPIRV_SHADER::COMPILATION_FAILED\n"
              << infoLog << std::endl;
    throw std::runtime_error("SPIR-V shader compilation failed: " +
                             programPath);
  }
}

ShaderProgram::~ShaderProgram() {
  if (glIsShader(bind)) {
    glDeleteShader(bind);
  }
}

unsigned int ShaderProgram::getBindValue() const { return bind; }
