#ifndef SHAPE_H
#define SHAPE_H

#include "../../include/glad/glad.h"
#include <GLFW/glfw3.h>
#include <vector>
using namespace std;
class Shape {

protected:
  vector<float> vertices;
  unsigned int VAO, VBO;

  virtual void generateVertices() = 0; // Force subclasses to redefine
  virtual void draw() const = 0;

public:
  Shape(unsigned int VAO, unsigned int VBO);
  virtual ~Shape();
  void setup();
};

#endif // !SHAPE_H
