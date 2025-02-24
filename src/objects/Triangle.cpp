#include "Triangle.h"
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/trigonometric.hpp>
#include <math.h>
Triangle::Triangle(unsigned int VAO, unsigned int VBO,
                   const float *inputVertices, Shader shader,
                   unsigned int texture)
    : Shape(VAO, VBO, shader, texture) {
  generateVertices(inputVertices, 24);
  computeAverageTriangleSide(inputVertices);
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

// does not draw the new triangle locations
// need to call draw after this tranformation
void Triangle::rotateClockwise() {

  shader.useShader();
  float timeValue = glfwGetTime();
  float angle = timeValue;
  glm::mat4 transform = glm::mat4(1.0f);
  transform =
      glm::rotate(transform, angle,
                  glm::vec3(0.0f, 0.0f, averageSideLength * sqrt(3.0f) / 3));

  int transformLoc = glGetUniformLocation(shader.ID, "transform");
  glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
}

void Triangle::rotateCounterClockwise() {

  shader.useShader();
  float timeValue = glfwGetTime();
  float angle =
      timeValue; // or another value representing your desired rotation speed
  // Use a negative angle for counter-clockwise rotation.
  glm::mat4 transform = glm::mat4(1.0f);
  transform =
      glm::rotate(transform, -angle,
                  glm::vec3(0.0f, 0.0f, averageSideLength * sqrt(3.0f) / 3));

  int transformLoc = glGetUniformLocation(shader.ID, "transform");
  glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
}

// Compute the average triagle side length
void Triangle::computeAverageTriangleSide(const float *inputVertices) {
  int stride = 8;
  const float *vertexA = inputVertices;
  const float *vertexB = inputVertices + stride;
  const float *vertexC = inputVertices + (2 * stride);

  float sideAB, sideBC, sideCA;
  sideAB = distance(vertexA, vertexB);
  sideBC = distance(vertexB, vertexC);
  sideCA = distance(vertexC, vertexA);

  averageSideLength = (sideAB + sideBC + sideCA) / 3.0f;
}

// Computes Euclidean distance between two 3D points.
float Triangle::distance(const float *a, const float *b) {
  float dx = b[0] - a[0];
  float dy = b[1] - a[1];
  float dz = b[2] - a[2];
  return std::sqrt(dx * dx + dy * dy + dz * dz);
}
