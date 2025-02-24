#ifndef TRIANGLE_H
#define TRIANGLE_H

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
  ~Triangle() = default;
  void draw() override;
  void moveHorizontally(int delta_x) override;
  void moveVertically(int delta_y) override;
  void rotateClockwise() override;
  void rotateCounterClockwise() override;
  void computeAverageTriangleSide(const float *inputVertices);
  float distance(const float *a, const float *b);
};

#endif // !TRIANGLE_H
