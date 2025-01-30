
#include "../include/glad/glad.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#ifdef __cplusplus
extern "C" {
#endif

class Shader {
public:
  unsigned int ID;
  // Constructor for shader
  Shader(const char *vertexPath, const char *fragmentPath);

  // use/activate the shader
  void useShader();

  // utility functions:
  void setBool(const std::string &name, bool value) const;
  void setInt(const std::string &name, int value) const;
  void setFloat(const std::string &name, float value) const;
};

#ifdef __cplusplus
}
#endif
