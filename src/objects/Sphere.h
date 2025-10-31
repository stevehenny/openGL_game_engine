#pragma once
#include <vector>

#include <glm/glm.hpp>
#include <vector>

struct Vertex {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 texCoords;
};
std::vector<Vertex> generateSphere(float radius, unsigned int sectorCount,
                                   unsigned int stackCount);
std::vector<Vertex> generatePlane(float width, float depth,
                                  unsigned int xSegments,
                                  unsigned int zSegments, float yPos);
std::vector<unsigned int> generateIndices(unsigned int xSegments,
                                          unsigned int zSegments);
