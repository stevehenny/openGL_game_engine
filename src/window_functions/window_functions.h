
#pragma once

#include "EngineObject.h"
#include "top_level_include.h"
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
