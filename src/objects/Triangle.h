#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "../../include/glad/glad.h"
#include "Shape.h"
#include <GLFW/glfw3.h>
#include <vector>
using namespace std;

class Triangle : public Shape {

protected:
  void generateVertices() override;
  void draw() const override;
  void generateVertices(float vertices[], size_t count);

public:
  Triangle(unsigned int VAO, unsigned int VBO, float verticies[]);
};

#endif // !TRIANGLE_H
