#ifndef SHAPE_H
#define SHAPE_H

#include "../../include/glad/glad.h"
#include "EngineObject.h"
#include "shaders/Shader.h"
#include <GLFW/glfw3.h>
#include <vector>
using namespace std;
class Shape : public EngineObject {

protected:
  vector<float> vertices;
  unsigned int VAO, VBO;
  Shader shader;

public:
  Shape(unsigned int VAO, unsigned int VBO);
  virtual ~Shape();
  void setup();
  virtual void draw() const = 0;
};

#endif // !SHAPE_H
