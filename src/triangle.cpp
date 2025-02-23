#include <cstdlib>
#define STB_IMAGE_IMPLEMENTATION
#include "../include/glad/glad.h"
#include "shaders/Shader.h"
#include "stb_image.h"
#include "window_functions/window_functions.h"
#include <GLFW/glfw3.h>
#include <cmath>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/trigonometric.hpp>
#include <iostream>
#include <math.h>
#include <ostream>
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
  unsigned int texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  // set the texture wrapping/filtering options (on the currently bound texture
  // object)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // load and generate the texture
  int texture_width, texture_height, nrChannels;
  unsigned char *data =
      stbi_load(texturePath, &texture_width, &texture_height, &nrChannels, 0);
  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture_width, texture_height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "Failed to load texture" << std::endl;
    exit(1);
  }
  stbi_image_free(data);

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
  // Generate and bind a Vertex Array Object (VAO)
  unsigned int VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  // Generate and bind a Vertex Buffer Object (VBO)
  unsigned int VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // position attributes
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  // color attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  // Unbind the VBO and VAO
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  // Initialize Shader object using runtime arguments for shader paths
  Shader ourShader(vertexShaderPath, fragmentShaderPath);
  ourShader.useShader();
  glUniform1i(glGetUniformLocation(ourShader.ID, "outTexture"), 0);

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

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3); // Drawing a single triangle

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
