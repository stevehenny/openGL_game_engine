#include "CameraObject.h"
#include <glm/ext/quaternion_geometric.hpp>
#include <glm/geometric.hpp>
#include <glm/glm.hpp>

CameraObject::CameraObject() {
  position = glm::vec3(0.0f, 0.0f, 0.0f);
  yaw = -90.0f; // pointing towards -Z
  pitch = 0.0f; // level camera
  roll = 0.0f;  // keep level also
}

void CameraObject::move_forward_back(float delta_z) {
  // get z vector
  glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 cameraDirection = glm::normalize((position - cameraTarget));

  // get x vector up
  glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
  glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
}
