#ifndef SHADER_CLASS
#define SHADER_CLASS
#include <string>

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

#endif // !SHADER_CLASS
