
#ifndef WINDOW_FUNCTIONS
#define WINDOW_FUNCTIONS

#include "EngineObject.h"
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float4.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/trigonometric.hpp>
#include <stdint.h>

GLFWwindow *create_window(uint16_t x, uint16_t y);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void process_window_input(GLFWwindow *window);
void process_object_input(GLFWwindow *window, EngineObject *object);
void process_camera_movement(GLFWwindow *window, glm::mat4 &view);
#endif // !WINDOW_FUNCTIONS
