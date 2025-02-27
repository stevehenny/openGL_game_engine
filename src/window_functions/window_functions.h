#include "Square.h"
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <stdint.h>
#ifndef WINDOW_FUNCTIONS
#define WINDOW_FUNCTIONS

#include "EngineObject.h"
GLFWwindow *create_window(uint16_t x, uint16_t y);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void process_window_input(GLFWwindow *window);
void process_object_input(GLFWwindow *window, Square *sqaure);

#endif // !WINDOW_FUNCTIONS
