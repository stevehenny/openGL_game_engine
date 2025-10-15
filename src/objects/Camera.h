#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

using namespace glm;

class Camera {
public:
  Camera(vec3 &position, vec3 &camera_front, vec3 &camera_up,
         float display_width, float display_height, float sensitivity,
         float near_clip, float far_clip);
  static void mouse_callback(GLFWwindow *window, double xposIn, double yposIn);
  void look_around(double xposIn, double yposIn);
  mat4 get_view_matrix() const;
  mat4 get_projection_matrix() const;
  vec3 &get_position();
  vec3 &get_front();
  vec3 &get_up();
  ~Camera() = default;

private:
  vec3 &camera_position;
  vec3 &camera_front;
  vec3 &camera_up;
  float camera_pitch; // rotation around X axis
  float camera_yaw;   // rotation around Y axis
  float camera_roll;  // rotation around Z axis
  float lastX;
  float lastY;
  float sensitivity;
  float fov;
  float aspect;
  float near_clip;
  float far_clip;
};
