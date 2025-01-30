#include "include/glad/glad.h"
#include <GLFW/glfw3.h> // Example: using GLFW for context creation
#include <stdio.h>
#include <stdlib.h>

#define GRID_SIZE 40
#define WINDOW_SIZE_X 1920
#define WINDOW_SIZE_Y 1080
#define CELL_SIZE (WINDOW_SIZE_X / GRID_SIZE) * (WINDOW_SIZE_Y / WINDOW_SIZE_X)

struct Point {
  int x, y;
};

typedef struct {
  struct Point points[GRID_SIZE * GRID_SIZE];
  int index;
} GameState;

GameState game;

void render_game(GLFWwindow *window) {
  glClear(GL_COLOR_BUFFER_BIT);

  // Draw the boxes
  glColor3f(0.5f, 1.0f, 0.0f);
  for (int i = 0; i < game.index; i++) {
    struct Point p = game.points[i];
    float x = (float)p.x / GRID_SIZE * 2.0f - 1.0f;
    float y = (float)p.y / GRID_SIZE * 2.0f - 1.0f;

    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + 2.0f / GRID_SIZE, y);
    glVertex2f(x + 2.0f / GRID_SIZE, y + 2.0f / GRID_SIZE);
    glVertex2f(x, y + 2.0f / GRID_SIZE);
    glEnd();
  }
  glfwSwapBuffers(window);
}

// Handle keyboard input
void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mods) {
  if (action != GLFW_PRESS)
    return;

  switch (key) {
  case GLFW_KEY_A:
    struct Point point;
    point.x = rand() % GRID_SIZE;
    point.y = rand() % GRID_SIZE;
    game.points[game.index] = point;
    game.index++;
    break;
  case GLFW_KEY_ESCAPE:
    glfwSetWindowShouldClose(window, GLFW_TRUE);
    break;
  }
}
int main() {
  if (!glfwInit()) {
    return -1;
  }
  GLFWwindow *window = glfwCreateWindow(WINDOW_SIZE_X, WINDOW_SIZE_Y,
                                        "GLAD Example", NULL, NULL);
  if (!window) {
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetKeyCallback(window, key_callback);
  if (!gladLoadGL()) {
    printf("Failed to initialize GLAD\n");
    return -1;
  }
  while (!glfwWindowShouldClose(window)) {
    render_game(window);
    glfwPollEvents();
    glfwWaitEventsTimeout(0.01);
  }
  // OpenGL code here
  glfwTerminate();
  return 0;
}
