#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <stdint.h>
#ifndef WINDOW_FUNCTIONS
#define WINDOW_FUNCTIONS
#ifdef __cplusplus
extern "C" {
#endif
GLFWwindow *create_window(uint16_t x, uint16_t y);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void process_input(GLFWwindow *window);

#ifdef __cplusplus
}
#endif
#endif // !WINDOW_FUNCTIONS
