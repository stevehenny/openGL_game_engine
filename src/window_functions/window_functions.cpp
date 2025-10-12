#include "window_functions.h"
#include "EngineObject.h"
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

void process_object_input(GLFWwindow *window, EngineObject *object) {
  // object->moveHorizontally(0.01);
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    object->moveVertically(0.01);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    object->moveVertically(-0.01);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    object->moveHorizontally(-0.01);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    object->moveHorizontally(0.01);
}

void process_camera_movement(GLFWwindow *window, glm::mat4 &view) {

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, 0.01f));
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -0.01f));
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    view = glm::translate(view, glm::vec3(0.01f, 0.0f, 0.0f));
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    view = glm::translate(view, glm::vec3(-0.01f, 0.0f, 0.0f));
  if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    view = glm::rotate(view, glm::radians(1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
  if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    view = glm::rotate(view, glm::radians(-1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
  if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    view = glm::rotate(view, glm::radians(-1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
  if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    view = glm::rotate(view, glm::radians(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
}
