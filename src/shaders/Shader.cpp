
#include "Shader.h"
#include "glad/glad.h"
#include <fstream>
#include <iostream>
#include <sstream>

Shader::Shader(const char *vertexPath, const char *fragmentPath) {
  std::string vertexCode, fragmentCode;
  std::ifstream vShaderFile, fShaderFile;

  // Ensure ifstream objects can throw exceptions
  vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

  try {
    // Open shader files
    vShaderFile.open(vertexPath);
    fShaderFile.open(fragmentPath);
    std::stringstream vShaderStream, fShaderStream;

    // Read file contents into streams
    vShaderStream << vShaderFile.rdbuf();
    fShaderStream << fShaderFile.rdbuf();

    // Close file handlers
    vShaderFile.close();
    fShaderFile.close();

    // Convert streams to strings
    vertexCode = vShaderStream.str();
    fragmentCode = fShaderStream.str();
  } catch (std::ifstream::failure &e) {
    std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what()
              << std::endl;
    exit(EXIT_FAILURE);
  }

  if (vertexCode.empty() || fragmentCode.empty()) {
    std::cerr << "ERROR::SHADER::FILE_IS_EMPTY" << std::endl;
    exit(EXIT_FAILURE);
  }

  const char *vShaderCode = vertexCode.c_str();
  const char *fShaderCode = fragmentCode.c_str();

  unsigned int vertex, fragment;
  int success;
  char infoLog[512];

  // Compile Vertex Shader
  vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex, 1, &vShaderCode,
                 nullptr); // Fixed: nullptr instead of &success
  glCompileShader(vertex);

  glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertex, 512, NULL, infoLog);
    std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
              << infoLog << std::endl;
    exit(EXIT_FAILURE);
  }

  // Compile Fragment Shader
  fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment, 1, &fShaderCode,
                 nullptr); // Fixed: nullptr instead of &success
  glCompileShader(fragment);

  glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragment, 512, NULL, infoLog);
    std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
              << infoLog << std::endl;
    exit(EXIT_FAILURE);
  }

  // Shader Program
  ID = glCreateProgram();
  glAttachShader(ID, vertex);
  glAttachShader(ID, fragment);
  glLinkProgram(ID);

  glGetProgramiv(ID, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(ID, 512, NULL, infoLog);
    std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
              << infoLog << std::endl;
    exit(EXIT_FAILURE);
  }

  // Clean up shaders after linking
  glDeleteShader(vertex);
  glDeleteShader(fragment);
}
void Shader::useShader() { glUseProgram(ID); }

void Shader::setBool(const std::string &name, bool value) const {
  glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void Shader::setInt(const std::string &name, int value) const {
  glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setFloat(const std::string &name, float value) const {
  glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
