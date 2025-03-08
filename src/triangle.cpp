#include "Triangle.h"
#include "Shader.h"
#include "Square.h"
#include "Texture.h"
#include "glad/glad.h"
#include "window_functions.h"
#include <GLFW/glfw3.h>
#include <cmath>
#include <cstdlib>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/trigonometric.hpp>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  // Check if shader paths are provided via arguments
  if (argc < 4) {
    fprintf(stderr,
            "Usage: %s <vertex_shader_path> <fragment_shader_path> "
            "<texture_path>\n",
            argv[0]);
    return -1;
  }

  const char *vertexShaderPath = argv[1];
  const char *fragmentShaderPath = argv[2];
  const char *texturePath = argv[3];

  GLFWwindow *window = create_window(1920, 1080);
  // Initialize the texture
  float side_length = 1.0f;
  float height = sqrt(3) / 2 * side_length;
  float offsetY = height / 3.0f; // Centering the triangle

  float vertices[] = {
      // positions             // colors         //textures
      0.5f,  -offsetY,         0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
      1.0f, // Bottom right
      -0.5f, -offsetY,         0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
      1.0f, // Bottom left
      0.0f,  height - offsetY, 0.0f, 0.0f, 0.0f, 1.0f, 0.5f,
      0.0f, // Top
  };

  float squareVertices[] = {
      // Positions          // Colors          // Texture Coords
      -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // Bottom left
      0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, // Bottom right
      -0.5f, 0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // Top left
      0.5f,  0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f  // Top right
  };

  unsigned int VAO, VBO;

  unsigned int texture = loadTexture(texturePath);
  Shader ourShader = Shader(vertexShaderPath, fragmentShaderPath);
  Triangle *triangle = new Triangle(VAO, VBO, vertices, ourShader, texture);
  Triangle *triangle2 = new Triangle(VAO, VBO, vertices, ourShader, texture);
  Square *square = new Square(VAO, VBO, squareVertices, ourShader, texture);
  // Main render loop
  while (!glfwWindowShouldClose(window)) {
    // Input handling
    process_window_input(window);
    process_object_input(window, square);
    // Rendering commands
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    float timeValue = glfwGetTime();
    float angle = glm::radians(-1.0f); // Rotate by -1 degrees
    triangle->rotateClockwise(angle);
    triangle2->rotateCounterClockwise(angle);
    square->draw();

    // Swap buffers and poll IO events
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  delete triangle;
  delete triangle2;
  delete square;

  glfwTerminate();
  return 0;
}
