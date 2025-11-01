#ifndef SHAPE_H
#define SHAPE_H

#include "Object.h"
#include "Shader.h"
#include <vector>
using namespace std;
class Shape : public Object {

protected:
  vector<float> vertices;
  unsigned int VAO, VBO;
  Shader shader;
  unsigned int texture;

public:
  Shape(unsigned int VAO, unsigned int VBO, Shader shader,
        unsigned int texture);
  virtual ~Shape();
  void setup();
  virtual void draw() = 0;
};

#endif // !SHAPE_H
