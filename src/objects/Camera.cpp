#include "Camera.h"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/trigonometric.hpp>
#include <stdexcept>

Camera::Camera(vec3 &position, vec3 &camera_front, vec3 &camera_up,
               float display_width, float display_height, float sensitivity,
               float near_clip, float far_clip)
    : camera_position(position), camera_front(camera_front),
      camera_up(camera_up), lastX(display_width / 2.0f),
      lastY(display_height / 2.0), sensitivity(sensitivity),
      near_clip(near_clip), far_clip(far_clip) {
  camera_yaw = -90.0f; // pointing towards -Z
  camera_pitch = 0.0f; // level camera
  camera_roll = 0.0f;  // keep level also
  fov = 45.0f;         // default fov
  aspect = display_width / display_height;
}

void Camera::mouse_callback(GLFWwindow *window, double xposIn, double yposIn) {
  Camera *cam = static_cast<Camera *>(glfwGetWindowUserPointer(window));
  if (cam)
    cam->look_around(xposIn, yposIn);
  else
    throw std::runtime_error("CAMERA NOT SET GLOBALLY");
}
void Camera::look_around(double xposIn, double yposIn) {
  float xpos = static_cast<float>(xposIn);
  float ypos = static_cast<float>(yposIn);

  // calculate xoffset
  float xoffset = xpos - lastX;
  float yoffset = lastY - ypos;
  lastX = xpos;
  lastY = ypos;

  xoffset *= sensitivity;
  yoffset *= sensitivity;

  camera_yaw += xoffset;
  camera_pitch += yoffset;

  if (camera_pitch > 89.0f)
    camera_pitch = 89.0f;
  if (camera_pitch < -89.0f)
    camera_pitch = -89.0f;

  vec3 front;
  front.x = cos(radians(camera_yaw)) * cos(radians(camera_pitch));
  front.y = sin(radians(camera_pitch));
  front.z = sin(glm::radians(camera_yaw)) * cos(radians(camera_pitch));
  camera_front = normalize(front);
}
mat4 Camera::get_view_matrix() const {
  return glm::lookAt(camera_position, camera_position + camera_front,
                     camera_up);
}
mat4 Camera::get_projection_matrix() const {
  return glm::perspective(fov, aspect, near_clip, far_clip);
}
vec3 &Camera::get_position() { return camera_position; }
vec3 &Camera::get_front() { return camera_front; }
vec3 &Camera::get_up() { return camera_up; }
// void Camera::move_forward_back(float delta_z) {
//   // get z vector
//   glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
//   glm::vec3 cameraDirection = glm::normalize((position - cameraTarget));
//
//   // get x vector
//   glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
//   glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
// }
