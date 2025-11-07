#pragma once
#include "top_level_include.h"
#include <glm/glm.hpp>
#include <string>
#include <vector>
using std::vector;
class ShaderProgram; // forward declaration

class Shader {
public:
  unsigned int ID;

  Shader(const ShaderProgram &vertex, const ShaderProgram &fragment);
  ~Shader();

  void useShader() const;

  // ---- Uniform setters by SPIR-V layout location ----
  void setBool(unsigned int location, bool value) const;
  void setInt(unsigned int location, int value) const;
  void setFloat(unsigned int location, float value) const;

  void setVec2(unsigned int location, const glm::vec2 &value) const;
  void setVec2(unsigned int location, float x, float y) const;

  void setVec3(unsigned int location, const glm::vec3 &value) const;
  void setVec3(unsigned int location, float x, float y, float z) const;

  void setVec4(unsigned int location, const glm::vec4 &value) const;
  void setVec4(unsigned int location, float x, float y, float z, float w) const;

  void setMat2(unsigned int location, const glm::mat2 &mat) const;
  void setMat3(unsigned int location, const glm::mat3 &mat) const;
  void setMat4(unsigned int location, const glm::mat4 &mat) const;

  void setUniformTextures(unsigned int location,
                          vector<GLuint> &textures) const;
};
