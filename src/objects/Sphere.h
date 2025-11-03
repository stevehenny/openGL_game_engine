#pragma once
#include "Common.h"
#include "Object.h"
#include "Shader.h"
#include <glm/glm.hpp>
#include <memory>
#include <vector>

using std::vector;

class Sphere : public Object {

public:
  Sphere(float radius, unsigned int sectorCount, unsigned int stackCount);
  ~Sphere();
  void draw() override;

private:
  auto generateSphere(float radius, unsigned int sectorCount,
                      unsigned int stackCount) -> vector<Vertex>;

  auto generateVertexArray(float radius, unsigned int sectorCount,
                           unsigned int stackCount) -> VertexArray;
  class Impl;
  std::unique_ptr<Impl> resources;
};

using glm::vec3, glm::vec2, std::vector;

auto generateVertexArray(float radius, unsigned int sectorCount,
                         unsigned int stackCount) -> VertexArray;
std::vector<Vertex> generateSphere(float radius, unsigned int sectorCount,
                                   unsigned int stackCount);
std::vector<Vertex> generatePlane(float width, float depth,
                                  unsigned int xSegments,
                                  unsigned int zSegments, float yPos);
std::vector<unsigned int> generateIndices(unsigned int xSegments,
                                          unsigned int zSegments);
std::vector<unsigned int> generateGridLineIndices(unsigned int xSegments,
                                                  unsigned int zSegments);
std::vector<PlaneConstraint>
generatePlaneConstraints(vector<Vertex> &planeVertices, unsigned int xSegments,
                         unsigned int zSegments);
