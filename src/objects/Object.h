
#pragma once

#include <glm/glm.hpp>

using glm::vec3;
class Object {

public:
  Object();
  virtual ~Object() = default;
  virtual void draw() = 0;
};
