#include "Object.h"
#include <glm/glm.hpp>
#include <memory>

using glm::vec4;
class Particles : public Object {

public:
private:
  vec4 position;
  vec4 velocity;
  float mass;
  class Impl;
  std::unique_ptr<Impl> resources;
};
