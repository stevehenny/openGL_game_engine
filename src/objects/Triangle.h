#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "../../include/glad/glad.h"
#include "Shape.h"
#include <GLFW/glfw3.h>
#include <vector>
using namespace std;

class Triangle : public Shape {

protected:
  void generateVertices(const float *inputVertices, size_t count);

public:
  Triangle(unsigned int VAO, unsigned int VBO, const float *inputVertices);
  ~Triangle() = default;
  void draw() const override;
};

#endif // !TRIANGLE_H
