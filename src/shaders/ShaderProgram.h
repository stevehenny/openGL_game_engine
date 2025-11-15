#pragma once

#include <cstdint>
#include <string>

namespace ShaderTypes {
constexpr inline uint8_t FRAGMENT = 1;
constexpr inline uint8_t VERTEX = 2;
constexpr inline uint8_t COMPUTE = 3;
} // namespace ShaderTypes
class ShaderProgram {

public:
  unsigned int getBindValue() const;
  ShaderProgram(const std::string &ProgramPath, uint8_t type);
  ~ShaderProgram();

private:
  unsigned int bind;
  uint8_t type;
};
