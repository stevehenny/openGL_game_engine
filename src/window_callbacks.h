#include <GLFW/glfw3.h>
#ifndef WINDOW_CALLBACKS
#define WINDOW_CALLBACKS

#ifdef __cplusplus
extern "C" {
#endif

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void process_input(GLFWwindow *window);

#ifdef __cplusplus
}
#endif
#endif // !WINDOW_CALLBACKS
