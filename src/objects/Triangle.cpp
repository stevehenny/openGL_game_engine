#include "Triangle.h"
#include "Shape.h"
#include <math.h>

Triangle::Triangle(unsigned int VAO, unsigned int VBO,
                   const float *inputVertices)
    : Shape(VAO, VBO) {
  generateVertices(inputVertices, 24);
  setup();
}

void Triangle::generateVertices(const float *inputVertices, size_t count) {
  vertices.assign(inputVertices, inputVertices + count);
}

void Triangle::draw() const {
  glBindVertexArray(VAO);
  glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 8);
  glBindVertexArray(0);
}
