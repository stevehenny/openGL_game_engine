#ifndef SHAPE_H
#define SHAPE_H

#include "EngineObject.h"
#include "Shader.h"
#include "Texture.h"
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <vector>
using namespace std;
class Shape : public EngineObject {

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
  virtual void draw() const = 0;
};

#endif // !SHAPE_H
