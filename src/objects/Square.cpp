// Square.cpp
#include "Square.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <math.h>

Square::Square(unsigned int VAO, unsigned int VBO, const float *inputVertices,
               Shader shader, unsigned int texture)
    : Shape(VAO, VBO, shader, texture) {
  generateVertices(inputVertices, 32);
  computeSideLength(inputVertices);
  setup();
}

Square::~Square() {
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
}

void Square::generateVertices(const float *inputVertices, size_t count) {
  vertices.assign(inputVertices, inputVertices + count);
}

void Square::draw() {
  shader.useShader();
  int transformLoc = glGetUniformLocation(shader.ID, "transform");
  glUniformMatrix4fv(transformLoc, 1, GL_FALSE,
                     glm::value_ptr(currentTransform));
  glBindVertexArray(VAO);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, vertices.size() / 8);
  glBindVertexArray(0);
}

void Square::moveHorizontally(int delta_x) {}

void Square::moveVertically(int delta_y) {}

void Square::rotateClockwise() {
  shader.useShader();
  float timeValue = glfwGetTime();
  float angle = timeValue;
  glm::mat4 transform = glm::mat4(1.0f);
  currentTransform = glm::rotate(transform, angle, glm::vec3(0.0f, 0.0f, 1.0f));
  int transformLoc = glGetUniformLocation(shader.ID, "transform");
  glUniformMatrix4fv(transformLoc, 1, GL_FALSE,
                     glm::value_ptr(currentTransform));
  draw();
}

void Square::rotateCounterClockwise() {
  shader.useShader();
  float timeValue = glfwGetTime();
  float angle = timeValue;
  glm::mat4 transform = glm::mat4(1.0f);
  currentTransform =
      glm::rotate(transform, -angle, glm::vec3(0.0f, 0.0f, 1.0f));
  int transformLoc = glGetUniformLocation(shader.ID, "transform");
  glUniformMatrix4fv(transformLoc, 1, GL_FALSE,
                     glm::value_ptr(currentTransform));
  draw();
}

void Square::computeSideLength(const float *inputVertices) {
  int stride = 8;
  const float *vertexA = inputVertices;
  const float *vertexB = inputVertices + stride;
  sideLength = distance(vertexA, vertexB);
}

float Square::distance(const float *a, const float *b) {
  float dx = b[0] - a[0];
  float dy = b[1] - a[1];
  float dz = b[2] - a[2];
  return std::sqrt(dx * dx + dy * dy + dz * dz);
}
