#include "window_functions.h"
#include "Square.h"
#include <GLFW/glfw3.h>
#include <stdio.h>

GLFWwindow *create_window(uint16_t x, uint16_t y) {

  // Initialize GLFW
  if (!glfwInit()) {
    fprintf(stderr, "Failed to initialize GLFW\n");
    return NULL;
  }

  // Configure GLFW to use OpenGL 3.3 Core Profile
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Create a window
  GLFWwindow *window = glfwCreateWindow(x, y, "OpenGL Triangle", NULL, NULL);
  if (!window) {
    fprintf(stderr, "Failed to create GLFW window\n");
    glfwTerminate();
    return NULL;
  }
  glfwMakeContextCurrent(window);

  // Load OpenGL function pointers using GLAD
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    fprintf(stderr, "Failed to initialize GLAD\n");
    return NULL;
  }

  // Set the viewport and framebuffer size callback
  glViewport(0, 0, x, y);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  return window;
}
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void process_window_input(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
}

void process_object_input(GLFWwindow *window, Square *square) {
  // square->moveHorizontally(0.01);
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    square->moveVertically(0.01);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    square->moveVertically(-0.01);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    square->moveHorizontally(-0.01);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    square->moveHorizontally(0.01);
}
