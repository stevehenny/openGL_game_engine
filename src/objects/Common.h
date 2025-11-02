#pragma once
#include <glm/glm.hpp>

using glm::vec3, glm::vec2;
struct Vertex {
  vec3 position;
  vec3 prevPos;
  vec3 normal;
  vec3 force;
  vec2 texCoords;
  float invMass;
};

struct Edge {
  int a, b;
  float restLength;
  float stifness; // [0,1] for PBD, or spring constant k for mas-spring
};

struct PlaneConstraint {
  unsigned int i1, i2;
  float restLength;
};

struct Material {
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float shininess;
};

struct Light {
  vec3 position;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

struct Color {
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

// padded to 32 bytes
// GLSL will pad to in bytes of 16,
// so to match on the host side we
// need to manually pad
struct alignas(16) GravityObject {
  vec3 position;
  float mass;
};

static_assert(sizeof(GravityObject) == 16,
              "'GravityObject' is not padded correctly!\n");

namespace physics_constants {
constexpr inline double G = 6.674e-11;
}
