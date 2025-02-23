#include "Triangle.h"
#include "Shape.h"
#include <math.h>

Triangle::Triangle(unsigned int VAO, unsigned int VBO, float vertices[])
    : Shape(VAO, VBO) {
  generateVertices(vertices, 24);
  setup();
}

void Triangle::generateVertices() {
  // Default triangle if no external data is provided.
  float side_length = 1.0f;
  float height = sqrt(3) / 2 * side_length;
  float offsetY = height / 3.0f;
  float defaultVertices[24] = {
      0.5f,  -offsetY,         0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
      1.0f, // Bottom right
      -0.5f, -offsetY,         0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
      1.0f, // Bottom left
      0.0f,  height - offsetY, 0.0f, 0.0f, 0.0f, 1.0f, 0.5f,
      0.0f // Top
  };
  generateVertices(defaultVertices, 24);
}

void Triangle::generateVertices(const float *Inputvertices, size_t count) {
  vertices.assign(Inputvertices, Inputvertices + count);
}

void Triangle::draw() const {
  glBindVertexArray(VAO);
  glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 8);
  glBindVertexArray(0);
}
