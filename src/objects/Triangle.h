#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Shader.h"
#include "Shape.h"
#include "glad/glad.h"
#include <GLFW/glfw3.h>
using namespace std;

class Triangle : public Shape {

private:
  unsigned int averageSideLength;

protected:
  void generateVertices(const float *inputVertices, size_t count);

public:
  Triangle(unsigned int VAO, unsigned int VBO, const float *inputVertices,
           Shader shader, unsigned int texture);
  ~Triangle() = default;
  void draw() const override;
  void moveHorizontally(int delta_x) override;
  void moveVertically(int delta_y) override;
  void rotateClockwise() override;
  void rotateCounterClockwise() override;
  void computeAverageTriangleSide(const float *inputVertices);
  float distance(const float *a, const float *b);
};

#endif // !TRIANGLE_H
