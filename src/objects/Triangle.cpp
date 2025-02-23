#include "Triangle.h"
#include "Shape.h"

Triangle::Triangle(unsigned int VAO, unsigned int VBO, float vertices[])
    : Shape(VAO, VBO) {
  generateVertices(vertices, 24);
  setup();
}

void Triangle::generateVertices() {}

void Triangle::generateVertices(float vertices[], size_t count) {}

void Triangle::draw() const {}
