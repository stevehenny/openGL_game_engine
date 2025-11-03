#pragma once
#include "top_level_include.h"
#include <glm/glm.hpp>
#include <memory>
#include <vector>
using glm::vec3, glm::vec2, std::vector;

struct Vertex {
  vec3 position;
  vec3 prevPos;
  vec3 normal;
  vec3 force;
  vec2 texCoords;
  float invMass;
};
struct VertexArray {
  vec3 *begin;
  vec3 *position;
  vec3 *prevPos;
  vec3 *normal;
  vec3 *force;
  vec2 *texCoords;

  void resize(size_t size) {
    // Compute total float count: 3+3+3+3+2 = 14 floats per vertex
    size_t floats_per_vertex = 14;
    data = std::make_unique<float[]>(size * floats_per_vertex);

    float *buf = data.get();

    position = reinterpret_cast<vec3 *>(buf);
    buf += size * 3;

    prevPos = reinterpret_cast<vec3 *>(buf);
    buf += size * 3;

    normal = reinterpret_cast<vec3 *>(buf);
    buf += size * 3;

    force = reinterpret_cast<vec3 *>(buf);
    buf += size * 3;

    texCoords = reinterpret_cast<vec2 *>(buf);
    buf += size * 2;

    begin = position;
  }

private:
  std::unique_ptr<float[]> data;
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

// padded to 16 bytes
// GLSL will pad to in bytes of 16,
// so to match on the host side we
// strictly align in the same manner
struct alignas(16) GravityObject {
  vec3 position;
  float mass;
};

static_assert(sizeof(GravityObject) == 16,
              "'GravityObject' is not padded correctly!\n");

namespace physics_constants {
constexpr inline double G = 6.674e-11;
}
