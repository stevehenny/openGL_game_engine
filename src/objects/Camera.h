#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

using glm::vec3, glm::mat4, glm::sin, glm::cos;

class Camera {
public:
  Camera(vec3 position, vec3 camera_front, vec3 camera_up, float display_width,
         float display_height, float sensitivity, float near_clip,
         float far_clip, float camera_speed);
  static void move_callback(GLFWwindow *window);
  static void mouse_callback(GLFWwindow *window, double xposIn, double yposIn);
  static void scroll_callback(GLFWwindow *window, double xoffset,
                              double yoffset);
  void move_camera(GLFWwindow *window);
  void look_around(double xposIn, double yposIn);
  void zoom_in(double xoffset, double yoffset);
  void update_camera_delta_time(float current_frame);
  mat4 get_view_matrix() const;
  mat4 get_projection_matrix() const;
  vec3 &get_position();
  vec3 &get_front();
  vec3 &get_up();
  ~Camera() = default;

private:
  vec3 camera_position;
  vec3 camera_front;
  vec3 camera_up;
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
  float delta_time; // used to determine movement, along with camera speed and
  float last_frame; // last_frame
  float camera_speed;
};
