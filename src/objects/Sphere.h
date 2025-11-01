#pragma once
#include "CommonStructs.h"
#include <glm/glm.hpp>
#include <vector>

using glm::vec3, glm::vec2, std::vector;

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
