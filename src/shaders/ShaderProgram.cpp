#include "ShaderProgram.h"
#include "Cube.h"
#include <fstream>
#include <iostream>
#include <sstream>
ShaderProgram::ShaderProgram(std::string &ProgramPath, uint8_t type)
    : type(type) {
  std::ifstream shaderProgramFile;

  shaderProgramFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  std::string programCode;

  try {
    shaderProgramFile.open(ProgramPath.c_str());

    std::stringstream programStream;
    programStream << shaderProgramFile.rdbuf();

    // close file
    shaderProgramFile.close();

    programCode = programStream.str();
  } catch (std::ifstream::failure &e) {
    std::cerr << "SHADER::NOT::PARSED::CORRECTLY";
    std::exit(1);
  }

  if (programCode.empty()) {
    std::cerr << "PROGRAM::CODE::IS::EMPTY";
    std::exit(1);
  }

  const char *code = programCode.c_str();

  unsigned int bind;
  int success;
  int infoLog[512];

  if (type == ShaderTypes::VERTEX)
    bind = glCreateShader(GL_VERTEX_SHADER);
  else if (type == ShaderTypes::FRAGMENT)
    bind = glCreateShader(GL_FRAGMENT_SHADER);
  else {
    throw std::runtime_error("INVALID::SHADER::TYPE::GIVEN");
  }
  glShaderSource(bind, 1, &code, nullptr);
  glCompileShader(bind);

  glGetShaderiv(bind, GL_COMPILE_STATUS, &success);
  if (!success) {
    std::cerr << "ERROR::SHADER::DID::NOT::COMPILE";
    std::exit(1);
  }
}

ShaderProgram::~ShaderProgram() { glDeleteShader(bind); }
