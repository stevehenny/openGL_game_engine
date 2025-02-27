// Square.h
#ifndef SQUARE_H
#define SQUARE_H

#include "Shader.h"
#include "Shape.h"
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/trigonometric.hpp>

class Square : public Shape {
public:
  Square(unsigned int VAO, unsigned int VBO, const float *inputVertices,
         Shader shader, unsigned int texture);
  ~Square();

  void draw() override;
  void moveHorizontally(float delta_x) override;
  void moveVertically(float delta_y) override;
  void rotateClockwise();
  void rotateCounterClockwise();

private:
  void generateVertices(const float *inputVertices, size_t count);
  void computeSideLength(const float *inputVertices);
  float distance(const float *a, const float *b);

  float sideLength;
  glm::mat4 currentTransform = glm::mat4(1.0f);
};

#endif // SQUARE_H
