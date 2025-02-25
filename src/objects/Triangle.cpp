#include "Triangle.h"
#include "Shape.h"
#include <math.h>
Triangle::Triangle(unsigned int VAO, unsigned int VBO,
                   const float *inputVertices, Shader shader,
                   unsigned int texture)
    : Shape(VAO, VBO, shader, texture) {
  generateVertices(inputVertices, 24);
  computeAverageTriangleSide(inputVertices);
  setup();
}

Triangle::~Triangle() {
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
}

void Triangle::generateVertices(const float *inputVertices, size_t count) {
  vertices.assign(inputVertices, inputVertices + count);
}

void Triangle::draw() {
  shader.useShader();
  // Send the current transform to the shader
  int transformLoc = glGetUniformLocation(shader.ID, "transform");
  glUniformMatrix4fv(transformLoc, 1, GL_FALSE,
                     glm::value_ptr(currentTransform));

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
  currentTransform =
      glm::rotate(transform, angle,
                  glm::vec3(0.0f, 0.0f, averageSideLength * sqrt(3.0f) / 3));

  int transformLoc = glGetUniformLocation(shader.ID, "transform");
  glUniformMatrix4fv(transformLoc, 1, GL_FALSE,
                     glm::value_ptr(currentTransform));
  draw();
}

void Triangle::rotateCounterClockwise() {

  shader.useShader();
  float timeValue = glfwGetTime();
  float angle = timeValue;
  glm::mat4 transform = glm::mat4(1.0f);
  currentTransform =
      glm::rotate(transform, -angle,
                  glm::vec3(0.0f, 0.0f, averageSideLength * sqrt(3.0f) / 3));

  int transformLoc = glGetUniformLocation(shader.ID, "transform");
  glUniformMatrix4fv(transformLoc, 1, GL_FALSE,
                     glm::value_ptr(currentTransform));
  draw();
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
