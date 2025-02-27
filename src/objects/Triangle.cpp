#include "Triangle.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
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

void Triangle::updateBuffer() {
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(float),
                  vertices.data());
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Triangle::draw() {
  shader.useShader();
  int transformLoc = glGetUniformLocation(shader.ID, "transform");
  glUniformMatrix4fv(transformLoc, 1, GL_FALSE,
                     glm::value_ptr(currentTransform));

  glBindVertexArray(VAO);
  glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 8);
  glBindVertexArray(0);
}

void Triangle::moveHorizontally(float delta_x) {
  shader.useShader();
  for (size_t i = 0; i < vertices.size(); i += 8) {
    vertices[i] += delta_x; // Modify X coordinate
  }
  updateBuffer();
  draw();
}

void Triangle::moveVertically(float delta_y) {
  shader.useShader();
  for (size_t i = 0; i < vertices.size(); i += 8) {
    vertices[i + 1] += delta_y; // Modify Y coordinate
  }
  updateBuffer();
  draw();
}

void Triangle::rotateClockwise(float angle) {
  shader.useShader();

  // Compute the centroid of the triangle
  float centerX = 0.0f, centerY = 0.0f;
  for (size_t i = 0; i < vertices.size(); i += 8) {
    centerX += vertices[i];
    centerY += vertices[i + 1];
  }
  centerX /= (vertices.size() / 8);
  centerY /= (vertices.size() / 8);

  // Create transformation matrices
  glm::mat4 translateToOrigin =
      glm::translate(glm::mat4(1.0f), glm::vec3(-centerX, -centerY, 0.0f));
  glm::mat4 rotation =
      glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 0.0f, 1.0f));
  glm::mat4 translateBack =
      glm::translate(glm::mat4(1.0f), glm::vec3(centerX, centerY, 0.0f));

  glm::mat4 transform = translateBack * rotation * translateToOrigin;

  // Apply transformation to each vertex
  for (size_t i = 0; i < vertices.size(); i += 8) {
    glm::vec4 pos = transform * glm::vec4(vertices[i], vertices[i + 1],
                                          vertices[i + 2], 1.0);
    vertices[i] = pos.x;
    vertices[i + 1] = pos.y;
  }

  updateBuffer();
  draw();
}

void Triangle::rotateCounterClockwise(float angle) { rotateClockwise(-angle); }

void Triangle::computeAverageTriangleSide(const float *inputVertices) {
  int stride = 8;
  const float *vertexA = inputVertices;
  const float *vertexB = inputVertices + stride;
  const float *vertexC = inputVertices + (2 * stride);

  float sideAB = distance(vertexA, vertexB);
  float sideBC = distance(vertexB, vertexC);
  float sideCA = distance(vertexC, vertexA);

  averageSideLength = (sideAB + sideBC + sideCA) / 3.0f;
}

float Triangle::distance(const float *a, const float *b) {
  float dx = b[0] - a[0];
  float dy = b[1] - a[1];
  float dz = b[2] - a[2];
  return std::sqrt(dx * dx + dy * dy + dz * dz);
}
