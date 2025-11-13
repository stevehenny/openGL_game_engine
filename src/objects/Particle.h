#include "Object.h"
#include <glm/glm.hpp>

using glm::vec4;
class Particle : public Object {

public:
private:
  vec4 position;
  vec4 velocity;
  float mass;
};
