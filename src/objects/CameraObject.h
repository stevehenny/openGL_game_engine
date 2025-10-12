#pragma once

#include "EngineObject.h"
#include <glm/glm.hpp>
class CameraObject : public EngineObject {
public:
  CameraObject();
  ~CameraObject() = default;
  void move_forward_back(float delta_z);
  void moveHorizontally(float delta_x);
  void moveVertically(float delta_y);
  void rotateClockwise(float angle);
  void rotateCounterClockwise(float angle);
  void updateBuffer();

private:
  glm::vec3 position;
  glm::vec3 front;
  glm::vec3 up;
  float pitch; // rotation around X axis
  float yaw;   // rotation around Y axis
  float roll;  // rotation around Z axis
};
