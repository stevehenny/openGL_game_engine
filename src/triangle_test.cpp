#include "Shader.h"
#include "ShaderProgram.h"
#include "glad/glad.h"
#include "window_functions.h"
#include <GLFW/glfw3.h>
#include <glm/ext/quaternion_transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/trigonometric.hpp>

int main(int argc, char *argv[]) {
  GLFWwindow *window = create_window(1920, 1080);

  float vertices[] = {                                      // pos
                      0.0f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, // colors
                      -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
                      0.5f,  -0.5f, 0.0f, 0.0f, 0.0f, 1.0f};

  float d3_verts[] = {
      // pos                // colors
      0.0f,  0.5f,  0.0f,  0.0f, 0.0f, 0.0f, // point 1
      -0.5f, -0.5f, 0.5f,  1.0f, 1.0f, 1.0f, // point 2
      0.5f,  -0.5f, 0.5f,  0.0f, 0.0f, 0.0f, // point 3
      0.0f,  -0.5f, -0.5f, 0.0f, 0.0f, 0.0f  // point 4
  };

  unsigned int indices[] = {
      0, 1, 2, // first side
      0, 2, 3, // second side
      0, 3, 1, // third side
      1, 2, 3  // fourth side
  };

  unsigned int VAO, VBO, EBO;

  // GENERATE VAO
  glGenVertexArrays(1, &VAO);

  // THEN GENERATE NECESSARY VBOs
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);
  // THEN BIND VBOs
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(d3_verts), d3_verts, GL_STATIC_DRAW);

  // bind indices
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  // THEN SET POSITIONS OF VBOs
  // Setting a position takes 2 steps:
  // 1. Defineing the position through glVertexAttribPointer
  // 2. Enabling the vertex attribute pointer
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  Shader shader =
      Shader(ShaderProgram{std::string(argv[1]), ShaderTypes::VERTEX},
             ShaderProgram{std::string(argv[2]), ShaderTypes::FRAGMENT});

  glEnable(GL_DEPTH_TEST);

  while (!glfwWindowShouldClose(window)) {
    process_window_input(window);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 transform = glm::mat4(1.0f); // identity
    glm::mat4 colorTransform = glm::mat4(1.0f);
    transform = glm::rotate(transform, (float)glfwGetTime(),
                            glm::vec3(0.0f, 1.0f, 0.0f));
    shader.useShader();
    shader.setMat4("transform", transform);
    shader.setMat4("colorTransform", transform);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glfwTerminate();
  return 0;
}
