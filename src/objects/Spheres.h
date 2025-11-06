#pragma once
#include "Common.h"
#include "Object.h"
#include "Shader.h"
#include <glm/glm.hpp>
#include <memory>
#include <vector>

using std::vector, glm::vec4, glm::mat4, glm::vec3;
namespace sphere_constants {
constexpr inline float DEFAULT_SPHERE_RADIUS{1.0f};
constexpr inline size_t SIZE_OF_SPHERE{7u};
constexpr inline size_t MAX_ENGINE_SPHERE_COUNT{100u};

}; // namespace sphere_constants
struct alignas(16) Sphere {
  vec4 objectColor;
  vec4 position;
  vec4 force;
  vec4 velocity;
  float radius;
  float mass;
  int textureInd;
};

// Factory class for generating Spheres
class Spheres : public Object {

public:
  Spheres(float radius, size_t sectorCount, size_t stackCount);
  ~Spheres();

  void renderSphere();
  void draw() override;
  Shader &getShader();
  vector<Sphere> &getSpheres();
  VertexArray &getVertexArray();
  vector<GLuint> &getIndices();
  void addSphere(const Sphere &sphere);
  void computeForces();
  void applyGravity(float dt);
  void updateSBBO(vector<Sphere> &spheres);
  void updateSBBO();
  void updateTextures(vector<GLuint> &texutres);

private:
  auto generateVertexArray() -> VertexArray;
  auto generateIndices() -> vector<GLuint>;

  float radius;
  size_t stackCount, sectorCount;
  VertexArray data;
  vector<GLuint> indices;
  vector<Sphere> spheres;
  vector<GLuint> textures;
  class Impl;
  std::unique_ptr<Impl> resources;
};

using glm::vec3, glm::vec2, std::vector;
