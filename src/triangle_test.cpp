#include "Shader.h"
#include "glad/glad.h"
#include "window_functions.h"
#include <GLFW/glfw3.h>

int main(int argc, char *argv[]) {
  GLFWwindow *window = create_window(1920, 1080);

  float vertices[] = {                                // pos
                      0.0f,  0.5f,  1.0f, 0.0f, 0.0f, // colors
                      -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
                      0.5f,  -0.5f, 0.0f, 0.0f, 1.0f};

  unsigned int VAO, VBO;

  // GENERATE VAO
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  // THEN GENERATE NECESSARY VBOs
  glGenBuffers(1, &VBO);

  // THEN BIND VBOs
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // THEN SET POSITIONS OF VBOs
  // Setting a position takes 2 steps:
  // 1. Defineing the position through glVertexAttribPointer
  // 2. Enabling the vertex attribute pointer
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void *)(2 * sizeof(float)));
  glEnableVertexAttribArray(1);

  Shader shader = Shader(argv[1], argv[2]);

  shader.useShader();

  while (!glfwWindowShouldClose(window)) {
    process_window_input(window);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    shader.useShader();
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glfwTerminate();
  return 0;
}
