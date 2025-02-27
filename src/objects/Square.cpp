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

void Square::updateBuffer() {
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(float),
                  vertices.data());
  glBindBuffer(GL_ARRAY_BUFFER, 0);
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

void Square::moveHorizontally(float delta_x) {
  shader.useShader();
  for (size_t i = 0; i < vertices.size(); i += 8) {
    vertices[i] += delta_x; // Modify X coordinate
  }
  updateBuffer(); // Update VBO
  draw();
}

void Square::moveVertically(float delta_y) {
  shader.useShader();
  for (size_t i = 0; i < vertices.size(); i += 8) {
    vertices[i + 1] += delta_y; // Modify Y coordinate
  }
  updateBuffer(); // Update VBO
  draw();
}

void Square::rotateClockwise(float angle) {
  shader.useShader();

  // Compute the center of the square
  float centerX = 0.0f, centerY = 0.0f;
  for (size_t i = 0; i < vertices.size(); i += 8) {
    centerX += vertices[i];     // Sum X coordinates
    centerY += vertices[i + 1]; // Sum Y coordinates
  }
  centerX /= (vertices.size() / 8); // Average X
  centerY /= (vertices.size() / 8); // Average Y

  // Create transformation matrices
  glm::mat4 translateToOrigin =
      glm::translate(glm::mat4(1.0f), glm::vec3(-centerX, -centerY, 0.0f));
  glm::mat4 rotation =
      glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 0.0f, 1.0f));
  glm::mat4 translateBack =
      glm::translate(glm::mat4(1.0f), glm::vec3(centerX, centerY, 0.0f));

  glm::mat4 transform = translateBack * rotation * translateToOrigin;

  // Apply the transformation to each vertex
  for (size_t i = 0; i < vertices.size(); i += 8) {
    glm::vec4 pos = transform * glm::vec4(vertices[i], vertices[i + 1],
                                          vertices[i + 2], 1.0);
    vertices[i] = pos.x;
    vertices[i + 1] = pos.y;
  }

  updateBuffer(); // Update VBO
  draw();
}

void Square::rotateCounterClockwise(float angle) {
  rotateClockwise(-angle); // Just negate the angle
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
