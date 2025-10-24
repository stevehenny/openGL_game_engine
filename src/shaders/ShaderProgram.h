#pragma once

#include <cstdint>
#include <string>

namespace ShaderTypes {
constexpr inline uint8_t FRAGMENT = 1;
constexpr inline uint8_t VERTEX = 2;
} // namespace ShaderTypes
class ShaderProgram {

  ShaderProgram(std::string &ProgramPath, uint8_t type);
  ~ShaderProgram();
  ShaderProgram(const ShaderProgram &) = delete;
  ShaderProgram(const ShaderProgram &&) = delete;
  ShaderProgram &operator()(const ShaderProgram &) = delete;
  ShaderProgram &operator()(const ShaderProgram &&) = delete;

public:
  unsigned int &getBindValue();

private:
  unsigned int bind;
  uint8_t type;
};
