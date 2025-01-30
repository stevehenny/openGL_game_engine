#include <GLFW/glfw3.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define GRID_SIZE 40
#define WINDOW_SIZE 600
#define CELL_SIZE (WINDOW_SIZE / GRID_SIZE)

typedef struct {
  int x, y;
} Point;

typedef struct {
  Point body[GRID_SIZE * GRID_SIZE];
  int length;
  Point direction;
} Snake;

typedef struct {
  Snake snake;
  Point food;
  bool game_over;
} GameState;

GameState game;

// Initialize the game
void init_game() {
  game.snake.length = 1;
  game.snake.body[0].x = GRID_SIZE / 2;
  game.snake.body[0].y = GRID_SIZE / 2;
  game.snake.direction.x = 1; // Start moving to the right
  game.snake.direction.y = 0;

  // Place the food randomly
  srand(time(NULL));
  game.food.x = rand() % GRID_SIZE;
  game.food.y = rand() % GRID_SIZE;

  game.game_over = false;
}

// Update the game state
void update_game() {
  if (game.game_over)
    return;

  // Move the snake
  Point next_head = {game.snake.body[0].x + game.snake.direction.x,
                     game.snake.body[0].y + game.snake.direction.y};

  // Check for collisions with walls
  if (next_head.x < 0 || next_head.x >= GRID_SIZE || next_head.y < 0 ||
      next_head.y >= GRID_SIZE) {
    game.game_over = true;
    return;
  }

  // Check for collisions with the snake's body
  for (int i = 0; i < game.snake.length; i++) {
    if (game.snake.body[i].x == next_head.x &&
        game.snake.body[i].y == next_head.y) {
      game.game_over = true;
      return;
    }
  }

  // Move the snake
  for (int i = game.snake.length; i > 0; i--) {
    game.snake.body[i] = game.snake.body[i - 1];
  }
  game.snake.body[0] = next_head;

  // Check for food collision
  if (next_head.x == game.food.x && next_head.y == game.food.y) {
    game.snake.length++;
    game.food.x = rand() % GRID_SIZE;
    game.food.y = rand() % GRID_SIZE;
  }
}

// Render the game
void render_game(GLFWwindow *window) {
  glClear(GL_COLOR_BUFFER_BIT);

  // Draw the snake
  glColor3f(0.0f, 1.0f, 0.0f);
  for (int i = 0; i < game.snake.length; i++) {
    Point p = game.snake.body[i];
    float x = (float)p.x / GRID_SIZE * 2.0f - 1.0f;
    float y = (float)p.y / GRID_SIZE * 2.0f - 1.0f;

    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + 2.0f / GRID_SIZE, y);
    glVertex2f(x + 2.0f / GRID_SIZE, y + 2.0f / GRID_SIZE);
    glVertex2f(x, y + 2.0f / GRID_SIZE);
    glEnd();
  }

  // Draw the food
  glColor3f(1.0f, 0.0f, 0.0f);
  float x = (float)game.food.x / GRID_SIZE * 2.0f - 1.0f;
  float y = (float)game.food.y / GRID_SIZE * 2.0f - 1.0f;

  glBegin(GL_QUADS);
  glVertex2f(x, y);
  glVertex2f(x + 2.0f / GRID_SIZE, y);
  glVertex2f(x + 2.0f / GRID_SIZE, y + 2.0f / GRID_SIZE);
  glVertex2f(x, y + 2.0f / GRID_SIZE);
  glEnd();

  glfwSwapBuffers(window);
}

// Handle keyboard input
void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mods) {
  if (action != GLFW_PRESS)
    return;

  switch (key) {
  case GLFW_KEY_UP:
    if (game.snake.direction.y == 0) {
      game.snake.direction.x = 0;
      game.snake.direction.y = 1;
    }
    break;
  case GLFW_KEY_DOWN:
    if (game.snake.direction.y == 0) {
      game.snake.direction.x = 0;
      game.snake.direction.y = -1;
    }
    break;
  case GLFW_KEY_LEFT:
    if (game.snake.direction.x == 0) {
      game.snake.direction.x = -1;
      game.snake.direction.y = 0;
    }
    break;
  case GLFW_KEY_RIGHT:
    if (game.snake.direction.x == 0) {
      game.snake.direction.x = 1;
      game.snake.direction.y = 0;
    }
    break;
  case GLFW_KEY_ESCAPE:
    glfwSetWindowShouldClose(window, GLFW_TRUE);
    break;
  }
}

// Main function
int main() {
  if (!glfwInit()) {
    fprintf(stderr, "Failed to initialize GLFW\n");
    return EXIT_FAILURE;
  }

  GLFWwindow *window =
      glfwCreateWindow(WINDOW_SIZE, WINDOW_SIZE, "Snake Game", NULL, NULL);
  if (!window) {
    fprintf(stderr, "Failed to create GLFW window\n");
    glfwTerminate();
    return EXIT_FAILURE;
  }

  glfwMakeContextCurrent(window);
  glfwSetKeyCallback(window, key_callback);

  init_game();

  // Main loop
  while (!glfwWindowShouldClose(window)) {
    update_game();
    render_game(window);
    glfwPollEvents();
    glfwWaitEventsTimeout(0.1); // Slow down the game loop
  }

  glfwDestroyWindow(window);
  glfwTerminate();

  return EXIT_SUCCESS;
}
