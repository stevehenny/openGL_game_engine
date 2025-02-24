#include "Shape.h"
#include "Shader.h"
#include <stdexcept>

Shape::Shape(unsigned int VAO, unsigned int VBO, Shader shader)
    : VAO(VAO), VBO(VBO), shader(shader) {}

Shape::~Shape() {
  glDeleteBuffers(1, &VBO);
  glDeleteVertexArrays(1, &VAO);
}

void Shape::setup() {
  if (vertices.size() == 0) {
    throw std::runtime_error("ERROR:DID NOT GENERATE VERTICES");
    return;
  }
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float),
               vertices.data(), GL_STATIC_DRAW);

  // Position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  // Color attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // Texture attribute
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}
