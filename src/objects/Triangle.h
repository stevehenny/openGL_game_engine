#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "../../include/glad/glad.h"
#include "Shape.h"
#include <GLFW/glfw3.h>
#include <vector>
using namespace std;

class Triangle : public Shape {

protected:
  void generateVertices() override;
  void draw() const override;
  void generateVertices(const float *Inputvertices, size_t count);

public:
  Triangle(unsigned int VAO, unsigned int VBO, float verticies[]);
  ~Triangle();
};

#endif // !TRIANGLE_H
