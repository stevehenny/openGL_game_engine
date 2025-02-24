#include "Triangle.h"
#include "Shader.h"
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
  unsigned int texture = loadTexture(texturePath);
  glBindTexture(GL_TEXTURE_2D, texture);
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
  unsigned int VAO, VBO;

  Shader ourShader(vertexShaderPath, fragmentShaderPath);
  Triangle triangle = Triangle(VAO, VBO, vertices, ourShader);

  // Initialize Shader object using runtime arguments for shader paths
  ourShader.useShader();
  glUniform1i(glGetUniformLocation(ourShader.ID, "outTexture"), VAO);
  triangle.draw();
  // Main render loop
  while (!glfwWindowShouldClose(window)) {
    // Input handling
    process_input(window);

    // Rendering commands
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Use the shader program and set the uniform value
    ourShader.useShader();
    float timeValue = glfwGetTime();
    float angle = timeValue;
    glm::mat4 transform = glm::mat4(1.0f);
    transform = glm::rotate(
        transform, angle, glm::vec3(0.0f, 0.0f, side_length * sqrt(3.0f) / 3));

    int transformLoc = glGetUniformLocation(ourShader.ID, "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

    triangle.draw();

    // Swap buffers and poll IO events
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // Cleanup
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);

  glfwTerminate();
  return 0;
}
