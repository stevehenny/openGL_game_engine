#include "Triangle.h"
#include <math.h>

Triangle::Triangle(unsigned int VAO, unsigned int VBO,
                   const float *inputVertices, Shader shader)
    : Shape(VAO, VBO, shader) {
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

void Triangle::moveHorizontally(int delta_x) {}

void Triangle::moveVertically(int delta_y) {}

void Triangle::rotateClockwise() {}

void Triangle::rotateCounterClockwise() {}
