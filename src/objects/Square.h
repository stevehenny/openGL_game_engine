// Square.h
#ifndef SQUARE_H
#define SQUARE_H

#include "Shader.h"
#include "Shape.h"

class Square : public Shape {
public:
  Square(unsigned int VAO, unsigned int VBO, const float *inputVertices,
         Shader shader, unsigned int texture);
  ~Square();

  void draw() override;
  void moveHorizontally(float delta_x) override;
  void moveVertically(float delta_y) override;
  void updateBuffer() override;
  void rotateClockwise(float angle) override;
  void rotateCounterClockwise(float angle) override;

private:
  void generateVertices(const float *inputVertices, size_t count);
  void computeSideLength(const float *inputVertices);
  float distance(const float *a, const float *b);

  float sideLength;
  glm::mat4 currentTransform = glm::mat4(1.0f);
};

#endif // SQUARE_H
