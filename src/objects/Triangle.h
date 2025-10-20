#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Shader.h"
#include "Shape.h"
using namespace std;

class Triangle : public Shape {

private:
  unsigned int averageSideLength;
  glm::mat4 currentTransform = glm::mat4(1.0f); // identity matrix

protected:
  void generateVertices(const float *inputVertices, size_t count);

public:
  Triangle(unsigned int VAO, unsigned int VBO, const float *inputVertices,
           Shader shader, unsigned int texture);
  ~Triangle();
  void draw() override;
  void moveHorizontally(float delta_x) override;
  void moveVertically(float delta_y) override;
  void rotateClockwise(float angle) override;
  void rotateCounterClockwise(float angle) override;
  void updateBuffer() override;
  void computeAverageTriangleSide(const float *inputVertices);
  float distance(const float *a, const float *b);
};

#endif // !TRIANGLE_H
